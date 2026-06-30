message('AEL test meson entered')

gtest_dep = dependency('gtest', main: true, required: true)
python = import('python').find_installation()

tools_dir = '../../extensions/amd-event-log/tools'
src_dir = '../../extensions/amd-event-log/src'
script_file = files(tools_dir / 'generate_lut_tests.py')
default_lut_file = files(tools_dir / 'reverse_lut_default.json')

# ---- LUT JSON ----
lut_json_opt = get_option('ael_lut_json')
lut_json_file = lut_json_opt == '' ? default_lut_file : files(lut_json_opt)

# ---- Generate test file ----
test_gen = custom_target(
    'ael_test_gen',
    output: 'test_ael_lut_generated.cpp',
    command: [
        python,
        script_file,  # ✅ Meson-managed file
        '--input',
        lut_json_file,
        '--output',
        '@OUTPUT@',
    ],
    depend_files: [script_file, lut_json_file],
    build_by_default: true,
)

# ---- Decoder sources ----
ael_decoder = files(
    src_dir / 'amd_ael_decoder.cpp',
    src_dir / 'amd_ael_decoder.hpp',
)

# ---- Build test ----
test_exe = executable(
    'test-ael-lut',
    [test_gen, ael_decoder],
    dependencies: [gtest_dep],
    include_directories: include_directories(src_dir),
)

test('ael_lut_generated', test_exe)
