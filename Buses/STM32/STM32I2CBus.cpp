#include "STM32I2CBus.h"
#include <cstdio>
#include "Config.h"

std::unordered_set<I2C_TypeDef*> STM32I2CBus::m_usedI2CPorts{};

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
        uint8_t command[] = {0x5A, 0x05, 0x00, 0x01, 0x60};

        HAL_I2C_Master_Transmit(
            m_i2cPort.get(),
            static_cast<uint16_t>(address << 1),
            command,
            5,
            timeoutMs
        );
        
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
    return m_pinOne;
}

GPIOPin STM32I2CBus::get_scl()
{
    return m_pinTwo;
}

bool STM32I2CBus::CheckPortAvailability()
{
    for (int p = 1; p < I2CMAX; ++p)
    {
        I2C_TypeDef* port = i2c_port_map.at(p);

        printf("Checking Port Availability");

        if(m_usedI2CPorts.find(port) == m_usedI2CPorts.end())
        {
            m_i2cPort->Instance = port;
            m_usedI2CPorts.insert(port);
            enable_i2c_clock_map.at(port)();
            return true;
        }
    }

    return false;
}

void STM32I2CBus::I2CSetup()
{
    GPIO_InitTypeDef gpioInit{};

    gpioInit.Pin = get_sda().pin | get_scl().pin;
    // Alternating Function Open Drain (Controlled by I2C peripheral and only drives low)
    gpioInit.Mode = GPIO_MODE_AF_OD;
    gpioInit.Pull = GPIO_PULLUP;
    gpioInit.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    gpioInit.Alternate = GPIO_AF4_I2C1; // depends on I2C instance + pins

    HAL_GPIO_Init(get_sda().gpioPort, &gpioInit);

    m_i2cPort->Init.OwnAddress1 = 0;
    m_i2cPort->Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    m_i2cPort->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    m_i2cPort->Init.OwnAddress2 = 0;
    m_i2cPort->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    m_i2cPort->Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

    SetI2CPortTimingParamaters(m_i2cPort);

    HAL_I2C_Init(m_i2cPort.get());

}
