import numpy as np
from logging import getLogger, Logger
from h5py import File
from datetime import datetime
from pathlib import Path
from time import sleep
from tqdm import tqdm
from threading import Event, Thread, Lock
from psutil import cpu_percent, virtual_memory
from pylsl import (
    StreamInfo,
    StreamInlet,
    StreamOutlet,
    resolve_bypred,
    cf_int16,
    cf_int32,
    cf_float32
)
from queue import Queue, Empty
from vispy import app, scene
from api.data_api import DataAPI, RawRecording


class RingBuffer:
    def __init__(self, size) -> None:
        self._size = size
        self._data0 = np.zeros(shape=(size, 2), dtype=float)
        self._index = 0
        self._is_full = False

    def _update_index(self) -> None:
        self._index += 1
        if self._index >= self._size:
            self._index = 0
            self._is_full = True

    def append(self, value: int) -> None:
        self._data0[self._index, 0] = self._index
        if self._is_full:
            self._data0[:-1, 1] = self._data0[1:, 1]
            self._data0[-1, 1] = value
        else:
            self._data0[self._index, 1] = value
        self._update_index()

    def append_with_timestamp(self, tb: float, value: int) -> None:
        if self._is_full:
            self._data0[:-1, 0] = self._data0[1:, 0]
            self._data0[:-1, 1] = self._data0[1:, 1]
            self._data0[-1, 0] = tb
            self._data0[-1, 1] = value
        else:
            self._data0[self._index:, 0] = tb
            self._data0[self._index:, 1] = value
        self._update_index()

    def get_data(self) -> np.ndarray:
        return self._data0


class ThreadLSL:
    _logger: Logger
    _event: Event
    _lock: Lock
    _thread: list[Thread]
    _exception: Queue
    _thread_active: list[int]
    _is_active: bool
    _num_missed: int=0

    def __init__(self) -> None:
        """Class for managing all threads for the LSL data processing
        :return:    None
        """
        self._logger = getLogger(__name__)
        self._event = Event()
        self._lock = Lock()
        self._exception = Queue()
        self._release_threads()

    @property
    def is_alive(self) -> bool:
        """Returning the state of all threads if they are alive"""
        if len(self._thread) == 0:
            return False
        else:
            return all([thread.is_alive() for thread in self._thread])

    @property
    def is_running(self) -> bool:
        """Returning the state of the thread handler if DAQ is running"""
        return self._event.is_set() and self._is_active

    def register(self, func, args) -> None:
        """Registering a thread with custom instruction
        :param func:    Function object for further processing in own thread
        :param args:    Arguments of the object for starting it
        :return:        None
        """
        if not len(self._thread):
            self._thread = [Thread(target=self._thread_watchdog_heartbeat, args=())]
        self._thread.append(Thread(target=func, args=args))

    def start(self) -> None:
        """
        Starting all threads including heartbeat watchdog in own thread
        :return:    None
        """
        self._thread_active = list()
        if len(self._thread) < 2:
            raise AssertionError("No threads registered")
        else:
            self._thread_active = [False for _ in self._thread[1:]]
            self._num_missed = 0
            self._is_active = True
            self._event.set()
            for idx, p in enumerate(self._thread):
                p.start()
            sleep(0.2)

    def stop(self) -> None:
        """Stopping all threads and waiting for shutdown all threads
        :return:        None
        """
        self._event.clear()
        for p in self._thread:
            p.join(timeout=1.)
        self._release_threads()

    def check_exception(self) -> None:
        """Function for checking if any exception information is available from any thread, and return it
        :return:    None
        """
        try:
            exc = self._exception.get_nowait()
        except Empty:
            pass
        else:
            raise exc

    def wait_for_seconds(self, wait_time_sec: float) -> None:
        """Waiting routine during start and stop of DAQ and returning errors if happens
        :param wait_time_sec:   Time to wait for a second
        :return:                None
        """
        sleep(1.)
        for _ in tqdm(range(int(wait_time_sec))):
            if self._is_active:
                self.check_exception()
                sleep(1.)
            else:
                raise RuntimeError(f"One thread is shutdown [{self._is_active}] - {self._thread_active}")

    def _release_threads(self) -> None:
        self._logger.debug(f"Empty thread")
        self._thread = []
        self._thread_active = []
        self._is_active = False

    def _thread_watchdog_heartbeat(self) -> None:
        while self._event.is_set():
            check_alive = all([thread.is_alive() for thread in self._thread[1:]])
            try:
                with self._lock:
                    checker = all(self._thread_active)
                    for i, _ in enumerate(self._thread_active):
                        self._thread_active[i] = False
                if check_alive and checker:
                    self._num_missed = 0
                else:
                    self._num_missed += 1
                if self._num_missed >= 5:
                    self._is_active = False
            except Exception as e:
                with self._lock:
                    self._exception.put(e)
            sleep(2.)

    def _thread_dummy(self, stime_idx: int) -> None:
        while self._event.is_set():
            try:
                with self._lock:
                    self._thread_active[stime_idx] = True
                sleep(0.1)
            except Exception as e:
                with self._lock:
                    self._exception.put(e)

    def _thread_stream_mock(self, stim_idx: int, name: str, channel_num: int=2, sampling_rate: float=200.) -> None:
        info = StreamInfo(
            name=name,
            type='mock_sinusoidal_daq',
            channel_count=channel_num,
            nominal_srate=sampling_rate,
            channel_format=cf_int16,
            source_id=f"{name}_uid"
        )
        outlet = StreamOutlet(info)

        idx = 0
        ite_num = 0
        offset = 2 ** 14 - 1
        signal_step = np.pi / 20
        channels = [ch for ch in range(channel_num)]
        while self._event.is_set():
            try:
                # Heartbeat
                if ite_num >= int(0.5 * sampling_rate):
                    ite_num = 0
                    with self._lock:
                        self._thread_active[stim_idx] = True
                else:
                    ite_num += 1
                # Process data
                outlet.push_sample(
                    x=[int(offset + offset * np.sin(idx + ch)) for ch in channels],
                    pushthrough=True
                )
                idx += signal_step
                sleep(1 / sampling_rate)
            except Exception as e:
                with self._lock:
                    self._exception.put(e)

    def _thread_stream_file(self, stim_idx: int, name: str, path2data: str, file_index: int=0, prefix: str='data') -> None:
        path0 = Path(path2data)
        if not path0.exists():
            raise AttributeError("File is not available")
        data: RawRecording = DataAPI(path2data=path0, data_prefix=prefix).read_data_file(file_index)

        info = StreamInfo(
            name=name,
            type='mock_file_daq',
            channel_count=int(data.num_channels),
            nominal_srate=float(data.sampling_rate),
            channel_format=cf_int16,
            source_id=f"{name}_uid"
        )
        outlet = StreamOutlet(info)
        mode_mock = 'mock' in name

        ite_num = 0
        while self._event.is_set():
            try:
                # Process data
                for stime, sdata in zip(data.time, data.data.T):
                    # Heartbeat
                    if ite_num >= int(0.5 * data.sampling_rate):
                        ite_num = 0
                        with self._lock:
                            self._thread_active[stim_idx] = True
                    else:
                        ite_num += 1

                    send = list() if mode_mock else [int(1e6 * stime)]
                    send.extend(sdata)
                    outlet.push_sample(
                        x=send,
                        pushthrough=True
                    )
                    sleep(1 / data.sampling_rate)
                    if not self._event.is_set():
                        break
            except Exception as e:
                with self._lock:
                    self._exception.put(e)

    def _thread_stream_data(self, stim_idx: int, name: str, daq_func, channel_num: int, sampling_rate: float) -> None:
        """Process for starting a Lab Streaming Layer (LSL) to process the data stream from DAQ system
        :param name:            String with name of the LSL stream (must match with recording process)
        :param daq_func:        Function to get data from DAQ device (returned list)
        :param channel_num:     Channel number to start stream from
        :param sampling_rate:   Floating value with sampling rate in Hz
        :return:                None
        """
        config = StreamInfo(
            name=name,
            type='custom_daq',
            channel_count=channel_num,
            nominal_srate=sampling_rate,
            channel_format=cf_int32,
            source_id=name + '_uid'
        )
        outlet = StreamOutlet(config)

        ite_num = 0
        while self._event.is_set():
            try:
                # Heartbeat
                if ite_num >= int(0.5 * sampling_rate):
                    ite_num = 0
                    with self._lock:
                        self._thread_active[stim_idx] = True
                else:
                    ite_num += 1
                # Data Processing
                data = daq_func()
                if not len(data):
                    continue
                outlet.push_sample(data)
            except Exception as e:
                with self._lock:
                    self._exception.put(e)

    def _thread_stream_util(self, stim_index: int, name: str, sampling_rate: float=2.) -> None:
        """Process for starting a Lab Streaming Layer (LSL) to process the utilization of the host computer
        :param stim_index:      Integer with array index to write into heartbeat feedback array
        :param name:            String with name of the LSL stream (must match with recording process)
        :param sampling_rate:   Float with sampling rate for determining the sampling rate
        :return:                None
        """
        if sampling_rate > 2.:
            raise ValueError("Please reduce sampling rate lower than 2.0 Hz")

        config = StreamInfo(
            name=name,
            type='utilization',
            channel_count=2,
            nominal_srate=sampling_rate,
            channel_format=cf_float32,
            source_id=name + '_uid'
        )
        outlet = StreamOutlet(config)

        while self._event.is_set():
            try:
                with self._lock:
                    self._thread_active[stim_index] = True
                outlet.push_sample([cpu_percent(interval=1 / sampling_rate), virtual_memory().percent])
                sleep(1 / sampling_rate)
            except Exception as e:
                with self._lock:
                    self._exception.put(e)

    def _thread_record_stream(self, stim_idx: int, name: str, path2save: Path | str, take_samples: list = ()) -> None:
        """Function for recording and saving the data pushed on LSL stream
        :param stim_idx:            Integer with array index to write into heartbeat feedback array
        :param name:                String with name of the LSL stream in order to catch it
        :param path2save:           Path to save the data (if it is a string, it will be auto-converted)
        :param take_samples:        List with indexes to take from LSL pull list for further processing
        :return: None
        """
        mode_util = 'util' in name
        mode_mock = 'mock' in name
        path = Path(path2save) if type(path2save) == str else path2save
        inlet = StreamInlet(resolve_bypred(f"name='{name}'")[0])
        # Extract meta
        channels = inlet.info().channel_count() if mode_util or not len(take_samples) else len(take_samples)
        sampling_rate = inlet.info().nominal_srate()
        sys_type = inlet.info().type()
        data_format = inlet.info().channel_format()
        time = datetime.today().strftime('%Y%m%d_%H%M%S')

        if not path.is_dir():
            path.mkdir(parents=True, exist_ok=True)
        with File(path.absolute() / f"{time}_{name}.h5", "w") as f:
            f.attrs["sampling_rate"] = sampling_rate
            f.attrs["channel_count"] = channels
            f.attrs["type"] = sys_type
            f.attrs["creation_date"] = datetime.today().strftime('%Y-%m-%d')
            f.attrs["data_format"] = data_format

            ts_dset = f.create_dataset("time", (0,), maxshape=(None,), dtype="float32")
            ts_dset.attrs["unit"] = "s"
            data_dset = f.create_dataset("data", (0, channels), maxshape=(None, channels), dtype="uint16")
            ts_dset.attrs["unit"] = ""

            process_list = take_samples if len(take_samples) > 0 else [i for i in range(channels)]
            while self._event.is_set():
                try:
                    samples, ts = inlet.pull_chunk(
                        max_samples=int(sampling_rate),
                        timeout=20e-3
                    )
                    if samples is None:
                        continue
                    else:
                        with self._lock:
                            self._thread_active[stim_idx] = True
                        idx = len(ts_dset)
                        ts_dset.resize((idx + len(ts),))
                        data_dset.resize((idx + len(ts), channels))
                        for id0, (sample, tb) in enumerate(zip(samples, ts)):
                            ts_dset[idx + id0] = tb if mode_util or mode_mock else 1e-6 * sample[0]
                            data_dset[idx + id0] = [sample[i] for i in process_list]
                        f.flush()
                except Exception as e:
                    self._exception.put(e)
            f.close()

    def _thread_plot_stream(
            self, stim_idx: int, name: str, window_length: float = 10.,
            take_samples: list = (), update_rate: float = 12.
    ) -> None:
        """Function for LSL to enable live plotting of the incoming results using VisPy
        :param stim_idx:        Integer with array index to write into heartbeat feedback array
        :param name:            String with name of the LSL stream to get data
        :param window_length:   Floating value with length of time window for plotting in seconds
        :param take_samples:    List with taking idx from LSL pulling event
        :param update_rate:     Floating value with update rate of the LSL datastream
        :return:                None
        """
        line_color = ['red', 'green', 'blue', 'lime']
        mode_util = 'util' in name
        inlet = StreamInlet(resolve_bypred(f"name='{name}'")[0])
        # --- Extract meta
        channels = inlet.info().channel_count() if mode_util or not len(take_samples) else len(take_samples)
        sampling_rate = inlet.info().nominal_srate()
        if sampling_rate > 2500.:
            raise AttributeError(f"Sampling rate {sampling_rate} is too high")
        # --- Build ring buffer and update func
        number_samples_pull = int(sampling_rate / update_rate) - 1 if sampling_rate > update_rate else 1
        number_samples_window = int(window_length * sampling_rate)
        buffer_lsl = [RingBuffer(number_samples_window) for _ in range(channels)]
        buffer_gpu = buffer_lsl.copy()
        if not take_samples:
            take_samples = list(range(channels))
        # --- Build app
        canvas = scene.SceneCanvas(
            size=(800, 450),
            title=f"Live Plot @{sampling_rate} Hz ({name})",
            keys=None,
            app="glfw",
            show=True,
        )
        view = canvas.central_widget.add_view()
        view.camera = "panzoom"
        view.camera.set_range(
            x=(0, number_samples_window - 1),
            y=(0, 65535) if not mode_util else (0, 100)
        )
        curves = list()
        for idx, buffer in enumerate(buffer_gpu):
            curves.append(scene.Line(
                pos=buffer.get_data(),
                width=2,
                color=line_color[idx % len(line_color)],
                parent=view.scene,
                antialias=False
            ))
        status_text = scene.Text(
            text="LSL: OK",
            color='green',
            parent=view.scene,
            pos=(0.95 * number_samples_window, 0),
            font_size=8
        )
        fps_text = scene.Text(
            text="FPS: 0",
            color='green',
            parent=view.scene,
            pos=(0.95 * number_samples_window, 0.98 * (65535 if not mode_util else 100)),
            font_size=8
        )

        def update_plot_data():
            nonlocal buffer_lsl
            while self._event.is_set():
                try:
                    # Process data
                    samples, _ = inlet.pull_chunk(
                        max_samples=number_samples_pull,
                        timeout=20e-3
                    )
                    if samples is None:
                        continue
                    else:
                        # Heartbeat
                        with self._lock:
                            self._thread_active[stim_idx+1] = True
                        for sample in samples:
                            for ch, idx in enumerate(take_samples):
                                buffer_lsl[ch].append(sample[idx])
                except Exception as e:
                    with self._lock:
                        self._exception.put(e)

        def update_plot_canvas(events):
            nonlocal buffer_gpu

            # Update plot properties
            if not self._event.is_set():
                app.quit()
            if not self._is_active:
                status_text.text = "LSL: DEAD"
                status_text.color = 'red'
            buffer_gpu = buffer_lsl
            for curve, buffer0 in zip(curves, buffer_gpu):
                curve.set_data(buffer0.get_data())

        def update_on_fps(fps):
            # Heartbeat func
            with self._lock:
                self._thread_active[stim_idx] = fps > 0
            fps_text.text = f"FPS: {fps:.1f}"

        # --- Starting the process
        self.register(func=update_plot_data, args=())
        self._thread_active.extend([0])
        self._thread[-1].start()
        canvas.measure_fps(callback=update_on_fps)

        app.Timer(
            interval=1/update_rate,
            connect=update_plot_canvas,
            start=True
        )
        app.run()
