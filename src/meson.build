configure_file(output: 'config.h',
    configuration: cdata,
)

src_inc = include_directories('.')

so_version = '0.1.0'

sources = [
  'asset_info.cpp',
  'utils.cpp',
  'firmware_version.cpp',
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
  dependencies: [
    phosphor_dbus_interfaces,
    phosphor_logging,
    libipmid,
  ],
  version: so_version,
  install: true,
  install_dir: get_option('libdir') / 'ipmid-providers'
)
