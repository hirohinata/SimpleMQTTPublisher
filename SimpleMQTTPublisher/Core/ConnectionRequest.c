#include <memory.h>
#include "ControlPacketFormat.h"

/* 3.1.1 CONNECT Fixed Header */
static int MQTT_WriteConnectFixedHeader(unsigned char* pBuffer, unsigned int dwBufferLength, unsigned char** ppRemainingLengthField)
{
    /* The Remaining Length is variable length, so it is rewritten later.
       Here, provisionally secure 1 byte area. */

    if (dwBufferLength < 2) return -1;

    if (pBuffer != NULL)
    {
        *pBuffer = (MQTT_CONNECT << 4);
        ++pBuffer;

        if (ppRemainingLengthField != NULL) {
            *ppRemainingLengthField = pBuffer;
        }
        *pBuffer = 0;     /* provisionally */
        ++pBuffer;
    }

    return 2;
}

/* 3.1.2.1 Protocol Name */
static int MQTT_WriteConnectProtocolName(unsigned char* pBuffer, unsigned int dwBufferLength)
{
    return MQTT_WriteUTF8EncodedString("MQTT", pBuffer, dwBufferLength);
}

/* 3.1.2.2 Protocol Version */
static int MQTT_WriteConnectProtocolVersion(unsigned char version, unsigned char* pBuffer, unsigned int dwBufferLength)
{
    if (dwBufferLength < 1) return -1;

    if (pBuffer != NULL)
    {
        *pBuffer = version;
    }

    return 1;
}

/* 3.1.2.3 Connect Flags */
static int MQTT_WriteConnectFlags(unsigned char* pBuffer, unsigned int dwBufferLength)
{
    if (dwBufferLength < 1) return -1;

    // TODO: set flags
    if (pBuffer != NULL)
    {
        *pBuffer = 0;
    }

    return 1;
}

/* 3.1.2.10 Keep Alive */
static int MQTT_WriteConnectKeepAlive(unsigned short wKeepAlive, unsigned char* pBuffer, unsigned int dwBufferLength)
{
    MQTT_TwoByteInteger keepAlive = { 0 };

    if (dwBufferLength < 2) return -1;

    if (MQTT_EncodeTwoByteInteger(wKeepAlive, &keepAlive)) return -1;

    if (pBuffer != NULL)
    {
        memcpy(pBuffer, &keepAlive, 2);
    }

    return 2;
}

/* 3.1.2.11 CONNECT Properties */
static int MQTT_WriteConnectProperties(unsigned char* pBuffer, unsigned int dwBufferLength)
{
    MQTT_VariableByteInteger propertiesLength = { 0 };

    // TODO: set properties
    if (MQTT_EncodeVariableByteInteger(0, &propertiesLength)) return -1;
    if (dwBufferLength < propertiesLength.EncodedByteNum) return -1;

    if (pBuffer != NULL)
    {
        memcpy(pBuffer, propertiesLength.EncodedByte, propertiesLength.EncodedByteNum);
    }

    return propertiesLength.EncodedByteNum;
}

/* 3.1.2 CONNECT Variable Header */
static int MQTT_WriteConnectVariableHeader(unsigned char* pBuffer, unsigned int dwBufferLength)
{
    // TODO: Make it possible to set from outside
    const unsigned char MQTT_PROTOCOL_VERSION = 5;
    unsigned short wKeepAlive = 0;

    unsigned int dwRestLength = dwBufferLength;

    {
        const int iProtocolNameLength = MQTT_WriteConnectProtocolName(pBuffer, dwRestLength);
        if (iProtocolNameLength < 0) return -1;
        pBuffer += iProtocolNameLength;
        dwRestLength -= iProtocolNameLength;
    }
    {
        const int iProtocolVersionLength = MQTT_WriteConnectProtocolVersion(MQTT_PROTOCOL_VERSION, pBuffer, dwRestLength);
        if (iProtocolVersionLength < 0) return -1;
        pBuffer += iProtocolVersionLength;
        dwRestLength -= iProtocolVersionLength;
    }
    {
        const int iFlagsLength = MQTT_WriteConnectFlags(pBuffer, dwRestLength);
        if (iFlagsLength < 0) return -1;
        pBuffer += iFlagsLength;
        dwRestLength -= iFlagsLength;
    }
    {
        const int iKeepAliveLength = MQTT_WriteConnectKeepAlive(wKeepAlive, pBuffer, dwRestLength);
        if (iKeepAliveLength < 0) return -1;
        pBuffer += iKeepAliveLength;
        dwRestLength -= iKeepAliveLength;
    }
    {
        const int iPropertiesLength = MQTT_WriteConnectProperties(pBuffer, dwRestLength);
        if (iPropertiesLength < 0) return -1;
        pBuffer += iPropertiesLength;
        dwRestLength -= iPropertiesLength;
    }

    return dwBufferLength - dwRestLength;
}

/* 3.1.3.1 Client Identifier (ClientID) */
static int MQTT_WriteConnectClientID(const char* pszClientID, unsigned char* pBuffer, unsigned int dwBufferLength)
{
    return MQTT_WriteUTF8EncodedString(pszClientID, pBuffer, dwBufferLength);
}

/* 3.1.3 CONNECT Payload */
static int MQTT_WriteConnectPayload(unsigned char* pBuffer, unsigned int dwBufferLength)
{
    unsigned int dwRestLength = dwBufferLength;

    {
        const int iClientIDLength = MQTT_WriteConnectClientID("SimpleMQTTPublisher", pBuffer, dwRestLength);
        if (iClientIDLength < 0) return -1;
        pBuffer += iClientIDLength;
        dwRestLength -= iClientIDLength;
    }

    // TODO: set other payload

    return dwBufferLength - dwRestLength;
}

int MQTT_CreateConnectPacket(unsigned char* pBuffer, unsigned int dwBufferLength)
{
    unsigned char* pRemainingLengthBuffer = NULL;
    unsigned char* pBufferStart = pBuffer;
    unsigned int dwRestLength = dwBufferLength;

    {
        const int iFixedHeaderLength = MQTT_WriteConnectFixedHeader(pBuffer, dwRestLength, &pRemainingLengthBuffer);
        if (iFixedHeaderLength < 0) return -1;
        if (pBuffer != NULL) pBuffer += iFixedHeaderLength;
        dwRestLength -= iFixedHeaderLength;
    }
    {
        const int iVariableHeaderLength = MQTT_WriteConnectVariableHeader(pBuffer, dwRestLength);
        if (iVariableHeaderLength < 0) return -1;
        if (pBuffer != NULL) pBuffer += iVariableHeaderLength;
        dwRestLength -= iVariableHeaderLength;
    }
    {
        const int iPayloadLength = MQTT_WriteConnectPayload(pBuffer, dwRestLength);
        if (iPayloadLength < 0) return -1;
        if (pBuffer != NULL) pBuffer += iPayloadLength;
        dwRestLength -= iPayloadLength;
    }

    /* Write Remaining Length */
    {
        MQTT_VariableByteInteger remainingLength = { 0 };
        unsigned int dwRemainingLength = dwBufferLength - dwRestLength - 2;

        if (MQTT_EncodeVariableByteInteger(dwRemainingLength, &remainingLength)) return -1;
        if (dwRestLength < remainingLength.EncodedByteNum - 1) return -1;

        if (pRemainingLengthBuffer != NULL)
        {
            if (1 < remainingLength.EncodedByteNum)
            {
                memmove(pRemainingLengthBuffer + remainingLength.EncodedByteNum, pRemainingLengthBuffer + 1, dwRemainingLength);
            }
            memcpy(pRemainingLengthBuffer, remainingLength.EncodedByte, remainingLength.EncodedByteNum);
        }
        dwRestLength -= remainingLength.EncodedByteNum - 1;
    }

    return dwBufferLength - dwRestLength;
}
