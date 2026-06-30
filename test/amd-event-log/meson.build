message('AEL test meson entered')

gtest_dep = dependency('gtest', main: true, required: true)
python = import('python').find_installation()

# ---- LUT JSON ----
lut_json_opt = get_option('ael_lut_json')

default_lut = meson.project_source_root() / 'extensions/amd-event-log/tools/reverse_lut_default.json'

lut_json = lut_json_opt == '' ? default_lut : lut_json_opt

# ---- Generate test file ----
test_gen = custom_target(
    'ael_test_gen',
    output : 'test_ael_lut_generated.cpp',
    command : [
        python,
        meson.project_source_root() /
            'extensions/amd-event-log/tools/generate_lut_tests.py',
        '--input', lut_json,
        '--output', '@OUTPUT@'
    ],
    depend_files : [
        lut_json,
        meson.project_source_root() /
            'extensions/amd-event-log/tools/generate_lut_tests.py'
    ],
    build_by_default : true
)

# ---- Add decoder code ----
ael_decoder = files(
    '../../extensions/amd-event-log/src/amd_ael_decoder.cpp',
    '../../extensions/amd-event-log/src/amd_ael_decoder.hpp'
)

# ---- Build test ----
test_exe = executable(
    'test-ael-lut',
    [
        test_gen,
        ael_decoder
    ],
    dependencies : [
        gtest_dep
    ],
    include_directories : include_directories(
        '../../extensions/amd-event-log/src',
        meson.current_build_dir()
    )
)

test('ael_lut_generated', test_exe)