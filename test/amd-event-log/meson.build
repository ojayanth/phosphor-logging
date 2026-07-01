gtest_dep = dependency('gtest', main: true, required: true)

python = import('python').find_installation()

lut_json_opt = get_option('ael_lut_json')

default_lut = meson.project_source_root() /'extensions/amd-event-log/data/reverse_lut_default.json'

lut_json = lut_json_opt == '' ? default_lut : lut_json_opt

test_gen = custom_target(
    'ael_test_gen',
    output : 'test_ael_lut_generated.cpp',
    command : [
        python,
        meson.project_source_root() /
          'extensions/amd-event-log/codegen/generate_lut_tests.py',
        '--input', lut_json,
        '--output', '@OUTPUT@'
    ],
    depend_files : [
        lut_json,
        meson.project_source_root() /
          'extensions/amd-event-log/codegen/generate_lut_tests.py'
    ],
    build_by_default : true
)

ael_decoder = files(
    '../../extensions/amd-event-log/src/amd_ael_decoder.cpp'
)

ael_src_build_dir = meson.project_build_root() /'extensions/amd-event-log/src'

test_exe = executable(
    'test-ael-lut',
    [ test_gen, ael_decoder ],
    dependencies : [ gtest_dep ],
    include_directories : include_directories(
        '../../extensions/amd-event-log/include',
        ael_src_build_dir
    )
)


test('ael_lut_generated', test_exe)