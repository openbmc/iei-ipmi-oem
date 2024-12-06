#include "types.hpp"
#include "utils.hpp"

#include <ipmid/api.hpp>

namespace ipmi
{
namespace iei
{
namespace power
{

void registerIEIPowerFunctions() __attribute__((constructor));

static constexpr auto settingsService = "xyz.openbmc_project.Settings";

static constexpr auto powerRestorePolicyPath =
    "/xyz/openbmc_project/control/host0/power_restore_policy";
static constexpr auto powerRestorePolicyIface =
    "xyz.openbmc_project.Control.Power.RestorePolicy";
static constexpr auto powerRestoreDelayProp = "PowerRestoreDelay";

RspType<> ipmiIEISetPowerDelay(ipmi::Context::ptr ctx,
                               uint16_t powerRestoreDelay)
{
    uint64_t powerRestoreDelayInUs = powerRestoreDelay * 1000 * 1000;

    auto ec = iei::setDbusProperty(
        ctx, settingsService, powerRestorePolicyPath, powerRestorePolicyIface,
        powerRestoreDelayProp, powerRestoreDelayInUs);

    return ipmi::response(ec);
}

RspType<uint16_t> ipmiIEIGetPowerDelay(ipmi::Context::ptr ctx)
{
    uint64_t powerRestoreDelayInUs = 0;

    auto ec = iei::getDbusProperty(
        ctx, settingsService, powerRestorePolicyPath, powerRestorePolicyIface,
        powerRestoreDelayProp, powerRestoreDelayInUs);
    if (ec)
    {
        return ipmi::response(ec);
    }

    uint16_t powerRestoreDelay = powerRestoreDelayInUs / (1000 * 1000);

    return ipmi::responseSuccess(powerRestoreDelay);
}

void registerIEIPowerFunctions()
{
    // Set power delay
    ipmi::registerHandler(prioOemBase, netFnIEI, cmdSetPowerDelay,
                          ipmi::Privilege::Operator, ipmiIEISetPowerDelay);

    // Get power delay
    ipmi::registerHandler(prioOemBase, netFnIEI, cmdGetPowerDelay,
                          ipmi::Privilege::Operator, ipmiIEIGetPowerDelay);
}

} // namespace power
} // namespace iei
} // namespace ipmi
