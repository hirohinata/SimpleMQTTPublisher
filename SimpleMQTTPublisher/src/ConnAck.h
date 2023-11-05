#pragma once

typedef enum
{
    Success = 0,
    UnspecifiedError = 128,
    MalformedPacket = 129,
    ProtocolError = 130,
    ImplementationSpecificError = 131,
    UnsupportedProtocolVersion = 132,
    ClientIdentifierNotValid = 133,
    BadUserNameOrPassword = 134,
    NotAuthorized = 135,
    ServerUnavailable = 136,
    ServerBusy = 137,
    Banned = 138,
    BadAuthenticationMethod = 140,
    TopicNameInvalid = 144,
    PacketTooLarge = 149,
    QuotaExceeded = 151,
    PayloadFormatInvalid = 153,
    RetainNotSupported = 154,
    QoSNotSupported = 155,
    UseAnotherServer = 156,
    ServerMoved = 157,
    ConnectionRateExceeded = 159,
} MQTT_ConnectReasonCode;

typedef struct
{
    MQTT_ConnectReasonCode connectReasonCode;

    /* Boolean value (0 or 1). */
    unsigned char bSessionPresent;
    unsigned char reserved[3];
} MQTT_ConnectAckResult;

int MQTT_ParseConnectAckPacket(unsigned char* pBuffer, unsigned int dwBufferLength, MQTT_ConnectAckResult* pResult);
