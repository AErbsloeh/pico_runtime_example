import pytest
import random
from threading import Thread, Event
from time import sleep
from pathlib import Path
from shutil import rmtree

from api import get_path_to_project
from api.lsl import (
    start_stream_data,
    start_stream_utilization,
    record_stream,
    start_live_plotting
)


@pytest.fixture(scope="session", autouse=True)
def before_all_tests():
    path = Path(get_path_to_project("test_data"))
    if path.exists():
        rmtree(path)
    path.mkdir(parents=True, exist_ok=True)


@pytest.fixture
def path():
    path = Path(get_path_to_project("test_data"))
    yield path


def test_lsl_data(path: Path) -> None:
    lsl_events = Event()
    def dummy_data_lsl(channel_num: int = 8) -> list:
        return [int(2 ** 16 * random.random()) for _ in range(channel_num)]

    process = [
        Thread(target=start_stream_data, args=("data", 8, dummy_data_lsl, lsl_events)),
        Thread(target=record_stream, args=("data", path, lsl_events))
    ]
    lsl_events.set()
    for p in process:
        p.start()

    sleep(10.)
    lsl_events.clear()
    for p in process:
        p.join(timeout=1.)

    data_overview = [str(file) for file in path.glob("*.h5") if 'data' in str(file)]
    assert len(data_overview) > 0


def test_lsl_util(path: Path) -> None:
    lsl_events = Event()
    process = [
        Thread(target=start_stream_utilization, args=("util", lsl_events, 2.)),
        Thread(target=record_stream, args=("util", path, lsl_events)),
        Thread(target=start_live_plotting, args=("util", lsl_events, ())),
    ]

    lsl_events.set()
    for p in process:
        p.start()

    sleep(10.)
    lsl_events.clear()
    for p in process:
        p.join(timeout=1.)

    data_overview = [str(file) for file in path.glob("*.h5") if 'util' in str(file)]
    assert len(data_overview) > 0


if __name__ == "__main__":
    pytest.main([__file__])
