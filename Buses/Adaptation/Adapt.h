#pragma once


#if defined(ESP_PLATFORM)
    #include "driver/gpio.h"
    #include "freertos/freertos.h"
    using IOPin = gpio_num_t;
#elif defined(STM32F767xx)
    #include "stm32f7xx_hal.h"
    #include "FreeRTOS.h"
    #include "task.h"

    #include <memory>
    #include <functional>
    #include <unordered_map>

    struct GPIOPin
    {
    GPIO_TypeDef* gpioPort;
    uint16_t pin;

    bool operator==(const GPIOPin&) const = default;
    };
    using IOPin = GPIOPin;

    struct GPIOPinHash
    {
        std::size_t operator()(const GPIOPin& gpioPin) const
        {
            // std::hash is callable. Argument is what to create a hash for
            std::size_t portHashVal = std::hash<GPIO_TypeDef*>{}(gpioPin.gpioPort);
            std::size_t pinHashVal = std::hash<uint16_t>{}(gpioPin.pin);

            //Avoid collisions by not making flipped values equivalent
            return portHashVal ^ (pinHashVal << 1);
        }
    };

    #define I2CMAX 4
    
    const std::unordered_map<GPIO_TypeDef*, std::function<void()>> enable_gpio_port_clock_map =
    {
        { GPIOA, [](){ __HAL_RCC_GPIOA_CLK_ENABLE(); } },
        { GPIOB, [](){ __HAL_RCC_GPIOB_CLK_ENABLE(); } },
        { GPIOC, [](){ __HAL_RCC_GPIOC_CLK_ENABLE(); } },
        { GPIOD, [](){ __HAL_RCC_GPIOD_CLK_ENABLE(); } },
        { GPIOE, [](){ __HAL_RCC_GPIOE_CLK_ENABLE(); } },
        { GPIOF, [](){ __HAL_RCC_GPIOF_CLK_ENABLE(); } },
        { GPIOG, [](){ __HAL_RCC_GPIOG_CLK_ENABLE(); } },
        { GPIOH, [](){ __HAL_RCC_GPIOH_CLK_ENABLE(); } },
        { GPIOI, [](){ __HAL_RCC_GPIOI_CLK_ENABLE(); } },
        { GPIOJ, [](){ __HAL_RCC_GPIOJ_CLK_ENABLE(); } },
        { GPIOK, [](){ __HAL_RCC_GPIOK_CLK_ENABLE(); } }
    };

    const std::unordered_map<uint8_t, I2C_TypeDef*> i2c_port_map = 
    {
        {1, I2C1},
        {2, I2C2},
        {3, I2C3},
        {4, I2C4}
    };

    const std::unordered_map<I2C_TypeDef*, std::function<void()>> enable_i2c_clock_map =
    {
        { I2C1, [](){ __HAL_RCC_I2C1_CLK_ENABLE(); } },
        { I2C2, [](){ __HAL_RCC_I2C2_CLK_ENABLE(); } },
        { I2C3, [](){ __HAL_RCC_I2C3_CLK_ENABLE(); } }
    };

    inline void SetI2CPortTimingParamaters(std::unique_ptr<I2C_HandleTypeDef>& i2cPort)
    {
        i2cPort->Init.Timing = 0x20404768;
    }

#endif
