#include "ControlPacketFormat.h"

MQTT_ControlPacketFlags MQTT_ConstructPublishControlPacketFlags(
    unsigned char dup,
    unsigned char qos,
    unsigned char retain)
{
    return
        ((dup & 0x1) << 3) |
        ((qos & 0x3) << 1) |
        ((retain & 0x1) << 0);
}

void MQTT_DeconstructPublishControlPacketFlags(
    MQTT_ControlPacketFlags flags,
    unsigned char* pDup,
    unsigned char* pQos,
    unsigned char* pRetain)
{
    *pDup = (flags >> 3) & 0x1;
    *pQos = (flags >> 1) & 0x3;
    *pRetain = (flags >> 0) & 0x1;
}