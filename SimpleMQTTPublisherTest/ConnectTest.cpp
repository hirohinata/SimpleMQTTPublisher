#include "pch.h"

#include <vector>
#include "../SimpleMQTTPublisher/src/Connect.c"
#include "../SimpleMQTTPublisher/src/common/Utility.c"

namespace SimpleMQTTPublisherTest
{
    TEST_CLASS(ConnectTest)
    {
    public:
        TEST_METHOD(BaseTest)
        {
            MQTT_ConnectPacketOption option = {
                "SampleApp",    // pszClientID
                nullptr,        // pszUserName
                nullptr,        // pPassword
                0,              // wPasswordLength
                0,              // wKeepAlive
                0,              // bCleanStart
                { 0, 0, 0 }     // reserved
            };
            std::vector<unsigned char> buffer(24);

            /* If pBuffer is nullptr, MQTT_CreateConnectPacket returns the required buffer size. */
            Assert::AreEqual(static_cast<int>(buffer.size()), MQTT_CreateConnectPacket(nullptr, 0, &option));

            /* If dwBufferLength is less than the required buffer size, MQTT_CreateConnectPacket returns -1. */
            Assert::AreEqual(-1, MQTT_CreateConnectPacket(buffer.data(), static_cast<unsigned int>(buffer.size() - 1), &option));

            /* MQTT_CreateConnectPacket returns the packet size. */
            Assert::AreEqual(static_cast<int>(buffer.size()), MQTT_CreateConnectPacket(buffer.data(), static_cast<unsigned int>(buffer.size()), &option));

            /* Fixed Header */
            /* 7-4bit: MQTT Control Packet type (1)
               3-0bit: Reserved (0) */
            Assert::AreEqual(static_cast<unsigned char>(0x10), buffer[0]);

            /* Remaining Length */
            Assert::AreEqual(static_cast<unsigned char>(buffer.size() - 2), buffer[1]);

            /* Variable Header */
            /* Protocol Name Length */
            Assert::AreEqual(static_cast<unsigned short>(4), static_cast<unsigned short>(buffer[2] << 8 | buffer[3]));

            /* Protocol Name */
            Assert::AreEqual(static_cast<unsigned char>('M'), buffer[4]);
            Assert::AreEqual(static_cast<unsigned char>('Q'), buffer[5]);
            Assert::AreEqual(static_cast<unsigned char>('T'), buffer[6]);
            Assert::AreEqual(static_cast<unsigned char>('T'), buffer[7]);

            /* Protocol Version */
            Assert::AreEqual(static_cast<unsigned char>(5), buffer[8]);

            /* Connect Flags */
            Assert::AreEqual(static_cast<unsigned char>(0x00), buffer[9]);

            /* Keep Alive */
            Assert::AreEqual(static_cast<unsigned short>(0), static_cast<unsigned short>(buffer[10] << 8 | buffer[11]));

            /* Properties Length */
            Assert::AreEqual(static_cast<unsigned char>(0), buffer[12]);

            /* Payload */
            /* Client Identifier Length */
            Assert::AreEqual(static_cast<unsigned short>(9), static_cast<unsigned short>(buffer[13] << 8 | buffer[14]));

            /* Client Identifier */
            Assert::AreEqual(static_cast<unsigned char>('S'), buffer[15]);
            Assert::AreEqual(static_cast<unsigned char>('a'), buffer[16]);
            Assert::AreEqual(static_cast<unsigned char>('m'), buffer[17]);
            Assert::AreEqual(static_cast<unsigned char>('p'), buffer[18]);
            Assert::AreEqual(static_cast<unsigned char>('l'), buffer[19]);
            Assert::AreEqual(static_cast<unsigned char>('e'), buffer[20]);
            Assert::AreEqual(static_cast<unsigned char>('A'), buffer[21]);
            Assert::AreEqual(static_cast<unsigned char>('p'), buffer[22]);
            Assert::AreEqual(static_cast<unsigned char>('p'), buffer[23]);
        }

        TEST_METHOD(UserNameTest)
        {
            MQTT_ConnectPacketOption option = {
                "SampleApp",    // pszClientID
                "UserName",     // pszUserName
                nullptr,        // pPassword
                0,              // wPasswordLength
                0,              // wKeepAlive
                0,              // bCleanStart
                { 0, 0, 0 }     // reserved
            };

            std::vector<unsigned char> buffer(34);
            Assert::AreEqual(34, MQTT_CreateConnectPacket(buffer.data(), static_cast<unsigned int>(buffer.size()), &option));

            /* Fixed Header */
            /* Remaining Length */
            Assert::AreEqual(static_cast<unsigned char>(34 - 2), buffer[1]);

            /* Variable Header */
            /* Connect Flags */
            /* User Name Flag is bit 7 (0x80). */
            Assert::AreEqual(static_cast<unsigned char>(0x80), buffer[9]);

            /* Payload */
            /* User Name Length */
            Assert::AreEqual(static_cast<unsigned short>(8), static_cast<unsigned short>(buffer[24] << 8 | buffer[25]));

            /* User Name */
            Assert::AreEqual(static_cast<unsigned char>('U'), buffer[26]);
            Assert::AreEqual(static_cast<unsigned char>('s'), buffer[27]);
            Assert::AreEqual(static_cast<unsigned char>('e'), buffer[28]);
            Assert::AreEqual(static_cast<unsigned char>('r'), buffer[29]);
            Assert::AreEqual(static_cast<unsigned char>('N'), buffer[30]);
            Assert::AreEqual(static_cast<unsigned char>('a'), buffer[31]);
            Assert::AreEqual(static_cast<unsigned char>('m'), buffer[32]);
            Assert::AreEqual(static_cast<unsigned char>('e'), buffer[33]);
        }

        TEST_METHOD(PasswordTest)
        {
            const unsigned char* pPassword = reinterpret_cast<const unsigned char*>("Password");
            MQTT_ConnectPacketOption option = {
                "SampleApp",    // pszClientID
                nullptr,        // pszUserName
                pPassword,      // pPassword
                8,              // wPasswordLength
                0,              // wKeepAlive
                0,              // bCleanStart
                { 0, 0, 0 }     // reserved
            };

            std::vector<unsigned char> buffer(34);
            Assert::AreEqual(34, MQTT_CreateConnectPacket(buffer.data(), static_cast<unsigned int>(buffer.size()), &option));

            /* Fixed Header */
            /* Remaining Length */
            Assert::AreEqual(static_cast<unsigned char>(34 - 2), buffer[1]);

            /* Variable Header */
            /* Connect Flags */
            /* Password Flag is bit 6 (0x40). */
            Assert::AreEqual(static_cast<unsigned char>(0x40), buffer[9]);

            /* Payload */
            /* Password Length */
            Assert::AreEqual(static_cast<unsigned short>(8), static_cast<unsigned short>(buffer[24] << 8 | buffer[25]));

            /* Password */
            Assert::AreEqual(static_cast<unsigned char>('P'), buffer[26]);
            Assert::AreEqual(static_cast<unsigned char>('a'), buffer[27]);
            Assert::AreEqual(static_cast<unsigned char>('s'), buffer[28]);
            Assert::AreEqual(static_cast<unsigned char>('s'), buffer[29]);
            Assert::AreEqual(static_cast<unsigned char>('w'), buffer[30]);
            Assert::AreEqual(static_cast<unsigned char>('o'), buffer[31]);
            Assert::AreEqual(static_cast<unsigned char>('r'), buffer[32]);
            Assert::AreEqual(static_cast<unsigned char>('d'), buffer[33]);
        }

        TEST_METHOD(UserNameAndPasswordTest)
        {
            const unsigned char* pPassword = reinterpret_cast<const unsigned char*>("Password");
            MQTT_ConnectPacketOption option = {
                "SampleApp",    // pszClientID
                "UserName",     // pszUserName
                pPassword,      // pPassword
                8,              // wPasswordLength
                0,              // wKeepAlive
                0,              // bCleanStart
                { 0, 0, 0 }     // reserved
            };

            std::vector<unsigned char> buffer(44);
            Assert::AreEqual(44, MQTT_CreateConnectPacket(buffer.data(), static_cast<unsigned int>(buffer.size()), &option));

            /* Fixed Header */
            /* Remaining Length */
            Assert::AreEqual(static_cast<unsigned char>(44 - 2), buffer[1]);

            /* Variable Header */
            /* Connect Flags */
            /* User Name Flag is bit 7 (0x80). */
            /* Password Flag is bit 6 (0x40). */
            Assert::AreEqual(static_cast<unsigned char>(0xC0), buffer[9]);

            /* Payload */
            /* User Name Length */
            Assert::AreEqual(static_cast<unsigned short>(8), static_cast<unsigned short>(buffer[24] << 8 | buffer[25]));

            /* User Name */
            Assert::AreEqual(static_cast<unsigned char>('U'), buffer[26]);
            Assert::AreEqual(static_cast<unsigned char>('s'), buffer[27]);
            Assert::AreEqual(static_cast<unsigned char>('e'), buffer[28]);
            Assert::AreEqual(static_cast<unsigned char>('r'), buffer[29]);
            Assert::AreEqual(static_cast<unsigned char>('N'), buffer[30]);
            Assert::AreEqual(static_cast<unsigned char>('a'), buffer[31]);
            Assert::AreEqual(static_cast<unsigned char>('m'), buffer[32]);
            Assert::AreEqual(static_cast<unsigned char>('e'), buffer[33]);

            /* Password Length */
            Assert::AreEqual(static_cast<unsigned short>(8), static_cast<unsigned short>(buffer[34] << 8 | buffer[35]));

            /* Password */
            Assert::AreEqual(static_cast<unsigned char>('P'), buffer[36]);
            Assert::AreEqual(static_cast<unsigned char>('a'), buffer[37]);
            Assert::AreEqual(static_cast<unsigned char>('s'), buffer[38]);
            Assert::AreEqual(static_cast<unsigned char>('s'), buffer[39]);
            Assert::AreEqual(static_cast<unsigned char>('w'), buffer[40]);
            Assert::AreEqual(static_cast<unsigned char>('o'), buffer[41]);
            Assert::AreEqual(static_cast<unsigned char>('r'), buffer[42]);
            Assert::AreEqual(static_cast<unsigned char>('d'), buffer[43]);
        }

        TEST_METHOD(BinaryPasswordTest)
        {
            const unsigned char pPassword[] = { 0x01, 0x02, 0x03, 0x04, 0x05 };
            MQTT_ConnectPacketOption option = {
                "SampleApp",    // pszClientID
                nullptr,        // pszUserName
                pPassword,      // pPassword
                4,              // wPasswordLength
                0,              // wKeepAlive
                0,              // bCleanStart
                { 0, 0, 0 }     // reserved
            };

            /* option.wPasswordLength determines the password length.
               The size of the password area does not matter. */
            std::vector<unsigned char> buffer(30);
            Assert::AreEqual(30, MQTT_CreateConnectPacket(buffer.data(), static_cast<unsigned int>(buffer.size()), &option));

            /* Fixed Header */
            /* Remaining Length */
            Assert::AreEqual(static_cast<unsigned char>(30 - 2), buffer[1]);

            /* Payload */
            /* Password Length */
            Assert::AreEqual(static_cast<unsigned short>(4), static_cast<unsigned short>(buffer[24] << 8 | buffer[25]));

            /* Password */
            Assert::AreEqual(static_cast<unsigned char>(0x01), buffer[26]);
            Assert::AreEqual(static_cast<unsigned char>(0x02), buffer[27]);
            Assert::AreEqual(static_cast<unsigned char>(0x03), buffer[28]);
            Assert::AreEqual(static_cast<unsigned char>(0x04), buffer[29]);
        }

        TEST_METHOD(KeepAliveTest)
        {
            MQTT_ConnectPacketOption option = {
                "SampleApp",    // pszClientID
                nullptr,        // pszUserName
                nullptr,        // pPassword
                0,              // wPasswordLength
                12345,          // wKeepAlive
                0,              // bCleanStart
                { 0, 0, 0 }     // reserved
            };

            std::vector<unsigned char> buffer(24);
            Assert::AreEqual(24, MQTT_CreateConnectPacket(buffer.data(), static_cast<unsigned int>(buffer.size()), &option));

            /* Variable Header */
            /* Keep Alive */
            Assert::AreEqual(static_cast<unsigned short>(12345), static_cast<unsigned short>(buffer[10] << 8 | buffer[11]));
        }

        TEST_METHOD(CleanStartTest)
        {
            MQTT_ConnectPacketOption option = {
                "SampleApp",    // pszClientID
                nullptr,        // pszUserName
                nullptr,        // pPassword
                0,              // wPasswordLength
                0,              // wKeepAlive
                1,              // bCleanStart
                { 0, 0, 0 }     // reserved
            };

            std::vector<unsigned char> buffer(24);
            Assert::AreEqual(24, MQTT_CreateConnectPacket(buffer.data(), static_cast<unsigned int>(buffer.size()), &option));

            /* Variable Header */
            /* Connect Flags */
            /* Clean Start Flag is bit 1 (0x02). */
            Assert::AreEqual(static_cast<unsigned char>(0x02), buffer[9]);
        }
    };
}