#include "FreeRTOS.h"
#include "FreeRTOS_IP.h"
#include "task.h"
#include "app_mqtt.h"
#include "app_freertos_tasks.h"
#include "backoff_algorithm.h"
#include "transport_mbedtls.h"

#include <string.h>


uint32_t globalEntryTimeMs;
topicFilterContext_t xTopicFilterContext[ mqttexampleTOPIC_COUNT ];
MQTTPubAckInfo_t pOutgoingPublishRecords[ mqttexampleOUTGOING_PUBLISH_RECORD_LEN ];
MQTTPubAckInfo_t pIncomingPublishRecords[ mqttexampleINCOMING_PUBLISH_RECORD_LEN ];
static uint8_t ucSharedBuffer[ democonfigNETWORK_BUFFER_SIZE ];
static MQTTFixedBuffer_t xBuffer = {.pBuffer=ucSharedBuffer, .size=democonfigNETWORK_BUFFER_SIZE};
static uint16_t usSubscribePacketIdentifier;
static uint16_t usUnsubscribePacketIdentifier;
static uint16_t usPublishPacketIdentifier;


void app_initMqtt() {

    xTaskCreate( startMqttTask,
                 "MqttTask",
                 democonfigDEMO_STACKSIZE,
                 NULL,
                 (osPriority_t)osPriorityIdle,
                 NULL );
}

void startMqttTask(void *argument) {

    uint32_t ulPublishCount = 0U, ulTopicCount = 0U;
    const uint32_t ulMaxPublishCount = 5UL;
    NetworkContext_t xNetworkContext = { 0 };
    TlsTransportParams_t xTlsTransportParams = { 0 };
    NetworkCredentials_t xNetworkCredentials = { 0 };
    MQTTContext_t xMQTTContext = { 0 };
    MQTTStatus_t xMQTTStatus;
    TlsTransportStatus_t xNetworkStatus;

    ( void ) argument;

    xNetworkContext.pParams = &xTlsTransportParams;

    globalEntryTimeMs = getTimeMs();

    for( ; ; )
    {
        LogInfo( ( "---------STARTING DEMO---------\r\n" ) );

        initializeTopicBuffers();

        if( FreeRTOS_IsNetworkUp() == pdFALSE )
        {
            LogInfo( ( "Waiting for the network link up event..." ) );

            while( FreeRTOS_IsNetworkUp() == pdFALSE )
            {
                vTaskDelay( pdMS_TO_TICKS( 1000U ) );
            }
        }

        xNetworkStatus = connectToServerWithBackoffRetries( &xNetworkCredentials, &xNetworkContext );
        configASSERT( xNetworkStatus == TLS_TRANSPORT_SUCCESS );

        LogInfo( ( "Creating an MQTT connection to %s.\r\n", democonfigMQTT_BROKER_ENDPOINT ) );
        createMqttConnectionWithBroker( &xMQTTContext, &xNetworkContext );
        mqttSubscribeWithBackoffRetries( &xMQTTContext );

        for( ulPublishCount = 0; ulPublishCount < ulMaxPublishCount; ulPublishCount++ )
        {
            mqttPublishToTopics( &xMQTTContext );

            LogInfo( ( "Attempt to receive publishes from broker.\r\n" ) );
            xMQTTStatus = processLoopWithTimeout( &xMQTTContext, mqttexamplePROCESS_LOOP_TIMEOUT_MS );
            configASSERT( xMQTTStatus == MQTTSuccess );
            LogInfo( ( "Keeping Connection Idle...\r\n\r\n" ) );
            vTaskDelay( mqttexampleDELAY_BETWEEN_PUBLISHES_TICKS );
        }

        mqttUnsubscribeFromTopics( &xMQTTContext );
        xMQTTStatus = processLoopWithTimeout( &xMQTTContext, mqttexamplePROCESS_LOOP_TIMEOUT_MS );
        configASSERT( xMQTTStatus == MQTTSuccess );
        LogInfo( ( "Disconnecting the MQTT connection with %s.\r\n", democonfigMQTT_BROKER_ENDPOINT ) );
        xMQTTStatus = MQTT_Disconnect( &xMQTTContext );
        configASSERT( xMQTTStatus == MQTTSuccess );
        TLS_FreeRTOS_Disconnect( &xNetworkContext );

        for( ulTopicCount = 0; ulTopicCount < mqttexampleTOPIC_COUNT; ulTopicCount++ )
        {
            xTopicFilterContext[ ulTopicCount ].xSubAckStatus = MQTTSubAckFailure;
        }

        vTaskDelay( 5000 );
    }
};

uint32_t getTimeMs( void )
{
    TickType_t xTickCount = 0;
    uint32_t ulTimeMs = 0UL;

    xTickCount = xTaskGetTickCount();
    ulTimeMs = ( uint32_t ) xTickCount * (1000 / configTICK_RATE_HZ);
    ulTimeMs = ( uint32_t ) ( ulTimeMs - globalEntryTimeMs );

    return ulTimeMs;
}

void initializeTopicBuffers( void )
{
    uint32_t ulTopicCount;
    int xCharactersWritten;

    for( ulTopicCount = 0; ulTopicCount < 3; ulTopicCount++ )
    {
        xCharactersWritten = snprintf( (char*)xTopicFilterContext[ ulTopicCount ].pcTopicFilter,
                                       100,
                                       "%s%d", "/example/topic", ( int ) ulTopicCount );

        configASSERT( xCharactersWritten >= 0 && xCharactersWritten < mqttexampleTOPIC_BUFFER_SIZE );

        xTopicFilterContext[ ulTopicCount ].xSubAckStatus = MQTTSubAckFailure;
    }
}

TlsTransportStatus_t prvConnectToServerWithBackoffRetries( NetworkCredentials_t * pxNetworkCredentials, NetworkContext_t * pxNetworkContext )
{
    TlsTransportStatus_t xNetworkStatus;
    BackoffAlgorithmStatus_t xBackoffAlgStatus = BackoffAlgorithmSuccess;
    BackoffAlgorithmContext_t xReconnectParams;
    uint16_t usNextRetryBackOff = 0U;

    BackoffAlgorithm_InitializeParams( &xReconnectParams,
                                       mqttexampleRETRY_BACKOFF_BASE_MS,
                                       mqttexampleRETRY_MAX_BACKOFF_DELAY_MS,
                                       mqttexampleRETRY_MAX_ATTEMPTS );

    do {
        LogInfo( ( "Create a TCP connection to %s:%d.",
                   democonfigMQTT_BROKER_ENDPOINT,
                   democonfigMQTT_BROKER_PORT ) );
        xNetworkStatus = TLS_FreeRTOS_Connect( pxNetworkContext,
                                               democonfigMQTT_BROKER_ENDPOINT,
                                               democonfigMQTT_BROKER_PORT,
                                               pxNetworkCredentials,
                                               mqttexampleTRANSPORT_SEND_RECV_TIMEOUT_MS,
                                               mqttexampleTRANSPORT_SEND_RECV_TIMEOUT_MS );

        if( xNetworkStatus != TLS_TRANSPORT_SUCCESS )
        {
            uint32_t randomNumber;
            xApplicationGetRandomNumber(&randomNumber);
            xBackoffAlgStatus = BackoffAlgorithm_GetNextBackoff( &xReconnectParams, randomNumber, &usNextRetryBackOff );

            if( xBackoffAlgStatus == BackoffAlgorithmRetriesExhausted )
            {
                LogError( ( "Connection to the broker failed, all attempts exhausted." ) );
            }
            else if( xBackoffAlgStatus == BackoffAlgorithmSuccess )
            {
                LogWarn( ( "Connection to the broker failed. "
                           "Retrying connection with backoff and jitter." ) );
                vTaskDelay( pdMS_TO_TICKS( usNextRetryBackOff ) );
            }
        }
    } while( ( xNetworkStatus != TLS_TRANSPORT_SUCCESS ) && ( xBackoffAlgStatus == BackoffAlgorithmSuccess ) );

    return xNetworkStatus;
}

void createMqttConnectionWithBroker( MQTTContext_t * pxMQTTContext, NetworkContext_t * pxNetworkContext )
{
    MQTTStatus_t xResult;
    MQTTConnectInfo_t xConnectInfo;
    bool xSessionPresent;
    TransportInterface_t xTransport;

    xTransport.pNetworkContext = pxNetworkContext;
    xTransport.send = TLS_FreeRTOS_send;
    xTransport.recv = TLS_FreeRTOS_recv;
    xTransport.writev = NULL;

    xResult = MQTT_Init( pxMQTTContext, &xTransport, getTimeMs, prvEventCallback, &xBuffer );
    configASSERT( xResult == MQTTSuccess );
    xResult = MQTT_InitStatefulQoS( pxMQTTContext,
                                    pOutgoingPublishRecords,
                                    mqttexampleOUTGOING_PUBLISH_RECORD_LEN,
                                    pIncomingPublishRecords,
                                    mqttexampleINCOMING_PUBLISH_RECORD_LEN );
    configASSERT( xResult == MQTTSuccess );

    ( void ) memset( ( void * ) &xConnectInfo, 0x00, sizeof( xConnectInfo ) );

    xConnectInfo.cleanSession = true;

    xConnectInfo.pClientIdentifier = democonfigCLIENT_IDENTIFIER;
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

void prvEventCallback( MQTTContext_t * pxMQTTContext, MQTTPacketInfo_t * pxPacketInfo, MQTTDeserializedInfo_t * pxDeserializedInfo )
{
    ( void ) pxMQTTContext;

    if( ( pxPacketInfo->type & 0xF0U ) == MQTT_PACKET_TYPE_PUBLISH )
    {
        LogInfo( ( "PUBLISH received for packet id %u.\n\n",
                   pxDeserializedInfo->packetIdentifier ) );
        mqttProcessIncomingPublish( pxDeserializedInfo->pPublishInfo );
    }
    else
    {
        mqttProcessResponse( pxPacketInfo, pxDeserializedInfo->packetIdentifier );
    }
}

void mqttProcessIncomingPublish( MQTTPublishInfo_t * pxPublishInfo )
{
    uint32_t ulTopicCount;
    BaseType_t xTopicFound = pdFALSE;

    configASSERT( pxPublishInfo != NULL );

    LogInfo( ( "Incoming QoS : %d\n", pxPublishInfo->qos ) );

    for( ulTopicCount = 0; ulTopicCount < mqttexampleTOPIC_COUNT; ulTopicCount++ )
    {
        if( ( pxPublishInfo->topicNameLength == strlen( (char*)xTopicFilterContext[ ulTopicCount ].pcTopicFilter ) ) &&
            ( strncmp( (char*)xTopicFilterContext[ ulTopicCount ].pcTopicFilter, pxPublishInfo->pTopicName, pxPublishInfo->topicNameLength ) == 0 ) )
        {
            xTopicFound = pdTRUE;
            break;
        }
    }

    if( xTopicFound == pdTRUE )
    {
        LogInfo( ( "\r\nIncoming Publish Topic Name: %.*s matches a subscribed topic.\r\n",
                   pxPublishInfo->topicNameLength,
                   pxPublishInfo->pTopicName ) );
    }
    else
    {
        LogError( ( "Incoming Publish Topic Name: %.*s does not match a subscribed topic.\r\n",
                    pxPublishInfo->topicNameLength,
                    pxPublishInfo->pTopicName ) );
    }

    if( strncmp( mqttexampleMESSAGE, ( const char * ) ( pxPublishInfo->pPayload ), pxPublishInfo->payloadLength ) != 0 )
    {
        LogError( ( "Incoming Publish Message: %.*s does not match Expected Message: %s.\r\n",
                    pxPublishInfo->topicNameLength,
                    pxPublishInfo->pTopicName, mqttexampleMESSAGE ) );
    }
}

void mqttProcessResponse( MQTTPacketInfo_t * pxIncomingPacket, uint16_t usPacketId )
{
    uint32_t ulTopicCount = 0U;

    switch( pxIncomingPacket->type )
    {
        case MQTT_PACKET_TYPE_PUBACK:
            LogInfo( ( "PUBACK received for packet ID %u.\r\n", usPacketId ) );
            break;

        case MQTT_PACKET_TYPE_SUBACK:

            LogInfo( ( "SUBACK received for packet ID %u.", usPacketId ) );

            updateSubAckStatus( pxIncomingPacket );

            for( ulTopicCount = 0; ulTopicCount < mqttexampleTOPIC_COUNT; ulTopicCount++ )
            {
                if( xTopicFilterContext[ ulTopicCount ].xSubAckStatus != MQTTSubAckFailure )
                {
                    LogInfo( ( "Subscribed to the topic %s with maximum QoS %u.\r\n",
                               xTopicFilterContext[ ulTopicCount ].pcTopicFilter,
                               xTopicFilterContext[ ulTopicCount ].xSubAckStatus ) );
                }
            }

            configASSERT( usSubscribePacketIdentifier == usPacketId );
            break;

        case MQTT_PACKET_TYPE_UNSUBACK:
            LogInfo( ( "UNSUBACK received for packet ID %u.", usPacketId ) );
            configASSERT( usUnsubscribePacketIdentifier == usPacketId );
            break;

        case MQTT_PACKET_TYPE_PINGRESP:

            LogWarn( ( "PINGRESP should not be handled by the application "
                       "callback when using MQTT_ProcessLoop.\n" ) );
            break;

        case MQTT_PACKET_TYPE_PUBREC:
            LogInfo( ( "PUBREC received for packet id %u.\n\n",
                       usPacketId ) );
            break;

        case MQTT_PACKET_TYPE_PUBREL:

            LogInfo( ( "PUBREL received for packet id %u.\n\n",
                       usPacketId ) );
            break;

        case MQTT_PACKET_TYPE_PUBCOMP:

            LogInfo( ( "PUBCOMP received for packet id %u.\n\n",
                       usPacketId ) );
            break;

        default:
            LogWarn( ( "prvMQTTProcessResponse() called with unknown packet type:(%02X).\r\n",
                       pxIncomingPacket->type ) );
    }
}

void updateSubAckStatus( MQTTPacketInfo_t * pxPacketInfo )
{
    MQTTStatus_t xResult = MQTTSuccess;
    uint8_t * pucPayload = NULL;
    size_t ulSize = 0;
    uint32_t ulTopicCount = 0U;

    xResult = MQTT_GetSubAckStatusCodes( pxPacketInfo, &pucPayload, &ulSize );

    configASSERT( xResult == MQTTSuccess );

    for( ulTopicCount = 0; ulTopicCount < ulSize; ulTopicCount++ )
    {
        xTopicFilterContext[ ulTopicCount ].xSubAckStatus = pucPayload[ ulTopicCount ];
    }
}

void mqttSubscribeWithBackoffRetries( MQTTContext_t * pxMQTTContext )
{
    MQTTStatus_t xResult = MQTTSuccess;
    BackoffAlgorithmStatus_t xBackoffAlgStatus = BackoffAlgorithmSuccess;
    BackoffAlgorithmContext_t xRetryParams;
    uint16_t usNextRetryBackOff = 0U;
    MQTTSubscribeInfo_t xMQTTSubscription[ mqttexampleTOPIC_COUNT ];
    bool xFailedSubscribeToTopic = false;
    uint32_t ulTopicCount = 0U;

    ( void ) memset( ( void * ) &xMQTTSubscription, 0x00, sizeof( xMQTTSubscription ) );

    usSubscribePacketIdentifier = MQTT_GetPacketId( pxMQTTContext );

    for( ulTopicCount = 0; ulTopicCount < mqttexampleTOPIC_COUNT; ulTopicCount++ ) {
        xMQTTSubscription[ ulTopicCount ].qos = MQTTQoS2;
        xMQTTSubscription[ ulTopicCount ].pTopicFilter = (char*)xTopicFilterContext[ ulTopicCount ].pcTopicFilter;
        xMQTTSubscription[ ulTopicCount ].topicFilterLength = ( uint16_t ) strlen( (char*)xTopicFilterContext[ ulTopicCount ].pcTopicFilter );
    }

    BackoffAlgorithm_InitializeParams( &xRetryParams, mqttexampleRETRY_BACKOFF_BASE_MS, mqttexampleRETRY_MAX_BACKOFF_DELAY_MS, mqttexampleRETRY_MAX_ATTEMPTS );

    do {
        xResult = MQTT_Subscribe( pxMQTTContext,
                                  xMQTTSubscription,
                                  sizeof( xMQTTSubscription ) / sizeof( MQTTSubscribeInfo_t ),
                                  usSubscribePacketIdentifier );
        configASSERT( xResult == MQTTSuccess );

        for( ulTopicCount = 0; ulTopicCount < mqttexampleTOPIC_COUNT; ulTopicCount++ ) {
            LogInfo( ( "SUBSCRIBE sent for topic %s to broker.\n\n",
                       xTopicFilterContext[ ulTopicCount ].pcTopicFilter ) );
        }

        xResult = processLoopWithTimeout( pxMQTTContext, mqttexamplePROCESS_LOOP_TIMEOUT_MS );
        configASSERT( xResult == MQTTSuccess );

        xFailedSubscribeToTopic = false;

        for( ulTopicCount = 0; ulTopicCount < mqttexampleTOPIC_COUNT; ulTopicCount++ ) {
            if( xTopicFilterContext[ ulTopicCount ].xSubAckStatus == MQTTSubAckFailure ) {
                xFailedSubscribeToTopic = true;

                uint32_t randomNumber;
                xApplicationGetRandomNumber(&randomNumber);
                xBackoffAlgStatus = BackoffAlgorithm_GetNextBackoff( &xRetryParams, randomNumber, &usNextRetryBackOff );

                if( xBackoffAlgStatus == BackoffAlgorithmRetriesExhausted ) {
                    LogError( ( "Server rejected subscription request. All retry attempts have exhausted. Topic=%s",
                                xTopicFilterContext[ ulTopicCount ].pcTopicFilter ) );
                }
                else if( xBackoffAlgStatus == BackoffAlgorithmSuccess ) {
                    LogWarn( ( "Server rejected subscription request. Attempting to re-subscribe to topic %s.",
                               xTopicFilterContext[ ulTopicCount ].pcTopicFilter ) );
                    vTaskDelay( pdMS_TO_TICKS( usNextRetryBackOff ) );
                }
                break;
            }
        }

        configASSERT( xBackoffAlgStatus != BackoffAlgorithmRetriesExhausted );
    } while( ( xFailedSubscribeToTopic == true ) && ( xBackoffAlgStatus == BackoffAlgorithmSuccess ) );
}

void mqttPublishToTopics( MQTTContext_t * pxMQTTContext )
{
    MQTTStatus_t xResult;
    MQTTPublishInfo_t xMQTTPublishInfo;
    uint32_t ulTopicCount;

    for( ulTopicCount = 0; ulTopicCount < mqttexampleTOPIC_COUNT; ulTopicCount++ )
    {
        ( void ) memset( ( void * ) &xMQTTPublishInfo, 0x00, sizeof( xMQTTPublishInfo ) );

        xMQTTPublishInfo.qos = MQTTQoS2;
        xMQTTPublishInfo.retain = false;
        xMQTTPublishInfo.pTopicName = (char*)xTopicFilterContext[ ulTopicCount ].pcTopicFilter;
        xMQTTPublishInfo.topicNameLength = ( uint16_t ) strlen( (char*)xTopicFilterContext[ ulTopicCount ].pcTopicFilter );
        xMQTTPublishInfo.pPayload = mqttexampleMESSAGE;
        xMQTTPublishInfo.payloadLength = strlen( mqttexampleMESSAGE );

        usPublishPacketIdentifier = MQTT_GetPacketId( pxMQTTContext );

        LogInfo( ( "Publishing to the MQTT topic %s.\r\n", xTopicFilterContext[ ulTopicCount ].pcTopicFilter ) );
        xResult = MQTT_Publish( pxMQTTContext, &xMQTTPublishInfo, usPublishPacketIdentifier );
        configASSERT( xResult == MQTTSuccess );
    }
}

MQTTStatus_t processLoopWithTimeout( MQTTContext_t * pMqttContext, uint32_t ulTimeoutMs )
{
    uint32_t ulMqttProcessLoopTimeoutTime;
    uint32_t ulCurrentTime;

    MQTTStatus_t eMqttStatus = MQTTSuccess;

    ulCurrentTime = pMqttContext->getTime();
    ulMqttProcessLoopTimeoutTime = ulCurrentTime + ulTimeoutMs;

    while( ( ulCurrentTime < ulMqttProcessLoopTimeoutTime ) &&
           ( eMqttStatus == MQTTSuccess || eMqttStatus == MQTTNeedMoreBytes ) ) {
        eMqttStatus = MQTT_ProcessLoop( pMqttContext );
        ulCurrentTime = pMqttContext->getTime();
    }

    if( eMqttStatus == MQTTNeedMoreBytes ) {
        eMqttStatus = MQTTSuccess;
    }

    return eMqttStatus;
}

void mqttUnsubscribeFromTopics( MQTTContext_t * pxMQTTContext )
{
    MQTTStatus_t xResult;
    MQTTSubscribeInfo_t xMQTTSubscription[ mqttexampleTOPIC_COUNT ];
    uint32_t ulTopicCount;

    memset( ( void * ) &xMQTTSubscription, 0x00, sizeof( xMQTTSubscription ) );

    for( ulTopicCount = 0; ulTopicCount < mqttexampleTOPIC_COUNT; ulTopicCount++ )
    {
        xMQTTSubscription[ ulTopicCount ].qos = MQTTQoS2;
        xMQTTSubscription[ ulTopicCount ].pTopicFilter = (char*)xTopicFilterContext[ ulTopicCount ].pcTopicFilter;
        xMQTTSubscription[ ulTopicCount ].topicFilterLength = ( uint16_t ) strlen( (char*)xTopicFilterContext[ ulTopicCount ].pcTopicFilter );

        LogInfo( ( "Unsubscribing from topic %s.\r\n", xTopicFilterContext[ ulTopicCount ].pcTopicFilter ) );
    }

    usUnsubscribePacketIdentifier = MQTT_GetPacketId( pxMQTTContext );
    configASSERT( usUnsubscribePacketIdentifier != 0 );

    xResult = MQTT_Unsubscribe( pxMQTTContext,
                                xMQTTSubscription,
                                sizeof( xMQTTSubscription ) / sizeof( MQTTSubscribeInfo_t ),
                                usUnsubscribePacketIdentifier );

    configASSERT( xResult == MQTTSuccess );
}
