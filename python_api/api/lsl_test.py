import pytest
import random
from multiprocessing import Process
from time import sleep
from pathlib import Path
from shutil import rmtree
from .lsl import start_stream_data, start_stream_utilization, record_stream
from api import get_path_to_project


def dummy_data_lsl(channel_num: int=8) -> list:
    return [int(2**16 * random.random()) for _ in range(channel_num)]


@pytest.fixture(scope="session", autouse=True)
def path():
    path = Path(get_path_to_project("test_data"))
    if path.exists():
        rmtree(path)
    path.mkdir(parents=True, exist_ok=True)
    yield path


def test_lsl_data(path: Path) -> None:
    process = [Process(target=start_stream_data, args=("data", 8, dummy_data_lsl))]
    process.append(Process(target=record_stream, args=("data", path)))

    for p in process:
        p.start()
    sleep(10.)
    for p in process:
        p.terminate()

    data_overview = [str(file) for file in path.glob("*.h5") if 'data' in str(file)]
    assert len(data_overview) > 0


def test_lsl_util(path: Path) -> None:
    process = [Process(target=start_stream_utilization, args=("util", 2.))]
    process.append(Process(target=record_stream, args=("util", path)))

    for p in process:
        p.start()
    sleep(10.)
    for p in process:
        p.terminate()

    data_overview = [str(file) for file in path.glob("*.h5") if 'util' in str(file)]
    assert len(data_overview) > 0


if __name__ == "__main__":
    pytest.main([__file__])
