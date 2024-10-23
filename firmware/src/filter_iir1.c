// --- Generating an IIR filter template (Direct Form II)
// Copyright @ UDE-IES
// Code generated on: 10/22/2024, 12:38:53
// Params: N = 2, f_c = [100, 200] Hz @ 1000.0 Hz (all, butter)
// Used filter coefficient order (a_0, a_1, ... a_N, b_0, b_1, ..., b_N)
# include "src/filter_iir_template.h"
DEF_NEW_IIR_FILTER_IMPL(1, int16_t, 3, 2, 1.0, -0.937152698242062, 0.15838444032453636, 0.15838444032453636, -0.937152698242062, 1.0)
