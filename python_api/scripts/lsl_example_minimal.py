from datetime import date
from h5py import File
from time import sleep
import math
from multiprocessing import Process
from pathlib import Path
import psutil
from pylsl import StreamInfo, StreamOutlet, StreamInlet, resolve_bypred, cf_int16, cf_float32, FOREVER


def start_stream_data(name: str, channel_num: int, sampling_rate: float) -> None:
    info = StreamInfo(
        name=name,
        type='custom_daq',
        channel_count=channel_num,
        nominal_srate=sampling_rate,
        channel_format=cf_int16,
        source_id=name + '_uid'
    )
    outlet = StreamOutlet(info)

    idx = 0
    while True:
        idx = idx + math.pi / 20
        outlet.push_sample([int((2**15-1) * math.sin(idx + math.pi * ch / channel_num)) for ch in range(channel_num)])
        sleep(1/sampling_rate)


def start_stream_utilization(name: str, sampling_rate: float) -> None:
    info = StreamInfo(
        name=name,
        type='utilization',
        channel_count=2,
        nominal_srate=sampling_rate,
        channel_format=cf_float32,
        source_id=name + '_uid'
    )
    outlet = StreamOutlet(info)

    idx = 0
    while True:
        idx = idx + math.pi / 20
        outlet.push_sample([psutil.cpu_percent(interval=1/sampling_rate), psutil.virtual_memory().percent])
        sleep(1 / sampling_rate)


def record_stream(name: str, path2save: Path) -> None:
    inlet = StreamInlet(resolve_bypred(f"name='{name}'")[0])

    # Extract meta
    channels = inlet.info().channel_count()
    sampling_rate = inlet.info().nominal_srate()
    type = inlet.info().type()
    data_format = inlet.info().channel_format()
    time = date.today().strftime('%Y%m%d_%H%M')

    if not path2save.is_dir():
        path2save.mkdir(parents=True, exist_ok=True)
    with File(path2save.absolute() / f"{time}_{name}.h5", "w") as f:
        f.attrs["sampling_rate"] = sampling_rate
        f.attrs["channel_count"] = channels
        f.attrs["type"] = type
        f.attrs["creation_date"] = date.today().strftime('%Y-%m-%d')
        f.attrs["data_format"] = data_format

        ts_dset = f.create_dataset("time", (0,), maxshape=(None,), dtype="float32")
        ts_dset.attrs["unit"] = "s"
        data_dset = f.create_dataset("data", (0, channels), maxshape=(None, channels), dtype="int16")
        ts_dset.attrs["unit"] = ""

        first_run = True
        timestamp_first = 0.
        idx = 0
        while True:
            sample, ts = inlet.pull_sample(timeout=FOREVER)

            # Processing
            if sample is None:
                continue
            if first_run:
                timestamp_first = ts
                first_run = False

            # Write into file
            ts_dset.resize((idx + 1,))
            ts_dset[idx] = ts - timestamp_first
            data_dset.resize((idx + 1, channels))
            data_dset[idx] = sample

            idx += 1
            f.flush()

if __name__ == "__main__":
    process = list()
    process.append(Process(target=start_stream_data, args=("data", 100, 32)))
    process.append(Process(target=start_stream_utilization, args=("util", 10)))
    process.append(Process(target=record_stream, args=("data", Path("../data"))))
    process.append(Process(target=record_stream, args=("util", Path("../data"))))

    for p in process:
        p.start()

    process[2].join()
    for p in process:
        p.terminate()

