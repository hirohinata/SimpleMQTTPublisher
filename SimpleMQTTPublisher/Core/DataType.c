#include "DataType.h"

void MQTT_EncodeVariableByteInteger(
    unsigned int value,
    MQTT_VariableByteInteger* pVariableByteInteger)
{
    pVariableByteInteger->Value = value;
    pVariableByteInteger->EncodedByte[0] = 0;
    pVariableByteInteger->EncodedByte[1] = 0;
    pVariableByteInteger->EncodedByte[2] = 0;
    pVariableByteInteger->EncodedByte[3] = 0;
    pVariableByteInteger->EncodedByteNum = 0;

    do {
        const int index = pVariableByteInteger->EncodedByteNum;
        pVariableByteInteger->EncodedByte[index] = value % 128;
        value = value / 128;

        // if there are more data to encode, set the top bit of this byte
        if (0 < value)
        {
            pVariableByteInteger->EncodedByte[index] |= 128;
        }

        ++pVariableByteInteger->EncodedByteNum;
    } while (0 < value);
}

void MQTT_DecodeVariableByteInteger(
    unsigned char* pBuffer,
    MQTT_VariableByteInteger* pVariableByteInteger)
{
    unsigned int multiplier = 1;
    unsigned char encodedByte = 0;

    pVariableByteInteger->Value = 0;
    pVariableByteInteger->EncodedByte[0] = 0;
    pVariableByteInteger->EncodedByte[1] = 0;
    pVariableByteInteger->EncodedByte[2] = 0;
    pVariableByteInteger->EncodedByte[3] = 0;
    pVariableByteInteger->EncodedByteNum = 0;

    do
    {
        encodedByte = *pBuffer;
        pVariableByteInteger->EncodedByte[pVariableByteInteger->EncodedByteNum] = encodedByte;
        pVariableByteInteger->Value += (encodedByte & 127) * multiplier;
        multiplier *= 128;
        ++pBuffer;
        ++pVariableByteInteger->EncodedByteNum;
    } while ((encodedByte & 128) != 0);
}
