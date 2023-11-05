#pragma once

typedef struct
{
    /* Null terminated utf-8 string (Recommend less than 23 bytes). */
    const char* pszClientID;

    /* Null terminated utf-8 string. Empty string is also valid. */
    const char* pszUserName;

    /* 0 to 65535 Bytes binary data. */
    const unsigned char* pPassword;
    unsigned short wPasswordLength;

    /* A time interval measured in seconds. 0 effects turning off this mechanism. */
    unsigned short wKeepAlive;

    /* Boolean value (0 or 1). */
    unsigned char bCleanStart;

    /* TODO: Support Will Flag */
    /* TODO: Support Properties */
    unsigned char reserved[3];
} MQTT_ConnectPacketOption;

int MQTT_CreateConnectPacket(unsigned char* pBuffer, unsigned int dwBufferLength, const MQTT_ConnectPacketOption* pOption);
