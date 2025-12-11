from h5py import File
from datetime import datetime
from pathlib import Path
from time import sleep
from psutil import cpu_percent, virtual_memory
from pylsl import StreamInfo, StreamInlet, StreamOutlet, resolve_bypred, FOREVER, cf_int32, cf_float32


def start_stream_data(name: str, channel_num: int, data_daq_func) -> None:
    """Process for starting a Lab Streaming Layer (LSL) to process the data stream from DAQ system
    :param name:            String with name of the LSL stream (must match with recording process)
    :param channel_num:     Channel number to start stream from
    :param data_daq_func:   Function to get data from DAQ device (returned list)
    :return:                None
    """
    config = StreamInfo(
        name=name,
        type='custom_daq',
        channel_count=channel_num,
        nominal_srate=1.,
        channel_format=cf_int32,
        source_id=name + '_uid'
    )
    outlet = StreamOutlet(config)

    while True:
        data = data_daq_func()
        if not len(data):
            continue
        outlet.push_sample(data)


def start_stream_utilization(name: str, sampling_rate: float=2.) -> None:
    """Process for starting a Lab Streaming Layer (LSL) to process the utilization of the host computer
    :param name:             String with name of the LSL stream (must match with recording process)
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
    while True:
        outlet.push_sample([cpu_percent(interval=1 / sampling_rate), virtual_memory().percent])
        sleep(1 / sampling_rate)


def record_stream(name: str, path2save: Path | str, take_sample: list=(), subtract_channel: int=0) -> None:
    """Function for recording and saving the data pushed on LSL stream
    :param name:                String with name of the LSL stream in order to catch it
    :param path2save:           Path to save the data (if string, it will be auto-converted)
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

        process_list = take_sample if len(take_sample) > 0 else [i for i in range(channels)]

        idx = 0
        while True:
            # Processing
            sample, ts = inlet.pull_sample(timeout=FOREVER)
            if sample is None:
                continue

            # Write into file
            ts_dset.resize((idx + 1,))
            ts_dset[idx] = ts if mode_util else 1e-6 * sample[0]
            data_dset.resize((idx + 1, channels))
            data_dset[idx] = [sample[i] for i in process_list]
            idx += 1
            f.flush()
