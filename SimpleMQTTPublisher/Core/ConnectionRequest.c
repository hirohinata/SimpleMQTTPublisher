#include <memory.h>
#include "ConnectionRequest.h"
#include "ControlPacketFormat.h"

static const unsigned char MQTT_PROTOCOL_VERSION = 5;

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
    return MQTT_WriteNullTerminatedUTF8EncodedString("MQTT", pBuffer, dwBufferLength);
}

/* 3.1.2.2 Protocol Version */
static int MQTT_WriteConnectProtocolVersion(unsigned char* pBuffer, unsigned int dwBufferLength)
{
    if (dwBufferLength < 1) return -1;

    if (pBuffer != NULL)
    {
        *pBuffer = MQTT_PROTOCOL_VERSION;
    }

    return 1;
}

/* 3.1.2.3 Connect Flags */
static int MQTT_WriteConnectFlags(unsigned char* pBuffer, unsigned int dwBufferLength, const MQTT_ConnectPacketOption* pOption)
{
    if (dwBufferLength < 1) return -1;

    if (pBuffer != NULL)
    {
        /* TODO: Support Will Flag */
        *pBuffer =
            (pOption->pszUserName != NULL ? 0x80 : 0x00) |
            (pOption->pPassword != NULL ? 0x40 : 0x00) |
            (pOption->bCleanStart ? 0x02 : 0x00);
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
static int MQTT_WriteConnectVariableHeader(unsigned char* pBuffer, unsigned int dwBufferLength, const MQTT_ConnectPacketOption* pOption)
{
    unsigned int dwRestLength = dwBufferLength;

    {
        const int iProtocolNameLength = MQTT_WriteConnectProtocolName(pBuffer, dwRestLength);
        if (iProtocolNameLength < 0) return -1;
        if (pBuffer != NULL) pBuffer += iProtocolNameLength;
        dwRestLength -= iProtocolNameLength;
    }
    {
        const int iProtocolVersionLength = MQTT_WriteConnectProtocolVersion(pBuffer, dwRestLength);
        if (iProtocolVersionLength < 0) return -1;
        if (pBuffer != NULL) pBuffer += iProtocolVersionLength;
        dwRestLength -= iProtocolVersionLength;
    }
    {
        const int iFlagsLength = MQTT_WriteConnectFlags(pBuffer, dwRestLength, pOption);
        if (iFlagsLength < 0) return -1;
        if (pBuffer != NULL) pBuffer += iFlagsLength;
        dwRestLength -= iFlagsLength;
    }
    {
        const int iKeepAliveLength = MQTT_WriteConnectKeepAlive(pOption->wKeepAlive, pBuffer, dwRestLength);
        if (iKeepAliveLength < 0) return -1;
        if (pBuffer != NULL) pBuffer += iKeepAliveLength;
        dwRestLength -= iKeepAliveLength;
    }
    {
        const int iPropertiesLength = MQTT_WriteConnectProperties(pBuffer, dwRestLength);
        if (iPropertiesLength < 0) return -1;
        if (pBuffer != NULL) pBuffer += iPropertiesLength;
        dwRestLength -= iPropertiesLength;
    }

    return dwBufferLength - dwRestLength;
}

/* 3.1.3.1 Client Identifier (ClientID) */
static int MQTT_WriteConnectClientID(const char* pszClientID, unsigned char* pBuffer, unsigned int dwBufferLength)
{
    return MQTT_WriteNullTerminatedUTF8EncodedString(pszClientID, pBuffer, dwBufferLength);
}

/* 3.1.3.5 User Name */
static int MQTT_WriteConnectUserName(const char* pszUserName, unsigned char* pBuffer, unsigned int dwBufferLength)
{
    if (pszUserName == NULL) return 0;
    return MQTT_WriteNullTerminatedUTF8EncodedString(pszUserName, pBuffer, dwBufferLength);
}

/* 3.1.3.6 Password */
static int MQTT_WriteConnectPassword(const unsigned char* pPassword, unsigned short wPasswordLength, unsigned char* pBuffer, unsigned int dwBufferLength)
{
    if (pPassword == NULL) return 0;
    return MQTT_WriteBinaryData(pPassword, wPasswordLength, pBuffer, dwBufferLength);
}

/* 3.1.3 CONNECT Payload */
static int MQTT_WriteConnectPayload(unsigned char* pBuffer, unsigned int dwBufferLength, const MQTT_ConnectPacketOption* pOption)
{
    unsigned int dwRestLength = dwBufferLength;

    {
        const int iClientIDLength = MQTT_WriteConnectClientID(pOption->pszClientID, pBuffer, dwRestLength);
        if (iClientIDLength < 0) return -1;
        if (pBuffer != NULL) pBuffer += iClientIDLength;
        dwRestLength -= iClientIDLength;
    }
    /* TODO: Support Will Flag */
    {
        const int iUserNameLength = MQTT_WriteConnectUserName(pOption->pszUserName, pBuffer, dwRestLength);
        if (iUserNameLength < 0) return -1;
        if (pBuffer != NULL) pBuffer += iUserNameLength;
        dwRestLength -= iUserNameLength;
    }
    {
        const int iPasswordLength = MQTT_WriteConnectPassword(pOption->pPassword, pOption->wPasswordLength, pBuffer, dwRestLength);
        if (iPasswordLength < 0) return -1;
        if (pBuffer != NULL) pBuffer += iPasswordLength;
        dwRestLength -= iPasswordLength;
    }

    return dwBufferLength - dwRestLength;
}

int MQTT_CreateConnectPacket(unsigned char* pBuffer, unsigned int dwBufferLength, const MQTT_ConnectPacketOption* pOption)
{
    unsigned char* pRemainingLengthBuffer = NULL;
    unsigned int dwRestLength = dwBufferLength;

    if (pOption == NULL) return -1;

    {
        const int iFixedHeaderLength = MQTT_WriteConnectFixedHeader(pBuffer, dwRestLength, &pRemainingLengthBuffer);
        if (iFixedHeaderLength < 0) return -1;
        if (pBuffer != NULL) pBuffer += iFixedHeaderLength;
        dwRestLength -= iFixedHeaderLength;
    }
    {
        const int iVariableHeaderLength = MQTT_WriteConnectVariableHeader(pBuffer, dwRestLength, pOption);
        if (iVariableHeaderLength < 0) return -1;
        if (pBuffer != NULL) pBuffer += iVariableHeaderLength;
        dwRestLength -= iVariableHeaderLength;
    }
    {
        const int iPayloadLength = MQTT_WriteConnectPayload(pBuffer, dwRestLength, pOption);
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
