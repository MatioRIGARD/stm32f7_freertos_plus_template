#include "NetworkInterface.h"

static const uint8_t ucIPAddress[ 4 ] = { 192, 168, 0, 201 };
static const uint8_t ucNetMask[ 4 ] = { 255, 255, 255, 0 };
static const uint8_t ucGatewayAddress[ 4 ] = { 192, 168, 0, 1 };

static const uint8_t ucDNSServerAddress[ 4 ] = { 1, 1, 1, 1 };

static uint8_t ucMACAddress[ 6 ] = { 0x02, 0x12, 0x22, 0x33, 0x44, 0x55 };

void app_initNetwork(void) {
    FreeRTOS_IPInit( ucIPAddress,
                     ucNetMask,
                     ucGatewayAddress,
                     ucDNSServerAddress,
                     ucMACAddress );
}
