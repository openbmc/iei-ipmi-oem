#include "utils.hpp"

namespace ipmi
{
namespace iei
{

Cc getService(ipmi::Context::ptr ctx, const std::string& interface,
              const std::string& path, std::string& service)
{
    auto ec = ipmi::getService(ctx, interface, path, service);
    if (ec)
    {
        lg2::error(
            "getService for (path:{PATH}, interface:{INTERFACE}) failed with {ERRMSG}",
            "PATH", path, "INTERFACE", interface, "ERRMSG", ec.message());
        return ccUnspecifiedError;
    }

    return ccSuccess;
}

Cc setDbusProperty(ipmi::Context::ptr ctx, const std::string& service,
                   const std::string& objPath, const std::string& interface,
                   const std::string& property, const Value& value)
{
    auto ec = ipmi::setDbusProperty(ctx, service, objPath, interface, property,
                                    value);
    if (ec)
    {
        lg2::error(
            "setDbusProperty for (service:{SERVICE}, path:{PATH}, interface:{INTERFACE}) failed with {ERRMSG}",
            "SERVICE", service, "PATH", objPath, "INTERFACE", interface,
            "ERRMSG", ec.message());
        if (ec == boost::system::errc::make_error_code(
                      boost::system::errc::invalid_argument))
        {
            return ccInvalidFieldRequest;
        }
        return ccUnspecifiedError;
    }

    return ccSuccess;
}

Cc setDbusProperty(ipmi::Context::ptr ctx, const std::string& objPath,
                   const std::string& interface, const std::string& property,
                   const Value& value)
{
    std::string service;
    auto cc = iei::getService(ctx, interface, objPath, service);
    if (cc != ccSuccess)
    {
        return cc;
    }

    return iei::setDbusProperty(ctx, service, objPath, interface, property,
                                value);
}

} // namespace iei
} // namespace ipmi
