#include <memory.h>
#include "Utility.h"
#include "ControlPacketFormat.h"

int MQTT_WriteRemainingLength(unsigned char* pRemainingLengthBuffer, unsigned int dwBufferLength, unsigned int dwRestLength)
{
    MQTT_VariableByteInteger remainingLength = { 0 };
    unsigned int dwRemainingLength = dwBufferLength - dwRestLength - 2;

    if (MQTT_EncodeVariableByteInteger(dwRemainingLength, &remainingLength)) return -1;
    if (dwRestLength < remainingLength.EncodedByteNum - 1) return -1;

    if (pRemainingLengthBuffer != 0)
    {
        if (1 < remainingLength.EncodedByteNum)
        {
            memmove(pRemainingLengthBuffer + remainingLength.EncodedByteNum, pRemainingLengthBuffer + 1, dwRemainingLength);
        }
        memcpy(pRemainingLengthBuffer, remainingLength.EncodedByte, remainingLength.EncodedByteNum);
    }

    return remainingLength.EncodedByteNum - 1;
}
