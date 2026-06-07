#include "STM32I2CBus.h"

STM32I2CBus::STM32I2CBus(GPIOPin sda, GPIOPin scl) : STM32Bus(sda, scl)
{
    if(CheckPortAvailability())
    {
        I2CSetup();
    }
    else
    {
        printf("No port available!");
    }
}

bool STM32I2CBus::ReadByte(const uint8_t address, std::array<uint8_t, 9>& packet,
            const std::optional<uint8_t> registerAddress) const
{
    constexpr uint32_t timeoutMs = 100;

    HAL_StatusTypeDef status;

    if (registerAddress != std::nullopt)
    {
        status = HAL_I2C_Mem_Read(
            m_i2cPort.get(),
            static_cast<uint16_t>(address << 1),
            static_cast<uint16_t>(*registerAddress),
            I2C_MEMADD_SIZE_8BIT,
            packet.data(),
            packet.size(),
            timeoutMs
        );
    }
    else
    {
        status = HAL_I2C_Master_Receive(
            m_i2cPort.get(),
            static_cast<uint16_t>(address << 1),
            packet.data(),
            packet.size(),
            timeoutMs
        );
    }

    return status == HAL_OK;
}

GPIOPin STM32I2CBus::get_sda()
{
    return pinOne;
}

GPIOPin STM32I2CBus::get_scl()
{
    return pinTwo;
}

I2C_TypeDef* 

bool STM32I2CBus::CheckPortAvailability()
{
    for (int p = 1; p < I2CMAX; ++p)
    {
        I2C_TypeDef* port = i2cPortMap[p];

        printf("Checking Port Availability");

        if(m_usedI2CPorts.find(port) == m_usedI2CPorts.end())
        {
            m_i2cPort->Instance = port;
            m_usedI2CPorts.insert(port);
            return true;
        }
    }

    return false;
}

void STM32I2CBus::I2CSetup()
{
    GPIO_InitTypeDef gpioInit{};

    gpioInit.Pin = get_sda().pin | get_scl().pin;
    gpioInit.Mode = GPIO_MODE_AF_OD;
    gpioInit.Pull = GPIO_PULLUP;
    gpioInit.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    gpioInit.Alternate = GPIO_AF4_I2C1; // depends on I2C instance + pins

    HAL_GPIO_Init(get_sda().gpioPort, &gpioInit);

    m_i2cPort->Instance = I2C1; // or I2C2/I2C3/etc.

#if defined(USE_STM32F7)
    m_i2cPort->Init.Timing = 0x20404768; // example only, must calculate
#elif defined(USE_STM32F4)
    m_i2cPort->Init.ClockSpeed = 100000;
    m_i2cPort->Init.DutyCycle = I2C_DUTYCYCLE_2;
#endif

    m_i2cPort->Init.OwnAddress1 = 0;
    m_i2cPort->Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    m_i2cPort->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    m_i2cPort->Init.OwnAddress2 = 0;
    m_i2cPort->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    m_i2cPort->Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

    HAL_I2C_Init(m_i2cPort.get());
}

