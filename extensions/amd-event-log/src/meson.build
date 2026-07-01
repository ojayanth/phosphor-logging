python = import('python').find_installation(required: true)

lut_json_opt = get_option('ael_lut_json')
default_lut = meson.current_source_dir() / '../data/reverse_lut_default.json'
lut_json_file = lut_json_opt == '' ? default_lut : lut_json_opt

lut_gen = custom_target(
    'amd_lut_gen',
    output : 'amd_ael_lut_gen.hpp',
    command : [
        python,
        meson.current_source_dir() / '../codegen/generate_lut.py',
        '--input', lut_json_file,
        '--output', '@OUTPUT@'
    ],
    depend_files : [
        lut_json_file,
        meson.current_source_dir() / '../codegen/generate_lut.py'
    ],
    build_by_default : true,
    console : true
)

ael_sources = files(
    'entry_points.cpp',
    'amd_ael_decoder.cpp'
)

ael_inc = include_directories(
    '../include',
    meson.current_build_dir()
)

ael_dep = declare_dependency(
    include_directories : ael_inc
)

log_manager_ext_sources += ael_sources + [lut_gen]

log_manager_ext_deps += [
    phosphor_logging_dep,
    conf_h_dep,
    ael_dep
]