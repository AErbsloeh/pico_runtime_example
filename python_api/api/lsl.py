import numpy as np
from h5py import File
from collections import deque
from datetime import datetime
from pathlib import Path
from time import sleep
from threading import Event
from psutil import (
    cpu_percent,
    virtual_memory
)
from pylsl import (
    StreamInfo,
    StreamInlet,
    StreamOutlet,
    resolve_bypred,
    FOREVER,
    cf_int32,
    cf_float32
)
from vispy import (
    app,
    scene
)


def start_stream_data(name: str, channel_num: int, data_daq_func, event_func: Event, sampling_rate: float) -> None:
    """Process for starting a Lab Streaming Layer (LSL) to process the data stream from DAQ system
    :param name:            String with name of the LSL stream (must match with recording process)
    :param channel_num:     Channel number to start stream from
    :param data_daq_func:   Function to get data from DAQ device (returned list)
    :param event_func:      Threading Event manager to control while loop
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

    while event_func.is_set():
        data = data_daq_func()
        if not len(data):
            continue
        outlet.push_sample(data)

def start_stream_utilization(name: str, event_func: Event, sampling_rate: float=2.) -> None:
    """Process for starting a Lab Streaming Layer (LSL) to process the utilization of the host computer
    :param name:            String with name of the LSL stream (must match with recording process)
    :param event_func:      Threading Event manager to control while loop
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
    while event_func.is_set():
        outlet.push_sample([cpu_percent(interval=1 / sampling_rate), virtual_memory().percent])
        sleep(1 / sampling_rate)


def record_stream(name: str, path2save: Path | str, event_func: Event, take_sample: list=(), subtract_channel: int=0) -> None:
    """Function for recording and saving the data pushed on LSL stream
    :param name:                String with name of the LSL stream in order to catch it
    :param path2save:           Path to save the data (if it is a string, it will be auto-converted)
    :param event_func:          Threading Event manager to control while loop
    :param take_sample:         List with indexes to take from LSL pull list for further processing
    :param subtract_channel:    Integer with number of channels to subtract from data stream
    :return: None
    """
    mode_util = 'util' in name
    path = Path(path2save) if type(path2save) == str else path2save
    inlet = StreamInlet(resolve_bypred(f"name='{name}'")[0])

    # Extract meta
    channels = inlet.info().channel_count() - (subtract_channel if not mode_util else 0)
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

        idx = 0
        process_list = take_sample if len(take_sample) > 0 else [i for i in range(channels)]
        while event_func.is_set():
            samples, ts = inlet.pull_chunk(
                max_samples=int(sampling_rate),
                timeout=0.0
            )
            if samples is None:
                continue
            else:
                ts_dset.resize((idx+len(ts),))
                data_dset.resize((idx + len(ts), channels))
                for id0, (sample, tb) in enumerate(zip(samples, ts)):
                    ts_dset[idx+id0] = tb if mode_util else 1e-6 * sample[0]
                    data_dset[idx+id0] = [sample[i] for i in process_list]
                idx += len(ts)
                f.flush()


def start_live_plotting(name: str, event_func: Event, take_samples: list=(), window_length: float=10., subtract_channel:int=0, update_rate: float=5.) -> None:
    """Function for LSL to enable live plotting of the incoming results
    :param name:            String with name of the LSL stream to get data
    :param event_func:      Event manager from Threading package for controlling while loop
    :param take_samples:    List with taking idx from LSL pulling event
    :param window_length:   Floating value with length of time window for plotting in seconds
    :param subtract_channel:Integer with number of channels to subtract from data stream
    :param update_rate:     Floating value with update rate of the plot
    :return:                None
    """
    line_color = ['red', 'green', 'blue', 'lime']
    mode_util = 'util' in name
    inlet = StreamInlet(resolve_bypred(f"name='{name}'")[0])
    # --- Extract meta
    channels = inlet.info().channel_count() - (subtract_channel if not mode_util else 0)
    sampling_rate = inlet.info().nominal_srate()
    if sampling_rate > 2000.:
        raise AttributeError(f"Sampling rate {sampling_rate} is too high")

    # --- Build app
    canvas = scene.SceneCanvas(
        size=(800, 450),
        title=f"Live Plot @{sampling_rate} Hz ({name})",
        keys="interactive",
        app="glfw",
        show=True,
    )
    view = canvas.central_widget.add_view()
    view.camera = "panzoom"
    curves = list()
    for idx in range(channels):
        curves.append(scene.Line(
            width=2,
            color=line_color[idx % len(line_color)],
            parent=view.scene
        ))
    del idx
    # --- Build ring buffer and update func
    number_samples_pull = int(sampling_rate / update_rate)
    number_samples_window = int(window_length * sampling_rate)
    buffer_time = deque(maxlen=number_samples_window)
    buffer_sigs = [deque(maxlen=number_samples_window) for _ in range(channels)]

    def update_util(event):
        # Getting data
        samples, tb = inlet.pull_sample(timeout=0.0)
        if samples is None:
            return
        buffer_time.extend([tb])
        for idx0 in range(channels):
            buffer_sigs[idx0].extend([samples[idx0]])
        if not event_func.is_set():
            canvas.close()
            app.quit()
        if len(buffer_time) < 2:
            return
        # Process for data for plot
        x = np.asarray(buffer_time, dtype=np.float32)
        for idx1, curve in enumerate(curves):
            y = np.asarray(buffer_sigs[idx1], dtype=np.float32)
            curve.set_data(np.column_stack((x, y)))
        view.camera.set_range(
            x=(x.min(), x.max()),
            y=(0, 100)
        )
    def update_data(event):
        # Getting data
        samples = inlet.pull_chunk(
            max_samples=number_samples_pull,
            timeout=0.0
        )[0]
        if samples is None:
            return
        for sample in samples:
            buffer_time.extend([1e-6*sample[0]])
            for idx0 in range(channels):
                buffer_sigs[idx0].extend([sample[take_samples[idx0]]])
        if not event_func.is_set():
            canvas.close()
            app.quit()
        if len(buffer_time) < 2:
            return
        # Process for data for plot
        x = np.asarray(buffer_time, dtype=np.float32)
        for idx1, curve in enumerate(curves):
            y = np.asarray(buffer_sigs[idx1], dtype=np.float32)
            curve.set_data(np.column_stack((x, y)))
        # TODO: Add dynamic y-axis scaling
        view.camera.set_range(
            x=(x.min(), x.max()),
            y=(0, 65536)
        )
    # --- Starting the process
    app.Timer(
        interval='auto',
        connect=update_util if mode_util else update_data,
        start=True
    )
    app.run()
