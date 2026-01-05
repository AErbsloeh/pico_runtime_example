from api import DeviceAPI


if __name__ == '__main__':
    DeviceAPI().do_reset()
    dut = DeviceAPI()
    dut.update_daq_sampling_rate(2000.)

    dut.start_daq(
        do_plot=True,
        window_sec=20.,
        track_util=True
    )
    dut.wait_daq(6*60*60)
    #dut.wait_daq(30.)
    dut.stop_daq()
