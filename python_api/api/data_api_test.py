import pytest
from pathlib import Path
from .data_api import DataAPI, RawRecording


@pytest.fixture(scope="session", autouse=True)
def path():
    path = Path("../test_data")
    yield path


def test_loader_data(path: Path):
    dut = DataAPI(path, data_prefix='mock')
    data = dut.read_data_file(0)
    file = dut.get_file_name_data(0)
    assert 'mock' in file
    assert type(data) == RawRecording


def test_loader_util(path: Path):
    dut = DataAPI(path)
    data = dut.read_utilization_file(0)
    file = dut.get_file_name_util(0)
    assert 'util' in file
    assert type(data) == RawRecording


if __name__ == "__main__":
    pytest.main([__file__])
