#include "types.hpp"
#include "utils.hpp"

#include <ipmid/api.hpp>

#include <regex>

namespace ipmi
{
namespace iei
{
namespace firmware_version
{

void registerIEIFirmwareVersionFunctions() __attribute__((constructor));

static constexpr auto versionInterface = "xyz.openbmc_project.Software.Version";

Cc getBMCVersionValue(ipmi::Context::ptr ctx, const std::string& objPath,
                      const std::string& interface, std::string& version)
{
    auto cc = iei::getDbusProperty(ctx, objPath, interface, "Version", version);
    if (cc)
    {
        return cc;
    }

    const std::regex patternId("v[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}");
    std::smatch result;
    if (!std::regex_search(version, result, patternId))
    {
        lg2::error(
            "The BMC version does not contain a version number, VERSION = {VERSION}",
            "VERSION", version);
        return ccUnspecifiedError;
    }
    version = result[0];
    version.erase(0, 1);
    return ccSuccess;
}

Cc getFWVersionValue(ipmi::Context::ptr ctx, const std::string& objPath,
                     const std::string& interface, std::string& version)
{
    return iei::getDbusProperty(ctx, objPath, interface, "Version", version);
}

enum FirmwareType : uint8_t
{
    BMC = 0x01,
    BIOS = 0x03,
};

RspType<std::string> ipmiIEIGetFWVersion(
    ipmi::Context::ptr ctx, uint8_t fwIndex, uint8_t /* devIndex */)
{
    std::string fwPath{};
    std::string value{};
    Cc cc = ccSuccess;

    switch (fwIndex)
    {
        case BMC:
        {
            fwPath = "/xyz/openbmc_project/software/bmc_active";
            cc = getBMCVersionValue(ctx, fwPath, versionInterface, value);
            break;
        }
        case BIOS:
        {
            fwPath = "/xyz/openbmc_project/software/bios_active";
            cc = getFWVersionValue(ctx, fwPath, versionInterface, value);
            break;
        }
        default:
            return ipmi::responseParmOutOfRange();
    }

    if (cc)
    {
        return ipmi::response(cc);
    }

    return ipmi::responseSuccess(value);
}

void registerIEIFirmwareVersionFunctions()
{
    ipmi::registerHandler(prioOemBase, netFnIEI, cmdGetFWVersion,
                          ipmi::Privilege::User, ipmiIEIGetFWVersion);
}

} // namespace firmware_version
} // namespace iei
} // namespace ipmi
