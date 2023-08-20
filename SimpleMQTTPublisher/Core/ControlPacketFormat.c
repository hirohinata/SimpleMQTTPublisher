#include "ControlPacketFormat.h"

int MQTT_EncodePublishControlPacketFlags(
    unsigned char dup,
    unsigned char qos,
    unsigned char retain,
    MQTT_ControlPacketFlags* pFlags)
{
    if (1 < dup || 3 < qos || 1 < retain) return -1;

    // QoS 3 is reserved
    if (qos == 3) return -1;

    *pFlags =
        ((dup & 0x1) << 3) |
        ((qos & 0x3) << 1) |
        ((retain & 0x1) << 0);
    return 0;
}

int MQTT_DecodePublishControlPacketFlags(
    MQTT_ControlPacketFlags flags,
    unsigned char* pDup,
    unsigned char* pQos,
    unsigned char* pRetain)
{
    if (0x0F <= flags) return -1;

    *pDup = (flags >> 3) & 0x1;
    *pQos = (flags >> 1) & 0x3;
    *pRetain = (flags >> 0) & 0x1;

    // QoS 3 is reserved
    if (*pQos == 3) return -1;

    return 0;
}