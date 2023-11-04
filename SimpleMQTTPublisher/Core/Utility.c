#include <memory.h>
#include "Utility.h"
#include "ControlPacketFormat.h"

int MQTT_WriteRemainingLength(unsigned char* pRemainingLengthBuffer, unsigned int dwBufferLength, unsigned int dwRestLength)
{
    MQTT_VariableByteInteger remainingLength = { 0 };

    /* It is secured the remaining length min size (1Byte).
       dwRemainingLength = the wrote size (dwBufferLength - dwRestLength) - the secured size (1Byte) - the packet fixed header size (1Byte).*/
    const unsigned int dwRemainingLength = dwBufferLength - dwRestLength - 2;

    /* Encode the remaining length as a variable byte integer.
       If the encoding fails, return -1 to indicate an error. */
    if (MQTT_EncodeVariableByteInteger(dwRemainingLength, &remainingLength)) return -1;

    /* Check if there is enough space in the buffer to write the encoded remaining length.
       If there isn't, return -1 to indicate an error.
       Already 1Byte is secured, so compare with EncodedByteNum - 1. */
    if (dwRestLength < remainingLength.EncodedByteNum - 1) return -1;

    if (pRemainingLengthBuffer != 0)
    {
        /* Only 1 Byte is secured, so if write 2 Bytes or more, the memory after that needs to move. */
        if (1 < remainingLength.EncodedByteNum)
        {
            memmove(pRemainingLengthBuffer + remainingLength.EncodedByteNum, pRemainingLengthBuffer + 1, dwRemainingLength);
        }

        /* Write the encoded remaining length to the buffer. */
        memcpy(pRemainingLengthBuffer, remainingLength.EncodedByte, remainingLength.EncodedByteNum);
    }

    /* Return the number of additional bytes written to the buffer. */
    return remainingLength.EncodedByteNum - 1;
}
