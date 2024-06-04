configure_file(output: 'config.h',
    configuration: cdata,
)

src_inc = include_directories('.')

so_version = '0.1.0'

sources = [
  'iei_oem.cpp',
]

deps = [
  phosphor_dbus_interfaces_dep,
  phosphor_logging_dep,
  ipmid_dep,
  sdbusplus_dep,
]

shared_library(
  'iei-ipmi-oem',
  sources,
  dependencies: deps,
  version: so_version,
  install: true,
  install_dir: get_option('libdir') / 'ipmid-providers'
)
