# ---- Python ----
python = import('python').find_installation(required: true)

# ---- LUT configuration ----
lut_json_opt = get_option('ael_lut_json')
default_lut = meson.current_source_dir() / '../tools/reverse_lut_default.json'

lut_json_file = lut_json_opt == '' ? default_lut : lut_json_opt

# ---- LUT generation ----
lut_gen = custom_target(
    'amd_lut_gen',
    output : 'amd_ael_lut_gen.hpp',
    command : [
        python,
        meson.current_source_dir() / '../tools/generate_lut.py',
        '--input', lut_json_file,
        '--output', '@OUTPUT@'
    ],
    depend_files : [
        lut_json_file,
        meson.current_source_dir() / '../tools/generate_lut.py'
    ],
    build_by_default : true,
    console : true
)

# ---- Extension sources ----
ael_sources = files(
    'entry_points.cpp',
    'amd_ael_decoder.cpp'
)

ael_generated = [lut_gen]

# ---- Inject into phosphor-log-manager ----
log_manager_ext_sources += ael_sources + ael_generated

# ---- Dependencies ----
log_manager_ext_deps += [
    phosphor_logging_dep,
    conf_h_dep
]