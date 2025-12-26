from api import DeviceAPI
from time import sleep


if __name__ == '__main__':
    DeviceAPI().do_reset()

    dut = DeviceAPI()
    dut.update_daq_sampling_rate(50.)

    print("START")
    dut.start_daq(
        do_plot=True,
        window_sec=20.,
        track_util=True
    )
    sleep(1200.)

    print("STOP")
    dut.stop_daq()
