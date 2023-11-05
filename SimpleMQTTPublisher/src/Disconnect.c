#include "Disconnect.h"
#include "common/ControlPacketFormat.h"
#include "common/Utility.h"

/* 3.14.1 DISCONNECT Fixed Header */
static int MQTT_WriteDisconnectFixedHeader(unsigned char* pBuffer, unsigned int dwBufferLength, unsigned char** ppRemainingLengthField)
{
    /* The Remaining Length is variable length, so it is rewritten later.
       Here, provisionally secure 1 byte area. */

    if (dwBufferLength < 2) return -1;

    if (pBuffer != 0)
    {
        *pBuffer = (MQTT_DISCONNECT << 4);
        ++pBuffer;

        if (ppRemainingLengthField != 0) {
            *ppRemainingLengthField = pBuffer;
        }
        *pBuffer = 0;     /* provisionally */
        ++pBuffer;
    }

    return 2;
}

int MQTT_CreateDisconnectPacket(unsigned char* pBuffer, unsigned int dwBufferLength)
{
    unsigned char* pRemainingLengthBuffer = 0;
    unsigned int dwRestLength = dwBufferLength;

    {
        const int iFixedHeaderLength = MQTT_WriteDisconnectFixedHeader(pBuffer, dwRestLength, &pRemainingLengthBuffer);
        if (iFixedHeaderLength < 0) return -1;
        if (pBuffer != 0) pBuffer += iFixedHeaderLength;
        dwRestLength -= iFixedHeaderLength;
    }

    /* Write Remaining Length */
    {
        int iAppendedByteNum = MQTT_WriteRemainingLength(pRemainingLengthBuffer, dwBufferLength, dwRestLength);
        if (iAppendedByteNum < 0) return -1;
        dwRestLength -= iAppendedByteNum;
    }

    return dwBufferLength - dwRestLength;
}
