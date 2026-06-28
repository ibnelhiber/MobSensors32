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
	 m_indexes = {1, 0};
}

TFLuna::TFLuna(std::unique_ptr<UARTBus> bus) : Sensor(std::move(bus))
{
	m_indexes = {3, 2};
}


float TFLuna::SetupData(std::vector<uint8_t> rawDataVector)
{
	float currentDistance =  (rawDataVector[m_indexes.highIndex] << 8 
        | rawDataVector[m_indexes.lowIndex])/ 100.00;
    return currentDistance;
}

void TFLuna::ReadSensorI2C_()
{
	std::vector<uint8_t> packet(9);
	uint8_t distance_register_addr = 0x00;
	const std::vector<uint8_t> command = {0x5A, 0x05, 0x00, 0x01, 0x60};
	m_i2cBus->ReadFromRegister(m_i2cAddress, packet, distance_register_addr);

	m_distance = SetupData(packet);

	printf("Distance: %f\n", m_distance);
}

void TFLuna::ReadSensorUART_()
{
	std::vector<uint8_t> packet(9);
	m_uartBus->Read(packet);

	m_distance = SetupData(packet);

	printf("Distance: %f\n", m_distance);
}

