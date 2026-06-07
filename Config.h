#pragma once

/*
* Possible configurations are:
* USE_ESP32,
* USE_STM32F7
* USE_STM32F4
*
*/


#if defined(USE_ESP32)
    #include "driver/gpio.h"
    #include "freertos/freertos.h"
    #include "ESP32/ESP32I2CBus.h"
    using IOPin = gpio_num_t;
#elif defined(USE_STM32F7)
    #include "stm32f7xx_hal.h"
    #define I2CMAX 4
#elif defined(USE_STM32F4)
    #include "stm32f4xx_hal.h"
    #define I2CMAX 3
#endif
