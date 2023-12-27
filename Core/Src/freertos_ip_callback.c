#include "FreeRTOS_IP.h"

// read this again for implementation : https://www.freertos.org/FreeRTOS-Plus/FreeRTOS_Plus_TCP/API/vApplicationIPNetworkEventHook.html
void vApplicationIPNetworkEventHook( eIPCallbackEvent_t eNetworkEvent ) {

}

// Declare if needed (dns related). 
// Probably need to set ipconfigUSE_LLMNR to 1 in FreeRTOSIPConfig.h file
// Read this again for implementation : https://www.freertos.org/FreeRTOS-Plus/FreeRTOS_Plus_TCP/API/xApplicationDNSQueryHook.html
/*
BaseType_t xApplicationDNSQueryHook( const char * pcName ) {

}
*/

// generate random number. See in doc of STM32 how to. 
// read this again  for implementation : https://www.freertos.org/FreeRTOS-Plus/FreeRTOS_Plus_TCP/API/xApplicationGetRandomNumber.html
BaseType_t xApplicationGetRandomNumber( uint32_t * pulNumber ) {
    // modify with random generator, return pdTRUE if success
    if(1) return pdTRUE;
    else return pdFALSE;
}

// read this again  for implementation : https://www.freertos.org/FreeRTOS-Plus/FreeRTOS_Plus_TCP/API/ulApplicationGetNextSequenceNumber.html
uint32_t ulApplicationGetNextSequenceNumber( uint32_t ulSourceAddress,
                                             uint16_t usSourcePort,
                                             uint32_t ulDestinationAddress,
                                             uint16_t usDestinationPort ) {
    return 0;
}