import numpy as np


def get_embedded_datatype(bits_used: int, out_signed: bool=True) -> str:
    """Determine the C datatype for processing data
    Args:
        bits_used:      Integer with bit value
        out_signed:     Boolean if datatype is signed or unsigned
    Return:
        String with datatype in C
    """
    bit_compare = np.array((8, 16, 32, 64))
    used_bitval = np.argwhere((bit_compare / bits_used) - 1 >= 0).flatten()[0]
    return ('' if out_signed else 'u') + 'int' + f'{bit_compare[used_bitval]}' + f'_t'


def replace_variables_with_parameters(string_input: list, parameters: dict) -> list:
    """Function for search parameter in string list and replace with new defined real values
    Args:
        string_input:   List with input strings from file
        parameters:     Dictionary with parameters (key and value)
    Returns:
        List with corrected string output
    """
    string_output = list()
    for line in string_input:
        if '{$' not in line:
            used_line = line
        else:
            overview_split = line.split('{$')
            used_line = line
            for split_param in overview_split[1:]:
                param_search = split_param.split('}')[0]
                for key, value in parameters.items():
                    if param_search == key:
                        used_line = used_line.replace(f'{{${param_search}}}', value)
                        break
        string_output.append(used_line)
    return string_output
