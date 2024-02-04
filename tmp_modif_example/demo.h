
#include "FreeRTOSConfig.h"
#include "logging_levels.h"

#ifndef LIBRARY_LOG_NAME
    #define LIBRARY_LOG_NAME    "MQTT-wolfSSL"
#endif

#ifndef LIBRARY_LOG_LEVEL
    #define LIBRARY_LOG_LEVEL    LOG_INFO
#endif

extern void vLoggingPrintf( const char * pcFormatString,
                            ... );


#ifndef SdkLog
    #define SdkLog( message )    vLoggingPrintf message
#endif

#include "logging_stack.h"

#define democonfigCLIENT_IDENTIFIER    "insert here."

//#define democonfigMQTT_BROKER_ENDPOINT    "...insert here..."
//#define democonfigMQTT_BROKER_PORT    "...insert here..."
//// #define democonfigCREDENTIALS_IN_BUFFER
//#define democonfigROOT_CA_PEM    "...insert here..."
//#define democonfigCLIENT_CERTIFICATE_PEM    "...insert here..."
//#define democonfigCLIENT_PRIVATE_KEY_PEM    "...insert here..."

#define democonfigDEMO_STACKSIZE         configMINIMAL_STACK_SIZE

#define democonfigNETWORK_BUFFER_SIZE    ( 1024U )