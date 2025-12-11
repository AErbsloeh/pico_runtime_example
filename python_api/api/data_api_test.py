import pytest
from pathlib import Path
from .data_api import DataAPI, RawRecording


@pytest.fixture(scope="session", autouse=True)
def path():
    path = Path("../test")
    yield path


def test_loader_data(path: Path):
    dut = DataAPI(path)
    data = dut.read_file(0)
    file = dut.get_file_name(0)
    assert 'data' in file
    assert type(data) == RawRecording


def test_loader_util(path: Path):
    dut = DataAPI(path)
    data = dut.read_file(1)
    file = dut.get_file_name(1)
    assert 'util' in file
    assert type(data) == RawRecording


if __name__ == "__main__":
    pytest.main([__file__])
