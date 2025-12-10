from api import DeviceAPI


if __name__ == '__main__':
    DeviceAPI().do_reset()

    dut = DeviceAPI()
    dut.update_daq_sampling_rate(500.)
    dut.start_daq()
