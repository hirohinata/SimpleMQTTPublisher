#include "ConnectAcknowledgement.h"
#include "ControlPacketFormat.h"

/* 3.2.1 CONNACK Fixed Header */
static int MQTT_ParseConnAckFixedHeader(unsigned char* pBuffer, unsigned int dwBufferLength)
{
    MQTT_VariableByteInteger remainingLength = { 0 };

    if (pBuffer == 0 || dwBufferLength < 2) return -1;

    if (((pBuffer[0] >> 4) & 0x0F) != MQTT_CONNACK) return -1;

    if (MQTT_DecodeVariableByteInteger(&pBuffer[1], &remainingLength) != 0) return -1;
    if (dwBufferLength < 1 + remainingLength.EncodedByteNum + remainingLength.Value) return -1;

    return 1 + remainingLength.EncodedByteNum;
}

/* 3.2.2.3 CONNACK Properties */
static int MQTT_ParseConnAckProperties(unsigned char* pBuffer, unsigned int dwBufferLength)
{
    MQTT_VariableByteInteger propertiesLength = { 0 };
    unsigned int dwRestLength = dwBufferLength;

    if (pBuffer == 0 || dwBufferLength < 1) return -1;

    if (MQTT_DecodeVariableByteInteger(pBuffer, &propertiesLength) != 0) return -1;
    if (dwBufferLength < propertiesLength.EncodedByteNum + propertiesLength.Value) return -1;

    pBuffer += propertiesLength.EncodedByteNum;
    dwRestLength -= propertiesLength.EncodedByteNum;

    /* TODO: parse properties */
    return propertiesLength.EncodedByteNum + propertiesLength.Value;
}

/* 3.2.2 CONNACK Variable Header */
static int MQTT_ParseConnAckVariableHeader(unsigned char* pBuffer, unsigned int dwBufferLength, MQTT_ConnectAckResult* pResult)
{
    int iPropertiesLength = 0;

    if (pBuffer == 0 || dwBufferLength < 3) return -1;

    pResult->bSessionPresent = (pBuffer[0] & 0x01) ? 1 : 0;
    pResult->connectReasonCode = (MQTT_ConnectReasonCode)pBuffer[1];

    iPropertiesLength = MQTT_ParseConnAckProperties(&pBuffer[2], dwBufferLength - 2);
    if (iPropertiesLength < 0) return -1;
    return 2 + iPropertiesLength;
}

int MQTT_ParseConnectAckPacket(unsigned char* pBuffer, unsigned int dwBufferLength, MQTT_ConnectAckResult* pResult)
{
    unsigned int dwRestLength = dwBufferLength;

    if (pBuffer == 0 || pResult == 0) return -1;

    {
        const int iFixedHeaderLength = MQTT_ParseConnAckFixedHeader(pBuffer, dwRestLength);
        if (iFixedHeaderLength < 0) return -1;
        pBuffer += iFixedHeaderLength;
        dwRestLength -= iFixedHeaderLength;
    }
    {
        const int iVariableHeaderLength = MQTT_ParseConnAckVariableHeader(pBuffer, dwRestLength, pResult);
        if (iVariableHeaderLength < 0) return -1;
        pBuffer += iVariableHeaderLength;
        dwRestLength -= iVariableHeaderLength;
    }

    return dwBufferLength - dwRestLength;
}
