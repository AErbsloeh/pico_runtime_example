import h5py
import numpy as np
from dataclasses import dataclass
from pathlib import Path


@dataclass
class RawRecording:
    sampling_rate: float
    num_channels: int
    time: np.ndarray
    data: np.ndarray
    type: str


class DataAPI:
    _overview: list

    def __init__(self, path2data: Path | str) -> None:
        """"""
        path = Path(path2data) if type(path2data) == str else path2data
        self._overview = [file for file in path.glob("*.h5")]


    def read_file(self, file_number: int=0) -> RawRecording:
        """"""
        with h5py.File(self._overview[file_number], "r") as f:
            print("Datasets in file:", list(f.keys()))
            print("Meta info:", list(f.attrs.keys()))

            data = RawRecording(
                sampling_rate=f.attrs["sampling_rate"],
                num_channels=f.attrs["channel_count"],
                time=f["time"][:],
                data=f["data"][:],
                type=f.attrs["type"],
            )
            f.close()
        return data
