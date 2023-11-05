#include "pch.h"

#include <vector>
#include "../SimpleMQTTPublisher/src/common/DataType.c"

static bool operator==(const MQTT_VariableByteInteger& lhs, const MQTT_VariableByteInteger& rhs)
{
    return lhs.Value == rhs.Value
        && lhs.EncodedByteNum == rhs.EncodedByteNum
        && lhs.EncodedByte[0] == rhs.EncodedByte[0]
        && lhs.EncodedByte[1] == rhs.EncodedByte[1]
        && lhs.EncodedByte[2] == rhs.EncodedByte[2]
        && lhs.EncodedByte[3] == rhs.EncodedByte[3];
}

static std::wstring ToString(const MQTT_VariableByteInteger& variableByteInteger)
{
    std::wstringstream ss;
    ss << L"Value: " << variableByteInteger.Value
        << L", EncodedByteNum: " << variableByteInteger.EncodedByteNum
        << L", EncodedByte: " << variableByteInteger.EncodedByte[0]
        << L", " << variableByteInteger.EncodedByte[1]
        << L", " << variableByteInteger.EncodedByte[2]
        << L", " << variableByteInteger.EncodedByte[3];
    return ss.str();
}

namespace SimpleMQTTPublisherTest
{
    TEST_CLASS(DataTypeTest)
    {
    public:
        TEST_METHOD(MQTT_WriteNullTerminatedUTF8EncodedStringTest)
        {
            std::vector<unsigned char> buffer(65537, 0);

            // Consume (2 + StringLength) bytes. The 2 is for the string length.
            Assert::AreEqual(2, MQTT_WriteNullTerminatedUTF8EncodedString("", NULL, 0));
            Assert::AreEqual(3, MQTT_WriteNullTerminatedUTF8EncodedString("a", NULL, 0));

            // The max string length is 65535.
            Assert::AreEqual(65537, MQTT_WriteNullTerminatedUTF8EncodedString(std::string(65535, 'a').c_str(), NULL, 0));
            Assert::AreEqual(-1, MQTT_WriteNullTerminatedUTF8EncodedString(std::string(65536, 'a').c_str(), NULL, 0));

            // The buffer length must be greater than or equal to (2 + StringLength).
            Assert::AreEqual(-1, MQTT_WriteNullTerminatedUTF8EncodedString("", buffer.data(), 1));
            Assert::AreEqual(2, MQTT_WriteNullTerminatedUTF8EncodedString("", buffer.data(), 2));
            Assert::AreEqual((unsigned char)0, buffer[0]);
            Assert::AreEqual((unsigned char)0, buffer[1]);

            Assert::AreEqual(-1, MQTT_WriteNullTerminatedUTF8EncodedString("a", buffer.data(), 2));
            Assert::AreEqual(3, MQTT_WriteNullTerminatedUTF8EncodedString("a", buffer.data(), 3));
            Assert::AreEqual((unsigned char)0, buffer[0]);
            Assert::AreEqual((unsigned char)1, buffer[1]);
            Assert::AreEqual((unsigned char)'a', buffer[2]);

            Assert::AreEqual(65537, MQTT_WriteNullTerminatedUTF8EncodedString(std::string(65535, 'a').c_str(), buffer.data(), 65537));
            Assert::AreEqual((unsigned char)0xFF, buffer[0]);
            Assert::AreEqual((unsigned char)0xFF, buffer[1]);
            Assert::AreEqual((unsigned char)'a', buffer[65536]);
        }

        TEST_METHOD(MQTT_EncodeVariableByteIntegerTest)
        {
            MQTT_VariableByteInteger variableByteInteger = { 0 };

            Assert::AreEqual(0, MQTT_EncodeVariableByteInteger(0, &variableByteInteger));
            Assert::AreEqual({ 0, { 0, 0, 0, 0 }, 1 }, variableByteInteger);

            Assert::AreEqual(0, MQTT_EncodeVariableByteInteger(1, &variableByteInteger));
            Assert::AreEqual({ 1, { 1, 0, 0, 0 }, 1 }, variableByteInteger);

            Assert::AreEqual(0, MQTT_EncodeVariableByteInteger(127, &variableByteInteger));
            Assert::AreEqual({ 127, { 127, 0, 0, 0 }, 1 }, variableByteInteger);

            Assert::AreEqual(0, MQTT_EncodeVariableByteInteger(128, &variableByteInteger));
            Assert::AreEqual({ 128, { 128, 1, 0, 0 }, 2 }, variableByteInteger);

            Assert::AreEqual(0, MQTT_EncodeVariableByteInteger(129, &variableByteInteger));
            Assert::AreEqual({ 129, { 129, 1, 0, 0 }, 2 }, variableByteInteger);

            Assert::AreEqual(0, MQTT_EncodeVariableByteInteger(255, &variableByteInteger));
            Assert::AreEqual({ 255, { 255, 1, 0, 0 }, 2 }, variableByteInteger);

            Assert::AreEqual(0, MQTT_EncodeVariableByteInteger(256, &variableByteInteger));
            Assert::AreEqual({ 256, { 128, 2, 0, 0 }, 2 }, variableByteInteger);

            // 16383 = 128 * 127 + 127
            Assert::AreEqual(0, MQTT_EncodeVariableByteInteger(16383, &variableByteInteger));
            Assert::AreEqual({ 16383, { 255, 127, 0, 0 }, 2 }, variableByteInteger);

            Assert::AreEqual(0, MQTT_EncodeVariableByteInteger(16384, &variableByteInteger));
            Assert::AreEqual({ 16384, { 128, 128, 1, 0 }, 3 }, variableByteInteger);

            Assert::AreEqual(0, MQTT_EncodeVariableByteInteger(16385, &variableByteInteger));
            Assert::AreEqual({ 16385, { 129, 128, 1, 0 }, 3 }, variableByteInteger);

            // 16511 = 16384 + 127
            Assert::AreEqual(0, MQTT_EncodeVariableByteInteger(16511, &variableByteInteger));
            Assert::AreEqual({ 16511, { 255, 128, 1, 0 }, 3 }, variableByteInteger);

            Assert::AreEqual(0, MQTT_EncodeVariableByteInteger(16512, &variableByteInteger));
            Assert::AreEqual({ 16512, { 128, 129, 1, 0 }, 3 }, variableByteInteger);

            // 32767 = 16384 + 16383
            Assert::AreEqual(0, MQTT_EncodeVariableByteInteger(32767, &variableByteInteger));
            Assert::AreEqual({ 32767, { 255, 255, 1, 0 }, 3 }, variableByteInteger);

            Assert::AreEqual(0, MQTT_EncodeVariableByteInteger(32768, &variableByteInteger));
            Assert::AreEqual({ 32768, { 128, 128, 2, 0 }, 3 }, variableByteInteger);

            Assert::AreEqual(0, MQTT_EncodeVariableByteInteger(32769, &variableByteInteger));
            Assert::AreEqual({ 32769, { 129, 128, 2, 0 }, 3 }, variableByteInteger);

            Assert::AreEqual(0, MQTT_EncodeVariableByteInteger(49151, &variableByteInteger));
            Assert::AreEqual({ 49151, { 255, 255, 2, 0 }, 3 }, variableByteInteger);

            Assert::AreEqual(0, MQTT_EncodeVariableByteInteger(49152, &variableByteInteger));
            Assert::AreEqual({ 49152, { 128, 128, 3, 0 }, 3 }, variableByteInteger);

            // 2097151 = 128 * 128 * 127 + 128 * 127 + 127
            Assert::AreEqual(0, MQTT_EncodeVariableByteInteger(2097151, &variableByteInteger));
            Assert::AreEqual({ 2097151, { 255, 255, 127, 0 }, 3 }, variableByteInteger);

            Assert::AreEqual(0, MQTT_EncodeVariableByteInteger(2097152, &variableByteInteger));
            Assert::AreEqual({ 2097152, { 128, 128, 128, 1 }, 4 }, variableByteInteger);

            Assert::AreEqual(0, MQTT_EncodeVariableByteInteger(2097153, &variableByteInteger));
            Assert::AreEqual({ 2097153, { 129, 128, 128, 1 }, 4 }, variableByteInteger);

            // 2097278 = 2097151 + 128
            Assert::AreEqual(0, MQTT_EncodeVariableByteInteger(2097279, &variableByteInteger));
            Assert::AreEqual({ 2097279, { 255, 128, 128, 1 }, 4 }, variableByteInteger);

            Assert::AreEqual(0, MQTT_EncodeVariableByteInteger(2097280, &variableByteInteger));
            Assert::AreEqual({ 2097280, { 128, 129, 128, 1 }, 4 }, variableByteInteger);

            // 4194303 = 2097151 + 2097152
            Assert::AreEqual(0, MQTT_EncodeVariableByteInteger(4194303, &variableByteInteger));
            Assert::AreEqual({ 4194303, { 255, 255, 255, 1 }, 4 }, variableByteInteger);

            Assert::AreEqual(0, MQTT_EncodeVariableByteInteger(4194304, &variableByteInteger));
            Assert::AreEqual({ 4194304, { 128, 128, 128, 2 }, 4 }, variableByteInteger);

            Assert::AreEqual(0, MQTT_EncodeVariableByteInteger(4194305, &variableByteInteger));
            Assert::AreEqual({ 4194305, { 129, 128, 128, 2 }, 4 }, variableByteInteger);

            // 268435455 = 128 * 128 * 128 * 127 + 128 * 128 * 127 + 128 * 127 + 127
            Assert::AreEqual(0, MQTT_EncodeVariableByteInteger(268435455, &variableByteInteger));
            Assert::AreEqual({ 268435455, { 255, 255, 255, 127 }, 4 }, variableByteInteger);

            Assert::AreNotEqual(0, MQTT_EncodeVariableByteInteger(268435456, &variableByteInteger));
            Assert::AreNotEqual(0, MQTT_EncodeVariableByteInteger(UINT_MAX, &variableByteInteger));
        }

        TEST_METHOD(MQTT_DecodeVariableByteIntegerTest)
        {
            static const auto Buffer = [](const std::initializer_list<unsigned char>& list) { return list.begin(); };

            MQTT_VariableByteInteger variableByteInteger = { 0 };

            Assert::AreEqual(0, MQTT_DecodeVariableByteInteger(Buffer({ 0, 0, 0, 0 }), &variableByteInteger));
            Assert::AreEqual({ 0, { 0, 0, 0, 0 }, 1 }, variableByteInteger);

            Assert::AreEqual(0, MQTT_DecodeVariableByteInteger(Buffer({ 1, 0, 0, 0 }), &variableByteInteger));
            Assert::AreEqual({ 1, { 1, 0, 0, 0 }, 1 }, variableByteInteger);

            Assert::AreEqual(0, MQTT_DecodeVariableByteInteger(Buffer({ 127, 0, 0, 0 }), &variableByteInteger));
            Assert::AreEqual({ 127, { 127, 0, 0, 0 }, 1 }, variableByteInteger);

            Assert::AreEqual(0, MQTT_DecodeVariableByteInteger(Buffer({ 128, 1, 0, 0 }), &variableByteInteger));
            Assert::AreEqual({ 128, { 128, 1, 0, 0 }, 2 }, variableByteInteger);

            Assert::AreEqual(0, MQTT_DecodeVariableByteInteger(Buffer({ 129, 1, 0, 0 }), &variableByteInteger));
            Assert::AreEqual({ 129, { 129, 1, 0, 0 }, 2 }, variableByteInteger);

            Assert::AreEqual(0, MQTT_DecodeVariableByteInteger(Buffer({ 255, 1, 0, 0 }), &variableByteInteger));
            Assert::AreEqual({ 255, { 255, 1, 0, 0 }, 2 }, variableByteInteger);

            Assert::AreEqual(0, MQTT_DecodeVariableByteInteger(Buffer({ 128, 2, 0, 0 }), &variableByteInteger));
            Assert::AreEqual({ 256, { 128, 2, 0, 0 }, 2 }, variableByteInteger);

            Assert::AreEqual(0, MQTT_DecodeVariableByteInteger(Buffer({ 129, 2, 0, 0 }), &variableByteInteger));
            Assert::AreEqual({ 257, { 129, 2, 0, 0 }, 2 }, variableByteInteger);

            Assert::AreEqual(0, MQTT_DecodeVariableByteInteger(Buffer({ 255, 127, 0, 0 }), &variableByteInteger));
            Assert::AreEqual({ 16383, { 255, 127, 0, 0 }, 2 }, variableByteInteger);

            Assert::AreEqual(0, MQTT_DecodeVariableByteInteger(Buffer({ 128, 128, 1, 0 }), &variableByteInteger));
            Assert::AreEqual({ 16384, { 128, 128, 1, 0 }, 3 }, variableByteInteger);

            Assert::AreEqual(0, MQTT_DecodeVariableByteInteger(Buffer({ 129, 128, 1, 0 }), &variableByteInteger));
            Assert::AreEqual({ 16385, { 129, 128, 1, 0 }, 3 }, variableByteInteger);

            Assert::AreEqual(0, MQTT_DecodeVariableByteInteger(Buffer({ 255, 255, 127, 0 }), &variableByteInteger));
            Assert::AreEqual({ 2097151, { 255, 255, 127, 0 }, 3 }, variableByteInteger);

            Assert::AreEqual(0, MQTT_DecodeVariableByteInteger(Buffer({ 128, 128, 128, 1 }), &variableByteInteger));
            Assert::AreEqual({ 2097152, { 128, 128, 128, 1 }, 4 }, variableByteInteger);

            Assert::AreEqual(0, MQTT_DecodeVariableByteInteger(Buffer({ 129, 128, 128, 1 }), &variableByteInteger));
            Assert::AreEqual({ 2097153, { 129, 128, 128, 1 }, 4 }, variableByteInteger);

            Assert::AreEqual(0, MQTT_DecodeVariableByteInteger(Buffer({ 255, 255, 255, 127 }), &variableByteInteger));
            Assert::AreEqual({ 268435455, { 255, 255, 255, 127 }, 4 }, variableByteInteger);

            // Varible Byte Integer must be encoded in 1 to 4 bytes
            Assert::AreNotEqual(0, MQTT_DecodeVariableByteInteger(Buffer({ 128, 128, 128, 128 }), &variableByteInteger));

            // if continuation bit (7th bit) is not 0, the next byte must be greater than 0
            Assert::AreNotEqual(0, MQTT_DecodeVariableByteInteger(Buffer({ 128, 0, 0, 0 }), &variableByteInteger));
            Assert::AreNotEqual(0, MQTT_DecodeVariableByteInteger(Buffer({ 128, 128, 0, 0 }), &variableByteInteger));
            Assert::AreNotEqual(0, MQTT_DecodeVariableByteInteger(Buffer({ 128, 128, 128, 0 }), &variableByteInteger));

            // if continuation bit (7th bit) is 0, the next byte is not be read
            Assert::AreEqual(0, MQTT_DecodeVariableByteInteger(Buffer({ 1, 2, 3, 4 }), &variableByteInteger));
            Assert::AreEqual({ 1, { 1, 0, 0, 0 }, 1 }, variableByteInteger);

            Assert::AreEqual(0, MQTT_DecodeVariableByteInteger(Buffer({ 128, 1, 2, 3 }), &variableByteInteger));
            Assert::AreEqual({ 128, { 128, 1, 0, 0 }, 2 }, variableByteInteger);

            Assert::AreEqual(0, MQTT_DecodeVariableByteInteger(Buffer({ 255, 127, 1, 2 }), &variableByteInteger));
            Assert::AreEqual({ 16383, { 255, 127, 0, 0 }, 2 }, variableByteInteger);

            Assert::AreEqual(0, MQTT_DecodeVariableByteInteger(Buffer({ 128, 128, 1, 2 }), &variableByteInteger));
            Assert::AreEqual({ 16384, { 128, 128, 1, 0 }, 3 }, variableByteInteger);
        }
    };
}