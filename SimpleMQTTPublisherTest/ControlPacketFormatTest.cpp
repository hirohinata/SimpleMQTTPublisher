#include "pch.h"

#include "../SimpleMQTTPublisher/src/common/ControlPacketFormat.c"

namespace SimpleMQTTPublisherTest
{
    TEST_CLASS(ControlPacketFormatTest)
    {
    public:
        TEST_METHOD(MQTT_EncodePublishControlPacketFlagsTest)
        {
            MQTT_ControlPacketFlags flags = 0;
            Assert::AreEqual(0, MQTT_EncodePublishControlPacketFlags(0, 0, 0, &flags));
            Assert::AreEqual<MQTT_ControlPacketFlags>(0x00, flags);
            Assert::AreEqual(0, MQTT_EncodePublishControlPacketFlags(0, 0, 1, &flags));
            Assert::AreEqual<MQTT_ControlPacketFlags>(0x01, flags);
            Assert::AreEqual(0, MQTT_EncodePublishControlPacketFlags(0, 1, 0, &flags));
            Assert::AreEqual<MQTT_ControlPacketFlags>(0x02, flags);
            Assert::AreEqual(0, MQTT_EncodePublishControlPacketFlags(0, 1, 1, &flags));
            Assert::AreEqual<MQTT_ControlPacketFlags>(0x03, flags);
            Assert::AreEqual(0, MQTT_EncodePublishControlPacketFlags(0, 2, 0, &flags));
            Assert::AreEqual<MQTT_ControlPacketFlags>(0x04, flags);
            Assert::AreEqual(0, MQTT_EncodePublishControlPacketFlags(0, 2, 1, &flags));
            Assert::AreEqual<MQTT_ControlPacketFlags>(0x05, flags);
            Assert::AreEqual(0, MQTT_EncodePublishControlPacketFlags(1, 0, 0, &flags));
            Assert::AreEqual<MQTT_ControlPacketFlags>(0x08, flags);
            Assert::AreEqual(0, MQTT_EncodePublishControlPacketFlags(1, 0, 1, &flags));
            Assert::AreEqual<MQTT_ControlPacketFlags>(0x09, flags);
            Assert::AreEqual(0, MQTT_EncodePublishControlPacketFlags(1, 1, 0, &flags));
            Assert::AreEqual<MQTT_ControlPacketFlags>(0x0A, flags);
            Assert::AreEqual(0, MQTT_EncodePublishControlPacketFlags(1, 1, 1, &flags));
            Assert::AreEqual<MQTT_ControlPacketFlags>(0x0B, flags);
            Assert::AreEqual(0, MQTT_EncodePublishControlPacketFlags(1, 2, 0, &flags));
            Assert::AreEqual<MQTT_ControlPacketFlags>(0x0C, flags);
            Assert::AreEqual(0, MQTT_EncodePublishControlPacketFlags(1, 2, 1, &flags));
            Assert::AreEqual<MQTT_ControlPacketFlags>(0x0D, flags);

            Assert::AreNotEqual(0, MQTT_EncodePublishControlPacketFlags(2, 0, 0, &flags));
            Assert::AreNotEqual(0, MQTT_EncodePublishControlPacketFlags(0, 3, 0, &flags));
            Assert::AreNotEqual(0, MQTT_EncodePublishControlPacketFlags(0, 4, 0, &flags));
            Assert::AreNotEqual(0, MQTT_EncodePublishControlPacketFlags(0, 0, 2, &flags));
        }

        TEST_METHOD(MQTT_DecodePublishControlPacketFlagsTest)
        {
            unsigned char dup = 0;
            unsigned char qos = 0;
            unsigned char retain = 0;

            Assert::AreEqual(0, MQTT_DecodePublishControlPacketFlags(0x00, &dup, &qos, &retain));
            Assert::AreEqual<unsigned char>(0, dup);
            Assert::AreEqual<unsigned char>(0, qos);
            Assert::AreEqual<unsigned char>(0, retain);

            Assert::AreEqual(0, MQTT_DecodePublishControlPacketFlags(0x01, &dup, &qos, &retain));
            Assert::AreEqual<unsigned char>(0, dup);
            Assert::AreEqual<unsigned char>(0, qos);
            Assert::AreEqual<unsigned char>(1, retain);

            Assert::AreEqual(0, MQTT_DecodePublishControlPacketFlags(0x02, &dup, &qos, &retain));
            Assert::AreEqual<unsigned char>(0, dup);
            Assert::AreEqual<unsigned char>(1, qos);
            Assert::AreEqual<unsigned char>(0, retain);

            Assert::AreEqual(0, MQTT_DecodePublishControlPacketFlags(0x03, &dup, &qos, &retain));
            Assert::AreEqual<unsigned char>(0, dup);
            Assert::AreEqual<unsigned char>(1, qos);
            Assert::AreEqual<unsigned char>(1, retain);

            Assert::AreEqual(0, MQTT_DecodePublishControlPacketFlags(0x04, &dup, &qos, &retain));
            Assert::AreEqual<unsigned char>(0, dup);
            Assert::AreEqual<unsigned char>(2, qos);
            Assert::AreEqual<unsigned char>(0, retain);

            Assert::AreEqual(0, MQTT_DecodePublishControlPacketFlags(0x05, &dup, &qos, &retain));
            Assert::AreEqual<unsigned char>(0, dup);
            Assert::AreEqual<unsigned char>(2, qos);
            Assert::AreEqual<unsigned char>(1, retain);

            // QoS 3 is reserved
            Assert::AreNotEqual(0, MQTT_DecodePublishControlPacketFlags(0x06, &dup, &qos, &retain));
            Assert::AreNotEqual(0, MQTT_DecodePublishControlPacketFlags(0x07, &dup, &qos, &retain));

            Assert::AreEqual(0, MQTT_DecodePublishControlPacketFlags(0x08, &dup, &qos, &retain));
            Assert::AreEqual<unsigned char>(1, dup);
            Assert::AreEqual<unsigned char>(0, qos);
            Assert::AreEqual<unsigned char>(0, retain);

            Assert::AreEqual(0, MQTT_DecodePublishControlPacketFlags(0x09, &dup, &qos, &retain));
            Assert::AreEqual<unsigned char>(1, dup);
            Assert::AreEqual<unsigned char>(0, qos);
            Assert::AreEqual<unsigned char>(1, retain);

            Assert::AreEqual(0, MQTT_DecodePublishControlPacketFlags(0x0A, &dup, &qos, &retain));
            Assert::AreEqual<unsigned char>(1, dup);
            Assert::AreEqual<unsigned char>(1, qos);
            Assert::AreEqual<unsigned char>(0, retain);

            Assert::AreEqual(0, MQTT_DecodePublishControlPacketFlags(0x0B, &dup, &qos, &retain));
            Assert::AreEqual<unsigned char>(1, dup);
            Assert::AreEqual<unsigned char>(1, qos);
            Assert::AreEqual<unsigned char>(1, retain);

            Assert::AreEqual(0, MQTT_DecodePublishControlPacketFlags(0x0C, &dup, &qos, &retain));
            Assert::AreEqual<unsigned char>(1, dup);
            Assert::AreEqual<unsigned char>(2, qos);
            Assert::AreEqual<unsigned char>(0, retain);

            Assert::AreEqual(0, MQTT_DecodePublishControlPacketFlags(0x0D, &dup, &qos, &retain));
            Assert::AreEqual<unsigned char>(1, dup);
            Assert::AreEqual<unsigned char>(2, qos);
            Assert::AreEqual<unsigned char>(1, retain);

            // QoS 3 is reserved
            Assert::AreNotEqual(0, MQTT_DecodePublishControlPacketFlags(0x0E, &dup, &qos, &retain));
            Assert::AreNotEqual(0, MQTT_DecodePublishControlPacketFlags(0x0F, &dup, &qos, &retain));

            // Invalid flags
            Assert::AreNotEqual(0, MQTT_DecodePublishControlPacketFlags(0x10, &dup, &qos, &retain));
            Assert::AreNotEqual(0, MQTT_DecodePublishControlPacketFlags(0xFF, &dup, &qos, &retain));
        }
    };
}