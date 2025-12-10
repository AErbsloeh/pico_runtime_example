from h5py import File
from datetime import date
from pathlib import Path
from pylsl import StreamInlet, resolve_bypred, FOREVER


def record_stream(name: str, path2save: Path | str, subtract_channel: int=2) -> None:
    """Function for recording and saving the data pushed on LSL stream
    :param name:                String with name of the LSL stream in order to catch it
    :param path2save:           Path to save the data (if string, it will be auto-converted)
    :param subtract_channel:    Integer with number of channels to subtract from data stream
    :return: None
    """
    path = Path(path2save) if type(path2save) == str else path2save
    inlet = StreamInlet(resolve_bypred(f"name='{name}'")[0])

    # Extract meta
    channels = inlet.info().channel_count() - subtract_channel
    sampling_rate = inlet.info().nominal_srate()
    sys_type = inlet.info().type()
    data_format = inlet.info().channel_format()
    time = date.today().strftime('%Y%m%d_%H%M')

    if not path.is_dir():
        path.mkdir(parents=True, exist_ok=True)
    with File(path.absolute() / f"{time}_{name}.h5", "w") as f:
        f.attrs["sampling_rate"] = sampling_rate
        f.attrs["channel_count"] = channels
        f.attrs["type"] = sys_type
        f.attrs["creation_date"] = date.today().strftime('%Y-%m-%d')
        f.attrs["data_format"] = data_format

        ts_dset = f.create_dataset("time", (0,), maxshape=(None,), dtype="float32")
        ts_dset.attrs["unit"] = "s"
        data_dset = f.create_dataset("data", (0, channels), maxshape=(None, channels), dtype="uint16")
        ts_dset.attrs["unit"] = ""

        idx = 0
        while True:
            sample: list = inlet.pull_sample(timeout=FOREVER)[0]
            # Processing
            if sample is None:
                continue
            # Write into file
            ts_dset.resize((idx + 1,))
            ts_dset[idx] = 1e-6 * sample[0]
            data_dset.resize((idx + 1, channels))
            data_dset[idx] = [sample[1], sample[-1]]
            idx += 1
            f.flush()
