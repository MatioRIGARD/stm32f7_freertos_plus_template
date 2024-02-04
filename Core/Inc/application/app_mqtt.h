#ifndef APP_MQTT_H
#define APP_MQTT_H

#include "transport_wolfSSL.h"
#include "core_mqtt.h"

#define democonfigMQTT_BROKER_ENDPOINT                      "192.168.0.11"
#define democonfigMQTT_BROKER_PORT                          ((uint16_t) 44444)
#define democonfigCLIENT_IDENTIFIER                         "testClient"__TIME__
#define democonfigNETWORK_BUFFER_SIZE                       (1024U)
#define democonfigDEMO_STACKSIZE                            (512U)  // 60 minimum?

#define mqttexampleTOPIC_COUNT                              (3U)
#define mqttexampleTOPIC_BUFFER_SIZE                        (100U)
#define mqttexampleRETRY_BACKOFF_BASE_MS                    (500U)
#define mqttexampleRETRY_MAX_BACKOFF_DELAY_MS               (5000U)
#define mqttexampleRETRY_MAX_ATTEMPTS                       (5U)
#define mqttexampleTRANSPORT_SEND_RECV_TIMEOUT_MS           (200U)
#define mqttexampleOUTGOING_PUBLISH_RECORD_LEN              (15U)
#define mqttexampleINCOMING_PUBLISH_RECORD_LEN              (15U)
#define mqttexampleKEEP_ALIVE_TIMEOUT_SECONDS               (60U)
#define mqttexampleCONNACK_RECV_TIMEOUT_MS                  (1000U)
#define mqttexampleMESSAGE                                  "Hello World!"
#define mqttexamplePROCESS_LOOP_TIMEOUT_MS                  (2000U)
#define mqttexampleDELAY_BETWEEN_PUBLISHES_TICKS            (pdMS_TO_TICKS (2000U))

/*
struct NetworkContext
{
    TlsTransportParams_t * pParams;
};
*/

typedef struct topicFilterContext
{
    uint8_t pcTopicFilter[ mqttexampleTOPIC_BUFFER_SIZE ];
    MQTTSubAckStatus_t xSubAckStatus;
} topicFilterContext_t;

void app_initMqtt();
void startMqttTask(void *argument);

// tools
uint32_t getTimeMs( void );
void initializeTopicBuffers( void );

// MQTT
TlsTransportStatus_t connectToServerWithBackoffRetries( NetworkCredentials_t * pxNetworkCredentials, NetworkContext_t * pxNetworkContext );
void createMqttConnectionWithBroker( MQTTContext_t * pxMQTTContext, NetworkContext_t * pxNetworkContext );
void prvEventCallback( MQTTContext_t * pxMQTTContext, MQTTPacketInfo_t * pxPacketInfo, MQTTDeserializedInfo_t * pxDeserializedInfo );
void mqttProcessIncomingPublish( MQTTPublishInfo_t * pxPublishInfo );
void mqttProcessResponse( MQTTPacketInfo_t * pxIncomingPacket, uint16_t usPacketId );
void updateSubAckStatus( MQTTPacketInfo_t * pxPacketInfo );
void mqttSubscribeWithBackoffRetries( MQTTContext_t * pxMQTTContext );
void mqttPublishToTopics( MQTTContext_t * pxMQTTContext );
MQTTStatus_t processLoopWithTimeout( MQTTContext_t * pMqttContext, uint32_t ulTimeoutMs );
void mqttUnsubscribeFromTopics( MQTTContext_t * pxMQTTContext );

#endif // APP_MQTT_H