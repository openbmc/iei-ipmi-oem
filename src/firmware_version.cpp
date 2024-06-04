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

std::string getBMCVersionValue(ipmi::Context::ptr ctx,
                               const std::string& objPath,
                               const std::string& interface)
{
    std::string value{};
    auto cc = iei::getDbusProperty(ctx, objPath, interface, "Version", value);
    if (cc)
    {
        return "";
    }

    const std::regex patternId("v[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}");
    std::smatch result;
    if (!std::regex_search(value, result, patternId))
    {
        return "";
    }
    std::string versionStr = result[0];
    return versionStr.substr(1);
}

std::string getFWVersionValue(ipmi::Context::ptr ctx,
                              const std::string& objPath,
                              const std::string& interface)
{
    std::string value{};
    auto cc = iei::getDbusProperty(ctx, service, objPath, interface, "Version",
                                   value);
    if (cc)
    {
        return "";
    }
    return value;
}

enum FirmwareType : uint8_t
{
    BMC = 0x01,
    BIOS = 0x03,
};

RspType<std::string> ipmiIEIGetFWVersion(ipmi::Context::ptr ctx,
                                         uint8_t fwIndex,
                                         uint8_t /* devIndex */)
{
    std::string fwPath{};
    std::string value{};

    switch (fwIndex)
    {
        case BMC:
        {
            fwPath = "/xyz/openbmc_project/software/bmc_active";
            value = getBMCVersionValue(ctx, fwPath, versionInterface);
            break;
        }
        case BIOS:
        {
            fwPath = "/xyz/openbmc_project/software/bios_active";
            value = getFWVersionValue(ctx, fwPath, versionInterface);
            break;
        }
        default:
            return ipmi::responseParmOutOfRange();
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
