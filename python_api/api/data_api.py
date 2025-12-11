import h5py
import numpy as np
from dataclasses import dataclass
from pathlib import Path


@dataclass(frozen=True)
class RawRecording:
    """Data class with measured transient data
    Attributes:
        sampling_rate:  Float with sampling rate [Hz]
        num_channels:   Integer with number of channels
        time:           Numpy array with timestamps [sec]
        data:           Numpy array with raw data
        file:           String with path to file
    """
    sampling_rate: float
    num_channels: int
    time: np.ndarray
    data: np.ndarray
    type: str
    file: str


class DataAPI:
    _overview: list[Path]

    def __init__(self, path2data: Path | str) -> None:
        """Class for loading and processing the measured DAQ data
        :param path2data:   Path or string with path to the folder in which data is saved
        :return:            None
        """
        path = Path(path2data) if type(path2data) == str else path2data
        self._overview = [file.absolute() for file in path.glob("*.h5")]

    def get_file_name(self, file_number: int) -> str:
        """Returning the file name of the corresponding file_number"""
        return str(self._overview[file_number])

    def read_file(self, file_number: int) -> RawRecording:
        """Loading h5-file for further processing
        :param file_number:   Integer with file number
        :return:              Class RawRecording with measured meta information, timestamps and raw data
        """
        with h5py.File(self._overview[file_number], "r") as f:
            print("Datasets in file:", list(f.keys()))
            print("Meta info:", list(f.attrs.keys()))

            data = RawRecording(
                sampling_rate=f.attrs["sampling_rate"],
                num_channels=f.attrs["channel_count"],
                time=np.array(f["time"][:] - f["time"][0]),
                data=np.transpose(f["data"][:]),
                type=f.attrs["type"],
                file=self.get_file_name(file_number)
            )
            f.close()
        return data
