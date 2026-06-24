python = import('python').find_installation(required: true)

lut_json_opt = get_option('ael_lut_json')

default_lut = files('../data/reverse_lut_default.json')[0]
lut_json_file = lut_json_opt == '' ? default_lut : lut_json_opt

lut_script = files('../codegen/generate_lut.py')[0]

lut_gen = custom_target(
    'amd_lut_gen',
    output: 'amd_ael_lut_gen.hpp',
    command: [
        python,
        lut_script,
        '--input',
        lut_json_file,
        '--output',
        '@OUTPUT@',
    ],
    depend_files: [
        default_lut,
        lut_script,
    ],
    build_by_default: true,
)

# AEL sources
ael_sources = files(
    'amd_ael_decoder.cpp',
    'entry_points.cpp',
)

# Include dirs (IMPORTANT: include '.' for generated header)
ael_inc = include_directories(
    '../include',
    '.',  # gives build/src for generated LUT header
)

