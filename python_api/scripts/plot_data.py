import numpy as np
import matplotlib.pyplot as plt
from pathlib import Path
from api import DataAPI, RawRecording, get_path_to_project


def plot_histogram_time(packet: RawRecording, show_density: bool=False, show_plot: bool=True) -> None:
    plt.figure()
    plt.hist(np.diff(packet.time), density=show_density, cumulative=show_density, histtype="stepfilled", color='k')
    plt.xlabel('Sampling Period (s)')
    plt.ylabel('Bins')
    plt.grid(True)
    plt.tight_layout()
    if show_plot:
        plt.show()


def plot_transient_data(packet: RawRecording, show_plot: bool=True) -> None:
    min_size = np.min([packet.time.size, packet.data.shape[1]])

    plt.figure()
    plt.plot(packet.time[:min_size], packet.data[:,:min_size].T)
    plt.xlabel('Time (s)')
    plt.ylabel('Amplitude')
    plt.xlim(packet.time[0], packet.time[-1])
    plt.grid(True)
    plt.tight_layout()
    if show_plot:
        plt.show()


def plot_transient_util(packet: RawRecording, show_plot: bool=True) -> None:
    min_size = np.min([packet.time.size, packet.data.shape[1]])

    plt.figure()
    plt.plot(packet.time[:min_size], packet.data[0,:min_size], marker='.', color='k', label='CPU')
    plt.plot(packet.time[:min_size], packet.data[1,:min_size], marker='.', color='r', label='RAM')
    plt.xlabel('Time (s)')
    plt.ylabel('Utilization (%)')
    plt.legend(loc='best')
    plt.xlim(packet.time[0], packet.time[-1])
    plt.grid(True)
    plt.tight_layout()
    if show_plot:
        plt.show()


if __name__ == "__main__":
    path2data = Path(get_path_to_project()) / "data"
    use_case = 0

    data = DataAPI(path2data).read_data_file(use_case)
    util = DataAPI(path2data).read_utilization_file(use_case)

    plot_transient_data(data, show_plot=False)
    plot_transient_util(util, show_plot=False)
    plot_histogram_time(data, show_plot=True)
