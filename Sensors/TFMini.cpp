#include "TFMini.h"
#include <iostream>


TFMini::TFMini(std::shared_ptr<I2CBus> bus) : Sensor(bus)
{
	printf("Constructing TFMini with I2C bus");

	if (!m_i2cBus->CheckBusAvailability(m_i2cAddress)) 
	{
        printf("I2C bus not available!");
    }
	else
	{
		printf("I2C Bus Available!");
	}
	 m_indexes = {3, 2};
}

TFMini::TFMini(std::unique_ptr<UARTBus> bus) : 
	Sensor(std::move(bus))
{
	m_uartBus = std::move(bus);
	m_indexes = {3, 2};
}

void TFMini::ReadSensorI2C_()
{
	std::array<uint8_t,9> packet{};

	m_i2cBus->ReadByte(m_i2cAddress, packet);

	m_distance = SetupData(packet);
}

void TFMini::ReadSensorUART_()
{
	std::array<uint8_t,9> packet{};
	constexpr uint8_t distance_register_addr = 0x00;
	m_uartBus->ReadFromRegister(packet, distance_register_addr);

	m_distance = SetupData(packet);

	printf("Distance: %f\n", m_distance);
}

