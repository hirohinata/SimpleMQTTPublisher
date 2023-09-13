#pragma pack(1)

typedef unsigned char MQTT_Byte;

/* 1.5.1 Bits */
typedef struct
{
    unsigned char Bit0 : 1;
    unsigned char Bit1 : 1;
    unsigned char Bit2 : 1;
    unsigned char Bit3 : 1;
    unsigned char Bit4 : 1;
    unsigned char Bit5 : 1;
    unsigned char Bit6 : 1;
    unsigned char Bit7 : 1;
} MQTT_Bits;

/* 1.5.2 Two Byte Integer */
typedef struct
{
    unsigned char MSB;
    unsigned char LSB;
} MQTT_TwoByteInteger;

int MQTT_EncodeTwoByteInteger(
    unsigned int value,
    MQTT_TwoByteInteger* pTwoByteInteger);

int MQTT_DecodeTwoByteInteger(
    const unsigned char* pBuffer,
    MQTT_TwoByteInteger* pTwoByteInteger);

/* 1.5.3 Four Byte Integer */
typedef struct
{
    unsigned char MSB1;
    unsigned char MSB2;
    unsigned char MSB3;
    unsigned char LSB;
} MQTT_FourByteInteger;

/* 1.5.4 UTF-8 Encoded String */
typedef struct
{
    MQTT_TwoByteInteger Length;
    unsigned char* pString;     /* UTF-8 encoded character data, if length > 0. */
} MQTT_UTF8EncodedString;

int MQTT_WriteUTF8EncodedString(const char* pszString, unsigned char* pBuffer, unsigned int dwBufferLength);

/* 1.5.5 Variable Byte Integer */
typedef struct
{
    unsigned int Value;
    unsigned char EncodedByte[4];
    unsigned int EncodedByteNum;
} MQTT_VariableByteInteger;

int MQTT_EncodeVariableByteInteger(
    unsigned int value,
    MQTT_VariableByteInteger* pVariableByteInteger);

int MQTT_DecodeVariableByteInteger(
    const unsigned char* pBuffer,
    MQTT_VariableByteInteger* pVariableByteInteger);

/* 1.5.6 Binary Data */
typedef struct
{
    MQTT_TwoByteInteger Length;
    unsigned char* pData;       /* 0 to 65,535 Bytes */
} MQTT_BinaryData;

/* 1.5.7 UTF-8 String Pair */
typedef struct
{
    MQTT_UTF8EncodedString Name;
    MQTT_UTF8EncodedString Value;
} MQTT_UTF8StringPair;

#pragma pack()
