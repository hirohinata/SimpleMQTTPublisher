#pragma once

#include "DataType.h"

#pragma pack(1)

/* 2.1.2 MQTT Control Packet type */
typedef enum
{
    MQTT_RESERVED = 0,
    MQTT_CONNECT = 1,
    MQTT_CONNACK = 2,
    MQTT_PUBLISH = 3,
    MQTT_PUBACK = 4,
    MQTT_PUBREC = 5,
    MQTT_PUBREL = 6,
    MQTT_PUBCOMP = 7,
    MQTT_SUBSCRIBE = 8,
    MQTT_SUBACK = 9,
    MQTT_UNSUBSCRIBE = 10,
    MQTT_UNSUBACK = 11,
    MQTT_PINGREQ = 12,
    MQTT_PINGRESP = 13,
    MQTT_DISCONNECT = 14,
    MQTT_AUTH = 15
} MQTT_ControlPacketType;

/* 2.1.3 Flags */
typedef unsigned char MQTT_ControlPacketFlags;

int MQTT_EncodePublishControlPacketFlags(
    unsigned char dup,
    unsigned char qos,
    unsigned char retain,
    MQTT_ControlPacketFlags* pFlags);

int MQTT_DecodePublishControlPacketFlags(
    MQTT_ControlPacketFlags flags,
    unsigned char* pDup,
    unsigned char* pQos,
    unsigned char* pRetain);

/* 2.1.1 Fixed Header */
typedef struct
{
    MQTT_ControlPacketType ControlPacketType : 4;
    MQTT_ControlPacketFlags ControlPacketFlags : 4;
    unsigned char RemainingLength[4];       /* Variable Byte Integer(1 to 4 Bytes) */
} MQTT_FixedHeader;

/* 2.2.1 Packet Identifier */
typedef MQTT_TwoByteInteger MQTT_PacketIdentifier;

/* 2.2.2 Properties */
/* Image Code:
    typedef struct
    {
        VariableByteInteger PropertyLength;
        Property Properties[];
    } MQTT_Properties;

    typedef struct
    {
        VariableByteInteger PropertyIdentifier;
        Variant PropertyValue;      // See Table 2-4
    } Property;
*/

/* 2.2.2.1 Property Length */
typedef MQTT_TwoByteInteger MQTT_PropertyLength;

/* 2.2.2.2 Property */
typedef enum
{
    MQTT_PAYLOAD_FORMAT_INDICATOR = 1,
    MQTT_MESSAGE_EXPIRY_INTERVAL = 2,
    MQTT_CONTENT_TYPE = 3,
    MQTT_RESPONSE_TOPIC = 8,
    MQTT_CORRELATION_DATA = 9,
    MQTT_SUBSCRIPTION_IDENTIFIER = 11,
    MQTT_SESSION_EXPIRY_INTERVAL = 17,
    MQTT_ASSIGNED_CLIENT_IDENTIFIER = 18,
    MQTT_SERVER_KEEP_ALIVE = 19,
    MQTT_AUTHENTICATION_METHOD = 21,
    MQTT_AUTHENTICATION_DATA = 22,
    MQTT_REQUEST_PROBLEM_INFORMATION = 23,
    MQTT_WILL_DELAY_INTERVAL = 24,
    MQTT_REQUEST_RESPONSE_INFORMATION = 25,
    MQTT_RESPONSE_INFORMATION = 26,
    MQTT_SERVER_REFERENCE = 28,
    MQTT_REASON_STRING = 31,
    MQTT_RECEIVE_MAXIMUM = 33,
    MQTT_TOPIC_ALIAS_MAXIMUM = 34,
    MQTT_TOPIC_ALIAS = 35,
    MQTT_MAXIMUM_QOS = 36,
    MQTT_RETAIN_AVAILABLE = 37,
    MQTT_USER_PROPERTY = 38,
    MQTT_MAXIMUM_PACKET_SIZE = 39,
    MQTT_WILDCARD_SUBSCRIPTION_AVAILABLE = 40,
    MQTT_SUBSCRIPTION_IDENTIFIER_AVAILABLE = 41,
    MQTT_SHARED_SUBSCRIPTION_AVAILABLE = 42
} MQTT_PropertyIdentifier;

typedef MQTT_Byte MQTT_PayloadFormatIndicator;
typedef MQTT_FourByteInteger MQTT_MessageExpiryInterval;
typedef MQTT_UTF8EncodedString MQTT_ContentType;
typedef MQTT_UTF8EncodedString MQTT_ResponseTopic;
typedef MQTT_BinaryData MQTT_CorrelationData;
typedef MQTT_VariableByteInteger MQTT_SubscriptionIdentifier;
typedef MQTT_FourByteInteger MQTT_SessionExpiryInterval;
typedef MQTT_UTF8EncodedString MQTT_AssignedClientIdentifier;
typedef MQTT_TwoByteInteger MQTT_ServerKeepAlive;
typedef MQTT_UTF8EncodedString MQTT_AuthenticationMethod;
typedef MQTT_BinaryData MQTT_AuthenticationData;
typedef MQTT_Byte MQTT_RequestProblemInformation;
typedef MQTT_FourByteInteger MQTT_WillDelayInterval;
typedef MQTT_Byte MQTT_RequestResponseInformation;
typedef MQTT_UTF8EncodedString MQTT_ResponseInformation;
typedef MQTT_UTF8EncodedString MQTT_ServerReference;
typedef MQTT_UTF8EncodedString MQTT_ReasonString;
typedef MQTT_TwoByteInteger MQTT_ReceiveMaximum;
typedef MQTT_TwoByteInteger MQTT_TopicAliasMaximum;
typedef MQTT_TwoByteInteger MQTT_TopicAlias;
typedef MQTT_Byte MQTT_MaximumQoS;
typedef MQTT_Byte MQTT_RetainAvailable;
typedef MQTT_UTF8StringPair MQTT_UserProperty;
typedef MQTT_FourByteInteger MQTT_MaximumPacketSize;
typedef MQTT_Byte MQTT_WildcardSubscriptionAvailable;
typedef MQTT_Byte MQTT_SubscriptionIdentifierAvailable;
typedef MQTT_Byte MQTT_SharedSubscriptionAvailable;

/* 2.4 Reason Code */
typedef enum 
{
    MQTT_SUCCESS = 0,
    MQTT_NORMAL_DISCONNECTION = 0,
    MQTT_GRANTED_QOS_0 = 0,
    MQTT_GRANTED_QOS_1 = 1,
    MQTT_GRANTED_QOS_2 = 2,
    MQTT_DISCONNECT_WITH_WILL_MESSAGE = 4,
    MQTT_NO_MATCHING_SUBSCRIBERS = 16,
    MQTT_NO_SUBSCRIPTION_EXISTED = 17,
    MQTT_CONTINUE_AUTHENTICATION = 24,
    MQTT_RE_AUTHENTICATE = 25,
    MQTT_UNSPECIFIED_ERROR = 128,
    MQTT_MALFORMED_PACKET = 129,
    MQTT_PROTOCOL_ERROR = 130,
    MQTT_IMPLEMENTATION_SPECIFIC_ERROR = 131,
    MQTT_UNSUPPORTED_PROTOCOL_VERSION = 132,
    MQTT_CLIENT_IDENTIFIER_NOT_VALID = 133,
    MQTT_BAD_USER_NAME_OR_PASSWORD = 134,
    MQTT_NOT_AUTHORIZED = 135,
    MQTT_SERVER_UNAVAILABLE = 136,
    MQTT_SERVER_BUSY = 137,
    MQTT_BANNED = 138,
    MQTT_SERVER_SHUTTING_DOWN = 139,
    MQTT_BAD_AUTHENTICATION_METHOD = 140,
    MQTT_KEEP_ALIVE_TIMEOUT = 141,
    MQTT_SESSION_TAKEN_OVER = 142,
    MQTT_TOPIC_FILTER_INVALID = 143,
    MQTT_TOPIC_NAME_INVALID = 144,
    MQTT_PACKET_IDENTIFIER_IN_USE = 145,
    MQTT_PACKET_IDENTIFIER_NOT_FOUND = 146,
    MQTT_RECEIVE_MAXIMUM_EXCEEDED = 147,
    MQTT_TOPIC_ALIAS_INVALID = 148,
    MQTT_PACKET_TOO_LARGE = 149,
    MQTT_MESSAGE_RATE_TOO_HIGH = 150,
    MQTT_QUOTA_EXCEEDED = 151,
    MQTT_ADMINISTRATIVE_ACTION = 152,
    MQTT_PAYLOAD_FORMAT_INVALID = 153,
    MQTT_RETAIN_NOT_SUPPORTED = 154,
    MQTT_QOS_NOT_SUPPORTED = 155,
    MQTT_USE_ANOTHER_SERVER = 156,
    MQTT_SERVER_MOVED = 157,
    MQTT_SHARED_SUBSCRIPTIONS_NOT_SUPPORTED = 158,
    MQTT_CONNECTION_RATE_EXCEEDED = 159,
    MQTT_MAXIMUM_CONNECT_TIME = 160,
    MQTT_SUBSCRIPTION_IDENTIFIERS_NOT_SUPPORTED = 161,
    MQTT_WILDCARD_SUBSCRIPTIONS_NOT_SUPPORTED = 162
} MQTT_ReasonCode;

#pragma pack()
