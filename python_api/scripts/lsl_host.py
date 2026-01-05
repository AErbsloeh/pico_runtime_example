from pathlib import Path
from api import get_path_to_project
from api.lsl import ThreadLSL


if __name__ == "__main__":
    path2save = Path(get_path_to_project("data"))

    dut = ThreadLSL()
    channel_num = 8
    sample_rate = 2000
    path2data = get_path_to_project("test_data")

    dut.register(func=dut.lsl_stream_util, args=(0, 'util', 2.))
    dut.register(func=dut.lsl_record_stream, args=(1, 'util', path2save))
    dut.register(func=dut.lsl_stream_mock, args=(2, 'mock', channel_num, sample_rate))
    #dut.register(func=dut._thread_stream_file, args=(2, 'mock', path2data, 0, 'mock'))
    dut.register(func=dut.lsl_record_stream, args=(3, 'mock', path2save))
    dut.register(func=dut.lsl_plot_stream, args=(4, 'mock', 0.5))

    # --- Start and stop processes
    dut.start()
    dut.wait_for_seconds(8*60*60.)
    #dut.wait_for_seconds(60.)
    dut.stop()
