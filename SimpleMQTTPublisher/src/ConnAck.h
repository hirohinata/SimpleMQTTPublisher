#pragma once

typedef enum
{
    MQTT_ConnAck_Success = 0,
    MQTT_ConnAck_UnspecifiedError = 128,
    MQTT_ConnAck_MalformedPacket = 129,
    MQTT_ConnAck_ProtocolError = 130,
    MQTT_ConnAck_ImplementationSpecificError = 131,
    MQTT_ConnAck_UnsupportedProtocolVersion = 132,
    MQTT_ConnAck_ClientIdentifierNotValid = 133,
    MQTT_ConnAck_BadUserNameOrPassword = 134,
    MQTT_ConnAck_NotAuthorized = 135,
    MQTT_ConnAck_ServerUnavailable = 136,
    MQTT_ConnAck_ServerBusy = 137,
    MQTT_ConnAck_Banned = 138,
    MQTT_ConnAck_BadAuthenticationMethod = 140,
    MQTT_ConnAck_TopicNameInvalid = 144,
    MQTT_ConnAck_PacketTooLarge = 149,
    MQTT_ConnAck_QuotaExceeded = 151,
    MQTT_ConnAck_PayloadFormatInvalid = 153,
    MQTT_ConnAck_RetainNotSupported = 154,
    MQTT_ConnAck_QoSNotSupported = 155,
    MQTT_ConnAck_UseAnotherServer = 156,
    MQTT_ConnAck_ServerMoved = 157,
    MQTT_ConnAck_ConnectionRateExceeded = 159,
} MQTT_ConnectReasonCode;

typedef struct
{
    MQTT_ConnectReasonCode connectReasonCode;

    /* Boolean value (0 or 1). */
    unsigned char bSessionPresent;
    unsigned char reserved[3];
} MQTT_ConnectAckResult;

int MQTT_ParseConnectAckPacket(const unsigned char* pBuffer, unsigned int dwBufferLength, MQTT_ConnectAckResult* pResult);
