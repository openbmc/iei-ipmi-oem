#include "utils.hpp"

namespace ipmi
{
namespace iei
{

Cc getAllDbusObjects(ipmi::Context::ptr ctx, const std::string& serviceRoot,
                     const std::string& interface, ObjectTree& objectTree)
{
    auto ec = ipmi::getAllDbusObjects(ctx, serviceRoot, interface, objectTree);
    if (ec)
    {
        lg2::error(
            "getAllDbusObjects for (path: {ROOTPATH}, intf: {INTF}) failed with {ERRMSG}",
            "ROOTPATH", serviceRoot, "INTF", interface, "ERRMSG", ec.message());
        return ccUnspecifiedError;
    }

    return ccSuccess;
}

Cc getService(ipmi::Context::ptr ctx, const std::string& interface,
              const std::string& path, std::string& service)
{
    auto ec = ipmi::getService(ctx, interface, path, service);
    if (ec)
    {
        lg2::error(
            "getService for (path: {PATH}, interface:{INTERFACE}) failed with {ERRMSG}",
            "PATH", path, "INTERFACE", interface, "ERRMSG", ec.message());
        return ccUnspecifiedError;
    }

    return ccSuccess;
}

Cc setDbusProperty(ipmi::Context::ptr ctx, const std::string& service,
                   const std::string& objPath, const std::string interface,
                   const std::string property, const Value& value)
{
    auto ec = ipmi::setDbusProperty(ctx, service, objPath, interface, property,
                                    value);
    if (ec)
    {
        lg2::error(
            "setDbusProperty for (service: {SERVICE}, path: {PATH}, interface: {INTERFACE}) failed with {ERRMSG}",
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
                   const std::string interface, const std::string property,
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

Cc getAllDbusProperties(Context::ptr ctx, const std::string& service,
                        const std::string& objPath,
                        const std::string& interface, PropertyMap& properties)
{
    auto ec = ipmi::getAllDbusProperties(ctx, service, objPath, interface,
                                         properties);
    if (ec)
    {
        lg2::error(
            "getAllDbusProperties for (service: {SERVICE}, path: {PATH}, interface: {INTERFACE}) failed with {ERRMSG}",
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

} // namespace iei
} // namespace ipmi
