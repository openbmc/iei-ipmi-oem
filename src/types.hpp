#pragma once
#include <ipmid/api-types.hpp>

namespace ipmi
{

constexpr ipmi::NetFn netFnIEI = 0x3C;

namespace iei
{

constexpr ipmi::Cmd cmdGetFWVersion = 0x03;

// power
constexpr ipmi::Cmd cmdSetPowerDelay = 0x24;
constexpr ipmi::Cmd cmdGetPowerDelay = 0x25;

} // namespace iei

} // namespace ipmi
