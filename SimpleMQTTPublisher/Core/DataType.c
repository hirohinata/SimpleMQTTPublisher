#include "DataType.h"

int MQTT_EncodeVariableByteInteger(
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
        if (4 <= index) return -1;

        pVariableByteInteger->EncodedByte[index] = value % 128;
        value = value / 128;

        // if there are more data to encode, set the top bit of this byte
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
