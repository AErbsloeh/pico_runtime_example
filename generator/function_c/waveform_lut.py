from os import mkdir
from os.path import join, isdir, abspath
import numpy as np
from datetime import datetime
from shutil import copyfile

from generator.helper.signal_type import generation_sinusoidal_waveform
from generator.helper.translater import get_embedded_datatype, replace_variables_with_parameters


def generate_waveform_lut_c(bitsize_lut: int, f_sys: float, f_rpt: float, f_sine: float,
                            out_signed: bool=False, do_optimized: bool=False, device_id:str = '',
                            file_name: str='waveform_lut', path2save: str='', define_path: str='src') -> None:
    """Generating C file with SINE_LUT for sinusoidal waveform generation
    Args:
        bitsize_lut:    Used quantization level for generating sinusoidal waveform LUT
        f_sys:          System clock used in FPGA design
        f_rpt:          Frequency of the timer interrupt
        f_sine:         Target frequency of the sinusoidal waveform at output
        out_signed:     Decision if LUT values are signed [otherwise unsigned]
        do_optimized:   Decision if LUT resources should be minimized [only quarter and mirroring]
        device_id:      Device ID
        file_name:      File name of generated output
        path2save:      Path for saving the verilog_filter output files
        define_path:    Path for loading the header file in IDE [Default: 'src']
    Return:
        None
    """
    module_id_used = device_id if device_id else '0'

    # --- Step #1: Building template and data
    datatype_data_ext = get_embedded_datatype(bitsize_lut, out_signed)
    bitwidth_mcu = int(datatype_data_ext.split('int')[-1].split('_')[0])

    template = __generate_waveform_lut_template(do_optimized)
    lut_data = generation_sinusoidal_waveform(
        bitsize_lut=bitsize_lut,
        f_rpt=f_rpt,
        f_sine=f_sine,
        bitsize_chck=bitwidth_mcu,
        do_signed=out_signed,
        do_optimized=do_optimized
    )

    # --- Step #2: Generating the values for parameter dict
    params = {
        'datetime_created': datetime.now().strftime("%m/%d/%Y, %H:%M:%S"),
        'path2include':     define_path,
        'template_name':    'waveform_lut_template.h',
        'device_id':        module_id_used.upper(),
        'datatype_cnt':     get_embedded_datatype(np.log2(lut_data.size), out_signed=False),
        'datatype_int':     get_embedded_datatype(bitsize_lut, out_signed=out_signed),
        'num_cntsize':      str(int(f_sys / f_rpt)),
        'num_lutsine':      str(lut_data.size),
        'lut_offset':       str(0 if out_signed else (2 ** (bitwidth_mcu - 1))),
        'lut_data':         ', '.join(map(str, lut_data)),
        'lut_type':         'sinusoidal'
    }

    # --- Step #3: Replace string parameters with real values
    head = replace_variables_with_parameters(template['head'], params)
    func = replace_variables_with_parameters(template['func'], params)

    # --- Write lines to file
    # Checking if path is available
    if path2save and not isdir(path2save):
        mkdir(path2save)

    used_template_name = params["template_name"]
    copyfile(f'./../template_c/{used_template_name}', join(path2save, f'{used_template_name}'))
    print("\nCreating the C files\n====================================")
    with open(join(path2save, f'{file_name}{module_id_used.lower()}.h'), 'w') as v_handler:
        for line in head:
            v_handler.write(line + '\n')
    v_handler.close()

    with open(join(path2save, f'{file_name}{module_id_used.lower()}.c'), 'w') as v_handler:
        for line in func:
            v_handler.write(line + '\n')
    v_handler.close()
    print(f'... created C (*.h + *.c) in: {abspath(path2save)}')


def __generate_waveform_lut_template(do_full_opt: bool) -> dict:
    """Generating the C template for generating the waveforms
    Args:
        do_full_opt:    Get C functions for full LUT representation or reduced LUT with mirror techniques
    Return:
        Dictionary with files
    """
    use_option = 'OPT' if do_full_opt else 'FULL'
    header_temp = [
        f'// --- Generating a Waveform LUT Caller ({use_option})',
        '// Copyright @ UDE-IES',
        '// Code generated on: {$datetime_created}',
        '// Params: ',
        '// \tID = {$device_id},',
        '// \tdatatypes (data, counter),',
        '# include "{$path2include}/{$template_name}"',
        'DEF_NEW_WAVEFORM_LUT_PROTO({$device_id}, {$datatype_int}, {$datatype_cnt})'
    ]
    func_temp = [
        f'// --- Generating a Waveform LUT Caller ({use_option})',
        '// Copyright @ UDE-IES',
        '// Code generated on: {$datetime_created}',
        '// Params: ',
        '// \tID = {$device_id},',
        '// \tdatatypes (data, counter),',
        '// \tN_LUT = {$num_lutsine}',
        '// \tOffset = {$lut_offset}',
        '// \t Used LUT data order (a_0, a_1, ... a_N)',
        '# include "{$path2include}/{$template_name}"',
        'DEF_NEW_WAVEFORM_LUT_' + use_option + '_IMPL({$device_id}, {$datatype_int}, {$datatype_cnt}, {$num_lutsine}, {$lut_offset}, {$lut_data})'
    ]
    return {'head': header_temp, 'func': func_temp, 'params': []}


if __name__ == '__main__':
    path2save = '../../build'

    generate_waveform_lut_c(14, 32e6, 250e3, 10e3, device_id="0", do_optimized=False, out_signed=True, path2save=path2save)
    generate_waveform_lut_c(16, 125e6, 250e3, 1e3, device_id="1", do_optimized=True, out_signed=False, path2save=path2save)
