test(
    'test_plugin_cper',
    executable(
        'test-plugin-cper',
        'cper_plugin_test.cpp',
        link_with: [log_manager_lib],
        dependencies: [
            gtest_dep,
            gmock_dep,
            log_manager_deps,
        ],
        include_directories: include_directories(
            '../../',
            '../../gen',
        ),
    ),
)

test(
    'test_plugin_cper_dbus',
    executable(
        'test-plugin-cper-dbus',
        'cper_plugin_dbus_test.cpp',
        link_with: [log_manager_lib],
        dependencies: [
            gtest_dep,
            gmock_dep,
            log_manager_deps,
        ],
        include_directories: include_directories(
            '../../',
            '../../gen',
        ),
    ),
)
