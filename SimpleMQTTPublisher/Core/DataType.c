#include <string.h>
#include "DataType.h"

int MQTT_EncodeTwoByteInteger(
    unsigned int value,
    MQTT_TwoByteInteger* pTwoByteInteger)
{
    if (pTwoByteInteger == 0) return -1;
    if (65535 < value) return -1;

    pTwoByteInteger->MSB = (value >> 8) & 0xFF;
    pTwoByteInteger->LSB = (value >> 0) & 0xFF;
    return 0;
}

int MQTT_DecodeTwoByteInteger(
    const unsigned char* pBuffer,
    MQTT_TwoByteInteger* pTwoByteInteger)
{
    if (pBuffer == 0 || pTwoByteInteger == 0) return -1;

    pTwoByteInteger->MSB = pBuffer[0];
    pTwoByteInteger->LSB = pBuffer[1];
    return 0;
}

int MQTT_WriteUTF8EncodedString(const char* pszString, unsigned char* pBuffer, unsigned int dwBufferLength)
{
    MQTT_TwoByteInteger protocolNameLength = { 0 };
    const unsigned int stringLength = (unsigned int)strlen(pszString);

    if (MQTT_EncodeTwoByteInteger(stringLength, &protocolNameLength)) return -1;

    if (pBuffer != NULL)
    {
        if (dwBufferLength < 2 + stringLength) return -1;

        memcpy(pBuffer, &protocolNameLength, 2);
        memcpy(pBuffer + 2, pszString, stringLength);
    }

    return 2 + stringLength;
}

int MQTT_EncodeVariableByteInteger(
    unsigned int value,
    MQTT_VariableByteInteger* pVariableByteInteger)
{
    if (pVariableByteInteger == 0) return -1;

    pVariableByteInteger->Value = value;
    pVariableByteInteger->EncodedByte[0] = 0;
    pVariableByteInteger->EncodedByte[1] = 0;
    pVariableByteInteger->EncodedByte[2] = 0;
    pVariableByteInteger->EncodedByte[3] = 0;
    pVariableByteInteger->EncodedByteNum = 0;

    do {
        const unsigned int index = pVariableByteInteger->EncodedByteNum;
        if (4 <= index) return -1;

        pVariableByteInteger->EncodedByte[index] = value % 128;
        value = value / 128;

        /* if there are more data to encode, set the top bit of this byte */
        if (0 < value)
        {
            pVariableByteInteger->EncodedByte[index] |= 128;
        }

        ++pVariableByteInteger->EncodedByteNum;
    } while (0 < value);

    return 0;
}

int MQTT_DecodeVariableByteInteger(
    const unsigned char* pBuffer,
    MQTT_VariableByteInteger* pVariableByteInteger)
{
    unsigned int multiplier = 1;
    unsigned char encodedByte = 0;

    if (pBuffer == 0 || pVariableByteInteger == 0) return -1;

    pVariableByteInteger->Value = 0;
    pVariableByteInteger->EncodedByte[0] = 0;
    pVariableByteInteger->EncodedByte[1] = 0;
    pVariableByteInteger->EncodedByte[2] = 0;
    pVariableByteInteger->EncodedByte[3] = 0;
    pVariableByteInteger->EncodedByteNum = 0;

    do
    {
        encodedByte = *pBuffer;
        if (encodedByte == 0 && 1 < multiplier) return -1;

        pVariableByteInteger->EncodedByte[pVariableByteInteger->EncodedByteNum] = encodedByte;
        pVariableByteInteger->Value += (encodedByte & 127) * multiplier;
        multiplier *= 128;
        ++pBuffer;
        ++pVariableByteInteger->EncodedByteNum;

        if (4 < pVariableByteInteger->EncodedByteNum) return -1;
    } while ((encodedByte & 128) != 0);

    return 0;
}
