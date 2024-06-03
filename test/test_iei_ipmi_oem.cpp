#include "config.h"

#include "iei_oem.hpp"
#include "mocked_utils.hpp"

#include <ipmid/api.h>

#include <ipmid/api.hpp>
#include <sdbusplus/bus.hpp>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::_;
using ::testing::Invoke;
using ::testing::IsNull;
using ::testing::Return;
using ::testing::StrEq;
using ::testing::VariantWith;

namespace ipmi
{

void parseBIOSInfo(const std::vector<uint8_t>& data);

}

void ipmi_register_callback(ipmi_netfn_t, ipmi_cmd_t, ipmi_context_t,
                            ipmid_callback_t, ipmi_cmd_privilege_t)
{
    // Empty
}

class TestIpmiOem : public ::testing::Test
{
  public:
    TestIpmiOem() :
        mockedUtils(
            reinterpret_cast<const utils::MockedUtils&>(utils::getUtils()))
    {}
    virtual ~TestIpmiOem()
    {
        utils::freeUtils();
    }

    const utils::MockedUtils& mockedUtils;
};

TEST_F(TestIpmiOem, Empty)
{
    // Empty
}

TEST_F(TestIpmiOem, parseBIOSInfoEmpty)
{
    EXPECT_CALL(mockedUtils, setPropertyImpl(_, _, _, _, _, _)).Times(0);
    ipmi::parseBIOSInfo({});
}

TEST_F(TestIpmiOem, parseBIOSInfoValidBIOSVersion)
{
    std::vector<uint8_t> data{
        0x00, 0x30, 0x31, 0x2e, 0x30, 0x31, 0x2e, 0x30, 0x31, 0x2e,
        0x30, 0x31, 0x2e, 0x30, 0x31, 0x00, 0x30, 0x38, 0x2f, 0x31,
        0x31, 0x2f, 0x32, 0x30, 0x32, 0x30, 0x20, 0x32, 0x30, 0x3a,
        0x31, 0x39, 0x3a, 0x33, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    std::string dummyService = "com.test.bios.version";
    std::string expectedVersion = "01.01.01.01.01";

    std::shared_ptr<sdbusplus::asio::connection> dbus = getSdBus();
    EXPECT_CALL(mockedUtils,
                getService(*dbus, StrEq(BIOS_OBJPATH), StrEq(VERSION_IFACE)))
        .WillOnce(Return(dummyService));
    EXPECT_CALL(
        mockedUtils,
        setPropertyImpl(*dbus, StrEq(dummyService), StrEq(BIOS_OBJPATH),
                        StrEq(VERSION_IFACE), StrEq(VERSION),
                        VariantWith<std::string>(StrEq(expectedVersion))))
        .Times(1);
    ipmi::parseBIOSInfo(data);
}
