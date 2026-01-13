import pytest
from pathlib import Path
from .data_api import DataAPI, RawRecording


@pytest.fixture(scope="session", autouse=True)
def path():
    path = Path("../test_data")
    yield path


def test_loader_data(path: Path):
    dut = DataAPI(path)
    data = dut.read_data_file(0)
    file = dut.get_file_name_data(0)
    assert 'data' in file
    assert type(data) == RawRecording

    assert data.sampling_rate == 2000.
    assert data.num_channels == 3
    assert data.num_channels == data.data.shape[0]
    assert data.time.size == data.data.shape[1]


def test_loader_util(path: Path):
    dut = DataAPI(path)
    data = dut.read_utilization_file(0)
    file = dut.get_file_name_util(0)
    assert 'util' in file
    assert type(data) == RawRecording

    print(data)
    assert data.sampling_rate == 2.
    assert data.num_channels == 2
    assert data.num_channels == data.data.shape[0]
    assert data.time.size == data.data.shape[1]


if __name__ == "__main__":
    pytest.main([__file__])
