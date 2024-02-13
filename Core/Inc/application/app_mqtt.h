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
#define mqttexampleMESSAGE                                  ("Hello World!")
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
void app_initMqtt( void );

#ifndef SdkLog
    #define SdkLog( message )    vLoggingPrintf message
#endif

#include "logging_stack.h"

#define democonfigCLIENT_IDENTIFIER    "user01"
#define democonfigCLIENT_PASSWORD       "password"

#define democonfigMQTT_BROKER_ENDPOINT                      "192.168.0.11"
#define democonfigMQTT_BROKER_PORT                          ((uint16_t) 44444)

#define democonfigCREDENTIALS_IN_BUFFER

// WARNING!: This key is an example and have to be changed by your own key
#define democonfigROOT_CA_PEM    "-----BEGIN CERTIFICATE-----\n" \
    "MIIDmzCCAoMCFFzqOAwy2oOZyJLNwuwu5fkX0ODfMA0GCSqGSIb3DQEBCwUAMIGJ\n" \
    "MQswCQYDVQQGEwJGUjEOMAwGA1UECAwFU3RhdGUxDTALBgNVBAcMBENpdHkxDjAM\n" \
    "BgNVBAoMBWNhT3JnMRIwEAYDVQQLDAljYU9yZ1VuaXQxFTATBgNVBAMMDDE5Mi4x\n" \
    "NjguMC4xMTEgMB4GCSqGSIb3DQEJARYRZW1haWxAZXhhbXBsZS5jb20wHhcNMjQw\n" \
    "MjA3MjA1NTQ3WhcNMjQwMzA4MjA1NTQ3WjCBiTELMAkGA1UEBhMCRlIxDjAMBgNV\n" \
    "BAgMBVN0YXRlMQ0wCwYDVQQHDARDaXR5MQ4wDAYDVQQKDAVjYU9yZzESMBAGA1UE\n" \
    "CwwJY2FPcmdVbml0MRUwEwYDVQQDDAwxOTIuMTY4LjAuMTExIDAeBgkqhkiG9w0B\n" \
    "CQEWEWVtYWlsQGV4YW1wbGUuY29tMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIB\n" \
    "CgKCAQEA8AgUiMa6HMMUAR1HmMrdrF/hSDHuZR2c+jL2mf/VwFLD/+Ir8UHexWYL\n" \
    "qOMB1wUTHtiiPyg9b/jWNyKYBmbm/dwS5HMvpXcG3W6yDMsoW+rErbx26bzFfiwK\n" \
    "TrZ9qHO7IbBlQMzVXnHBPuYEy/yQ1e20+svM3RkVFBXYgpdmT50mm7D+tXalvDYb\n" \
    "peWjdF5zpb84Ytw8NbCx6Ur6/WCjdTdjUHrjw7Iq4xeJe19CJcZAjCKFkINXkNl9\n" \
    "glQCTuBWkX3i114jRF2YVXBiE1w2X0LH4kNFfngT/5LOM2LnszEjXdm0NuREOyn5\n" \
    "KlxFuPOkI//XH/laTsHiIkj7dRXEUQIDAQABMA0GCSqGSIb3DQEBCwUAA4IBAQBW\n" \
    "C9oAWCEZdUYjYuBO74uCNzFox0BomQjV7tSdJ8Y5mP7zq1Hl9YjevpeeO34b6hPr\n" \
    "5Bp93IYDRg2tY3UBtjeQlCoSM2/OWrad3ucDyU+Mrh54Gm8EKJiy5NfZMyWr5gog\n" \
    "/GIkjwNPI/io0UUhrWRkQpd43ovtjQ3r3PgdcY1lfaPkyvDwDXsWQQeSlND3Rp3p\n" \
    "4TkAtR18B1kw62NZInUwDilKW8Al7wOHu1Z/xshM130pEtGU+ax5LlJ9rnTMA6TE\n" \
    "KBNDB5u0/r0nSIRZntQgB3DuNRvkDqS55hBCdksABh+q8bWssDIYJh4owtjSp/aZ\n" \
    "I0Y/A9pMmPPWb/VEPBiN\n" \
    "-----END CERTIFICATE-----\n"

// WARNING!: This key is an example and have to be changed by your own key
#define democonfigCLIENT_CERTIFICATE_PEM    "-----BEGIN CERTIFICATE-----\n" \
    "MIIDoDCCAogCFDDYP6c2AlsxlNofrpSHeSmuy8cuMA0GCSqGSIb3DQEBCwUAMIGJ\n" \
    "MQswCQYDVQQGEwJGUjEOMAwGA1UECAwFU3RhdGUxDTALBgNVBAcMBENpdHkxDjAM\n" \
    "BgNVBAoMBWNhT3JnMRIwEAYDVQQLDAljYU9yZ1VuaXQxFTATBgNVBAMMDDE5Mi4x\n" \
    "NjguMC4xMTEgMB4GCSqGSIb3DQEJARYRZW1haWxAZXhhbXBsZS5jb20wHhcNMjQw\n" \
    "MjA3MjA1NTQ3WhcNMjQwMzA4MjA1NTQ3WjCBjjELMAkGA1UEBhMCRlIxDjAMBgNV\n" \
    "BAgMBVN0YXRlMQ0wCwYDVQQHDARDaXR5MQ8wDQYDVQQKDAZTZXJ2ZXIxFjAUBgNV\n" \
    "BAsMDVNlcnZlck9yZ1VuaXQxFTATBgNVBAMMDDE5Mi4xNjguMC4xMTEgMB4GCSqG\n" \
    "SIb3DQEJARYRZW1haWxAZXhhbXBsZS5jb20wggEiMA0GCSqGSIb3DQEBAQUAA4IB\n" \
    "DwAwggEKAoIBAQDsR1Sv6BbhtpVK/BgrEoaRGqQQXPCylh/8uqr8HcQZHYEt09WU\n" \
    "2SDVI10PluYXVZYKf463X2KEwrJ6z4CjgVLhTtSlWmicJ7gpOGkkRnPmqs4Jzhvj\n" \
    "P5KIPt+rlBzbPOFvnu2vtgnDvtvNJHDmzOR2ZkwHK6Sd3qf6HxkhaX6rcnz74xpy\n" \
    "PbPzGmAyuV3CiLYOq/4IqWz0bN7hUM1a5WjwfD6O53nXoMa+syCNMUpp0KUvQBBf\n" \
    "4T0oW7Xh+sbW3Q91E+qUOYn+H32VxGYJSu+xM4ntznwRHjky9lRlszihr5EtCEDQ\n" \
    "j65GDbcPmsw4u8GkqkKwphmwiGv9XgJ5wnDPAgMBAAEwDQYJKoZIhvcNAQELBQAD\n" \
    "ggEBAJ0NONMS2yFpk0hYe6Jzqz8jSKmcnBp00KGIsfzToBksyjpuQmU6JwOZG32I\n" \
    "bpHBgKgbH7+yFm63PgxJJ1ddeZ1BqECLBIfl5xTJD5O8yGDwRIw7o4Fw0zvsI7X8\n" \
    "4ZTExe/dn7s/dCvzV1Pqaw5G2hbVuIJFnDD/myeMa+tGGFGSwRga/S4/OZ4/63B0\n" \
    "e5G1yuxZTgpqVNJNGk65im0ak2UsIZDT6BP2XGdiAGLsBBlLOrtn3V2HnGgzuElP\n" \
    "cofNmSEWurM8PbSAAe+8FGLbykX2pQ1BqZd/9QuhsmsdujQ5FPCmXeBGLmxcH4JU\n" \
    "GHaEwN/KdM7Uuhugn439pQToxCs=\n" \
    "-----END CERTIFICATE-----\n"

// WARNING!: This key is an example and have to be changed by your own key
#define democonfigCLIENT_PRIVATE_KEY_PEM    "-----BEGIN RSA PRIVATE KEY-----\n" \
    "MIIEpAIBAAKCAQEA7EdUr+gW4baVSvwYKxKGkRqkEFzwspYf/Lqq/B3EGR2BLdPV\n" \
    "lNkg1SNdD5bmF1WWCn+Ot19ihMKyes+Ao4FS4U7UpVponCe4KThpJEZz5qrOCc4b\n" \
    "4z+SiD7fq5Qc2zzhb57tr7YJw77bzSRw5szkdmZMByuknd6n+h8ZIWl+q3J8++Ma\n" \
    "cj2z8xpgMrldwoi2Dqv+CKls9Gze4VDNWuVo8Hw+jud516DGvrMgjTFKadClL0AQ\n" \
    "X+E9KFu14frG1t0PdRPqlDmJ/h99lcRmCUrvsTOJ7c58ER45MvZUZbM4oa+RLQhA\n" \
    "0I+uRg23D5rMOLvBpKpCsKYZsIhr/V4CecJwzwIDAQABAoIBAGiFOWpDUfPxL+Om\n" \
    "1w4euNZxoYf3hSaLEIVdEgxLIpc1nRJntgCNeF+wsBcYWpqR6rikKr7lfBBcjFWV\n" \
    "e0NZDw/KLnkjIEmLrITXhnNckyLEBjcCLQX+9lQ88/bTo1w8d0FIXb6v2Wv0/wil\n" \
    "iSggFSRnJlS5sq1/dHFJ1nUYVMeEGTo8QQzaZBTKt8QcDP2cJtEH/6mkNLWK3H5W\n" \
    "QYzbjq8ub7ZwLHJbORWpiE7aG65GetuYJH4k7MD5ae8+G6/cuIZu9RtOjiwcecrS\n" \
    "hmuVdgf0u3AELxVsLiEmUTrBr6op2EGNUH/xGRY30ANB/R6PX5l9Y1o3YIcWqSx8\n" \
    "RRZTqYECgYEA9+M1nXwM518zbCppEns7JkXtOvvNq5waO7cK6gNNPbVoKz14oX1S\n" \
    "MVQx1SduATQ+7nCYllTMdkzfdvnzJeHgb1nml5F1vD2hYebsOH61RWrFxkqwJIua\n" \
    "0curlb4iKnXpl6uhGRtgIcRRc3zNj6CUkZRrYefLiYln1XLBADjED48CgYEA9ALc\n" \
    "zq00PliXM58Fv1+zLWLfDp+TUSYrm2SHZHVMdjGX9RNJQWTGGSXaRyoVdM9dllY3\n" \
    "yjzfJq9ZkeEP8pEu0EYz9bvVdcvFpLoeM0MvBBfwbpFqxHk8n7zxM66mN8omSsj+\n" \
    "g+lpbm/NSWsiyBUJOZw5XEn9qDYcVp4QBw+m6sECgYAEZSjsM+jeqVd2EYJva3Pv\n" \
    "B8pfA1vwMQJeEK7G22whLfg83AgphNlSOpXvm5VdvBlpjOGHuNUvO8myiFgBUfHy\n" \
    "x3tJMLEPt2AHJlsF3WlFY9FB5M0euMLz+VJ/gzGD4zdTEx32HgAkWSDAlXPnmgdI\n" \
    "VYNZ5ww60g7fP/ugD2GajQKBgQC1+HSGxPBTFmocGxmEMHBFBpjmJOcN+3NFvWCi\n" \
    "8D8HXdC++ZUF6Ys6Mdpoe9/obsSSWtXcQi0AgoA/pCHqeY75z4DsXxm/cSmBwTLI\n" \
    "c+/aE6hALL38q5UkYxlVu4dFz8FiwCXeZfE0ZLgpanZT1Cw/BFxakOtTyJa1mcbG\n" \
    "HUwTgQKBgQCBZgIb1+5VSmf/GXnd23Snj2KdoPDyntB3t52vGJHT1cdBpw6Qm4oR\n" \
    "UIP9ZqItj1nc0xFw6K1rL0EeJo8/f1f1XFx4ukGSQAjZJ5pu9Ci4WSEfgdkgBOu3\n" \
    "WiTosjnM6AcvdTs2rhYfQLuftABVR+dlODIHMByUBBb1sgALJV78qQ==\n" \
    "-----END RSA PRIVATE KEY-----\n"

#define democonfigDEMO_STACKSIZE         configMINIMAL_STACK_SIZE*4

#define democonfigNETWORK_BUFFER_SIZE    ( 1024U )

#endif // APP_MQTT_H