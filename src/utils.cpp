#include "utils.hpp"

#include <phosphor-logging/lg2.hpp>

#include <algorithm>

namespace utils
{

namespace // anonymous
{
constexpr auto MAPPER_BUSNAME = "xyz.openbmc_project.ObjectMapper";
constexpr auto MAPPER_PATH = "/xyz/openbmc_project/object_mapper";
constexpr auto MAPPER_INTERFACE = "xyz.openbmc_project.ObjectMapper";
} // namespace

const UtilsInterface& getUtils()
{
    static Utils utils;
    return utils;
}

std::string Utils::getService(sdbusplus::bus::bus& bus, const char* path,
                              const char* interface) const
{
    auto services = getServices(bus, path, interface);
    if (services.empty())
    {
        return {};
    }
    return services[0];
}

std::vector<std::string> Utils::getServices(sdbusplus::bus::bus& bus,
                                            const char* path,
                                            const char* interface) const
{
    auto mapper = bus.new_method_call(MAPPER_BUSNAME, MAPPER_PATH,
                                      MAPPER_INTERFACE, "GetObject");

    mapper.append(path, std::vector<std::string>({interface}));
    try
    {
        auto mapperResponseMsg = bus.call(mapper);

        std::vector<std::pair<std::string, std::vector<std::string>>>
            mapperResponse;
        mapperResponseMsg.read(mapperResponse);
        if (mapperResponse.empty())
        {
            lg2::error("Error reading mapper response");
            throw std::runtime_error("Error reading mapper response");
        }
        std::vector<std::string> ret;
        for (const auto& i : mapperResponse)
        {
            ret.emplace_back(i.first);
        }
        return ret;
    }
    catch (const sdbusplus::exception::SdBusError& ex)
    {
        lg2::error(
            "GetObject call failed, path: {PATH}, interface: {INTERFACE}",
            "PATH", path, "INTERFACE", interface);
        throw std::runtime_error("GetObject call failed");
    }
}

any Utils::getPropertyImpl(sdbusplus::bus::bus& bus, const char* service,
                           const char* path, const char* interface,
                           const char* propertyName) const
{
    auto method = bus.new_method_call(service, path,
                                      "org.freedesktop.DBus.Properties", "Get");
    method.append(interface, propertyName);
    try
    {
        PropertyType value{};
        auto reply = bus.call(method);
        reply.read(value);
        return any(value);
    }
    catch (const sdbusplus::exception::SdBusError& ex)
    {
        lg2::error(
            "GetProperty call failed, path: {PATH}, interface: {INTERFACE}, property: {PROPERTY}",
            "PATH", path, "INTERFACE", interface, "PROPERTY", propertyName);
        throw std::runtime_error("GetProperty call failed");
    }
}

void Utils::setPropertyImpl(sdbusplus::bus::bus& bus, const char* service,
                            const char* path, const char* interface,
                            const char* propertyName, ValueType&& value) const
{
    auto method = bus.new_method_call(service, path,
                                      "org.freedesktop.DBus.Properties", "Set");
    method.append(interface, propertyName, value);
    try
    {
        bus.call_noreply(method);
    }
    catch (const sdbusplus::exception::SdBusError& ex)
    {
        lg2::error(
            "SetProperty call failed, path: {PATH}, interface: {INTERFACE}, property: {PROPERTY}",
            "PATH", path, "INTERFACE", interface, "PROPERTY", propertyName);
        throw std::runtime_error("SetProperty call failed");
    }
}

} // namespace utils
