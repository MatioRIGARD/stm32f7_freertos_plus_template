#ifndef APP_MQTT_H
#define APP_MQTT_H


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


#include "FreeRTOSConfig.h"
#include "logging_levels.h"

#ifndef LIBRARY_LOG_NAME
    #define LIBRARY_LOG_NAME    "MQTT-wolfSSL"
#endif

#ifndef LIBRARY_LOG_LEVEL
    #define LIBRARY_LOG_LEVEL    LOG_INFO
#endif

extern void vLoggingPrintf( const char * pcFormatString, ... );
void vStartSimpleMQTTDemo( void );

#ifndef SdkLog
    #define SdkLog( message )    vLoggingPrintf message
#endif

#include "logging_stack.h"

#define democonfigCLIENT_IDENTIFIER    "insert here."

#define democonfigMQTT_BROKER_ENDPOINT                      "192.168.0.11"
#define democonfigMQTT_BROKER_PORT                          ((uint16_t) 44444)

#define democonfigCREDENTIALS_IN_BUFFER

// WARNING!: This key is an example and have to be changed by your own key
#define democonfigROOT_CA_PEM    "-----BEGIN CERTIFICATE-----\n" \
    "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\n" \
    "-----END CERTIFICATE-----\n"

// WARNING!: This key is an example and have to be changed by your own key
#define democonfigCLIENT_CERTIFICATE_PEM    "-----BEGIN CERTIFICATE-----\n" \
    "MIIEETCCAfkCFBzDWP+GS61J6WZ5WTTVL3HIVIXvMA0GCSqGSIb3DQEBCwUAMEUx\n" \
    "CzAJBgNVBAYTAkZSMRMwEQYDVQQIDApTb21lLVN0YXRlMSEwHwYDVQQKDBhJbnRl\n" \
    "cm5ldCBXaWRnaXRzIFB0eSBMdGQwHhcNMjQwMjA1MTgyOTEzWhcNMjUwMjA0MTgy\n" \
    "OTEzWjBFMQswCQYDVQQGEwJGUjETMBEGA1UECAwKU29tZS1TdGF0ZTEhMB8GA1UE\n" \
    "CgwYSW50ZXJuZXQgV2lkZ2l0cyBQdHkgTHRkMIIBIjANBgkqhkiG9w0BAQEFAAOC\n" \
    "AQ8AMIIBCgKCAQEA9KsqOQpX/dPPQWmAfJutK3NOeMm5zjF1zvgPeWPtYpgKJnTu\n" \
    "h1O15iArXQ2bdABvD0YvJTlB5PtGa/vyvJg7F9/kY6M1OrXfYHcZGsw0i3CoSlk/\n" \
    "uMDdnBhpOngEEwjH28/sJluahLm2/aqsUZr16y1BF2qvDM0Ro4TFba3crpPuKlJV\n" \
    "Oh8z3laVwR+qGk1LmMc8LB5ImuSUvV8oBbzkAGetEhA3I9C6BetENW5XuOKMokQg\n" \
    "Gqnn4CJHZO8fJTpOimtoDxoo9+n9J0Y/HahUaT51YWBnT+m3xrJb9APFfjHRiqNE\n" \
    "brCJOZzr4dM17CI59kEFWfnPhbn1wo2V/WQ+EwIDAQABMA0GCSqGSIb3DQEBCwUA\n" \
    "A4ICAQClZUTTsmDoZcPUDrw+zZwqzKalgLDMoCcxfw7F+71S2DvV9C4Pt4wRby3f\n" \
    "ateH4Z2Cpz2qMR/272t0StU4MOXaGNnsSN/Fld8isLPGxJAgCEBnKOr+QaezJl4R\n" \
    "WoycTnpwAw52kTDQa7OyXu+ROaqMHhxD/f97iacnr8MBzqqk+miIijlSfxUvUArh\n" \
    "aj7yaSllUVHWEwfDPHwAL7dMyW+vVeVvjeSUAmZGYbFlUv6dI4LdpMs3CUkqh81O\n" \
    "1mRi0LRTQj20sd57fMw87X9JlKQGXZbMbvnQw1d1xkOFeFt7KZp9gn+eF4ROLR1e\n" \
    "oUU4pmNlGINgiH5Qx/CPu6AjIjdEqEexd4oeObW0DIcIzcEIA54xGsaqflhPHQY/\n" \
    "eITqM7KMeJqn+ui67TMnh/2NSHQWKq7I+/J+K4fOOIXrTb5GKANJb42tKvL6lzT5\n" \
    "F96njQ3MikeVajBvODWkNK+PJHs/VZPx3N86SJbhgo7DLSqSYH958QH3+lDvZomK\n" \
    "+QnVo28a9B7V56TTFGEvCIKDyPdo/U7bJJsUkFFfiUxT7/akWVgeUT4KVT57pAQK\n" \
    "AL8kPl7l2fC0aXwPEWiwX86jOrdVIK1sB7YyppJOjVC+4XKiQ91UUvmEgTwLe0AJ\n" \
    "SBmxlrl6uOhn2Ozxv4m/SYsbyGYyqgJzwm234e+RsLK5zt8/9g==\n" \
    "-----END CERTIFICATE-----\n"

// WARNING!: This key is an example and have to be changed by your own key
#define democonfigCLIENT_PRIVATE_KEY_PEM    "-----BEGIN RSA PRIVATE KEY-----\n" \
    "MIIEogIBAAKCAQEA9KsqOQpX/dPPQWmAfJutK3NOeMm5zjF1zvgPeWPtYpgKJnTu\n" \
    "h1O15iArXQ2bdABvD0YvJTlB5PtGa/vyvJg7F9/kY6M1OrXfYHcZGsw0i3CoSlk/\n" \
    "uMDdnBhpOngEEwjH28/sJluahLm2/aqsUZr16y1BF2qvDM0Ro4TFba3crpPuKlJV\n" \
    "Oh8z3laVwR+qGk1LmMc8LB5ImuSUvV8oBbzkAGetEhA3I9C6BetENW5XuOKMokQg\n" \
    "Gqnn4CJHZO8fJTpOimtoDxoo9+n9J0Y/HahUaT51YWBnT+m3xrJb9APFfjHRiqNE\n" \
    "brCJOZzr4dM17CI59kEFWfnPhbn1wo2V/WQ+EwIDAQABAoIBAEicy+kl0rt4Mxhc\n" \
    "sU9J/Hjavb1HUSeKskX8hMsoBvmt2cVUCr6K738eqP/y2+HvhwZnc/J+LbpBgMLn\n" \
    "gUDLhS4uul9/vnpwI51De2XimO2RP8Dg//Ja+uoqrIA3wi3L+1SXPqSbpx4g59gc\n" \
    "4QtPDBEB20GR3eXUrply/Azy/n2owQSJGtTmcNiLW3qyuxzJ6JGvPuI3N4OesfGd\n" \
    "/x2OwGphZKZUQ3Cy8kYJgZcMHe0vAPcAAcMwLZAJmP2GOk6Q2nvyJHmf9JqBokRj\n" \
    "Q4F9FUDJM/nM2kW6vFLw+OsFP07ruo+xJe7H+Uu7+///v/yDl49VlJVvCwPDxxQh\n" \
    "pJei7UECgYEA/RJXA3eKdXiDY0yctEqSHYYmXNtq/SZfkbYnX7Rs7RPYFPOZhi/F\n" \
    "Ay+WaFqTrd77Yb5qo3nv1J/qFMQjWHMPjnVaDpNNfDh+STsX1SpHP6wXT9eNzjr0\n" \
    "d4L9mJDr4sqY0DCdSyOSyK7NYAXZLptjJWgXt6/FNiClhfs470XyRpECgYEA93/u\n" \
    "5x8yt4OltYVoo4d8JIXFiLd3s3MsnzQuDy/88HW2A3i3X0Hu4HFAJZS0bahZYpCn\n" \
    "OXW24U/C/X3f3Q+TRMwCu/bialORi+I18aP6g2gD8QkJAi+BbNXJZyyt+urWgTbp\n" \
    "O6gsJnjkxFMERM35i3K+PsCPMz9Y5N5axnJFtGMCgYA4B814GQlwxMPiQl4XlMMR\n" \
    "AliQCox/i/Vw33GMQPg07UXULbL6AK2P7QNSC2vS/SGPHIejtiWhX96Fxn9E4AS7\n" \
    "t8O+omJvdFqRgCHgskGgGUv4A3OEC42AwGHBW6S+S7jVr03ECxY+lGUoM2NIbGLh\n" \
    "L1DA/IufVk8PgPW0FDp14QKBgAqSBTMLyw+5aKdjBkANtkidCf230D3yylIWWW0+\n" \
    "oG/opxho3HQODdS6IYI5YqdYTFEAWpQCqgffbzWCXcBEPQbwK6G5fpwJ66psw98J\n" \
    "INbh52ExAJh+TOnIC60DZH7V7slf/gAVQ6V0/lhAL2t83I6PHP7zHGqU5wgHUxss\n" \
    "oDt/AoGAJlDfm5wra6lA86u4SYQgnjzhY8zg890yM1y383cUQ0yJMc/4UeLXZGhg\n" \
    "4S9vcKa9GAmCm/69SV/GDkD6dDCwSssqeFLnpOXnfAxq2NUoMvPCM+kWo+OpHN0e\n" \
    "mM3L5QfeGlQW4RMwvEC5LEnuZuH1G12LuurdC0iDCFsIMwKH9rg=\n" \
    "-----END RSA PRIVATE KEY-----\n"


#define democonfigDEMO_STACKSIZE         configMINIMAL_STACK_SIZE

#define democonfigNETWORK_BUFFER_SIZE    ( 1024U )

#endif // APP_MQTT_H