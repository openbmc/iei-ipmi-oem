#pragma once
#include <ipmid/api-types.hpp>

namespace ipmi
{

constexpr ipmi::NetFn netFnIei = 0x3C;
constexpr ipmi::NetFn netFnIntelMe = 0x30;

namespace iei
{

constexpr ipmi::Cmd cmdSetAssetInfo = 0x01;
constexpr ipmi::Cmd cmdGetFWVersion = 0x03;

} // namespace iei

namespace intel_me
{

constexpr ipmi::Cmd cmdGetPsuPinReading = 0xE2;

}

} // namespace ipmi
