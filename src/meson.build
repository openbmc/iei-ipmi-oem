configure_file(output: 'config.h',
    configuration: cdata,
)

src_inc = include_directories('.')

so_version = '0.1.0'

shared_library(
  'iei-ipmi-oem',
  'asset_info.cpp',
  'utils.cpp',
  'firmware_version.cpp',
  dependencies: [
    phosphor_dbus_interfaces,
    phosphor_logging,
    libipmid,
  ],
  version: so_version,
  install: true,
  install_dir: get_option('libdir') / 'ipmid-providers'
  )
