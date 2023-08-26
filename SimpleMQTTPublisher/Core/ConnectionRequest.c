#include <memory.h>
#include "ControlPacketFormat.h"

/* 3.1.1 CONNECT Fixed Header */
static int MQTT_WriteConnectFixedHeader(unsigned char** ppBuffer, unsigned int* pRestLength, unsigned char** ppRemainingLength)
{
    /* The Remaining Length is variable length, so it is rewritten later.
       Here, provisionally secure 1 byte area. */

    if (*pRestLength < 2) return -1;

    **ppBuffer = (MQTT_CONNECT << 4);
    ++*ppBuffer;

    *ppRemainingLength = *ppBuffer;
    **ppBuffer = 0;     /* provisionally */
    ++*ppBuffer;

    *pRestLength -= 2;
    return 0;
}

/* 3.1.2.1 Protocol Name */
static int MQTT_WriteConnectProtocolName(unsigned char** ppBuffer, unsigned int* pRestLength)
{
    return MQTT_WriteUTF8EncodedString("MQTT", ppBuffer, pRestLength);
}

/* 3.1.2.2 Protocol Version */
static int MQTT_WriteConnectProtocolVersion(unsigned char version, unsigned char** ppBuffer, unsigned int* pRestLength)
{
    if (*pRestLength < 1) return -1;

    **ppBuffer = version;
    ++*ppBuffer;

    *pRestLength -= 1;
    return 0;
}

/* 3.1.2.3 Connect Flags */
static int MQTT_WriteConnectFlags(unsigned char** ppBuffer, unsigned int* pRestLength)
{
    if (*pRestLength < 1) return -1;

    // TODO: set flags
    **ppBuffer = 0;
    ++*ppBuffer;

    *pRestLength -= 1;
    return 0;
}

/* 3.1.2.10 Keep Alive */
static int MQTT_WriteConnectKeepAlive(unsigned short wKeepAlive, unsigned char** ppBuffer, unsigned int* pRestLength)
{
    MQTT_TwoByteInteger keepAlive = { 0 };

    if (*pRestLength < 2) return -1;

    if (MQTT_EncodeTwoByteInteger(wKeepAlive, &keepAlive)) return -1;
    memcpy(*ppBuffer, &keepAlive, 2);
    *ppBuffer += 2;

    *pRestLength -= 2;
    return 0;
}

/* 3.1.2.11 CONNECT Properties */
static int MQTT_WriteConnectProperties(unsigned char** ppBuffer, unsigned int* pRestLength)
{
    MQTT_VariableByteInteger propertiesLength = { 0 };

    // TODO: set properties
    if (MQTT_EncodeVariableByteInteger(0, &propertiesLength)) return -1;
    if (*pRestLength < propertiesLength.EncodedByteNum) return -1;

    memcpy(*ppBuffer, propertiesLength.EncodedByte, propertiesLength.EncodedByteNum);
    *ppBuffer += propertiesLength.EncodedByteNum;

    *pRestLength -= propertiesLength.EncodedByteNum;
    return 0;
}

/* 3.1.2 CONNECT Variable Header */
static int MQTT_WriteConnectVariableHeader(unsigned char** ppBuffer, unsigned int* pRestLength)
{
    // TODO: Make it possible to set from outside
    const unsigned char MQTT_PROTOCOL_VERSION = 5;
    unsigned short wKeepAlive = 0;

    if (MQTT_WriteConnectProtocolName(ppBuffer, pRestLength)) return -1;
    if (MQTT_WriteConnectProtocolVersion(MQTT_PROTOCOL_VERSION, ppBuffer, pRestLength)) return -1;
    if (MQTT_WriteConnectFlags(ppBuffer, pRestLength)) return -1;
    if (MQTT_WriteConnectKeepAlive(wKeepAlive, ppBuffer, pRestLength)) return -1;
    if (MQTT_WriteConnectProperties(ppBuffer, pRestLength)) return -1;

    return 0;
}

/* 3.1.3.1 Client Identifier (ClientID) */
static int MQTT_WriteConnectClientID(const char* pszClientID, unsigned char** ppBuffer, unsigned int* pRestLength)
{
    return MQTT_WriteUTF8EncodedString(pszClientID, ppBuffer, pRestLength);
}

/* 3.1.3 CONNECT Payload */
static int MQTT_WriteConnectPayload(unsigned char** ppBuffer, unsigned int* pRestLength)
{
    if (MQTT_WriteConnectClientID("SimpleMQTTPublisher", ppBuffer, pRestLength)) return -1;
    // TODO: set other payload

    return 0;
}

int MQTT_CreateConnectPacket(unsigned char* pBuffer, unsigned int dwBufferLength)
{
    MQTT_VariableByteInteger remainingLength = { 0 };
    unsigned char* pRemainingLengthBuffer = 0;
    unsigned char* pBufferStart = pBuffer;
    unsigned int dwRestLength = dwBufferLength;
    unsigned int dwRemainingLength = 0;

    if (MQTT_WriteConnectFixedHeader(&pBuffer, &dwRestLength, &pRemainingLengthBuffer)) return -1;
    if (MQTT_WriteConnectVariableHeader(&pBuffer, &dwRestLength)) return -1;
    if (MQTT_WriteConnectPayload(&pBuffer, &dwRestLength)) return -1;

    /* Write Remaining Length */
    dwRemainingLength = dwBufferLength - dwRestLength - (unsigned int)(pRemainingLengthBuffer + 1 - pBufferStart);
    if (MQTT_EncodeVariableByteInteger(dwRemainingLength, &remainingLength)) return -1;
    if (dwRestLength < remainingLength.EncodedByteNum - 1) return -1;
    if (1 < remainingLength.EncodedByteNum)
    {
        memmove(pRemainingLengthBuffer + remainingLength.EncodedByteNum, pRemainingLengthBuffer + 1, dwRemainingLength);
    }
    memcpy(pRemainingLengthBuffer, remainingLength.EncodedByte, remainingLength.EncodedByteNum);
    dwRestLength += remainingLength.EncodedByteNum - 1;

    return dwBufferLength - dwRestLength;
}
