#ifndef WAVEFORM_LUT_TEMPLATE_H
#define WAVEFORM_LUT_TEMPLATE_H
#include <stdint.h>
#include <stdbool.h>


typedef struct {
    uint8_t state;
    uint16_t lut_position;
    uint16_t lut_length;
    uint16_t lut_offset;
    void *lut_data;
} WaveformSettings;


#define DEF_GET_LUT_VALUE_FULL(input_type) \
input_type read_waveform_value_runtime (WaveformSettings *filter, bool skip_last_point) { \
    input_type* lut_values = filter->lut_data; \
    \
    input_type data = filter->lut_offset + lut_values[filter->lut_position]; \
    if((filter->lut_position == filter->lut_length -1) && !skip_last_point){ \
        filter->lut_position = 0; \
        filter->state = 0; \
    } else if((filter->lut_position == filter->lut_length -2) && skip_last_point){ \
        filter->lut_position = 0; \
        filter->state = 0; \
    } else { \
        filter->lut_position++; \
        filter->state = 1; \
    }; \
    return data; \
}


#define DEF_NEW_WAVEFORM_LUT_FULL_IMPL(id, input_type, cnt_type, lut_lgth, offset, ...) \
    static DEF_GET_LUT_VALUE_FULL(input_type) \
    input_type read_waveform_value_runtime_ ## id (bool skip_last_point) { \
        static input_type lut_data_read [] = {__VA_ARGS__}; \
        static WaveformSettings settings = { \
            .state = 0, \
            .lut_position = 0, \
            .lut_length = lut_lgth, \
            .lut_offset = offset, \
            .lut_data = lut_data_read \
        }; \
        return read_waveform_value_runtime(& (settings), skip_last_point); \
    } \
    uint16_t get_waveform_lgth_## id (bool skip_last_point) { \
        return (skip_last_point) ? lut_lgth - 1 : lut_lgth; \
    }


#define DEF_GET_LUT_VALUE_OPT(input_type) \
input_type read_waveform_value_runtime (WaveformSettings *filter, bool skip_last_point) { \
    input_type* lut_values = filter->lut_data; \
    input_type data = 0; \
    \
    if(filter->state == 0){ \
        data = filter->lut_offset + lut_values[filter->lut_position]; \
        filter->lut_position++; \
        if(filter->lut_position == (filter->lut_length - 1)){ \
            filter->state = 1; \
        } else { \
            filter->state = 0; \
        }; \
    } else if(filter->state == 1){ \
        data = filter->lut_offset + lut_values[filter->lut_position]; \
        filter->lut_position--; \
        if(filter->lut_position == 0){ \
            filter->state = 2; \
        } else { \
            filter->state = 1; \
        }; \
    } else if(filter->state == 2){ \
        data = filter->lut_offset - lut_values[filter->lut_position]; \
        filter->lut_position++; \
        if(filter->lut_position == (filter->lut_length - 1)){ \
            filter->state = 3; \
        } else { \
            filter->state = 2; \
        }; \
    } else if(filter->state == 3){ \
        data = filter->lut_offset - lut_values[filter->lut_position]; \
        filter->lut_position--; \
        if(filter->lut_position == 0){ \
            if(skip_last_point){ \
                filter->state = 0; \
            } else { \
                filter->state = 4; \
            } \
        } else { \
            filter->state = 3; \
        }; \
    } else if(filter->state == 4){ \
        data = filter->lut_offset; \
        filter->lut_position = 0; \
        filter->state = 0; \
    } else { \
        data = filter->lut_offset; \
        filter->state = 0; \
        filter->lut_position = 0; \
    }; \
    return data; \
}


#define DEF_NEW_WAVEFORM_LUT_OPT_IMPL(id, input_type, cnt_type, lut_lgth, offset, ...) \
    static DEF_GET_LUT_VALUE_OPT(input_type) \
    input_type read_waveform_value_runtime_ ## id (bool skip_last_point) { \
        static input_type lut_data_read [] = {__VA_ARGS__}; \
        static WaveformSettings settings = { \
            .state = 0, \
            .lut_position = 0, \
            .lut_length = lut_lgth, \
            .lut_offset = offset, \
            .lut_data = lut_data_read \
        }; \
        return read_waveform_value_runtime(& (settings), skip_last_point); \
    } \
    uint16_t get_waveform_lgth_## id (bool skip_last_point) { \
        return (skip_last_point) ? 4* lut_lgth - 2 : 4* lut_lgth - 3; \
    }


#define DEF_NEW_WAVEFORM_LUT_PROTO(id, input_type, cnt_type) \
    input_type read_waveform_value_runtime_ ## id (bool skip_last_point); \
    uint16_t get_waveform_lgth_## id (bool skip_last_point);


#endif
