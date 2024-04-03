gtest = dependency('gtest', main: true, disabler: true, required: build_tests)
gmock = dependency('gmock', disabler: true, required: build_tests)

configure_file(output: 'config.h',
  configuration: cdata,
)
test_inc = include_directories('.')

test_iei_ipmi_oem = executable(
  'test_iei-ipmi-oem',
  '../src/iei_oem.cpp',
  'test_iei_ipmi_oem.cpp',
  'mocked_utils.cpp',
  'mocked_sdbus.cpp',
  include_directories: [test_inc, src_inc],
  dependencies: [
    gtest,
    gmock,
    phosphor_logging,
    phosphor_dbus_interfaces,
    sdbusplus,
  ])

test('test_iei-ipmi-oem', test_iei_ipmi_oem)
