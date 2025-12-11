from api import DeviceAPI
from time import sleep


if __name__ == '__main__':
    DeviceAPI().do_reset()

    dut = DeviceAPI()
    dut.update_daq_sampling_rate(100.)

    print("START")
    dut.start_daq(track_util=True)
    sleep(1000.)

    print("STOP")
    dut.stop_daq()
