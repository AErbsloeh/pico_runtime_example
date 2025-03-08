import numpy as np
from scipy.signal import correlate, find_peaks


def calculate_error_mbe(y_pred: np.ndarray | float, y_true: np.ndarray | float, do_print=False) -> float:
    """Calculating the distance-based metric with mean bias error
    Args:
        y_pred:     Numpy array or float value from prediction
        y_true:     Numpy array or float value from true label
        do_print:   Printing the value into terminal
    Returns:
        Float value with error
    """
    if isinstance(y_true, np.ndarray):
        error = float(np.sum(y_pred - y_true) / y_pred.size)
    else:
        error = y_pred - y_true
    if do_print:
        print(f"Mean bias error (MBE) = {error:.4f}")
    return error


def calculate_error_mae(y_pred: np.ndarray | float, y_true: np.ndarray | float, do_print=False) -> float:
    """Calculating the distance-based metric with mean absolute error
    Args:
        y_pred:     Numpy array or float value from prediction
        y_true:     Numpy array or float value from true label
        do_print:   Printing the value into terminal
    Returns:
        Float value with error
    """
    if isinstance(y_true, np.ndarray):
        error = float(np.sum(np.abs(y_pred - y_true)) / y_pred.size)
    else:
        error = float(np.abs(y_pred - y_true))
    if do_print:
        print(f"Mean absolute error (MAE) = {error:.4f}")
    return error


def calculate_error_mse(y_pred: np.ndarray | float, y_true: np.ndarray | float, do_print=False) -> float:
    """Calculating the distance-based metric with mean squared error
    Args:
        y_pred:     Numpy array or float value from prediction
        y_true:     Numpy array or float value from true label
        do_print:   Printing the value into terminal
    Returns:
        Float value with error
    """
    if isinstance(y_true, np.ndarray):
        error = float(np.sum((y_pred - y_true) ** 2) / y_pred.size)
    else:
        error = float(y_pred - y_true) ** 2
    if do_print:
        print(f"Mean squared error (MSE) = {error:.4f}")
    return error


def calculate_error_mpe(y_pred: np.ndarray | float, y_true: np.ndarray | float, do_print=False) -> float:
    """Calculating the distance-based metric with mean percentage error
    Args:
        y_pred:     Numpy array or float value from prediction
        y_true:     Numpy array or float value from true label
        do_print:   Printing the value into terminal
    Returns:
        Float value with error
    """
    if isinstance(y_true, np.ndarray):
        error = float(np.sum((y_true - y_pred) / y_true) / y_true.size)
    else:
        error = float((y_true - y_pred) / y_true)
    if do_print:
        print(f"Mean percentage error (MPE) = {error:.4f}")
    return error


def calculate_error_mape(y_pred: np.ndarray | float, y_true: np.ndarray | float, do_print=False) -> float:
    """Calculating the distance-based metric with mean absolute percentage error
    Args:
        y_pred:     Numpy array or float value from prediction
        y_true:     Numpy array or float value from true label
        do_print:   Printing the value into terminal
    Returns:
        Float value with error
    """
    if isinstance(y_true, np.ndarray):
        error = float(np.sum(np.abs(y_true - y_pred) / np.abs(y_true)) / y_true.size)
    else:
        error = float(abs(y_true - y_pred) / abs(y_true))
    if do_print:
        print(f"Mean absolute percentage error (MAPE) = {error:.4f}")
    return error


def calculate_error_rae(y_pred: np.ndarray | float, y_true: np.ndarray | float, do_print=False) -> float:
    """Calculating the distance-based metric with relative absolute error
    Args:
        y_pred:     Numpy array or float value from prediction
        y_true:     Numpy array or float value from true label
        do_print:   Printing the value into terminal
    Returns:
        Float value with error
    """
    mse = calculate_error_mse(y_pred, y_true)
    if isinstance(y_pred, np.ndarray):
        error = float(np.sqrt(mse) / calculate_error_mae(np.zeros(shape=y_pred.shape), y_true))
    else:
        error = float((mse ** 0.5) / calculate_error_mae(0.0, y_pred))
    if do_print:
        print(f"Relative absolute error (RAE) = {error:.4f}")
    return error


def calculate_error_rse(y_pred: np.ndarray | float, y_true: np.ndarray | float, do_print=False) -> float:
    """Calculating the distance-based metric with relative squared error
    Args:
        y_pred:     Numpy array or float value from prediction
        y_true:     Numpy array or float value from true label
        do_print:   Printing the value into terminal
    Returns:
        Float value with error
    """
    mse = calculate_error_mse(y_pred, y_true)
    y_true_mean = np.mean(y_true)
    if isinstance(y_pred, np.ndarray):
        error = float(mse / np.sum((y_pred - y_true_mean) ** 2) / y_pred.size)
    else:
        error = float(mse / (y_pred - y_true_mean) ** 2)
    if do_print:
        print(f"Relative squared error (RSE) = {error:.4f}")
    return error


def calculate_error_rmse(y_pred: np.ndarray, y_true: np.ndarray, do_print=False) -> float:
    """Calculating the Root Mean Squared Error
    Args:
        y_pred:     Numpy array or float value from prediction
        y_true:     Numpy array or float value from true label
        do_print:   Printing the value into terminal
    Returns:
        Float value with error
    """
    error = np.sqrt(np.sum(np.abs(y_pred - y_true) ** 2) / y_pred.size)
    if do_print:
        print(f"Root mean squared error (MSE) = {error:.4f}")
    return error


def calculate_error_rrmse(y_pred: np.ndarray, y_true: np.ndarray, do_print=False) -> float:
    """Calculating the Relative Root Mean Squared Error
    Args:
        y_pred:     Numpy array or float value from prediction
        y_true:     Numpy array or float value from true label
        do_print:   Printing the value into terminal
    Returns:
        Float value with error
    """
    val0 = np.sum(np.abs(y_true - y_pred) ** 2) / y_pred.size
    val1 = np.sum(np.abs(y_pred) ** 2)
    error = np.sqrt(val0/val1)
    if do_print:
        print(f"Relative Root Mean squared error (RRMSE) = {error:.4f}")
    return error


def calculate_error_rmsre(y_pred: np.ndarray, y_true: np.ndarray, do_print=False) -> float:
    """Calculating the Root Mean Squared Relative ErrorArgs:
        y_pred:     Numpy array or float value from prediction
        y_true:     Numpy array or float value from true label
        do_print:   Printing the value into terminal
    Returns:
        Float value with error
    """
    val0 = np.sum((y_true - y_pred / y_pred) ** 2)
    error = np.sqrt(val0 / y_pred.size)
    if do_print:
        print(f"Root Mean Squared Relative Error (RMSRE) = {error:.4f}")
    return error


def calculate_total_harmonics_distortion(freq: np.ndarray, spectral: np.ndarray,
                                         fsine: float, N_harmonics=4) -> [float, np.ndarray]:
    """Calculating the Total Harmonics Distortion (THD) of spectral input
    Args:
        freq:           Array with frequency values for spectral analysis
        spectral:       Array with Spectral input
        fsine:          Frequency of sinusoidal input from target
        N_harmonics:    Number of used harmonics for calculating THD
    Return:
          THD value (in dB) and corresponding frequency positions of peaks
    """
    # --- Limiting the search space
    pos_x0 = np.argwhere(freq >= 0.5 * fsine).flatten()[0]
    pos_x1 = np.argwhere(freq >= (N_harmonics + 0.5) * fsine).flatten()[0]
    search_y = spectral[pos_x0:pos_x1]
    search_x = freq[pos_x0:pos_x1]

    # --- Getting peaks values
    df = np.mean(np.diff(freq))
    xpos, _ = find_peaks(search_y, distance=int(0.8 * fsine / df))
    peaks_y = search_y[xpos]

    # --- Getting THD and return
    total_harmonics = 20 * np.log10(np.sqrt(np.sum(np.power(peaks_y[1:], 2))) / peaks_y[0])
    return total_harmonics, search_x[xpos]


def calculate_cosine_similarity(y_pred: np.ndarray, y_true: np.ndarray, do_print=False) -> float:
    """Calculating the Cosine Similarity of two different inputs (same size)
    Args:
        y_pred:     Numpy array or float value from prediction
        y_true:     Numpy array or float value from true label
        do_print:   Printing the value into terminal
    Returns:
        Float value with error
    """
    out = correlate(
        in1=y_pred / np.linalg.norm(y_pred),
        in2=y_true / np.linalg.norm(y_true),
        mode='full',
        method='auto'
    )
    cor_value = float(out[y_true.size])
    if do_print:
        print(f"\t Similarity coefficient = {100 * cor_value:.2f} %")
    return cor_value