#include "FreeRTOS_IP.h"
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_def.h"
#include "stm32f7xx_hal_rng.h"

#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

extern RNG_HandleTypeDef hRng;
extern UART_HandleTypeDef huart1;

// read this again for implementation : https://www.freertos.org/FreeRTOS-Plus/FreeRTOS_Plus_TCP/API/vApplicationIPNetworkEventHook.html
void vApplicationIPNetworkEventHook( eIPCallbackEvent_t eNetworkEvent ) {
    (void) eNetworkEvent;
}

// generate random number. See in doc of STM32 how to. 
// read this again  for implementation : https://www.freertos.org/FreeRTOS-Plus/FreeRTOS_Plus_TCP/API/xApplicationGetRandomNumber.html
BaseType_t xApplicationGetRandomNumber( uint32_t * pulNumber ) {
    // modify with random generator, return pdTRUE if success
    if (HAL_RNG_GenerateRandomNumber(&hRng, pulNumber) == HAL_OK) return pdPASS;
    else return pdFAIL;
}

// read this again  for implementation : https://www.freertos.org/FreeRTOS-Plus/FreeRTOS_Plus_TCP/API/ulApplicationGetNextSequenceNumber.html
uint32_t ulApplicationGetNextSequenceNumber( uint32_t ulSourceAddress,
                                             uint16_t usSourcePort,
                                             uint32_t ulDestinationAddress,
                                             uint16_t usDestinationPort )
{
    (void) ulSourceAddress;
    (void) usSourcePort;
    (void) ulDestinationAddress;
    (void) usDestinationPort;

    uint32_t randomNumber = 0;
    xApplicationGetRandomNumber( &randomNumber );
    return randomNumber;
}

void vLoggingPrintf( const char * pcFormat, ... )
{
	va_list args_list;
	va_start(args_list, pcFormat);
	int final_length = vsnprintf(NULL, 0, pcFormat, args_list) + 1;
	va_end(args_list);

	uint8_t* final_string = malloc(final_length);
	if(final_string == NULL) {
		return;
	}

	// Création de la chaîne finale
	va_start(args_list, pcFormat);
	vsnprintf((char *)final_string, final_length, pcFormat, args_list);
	va_end(args_list);

	HAL_UART_Transmit(&huart1, final_string, final_length, HAL_MAX_DELAY);

	free(final_string);
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
}
#endif /* USE_FULL_ASSERT */


/*
eDHCPCallbackAnswer_t xApplicationDHCPHook( eDHCPCallbackPhase_t eDHCPPhase,uint32_t ulIPAddress ) {
    ;
}

// Declare if needed (dns related). 
// Probably need to set ipconfigUSE_LLMNR to 1 in FreeRTOSIPConfig.h file
// Read this again for implementation : https://www.freertos.org/FreeRTOS-Plus/FreeRTOS_Plus_TCP/API/xApplicationDNSQueryHook.html
BaseType_t xApplicationDNSQueryHook( const char * pcName ) {

}
*/