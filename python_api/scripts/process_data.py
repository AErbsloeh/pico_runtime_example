import numpy as np
import matplotlib.pyplot as plt
from api import DataAPI, RawRecording


def plot_histogram_time(packet: RawRecording, show_plot: bool=True) -> None:
    plt.figure()
    plt.hist(np.diff(packet.time), bins=101, density=True, cumulative=False, histtype="stepfilled", color='k')
    plt.xlabel('Sampling Period (s)')
    plt.ylabel('Bins')
    plt.grid(True)
    plt.tight_layout()

    if show_plot:
        plt.show()


def plot_transient_data(packet: RawRecording, show_plot: bool=True) -> None:
    min_size = np.min([packet.time.size, packet.data.shape[0]])

    plt.figure()
    plt.plot(packet.time[:min_size], packet.data[:min_size,:])
    plt.xlabel('Time (s)')
    plt.ylabel('Amplitude')
    plt.xlim(packet.time[0], packet.time[-1])
    plt.grid(True)
    plt.tight_layout()

    if show_plot:
        plt.show()


def plot_transient_util(packet: RawRecording, show_plot: bool=True) -> None:
    min_size = np.min([packet.time.size, packet.data.shape[0]])

    plt.figure()
    plt.plot(packet.time[:min_size], packet.data[:min_size, 0], color='k', label='CPU')
    plt.plot(packet.time[:min_size], packet.data[:min_size, 1], color='r', label='RAM')
    plt.xlabel('Time (s)')
    plt.ylabel('Utilization (%)')
    plt.legend(loc='best')
    plt.xlim(packet.time[0], packet.time[-1])
    plt.grid(True)
    plt.tight_layout()

    if show_plot:
        plt.show()


if __name__ == "__main__":
    path2data = "../../data"
    data = DataAPI(path2data).read_file(0)
    util = DataAPI(path2data).read_file(1)

    plot_transient_data(data, False)
    plot_transient_util(util, False)
    plot_histogram_time(data, True)
