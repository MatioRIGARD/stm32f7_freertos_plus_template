# FreeRTOS-Plus template for STM32F7xx for VSCode

## How to use it

You can clone this repository and install the extension [stm32-for-vscode](https://marketplace.visualstudio.com/items?itemName=bmd.stm32-for-vscode) to build and debug.

You can find it here: 

After cloning, don't forget to load submodules with:

```bash
git submodule update --init --recursive
```

update 27/01/2024: Ping is now working

## More details

If you want to know how to use mosquitto broker, you can read this brief memo I wrote: [MQTT broker usage](doc/mqtt_broker_usage.md)

If you want to have more details about how I build this template, you can read this: [Tuto link](./doc/stm32f7_template_init.md)

If you need to use wolfssl (with MQTT for example), you need to build the lib wolfssl once using the command `make wolfssl_build`