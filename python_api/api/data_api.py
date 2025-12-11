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
    _prefix_data: str
    _prefix_util: str

    def __init__(self, path2data: Path | str, data_prefix: str="data", util_prefix: str="util") -> None:
        """Class for loading and processing the measured DAQ data
        :param path2data:   Path or string with path to the folder in which data is saved
        :return:            None
        """
        path = Path(path2data) if type(path2data) == str else path2data
        self._prefix_data = data_prefix
        self._prefix_util = util_prefix
        self._overview = [file.absolute() for file in path.glob("*.h5")]

    def get_overview_data(self) -> list[Path]:
        """Returning a list with data files in the folder"""
        return [file for file in self._overview if self._prefix_data in str(file)]

    def get_overview_util(self) -> list[Path]:
        """Returning a list with util files in the folder"""
        return [file for file in self._overview if self._prefix_util in str(file)]

    def get_file_name_data(self, file_number: int) -> str:
        """Returning the data file name of the corresponding use case"""
        return str(self.get_overview_data()[file_number])

    def get_file_name_util(self, file_number: int) -> str:
        """Returning the optional utilization file name of the corresponding use case"""
        return str(self.get_overview_util()[file_number])

    @staticmethod
    def _read_file(path2file: str) -> RawRecording:
        """Loading h5-file for further processing
        :param path2file:   Path with path to file
        :return:            Class RawRecording with measured meta information, timestamps and raw data
        """
        with h5py.File(path2file, "r") as f:
            print("Datasets in file:", list(f.keys()))
            print("Meta info:", list(f.attrs.keys()))

            data = RawRecording(
                sampling_rate=f.attrs["sampling_rate"],
                num_channels=f.attrs["channel_count"],
                time=np.array(f["time"][:] - f["time"][0]),
                data=np.transpose(f["data"][:]),
                type=f.attrs["type"],
                file=path2file
            )
            f.close()
        return data

    def read_data_file(self, file_number: int) -> RawRecording:
        """Reading data file
        :param file_number:     Integer with file number
        :return:                Class RawRecording with measured meta information, timestamps and raw data
        """
        return self._read_file(self.get_file_name_data(file_number))

    def read_utilization_file(self, file_number: int) -> RawRecording:
        """Reading utilization file
        :param file_number:     Integer with file number
        :return:                Class RawRecording with measured meta information, timestamps and raw data
        """
        return self._read_file(self.get_file_name_util(file_number))
