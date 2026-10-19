#pragma once
#include <ipmid/api-types.hpp>
#include <ipmid/utils.hpp>
#include <phosphor-logging/lg2.hpp>

namespace ipmi
{
namespace iei
{

Cc getService(ipmi::Context::ptr ctx, const std::string& interface,
              const std::string& path, std::string& service);

Cc setDbusProperty(ipmi::Context::ptr ctx, const std::string& service,
                   const std::string& objPath, const std::string& interface,
                   const std::string& property, const Value& value);

Cc setDbusProperty(ipmi::Context::ptr ctx, const std::string& objPath,
                   const std::string& interface, const std::string& property,
                   const Value& value);

template <typename Type>
Cc getDbusProperty(Context::ptr ctx, const std::string& service,
                   const std::string& objPath, const std::string& interface,
                   const std::string& property, Type& propertyValue)
{
    auto ec = ipmi::getDbusProperty(ctx, service, objPath, interface, property,
                                    propertyValue);
    if (ec)
    {
        lg2::error(
            "getDbusProperty for (service:{SERVICE}, path:{PATH}, interface:{INTERFACE}, property:{PROPERTY}) failed with {ERRMSG}",
            "SERVICE", service, "PATH", objPath, "INTERFACE", interface,
            "PROPERTY", property, "ERRMSG", ec.message());
        return ccUnspecifiedError;
    }
    return ccSuccess;
}

template <typename Type>
Cc getDbusProperty(Context::ptr ctx, const std::string& objPath,
                   const std::string& interface, const std::string& property,
                   Type& propertyValue)
{
    std::string service;
    auto cc = iei::getService(ctx, interface, objPath, service);
    if (cc != ccSuccess)
    {
        return cc;
    }

    return iei::getDbusProperty(ctx, service, objPath, interface, property,
                                propertyValue);
}

} // namespace iei
} // namespace ipmi
