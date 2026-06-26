#include "TFLuna.h"
#include <iostream>


TFLuna::TFLuna(std::shared_ptr<I2CBus> bus) : Sensor(bus)
{
	printf("Constructing TFLuna with I2C bus");

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

TFLuna::TFLuna(std::unique_ptr<UARTBus> bus) : Sensor(std::move(bus))
{
	m_uartBus = std::move(bus);
	m_indexes = {3, 2};
}

void TFLuna::ReadSensorI2C_()
{
	std::array<uint8_t,9> packet{};
	uint8_t distance_register_addr = 0x00;
	m_i2cBus->ReadFromRegister(m_i2cAddress, packet, distance_register_addr);

	m_distance = SetupData(packet);

	printf("Distance: %f\n", m_distance);
}

void TFLuna::ReadSensorUART_()
{
	std::array<uint8_t,9> packet{};
	constexpr uint8_t distance_register_addr = 0x00;
	m_uartBus->ReadFromRegister(packet, distance_register_addr);

	m_distance = SetupData(packet);

	printf("Distance: %f\n", m_distance);
}

