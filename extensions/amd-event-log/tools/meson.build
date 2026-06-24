python = import('python').find_installation()

# ---- Generate LUT ----

lut_json_opt = get_option('ael_lut_json')
default_lut = meson.current_source_dir() / 'reverse_lut_default.json'
lut_json_file = ''

if lut_json_opt == ''
    lut_json_file = default_lut
else
    lut_json_file = lut_json_opt
endif

lut_gen = custom_target(
    'amd_lut_gen',
    output : 'lut_gen.hpp',
    command : [
        python,
        meson.current_source_dir() / 'generate_lut.py',
        lut_json_file
    ],
    depend_files : lut_json_file,
    build_by_default : true
)

# ---- Library ----
libamd_event_log = static_library(
    'amd-event-log',
    sources : [
        'amd_event_log.cpp',
        lut_gen
    ],
    include_directories : include_directories('.'),
    dependencies : [
        phosphor_logging_dep
    ]
)

amd_event_log_dep = declare_dependency(
    link_with : libamd_event_log,
    include_directories : include_directories('.')
)