#include <ipmid/api-types.hpp>
#include <ipmid/utils.hpp>
#include <phosphor-logging/lg2.hpp>

namespace ipmi
{
namespace iei
{

Cc getAllDbusObjects(ipmi::Context::ptr ctx, const std::string& serviceRoot,
                     const std::string& interface, ObjectTree& objectTree);

Cc getService(ipmi::Context::ptr ctx, const std::string& interface,
              const std::string& path, std::string& service);

Cc setDbusProperty(ipmi::Context::ptr ctx, const std::string& service,
                   const std::string& objPath, const std::string interface,
                   const std::string property, const Value& value);

Cc setDbusProperty(ipmi::Context::ptr ctx, const std::string& objPath,
                   const std::string interface, const std::string property,
                   const Value& value);

Cc getAllDbusProperties(Context::ptr ctx, const std::string& service,
                        const std::string& objPath,
                        const std::string& interface, PropertyMap& properties);

template <typename Type>
Cc getDbusProperty(Context::ptr ctx, const std::string& service,
                   const std::string& objPath, const std::string& interface,
                   const std::string& property, Type& propertyValue)
{
    auto ec = ipmi::getDbusProperty(ctx, service, objPath, interface, property,
                                    propertyValue);
    if (ec)
    {
        lg2::error("getDbusProperty for "
                   "(service:{SERVICE}, path:{PATH}, interface:{INTERFACE}, "
                   "property:{PROPERTY}) "
                   "failed with {ERRMSG}",
                   "SERVICE", service, "PATH", objPath, "INTERFACE", interface,
                   "PROPERTY", property, "ERRMSG", ec.message());
        return ccUnspecifiedError;
    }
    return ccSuccess;
}

template <typename... InputArgs>
void callDbusMethod(ipmi::Context::ptr ctx, Cc& cc, const std::string& service,
                    const std::string& objPath, const std::string& interface,
                    const std::string& method, const InputArgs&... a)
{
    boost::system::error_code ec;
    cc = ccSuccess;
    ctx->bus->yield_method_call(ctx->yield, ec, service, objPath, interface,
                                method, a...);
    if (ec)
    {
        lg2::error("callDbusMethod for "
                   "(service:{SERVICE}, path:{PATH}, interface:{INTERFACE}, "
                   "method:{METHOD}) "
                   "failed with {ERRMSG}",
                   "SERVICE", service, "PATH", objPath, "INTERFACE", interface,
                   "METHOD", method, "ERRMSG", ec.message());
        cc = ccUnspecifiedError;
    }
    return;
}

template <typename RetType, typename... InputArgs>
RetType callDbusMethod(ipmi::Context::ptr ctx, Cc& cc,
                       const std::string& service, const std::string& objPath,
                       const std::string& interface, const std::string& method,
                       const InputArgs&... a)
{
    boost::system::error_code ec;
    cc = ccSuccess;
    auto r = ctx->bus->yield_method_call<RetType>(
        ctx->yield, ec, service, objPath, interface, method, a...);
    if (ec)
    {
        lg2::error("callDbusMethod for "
                   "(service:{SERVICE}, path:{PATH}, interface:{INTERFACE}, "
                   "method:{METHOD}) "
                   "failed with {ERRMSG}",
                   "SERVICE", service, "PATH", objPath, "INTERFACE", interface,
                   "METHOD", method, "ERRMSG", ec.message());
        cc = ccUnspecifiedError;
    }
    return r;
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
