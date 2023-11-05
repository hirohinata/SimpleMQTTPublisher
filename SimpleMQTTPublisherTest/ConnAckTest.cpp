#include "pch.h"

namespace SimpleMQTTPublisherTest
{
    TEST_CLASS(ConnAckTest)
    {
    public:
        TEST_METHOD(BaseTest)
        {
            const unsigned char buffer[] = {
                0x20,                                               /* MQTT Control Packet Type (2) and reserved. */
                0x03,                                               /* Remaining Length */
                0x00,                                               /* Session Present */
                static_cast<unsigned char>(MQTT_ConnAck_Success),   /* Connect Reason Code */
                0x00,                                               /* Properties Length */
            };
            MQTT_ConnectAckResult result = { MQTT_ConnAck_UnspecifiedError };
            Assert::AreEqual(5, MQTT_ParseConnectAckPacket(buffer, sizeof(buffer), &result));
            Assert::AreEqual(static_cast<unsigned char>(MQTT_ConnAck_Success), static_cast<unsigned char>(result.connectReasonCode));
            Assert::AreEqual(static_cast<unsigned char>(0), result.bSessionPresent);
        }
    };
}