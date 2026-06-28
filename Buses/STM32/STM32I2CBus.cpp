#include "STM32I2CBus.h"
#include <cstdio>
#include "Buses/Adaptation/Adapt.h"

std::unordered_set<I2C_TypeDef*> STM32I2CBus::m_usedI2CPorts{};

constexpr uint32_t TIME_OUT_MS = 100;

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


void STM32I2CBus::I2CSetup()
{
    GPIO_InitTypeDef sdaInit{};
    sdaInit.Pin = get_sda().pin;
    sdaInit.Mode = GPIO_MODE_AF_OD;
    sdaInit.Pull = GPIO_PULLUP;
    sdaInit.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    sdaInit.Alternate = enable_i2c_clock_map.at(m_i2cPort.Instance).portMacro;

    HAL_GPIO_Init(get_sda().gpioPort, &sdaInit);

    GPIO_InitTypeDef sclInit{};
    sclInit.Pin = get_scl().pin;
    sclInit.Mode = GPIO_MODE_AF_OD;
    sclInit.Pull = GPIO_PULLUP;
    sclInit.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    sclInit.Alternate = enable_i2c_clock_map.at(m_i2cPort.Instance).portMacro;

    HAL_GPIO_Init(get_scl().gpioPort, &sclInit);

    m_i2cPort.Init.OwnAddress1 = 0;
    m_i2cPort.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    m_i2cPort.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    m_i2cPort.Init.OwnAddress2 = 0;
    m_i2cPort.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    m_i2cPort.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

    SetI2CPortTimingParamaters(m_i2cPort);

    HAL_I2C_Init(&m_i2cPort);

}


bool STM32I2CBus::CheckPortAvailability()
{
    I2C_TypeDef* sda_port = i2c_pin_options_map.at(get_sda()).i2cPort;
    I2C_TypeDef* scl_port = i2c_pin_options_map.at(get_scl()).i2cPort;

    if(sda_port == scl_port && m_usedI2CPorts.find(sda_port) == m_usedI2CPorts.end())
    {
        m_i2cPort.Instance = sda_port;
        m_usedI2CPorts.insert(sda_port);
        enable_i2c_clock_map.at(sda_port).enableI2C();
        return true;
    }

    return false;
}


bool STM32I2CBus::Read(const uint8_t address, std::vector<uint8_t>& packet)
{
    HAL_StatusTypeDef status = HAL_I2C_Master_Receive(
        &m_i2cPort,
        static_cast<uint16_t>(address << 1),
        packet.data(),
        packet.size(),
        TIME_OUT_MS
    );

    return status == HAL_OK;
}

bool STM32I2CBus::ReadFromRegister(const uint8_t address, std::vector<uint8_t>& packet,
const int registerAddress)
{
    HAL_StatusTypeDef status = HAL_I2C_Mem_Read(
        &m_i2cPort,
        static_cast<uint16_t>(address << 1),
        static_cast<uint16_t>(registerAddress),
        I2C_MEMADD_SIZE_8BIT,
        packet.data(),
        packet.size(),
        TIME_OUT_MS
    );

    return status == HAL_OK;
}


bool STM32I2CBus::ReadAfterCommand(
    const uint8_t address,
    std::vector<uint8_t>& packet,
    const std::vector<uint8_t>& command)
{
    HAL_StatusTypeDef status;

    status = HAL_I2C_Master_Transmit(
        &m_i2cPort,
        static_cast<uint16_t>(address << 1),
        const_cast<uint8_t*>(command.data()),
        static_cast<uint16_t>(command.size()),
        TIME_OUT_MS
    );

    if (status != HAL_OK)
    {
        return false;
    }

    status = HAL_I2C_Master_Receive(
        &m_i2cPort,
        static_cast<uint16_t>(address << 1),
        packet.data(),
        static_cast<uint16_t>(packet.size()),
        TIME_OUT_MS
    );

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
