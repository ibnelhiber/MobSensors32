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

    typedef struct I2CSetupInfo
    {
        std::function<void()>> enableI2C;
        uint8_t portMacro;
    };

    const std::unordered_map<I2C_TypeDef*, I2CSetupInfo> enable_i2c_clock_map =
    {
        { I2C1, I2CSetupInfo([](){ __HAL_RCC_I2C1_CLK_ENABLE();}, GPIO_AF4_I2C1)},
        { I2C2, I2CSetupInfo([](){ __HAL_RCC_I2C2_CLK_ENABLE();}, GPIO_AF4_I2C2)},
        { I2C3, I2CSetupInfo([](){ __HAL_RCC_I2C3_CLK_ENABLE();}, GPIO_AF4_I2C3)},
        { I2C4, I2CSetupInfo([](){ __HAL_RCC_I2C3_CLK_ENABLE();}, GPIO_AF4_I2C4)}
    };

    enum class I2CRole
    {
        SDA,
        SCL
    };

    typedef struct I2CPinOptions
    {
        I2C_TypeDef* i2cPort;
        I2CRole possibleI2CRoles;
    };

    const std::unordered_map<GPIOPin, I2CPinOptions> i2c_pin_options_map =
    {
        // I2C1
        { {GPIOB, GPIO_PIN_6 }, { I2C1, I2CRole::SCL } },
        { {GPIOB, GPIO_PIN_7 }, { I2C1, I2CRole::SDA } },
        { {GPIOB, GPIO_PIN_8 }, { I2C1, I2CRole::SCL } },
        { {GPIOB, GPIO_PIN_9 }, { I2C1, I2CRole::SDA } },

        // I2C2
        { {GPIOB, GPIO_PIN_10}, { I2C2, I2CRole::SCL } },
        { {GPIOB, GPIO_PIN_11}, { I2C2, I2CRole::SDA } },

        { {GPIOF, GPIO_PIN_0 }, { I2C2, I2CRole::SDA } },
        { {GPIOF, GPIO_PIN_1 }, { I2C2, I2CRole::SCL } },

        { {GPIOH, GPIO_PIN_4 }, { I2C2, I2CRole::SCL } },
        { {GPIOH, GPIO_PIN_5 }, { I2C2, I2CRole::SDA } },

        // I2C3
        { {GPIOA, GPIO_PIN_8 }, { I2C3, I2CRole::SCL } },

        { {GPIOC, GPIO_PIN_9 }, { I2C3, I2CRole::SDA } },

        { {GPIOH, GPIO_PIN_7 }, { I2C3, I2CRole::SCL } },
        { {GPIOH, GPIO_PIN_8 }, { I2C3, I2CRole::SDA } },

        // I2C4
        { {GPIOD, GPIO_PIN_12}, { I2C4, I2CRole::SCL } },
        { {GPIOD, GPIO_PIN_13}, { I2C4, I2CRole::SDA } },

        { {GPIOF, GPIO_PIN_14}, { I2C4, I2CRole::SCL } },
        { {GPIOF, GPIO_PIN_15}, { I2C4, I2CRole::SDA } },

        { {GPIOH, GPIO_PIN_11}, { I2C4, I2CRole::SCL } },
        { {GPIOH, GPIO_PIN_12}, { I2C4, I2CRole::SDA } }
    };

    inline void SetI2CPortTimingParamaters(std::unique_ptr<I2C_HandleTypeDef>& i2cPort)
    {
        i2cPort->Init.Timing = 0x20404768;
    }

    typedef struct UARTSetupInfo
    {
        std::function<void()> enableUART;
        uint8_t alternateFunction;
    } UARTSetupInfo;

    const std::unordered_map<USART_TypeDef*, UARTSetupInfo> enable_uart_clock_map =
    {
        { USART1, { [](){ __HAL_RCC_USART1_CLK_ENABLE(); }, GPIO_AF7_USART1 } },
        { USART2, { [](){ __HAL_RCC_USART2_CLK_ENABLE(); }, GPIO_AF7_USART2 } },
        { USART3, { [](){ __HAL_RCC_USART3_CLK_ENABLE(); }, GPIO_AF7_USART3 } },
        { UART4,  { [](){ __HAL_RCC_UART4_CLK_ENABLE();  }, GPIO_AF8_UART4  } },
        { UART5,  { [](){ __HAL_RCC_UART5_CLK_ENABLE();  }, GPIO_AF8_UART5  } },
        { USART6, { [](){ __HAL_RCC_USART6_CLK_ENABLE(); }, GPIO_AF8_USART6 } },
        { UART7,  { [](){ __HAL_RCC_UART7_CLK_ENABLE();  }, GPIO_AF8_UART7  } },
        { UART8,  { [](){ __HAL_RCC_UART8_CLK_ENABLE();  }, GPIO_AF8_UART8  } }
    };

    enum class UARTRole
    {
        TX,
        RX
    };

    struct UARTPinOptions
    {
        USART_TypeDef* uartPort;
        UARTRole role;
    };

    const std::unordered_map<GPIOPin, UARTPinOptions> uart_pin_options_map =
    {
        // USART1
        {{GPIOA, GPIO_PIN_9 }, {USART1, UARTRole::TX}},
        {{GPIOA, GPIO_PIN_10}, {USART1, UARTRole::RX}},
        {{GPIOB, GPIO_PIN_6 }, {USART1, UARTRole::TX}},
        {{GPIOB, GPIO_PIN_7 }, {USART1, UARTRole::RX}},

        // USART2
        {{GPIOA, GPIO_PIN_2 }, {USART2, UARTRole::TX}},
        {{GPIOA, GPIO_PIN_3 }, {USART2, UARTRole::RX}},
        {{GPIOD, GPIO_PIN_5 }, {USART2, UARTRole::TX}},
        {{GPIOD, GPIO_PIN_6 }, {USART2, UARTRole::RX}},

        // USART3
        {{GPIOB, GPIO_PIN_10}, {USART3, UARTRole::TX}},
        {{GPIOB, GPIO_PIN_11}, {USART3, UARTRole::RX}},
        {{GPIOC, GPIO_PIN_10}, {USART3, UARTRole::TX}},
        {{GPIOC, GPIO_PIN_11}, {USART3, UARTRole::RX}},
        {{GPIOD, GPIO_PIN_8 }, {USART3, UARTRole::TX}},
        {{GPIOD, GPIO_PIN_9 }, {USART3, UARTRole::RX}},

        // UART4
        {{GPIOA, GPIO_PIN_0 }, {UART4, UARTRole::TX}},
        {{GPIOA, GPIO_PIN_1 }, {UART4, UARTRole::RX}},
        {{GPIOC, GPIO_PIN_10}, {UART4, UARTRole::TX}},
        {{GPIOC, GPIO_PIN_11}, {UART4, UARTRole::RX}},
        {{GPIOD, GPIO_PIN_1 }, {UART4, UARTRole::TX}},
        {{GPIOD, GPIO_PIN_0 }, {UART4, UARTRole::RX}},
        {{GPIOH, GPIO_PIN_13}, {UART4, UARTRole::TX}},
        {{GPIOH, GPIO_PIN_14}, {UART4, UARTRole::RX}},

        // UART5
        {{GPIOC, GPIO_PIN_12}, {UART5, UARTRole::TX}},
        {{GPIOD, GPIO_PIN_2 }, {UART5, UARTRole::RX}},

        // USART6
        {{GPIOC, GPIO_PIN_6 }, {USART6, UARTRole::TX}},
        {{GPIOC, GPIO_PIN_7 }, {USART6, UARTRole::RX}},
        {{GPIOG, GPIO_PIN_14}, {USART6, UARTRole::TX}},
        {{GPIOG, GPIO_PIN_9 }, {USART6, UARTRole::RX}},

        // UART7
        {{GPIOE, GPIO_PIN_8 }, {UART7, UARTRole::TX}},
        {{GPIOE, GPIO_PIN_7 }, {UART7, UARTRole::RX}},
        {{GPIOF, GPIO_PIN_7 }, {UART7, UARTRole::TX}},
        {{GPIOF, GPIO_PIN_6 }, {UART7, UARTRole::RX}},

        // UART8
        {{GPIOE, GPIO_PIN_1 }, {UART8, UARTRole::TX}},
        {{GPIOE, GPIO_PIN_0 }, {UART8, UARTRole::RX}}
    };

#endif
