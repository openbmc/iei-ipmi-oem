#pragma once
#include <ipmid/api-types.hpp>

namespace ipmi
{

constexpr ipmi::NetFn netFnIEI = 0x3C;
constexpr ipmi::NetFn netFnIntelMe = 0x30;

namespace iei
{

constexpr ipmi::Cmd cmdSetAssetInfo = 0x01;
constexpr ipmi::Cmd cmdGetFWVersion = 0x03;

// power
constexpr ipmi::Cmd cmdSetPowerDelay = 0x24;
constexpr ipmi::Cmd cmdGetPowerDelay = 0x25;

} // namespace iei

namespace intel_me
{

constexpr ipmi::Cmd cmdGetPsuPinReading = 0xE2;

}

} // namespace ipmi
