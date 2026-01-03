from api import DeviceAPI


if __name__ == '__main__':
    DeviceAPI().do_reset()
    dut = DeviceAPI()
    dut.update_daq_sampling_rate(750.)

    print("START")
    dut.start_daq(
        do_plot=True,
        window_sec=40.,
        track_util=True
    )
    dut.wait_daq(6*60*60)
    dut.stop_daq()
    print("STOP")
