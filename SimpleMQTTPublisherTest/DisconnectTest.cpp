#include "pch.h"

namespace SimpleMQTTPublisherTest
{
    TEST_CLASS(DisconnectTest)
    {
    public:
        TEST_METHOD(BaseTest)
        {
            unsigned char buffer[2] = { 0 };

            /* If pBuffer is nullptr, MQTT_CreateDisconnectPacket returns the required buffer size. */
            Assert::AreEqual(2, MQTT_CreateDisconnectPacket(nullptr, 0));

            /* MQTT_CreateConnectPacket returns the packet size. */
            Assert::AreEqual(2, MQTT_CreateDisconnectPacket(buffer, sizeof(buffer)));

            /* Fixed Header */
            /* 7-4bit: MQTT Control Packet type (DISCONNECT: 14)
               3-0bit: Reserved (0) */
            Assert::AreEqual(static_cast<unsigned char>(0xE0), buffer[0]);

            /* Remaining Length */
            Assert::AreEqual(static_cast< unsigned char>(0x00), buffer[1]);
        }
    };
}