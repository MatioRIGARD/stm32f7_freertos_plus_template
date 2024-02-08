#include <string.h>
#include <stdio.h>

#include "FreeRTOS.h"
#include "FreeRTOS_IP.h"
#include "task.h"

#include "app_mqtt.h"

#include "core_mqtt.h"

#include "transport_wolfSSL.h"


#ifndef democonfigMQTT_BROKER_ENDPOINT
    #error "Define the config democonfigMQTT_BROKER_ENDPOINT by following the instructions in file demo_config.h."
#endif
#ifndef democonfigROOT_CA_PEM
    #error "Please define Root CA certificate of the MQTT broker(democonfigROOT_CA_PEM) in demo_config.h."
#endif
#ifndef democonfigCLIENT_CERTIFICATE_PEM
    #error "Please define client certificate(democonfigCLIENT_CERTIFICATE_PEM) in demo_config.h."
#endif
#ifndef democonfigCLIENT_PRIVATE_KEY_PEM
    #error "Please define client private key(democonfigCLIENT_PRIVATE_KEY_PEM) in demo_config.h."
#endif
#ifndef democonfigCLIENT_IDENTIFIER
    #define democonfigCLIENT_IDENTIFIER    "testClient"__TIME__
#endif
#ifndef democonfigMQTT_BROKER_PORT
    #define democonfigMQTT_BROKER_PORT    ( 8883 )
#endif

#define mqttexampleTOPIC                                  democonfigCLIENT_IDENTIFIER "/test_topic"
#define mqttexampleDELAY_BETWEEN_DEMO_ITERATIONS_TICKS    ( pdMS_TO_TICKS( 5000U ) )
#define _MILLISECONDS_PER_SECOND                          ( 1000U )
#define _MILLISECONDS_PER_TICK                            ( _MILLISECONDS_PER_SECOND / configTICK_RATE_HZ )


static void prvMQTTDemoTask( void * pvParameters );
static void prvCreateMQTTConnectionWithBroker( MQTTContext_t * pxMQTTContext,
                                               NetworkContext_t * pxNetworkContext );
static void prvMQTTSubscribeToTopic( MQTTContext_t * pxMQTTContext );
static void prvMQTTPublishToTopic( MQTTContext_t * pxMQTTContext );
static void prvMQTTUnsubscribeFromTopic( MQTTContext_t * pxMQTTContext );
static uint32_t prvGetTimeMs( void );
static void prvMQTTProcessResponse( MQTTPacketInfo_t * pxIncomingPacket,
                                    uint16_t usPacketId );
static void prvMQTTProcessIncomingPublish( MQTTPublishInfo_t * pxPublishInfo );
static void prvEventCallback( MQTTContext_t * pxMQTTContext,
                              MQTTPacketInfo_t * pxPacketInfo,
                              MQTTDeserializedInfo_t * pxDeserializedInfo );
static void prvTLSConnect( NetworkCredentials_t * pxNetworkCredentials,
                           NetworkContext_t * pxNetworkContext );
static MQTTStatus_t prvProcessLoopWithTimeout( MQTTContext_t * pMqttContext,
                                               uint32_t ulTimeoutMs );


static uint8_t ucSharedBuffer[ democonfigNETWORK_BUFFER_SIZE ];
static uint32_t ulGlobalEntryTimeMs;
static uint16_t usPublishPacketIdentifier;
static uint16_t usSubscribePacketIdentifier;
static uint16_t usUnsubscribePacketIdentifier;
static MQTTFixedBuffer_t xBuffer = {
    ucSharedBuffer,
    democonfigNETWORK_BUFFER_SIZE
};
static MQTTPubAckInfo_t pOutgoingPublishRecords[ mqttexampleOUTGOING_PUBLISH_RECORD_LEN ];
static MQTTPubAckInfo_t pIncomingPublishRecords[ mqttexampleINCOMING_PUBLISH_RECORD_LEN ];

void app_initMqtt( void )
{
    xTaskCreate( prvMQTTDemoTask,          /* Function that implements the task. */
                 "MQTTDemo",               /* Text name for the task - only used for debugging. */
                 democonfigDEMO_STACKSIZE, /* Size of stack (in words, not bytes) to allocate for the task. */
                 NULL,                     /* Task parameter - not used in this case. */
                 tskIDLE_PRIORITY,         /* Task priority, must be between 0 and configMAX_PRIORITIES - 1. */
                 NULL );                   /* Used to pass out a handle to the created task - not used in this case. */
}

static void prvMQTTDemoTask( void * pvParameters )
{
    uint32_t ulPublishCount = 0U;
    const uint32_t ulMaxPublishCount = 5UL;
    NetworkContext_t xNetworkContext = { 0 };
    NetworkCredentials_t xNetworkCredentials = { 0 };
    MQTTContext_t xMQTTContext = { 0 };
    MQTTStatus_t xMQTTStatus;

    ( void ) pvParameters;

    ulGlobalEntryTimeMs = prvGetTimeMs();

    for( ; ; )
    {
        LogInfo( ( "---------STARTING DEMO---------" ) );

        if( FreeRTOS_IsNetworkUp() == pdFALSE )
        {
            LogInfo( ( "Waiting for the network link up event..." ) );

            while( FreeRTOS_IsNetworkUp() == pdFALSE )
            {
                vTaskDelay( pdMS_TO_TICKS( 1000U ) );
            }
        }

        LogInfo( ( "Creating a TLS connection to %s:%u.\r\n",
                   democonfigMQTT_BROKER_ENDPOINT,
                   democonfigMQTT_BROKER_PORT ) );
        prvTLSConnect( &xNetworkCredentials, &xNetworkContext );

        LogInfo( ( "Creating an MQTT connection to %s.\r\n", democonfigMQTT_BROKER_ENDPOINT ) );
        prvCreateMQTTConnectionWithBroker( &xMQTTContext, &xNetworkContext );

        LogInfo( ( "Attempt to subscribe to the MQTT topic %s.\r\n", mqttexampleTOPIC ) );
        prvMQTTSubscribeToTopic( &xMQTTContext );

        xMQTTStatus = prvProcessLoopWithTimeout( &xMQTTContext, mqttexamplePROCESS_LOOP_TIMEOUT_MS );

        configASSERT( xMQTTStatus == MQTTSuccess );

        for( ulPublishCount = 0; ulPublishCount < ulMaxPublishCount; ulPublishCount++ )
        {
            LogInfo( ( "Publish to the MQTT topic %s.\r\n", mqttexampleTOPIC ) );
            prvMQTTPublishToTopic( &xMQTTContext );

            LogInfo( ( "Attempt to receive publish message from broker.\r\n" ) );
            xMQTTStatus = prvProcessLoopWithTimeout( &xMQTTContext, mqttexamplePROCESS_LOOP_TIMEOUT_MS );
            configASSERT( xMQTTStatus == MQTTSuccess );

            LogInfo( ( "Keeping Connection Idle...\r\n\r\n" ) );
            vTaskDelay( mqttexampleDELAY_BETWEEN_PUBLISHES_TICKS );
        }

        LogInfo( ( "Unsubscribe from the MQTT topic %s.\r\n", mqttexampleTOPIC ) );
        prvMQTTUnsubscribeFromTopic( &xMQTTContext );

        xMQTTStatus = prvProcessLoopWithTimeout( &xMQTTContext, mqttexamplePROCESS_LOOP_TIMEOUT_MS );
        configASSERT( xMQTTStatus == MQTTSuccess );

        LogInfo( ( "Disconnecting the MQTT connection with %s.\r\n", democonfigMQTT_BROKER_ENDPOINT ) );
        MQTT_Disconnect( &xMQTTContext );

        TLS_FreeRTOS_Disconnect( &xNetworkContext );

        LogInfo( ( "prvMQTTDemoTask() completed an iteration successfully. Total free heap is %u.\r\n", xPortGetFreeHeapSize() ) );
        LogInfo( ( "Demo completed successfully.\r\n" ) );
        LogInfo( ( "-------DEMO FINISHED-------\r\n" ) );
        LogInfo( ( "Short delay before starting the next iteration.... \r\n\r\n" ) );
        vTaskDelay( mqttexampleDELAY_BETWEEN_DEMO_ITERATIONS_TICKS );
    }
}

static void prvTLSConnect( NetworkCredentials_t * pxNetworkCredentials,
                           NetworkContext_t * pxNetworkContext )
{
    BaseType_t xNetworkStatus;

    pxNetworkCredentials->pRootCa = ( const unsigned char * ) democonfigROOT_CA_PEM;
    pxNetworkCredentials->pClientCert = ( const unsigned char * ) democonfigCLIENT_CERTIFICATE_PEM;
    pxNetworkCredentials->pPrivateKey = ( const unsigned char * ) democonfigCLIENT_PRIVATE_KEY_PEM;

    #if defined( democonfigCREDENTIALS_IN_BUFFER )
        pxNetworkCredentials->rootCaSize = strlen( democonfigROOT_CA_PEM );
        pxNetworkCredentials->clientCertSize = strlen( democonfigCLIENT_CERTIFICATE_PEM );
        pxNetworkCredentials->privateKeySize = strlen( democonfigCLIENT_PRIVATE_KEY_PEM );
    #else
        pxNetworkCredentials->rootCaSize = sizeof( democonfigROOT_CA_PEM );
        pxNetworkCredentials->clientCertSize = sizeof( democonfigCLIENT_CERTIFICATE_PEM );
        pxNetworkCredentials->privateKeySize = sizeof( democonfigCLIENT_PRIVATE_KEY_PEM );
    #endif

    xNetworkStatus = TLS_FreeRTOS_Connect( pxNetworkContext,
                                           democonfigMQTT_BROKER_ENDPOINT,
                                           democonfigMQTT_BROKER_PORT,
                                           pxNetworkCredentials,
                                           mqttexampleTRANSPORT_SEND_RECV_TIMEOUT_MS,
                                           mqttexampleTRANSPORT_SEND_RECV_TIMEOUT_MS );
    configASSERT( xNetworkStatus == TLS_TRANSPORT_SUCCESS );
    LogInfo( ( "A mutually authenticated TLS connection established with %s:%u.\r\n",
               democonfigMQTT_BROKER_ENDPOINT,
               democonfigMQTT_BROKER_PORT ) );
}
static void prvCreateMQTTConnectionWithBroker( MQTTContext_t * pxMQTTContext,
                                               NetworkContext_t * pxNetworkContext )
{
    MQTTStatus_t xResult;
    MQTTConnectInfo_t xConnectInfo;
    bool xSessionPresent;
    TransportInterface_t xTransport;

    xTransport.pNetworkContext = pxNetworkContext;
    xTransport.send = TLS_FreeRTOS_send;
    xTransport.recv = TLS_FreeRTOS_recv;
    xTransport.writev = NULL;

    xResult = MQTT_Init( pxMQTTContext, &xTransport, prvGetTimeMs, prvEventCallback, &xBuffer );
    configASSERT( xResult == MQTTSuccess );
    xResult = MQTT_InitStatefulQoS( pxMQTTContext,
                                    pOutgoingPublishRecords,
                                    mqttexampleOUTGOING_PUBLISH_RECORD_LEN,
                                    pIncomingPublishRecords,
                                    mqttexampleINCOMING_PUBLISH_RECORD_LEN );
    configASSERT( xResult == MQTTSuccess );

    memset( ( void * ) &xConnectInfo, 0x00, sizeof( xConnectInfo ) );

    xConnectInfo.cleanSession = true;

    xConnectInfo.pClientIdentifier = democonfigCLIENT_IDENTIFIER;
    xConnectInfo.pPassword = democonfigCLIENT_PASSWORD;
    xConnectInfo.clientIdentifierLength = ( uint16_t ) strlen( democonfigCLIENT_IDENTIFIER );

    xConnectInfo.keepAliveSeconds = mqttexampleKEEP_ALIVE_TIMEOUT_SECONDS;

    xResult = MQTT_Connect( pxMQTTContext,
                            &xConnectInfo,
                            NULL,
                            mqttexampleCONNACK_RECV_TIMEOUT_MS,
                            &xSessionPresent );
    configASSERT( xResult == MQTTSuccess );

    LogInfo( ( "An MQTT connection is established with %s.", democonfigMQTT_BROKER_ENDPOINT ) );
}

static void prvMQTTSubscribeToTopic( MQTTContext_t * pxMQTTContext )
{
    MQTTStatus_t xResult;
    MQTTSubscribeInfo_t xMQTTSubscription[ 1 ];

    ( void ) memset( ( void * ) &xMQTTSubscription, 0x00, sizeof( xMQTTSubscription ) );

    xMQTTSubscription[ 0 ].qos = MQTTQoS1;
    xMQTTSubscription[ 0 ].pTopicFilter = mqttexampleTOPIC;
    xMQTTSubscription[ 0 ].topicFilterLength = ( uint16_t ) strlen( mqttexampleTOPIC );

    usSubscribePacketIdentifier = MQTT_GetPacketId( pxMQTTContext );

    xResult = MQTT_Subscribe( pxMQTTContext,
                              xMQTTSubscription,
                              sizeof( xMQTTSubscription ) / sizeof( MQTTSubscribeInfo_t ),
                              usSubscribePacketIdentifier );

    configASSERT( xResult == MQTTSuccess );
}

static void prvMQTTPublishToTopic( MQTTContext_t * pxMQTTContext )
{
    MQTTStatus_t xResult;
    MQTTPublishInfo_t xMQTTPublishInfo;

    ( void ) memset( ( void * ) &xMQTTPublishInfo, 0x00, sizeof( xMQTTPublishInfo ) );

    xMQTTPublishInfo.qos = MQTTQoS1;
    xMQTTPublishInfo.retain = false;
    xMQTTPublishInfo.pTopicName = mqttexampleTOPIC;
    xMQTTPublishInfo.topicNameLength = ( uint16_t ) strlen( mqttexampleTOPIC );
    xMQTTPublishInfo.pPayload = mqttexampleMESSAGE;
    xMQTTPublishInfo.payloadLength = strlen( mqttexampleMESSAGE );

    usPublishPacketIdentifier = MQTT_GetPacketId( pxMQTTContext );

    xResult = MQTT_Publish( pxMQTTContext, &xMQTTPublishInfo, usPublishPacketIdentifier );

    configASSERT( xResult == MQTTSuccess );
}

static void prvMQTTUnsubscribeFromTopic( MQTTContext_t * pxMQTTContext )
{
    MQTTStatus_t xResult;
    MQTTSubscribeInfo_t xMQTTSubscription[ 1 ];

    memset( ( void * ) &xMQTTSubscription, 0x00, sizeof( xMQTTSubscription ) );

    xMQTTSubscription[ 0 ].pTopicFilter = mqttexampleTOPIC;
    xMQTTSubscription[ 0 ].topicFilterLength = ( uint16_t ) strlen( mqttexampleTOPIC );

    usUnsubscribePacketIdentifier = MQTT_GetPacketId( pxMQTTContext );
    configASSERT( usUnsubscribePacketIdentifier != 0 );

    xResult = MQTT_Unsubscribe( pxMQTTContext,
                                xMQTTSubscription,
                                sizeof( xMQTTSubscription ) / sizeof( MQTTSubscribeInfo_t ),
                                usUnsubscribePacketIdentifier );

    configASSERT( xResult == MQTTSuccess );
}

static void prvMQTTProcessResponse( MQTTPacketInfo_t * pxIncomingPacket,
                                    uint16_t usPacketId )
{
    switch( pxIncomingPacket->type )
    {
        case MQTT_PACKET_TYPE_PUBACK:
            LogInfo( ( "PUBACK received for packet Id %u.\r\n", usPacketId ) );
            configASSERT( usPublishPacketIdentifier == usPacketId );
            break;

        case MQTT_PACKET_TYPE_SUBACK:
            LogInfo( ( "Subscribed to the topic %s.\r\n", mqttexampleTOPIC ) );
            configASSERT( usSubscribePacketIdentifier == usPacketId );
            break;

        case MQTT_PACKET_TYPE_UNSUBACK:
            LogInfo( ( "Unsubscribed from the topic %s.\r\n", mqttexampleTOPIC ) );
            configASSERT( usUnsubscribePacketIdentifier == usPacketId );
            break;

        case MQTT_PACKET_TYPE_PINGRESP:
            LogInfo( ( "Ping Response successfully received.\r\n" ) );
            break;

        default:
            LogWarn( ( "prvMQTTProcessResponse() called with unknown packet type:(%02X).\r\n",
                       pxIncomingPacket->type ) );
    }
}

static void prvMQTTProcessIncomingPublish( MQTTPublishInfo_t * pxPublishInfo )
{
    configASSERT( pxPublishInfo != NULL );

    LogInfo( ( "Incoming QoS : %d\n", pxPublishInfo->qos ) );

    if( ( pxPublishInfo->topicNameLength == strlen( mqttexampleTOPIC ) ) &&
        ( 0 == strncmp( mqttexampleTOPIC, pxPublishInfo->pTopicName, pxPublishInfo->topicNameLength ) ) )
    {
        LogInfo( ( "\r\nIncoming Publish Topic Name: %.*s matches subscribed topic.\r\n"
                   "Incoming Publish Message : %.*s\r\n",
                   pxPublishInfo->topicNameLength,
                   pxPublishInfo->pTopicName,
                   pxPublishInfo->payloadLength,
                   pxPublishInfo->pPayload ) );
    }
    else
    {
        LogInfo( ( "Incoming Publish Topic Name: %.*s does not match subscribed topic.\r\n",
                   pxPublishInfo->topicNameLength,
                   pxPublishInfo->pTopicName ) );
    }
}

static void prvEventCallback( MQTTContext_t * pxMQTTContext,
                              MQTTPacketInfo_t * pxPacketInfo,
                              MQTTDeserializedInfo_t * pxDeserializedInfo )
{
    ( void ) pxMQTTContext;

    if( ( pxPacketInfo->type & 0xF0U ) == MQTT_PACKET_TYPE_PUBLISH )
    {
        prvMQTTProcessIncomingPublish( pxDeserializedInfo->pPublishInfo );
    }
    else
    {
        prvMQTTProcessResponse( pxPacketInfo, pxDeserializedInfo->packetIdentifier );
    }
}

static uint32_t prvGetTimeMs( void )
{
    TickType_t xTickCount = 0;
    uint32_t ulTimeMs = 0UL;

    xTickCount = xTaskGetTickCount();

    ulTimeMs = ( uint32_t ) xTickCount * _MILLISECONDS_PER_TICK;

    ulTimeMs = ( uint32_t ) ( ulTimeMs - ulGlobalEntryTimeMs );

    return ulTimeMs;
}

static MQTTStatus_t prvProcessLoopWithTimeout( MQTTContext_t * pMqttContext,
                                               uint32_t ulTimeoutMs )
{
    uint32_t ulMqttProcessLoopTimeoutTime;
    uint32_t ulCurrentTime;

    MQTTStatus_t eMqttStatus = MQTTSuccess;

    ulCurrentTime = pMqttContext->getTime();
    ulMqttProcessLoopTimeoutTime = ulCurrentTime + ulTimeoutMs;

    while( ( ulCurrentTime < ulMqttProcessLoopTimeoutTime ) &&
           ( eMqttStatus == MQTTSuccess || eMqttStatus == MQTTNeedMoreBytes ) )
    {
        eMqttStatus = MQTT_ProcessLoop( pMqttContext );
        ulCurrentTime = pMqttContext->getTime();
    }

    if( eMqttStatus == MQTTNeedMoreBytes )
    {
        eMqttStatus = MQTTSuccess;
    }

    return eMqttStatus;
}
