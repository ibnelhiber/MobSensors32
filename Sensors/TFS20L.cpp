#include "TFS20L.h"
#include <iostream>


TFS20L::TFS20L(std::shared_ptr<I2CBus> bus) : Sensor(bus)
{
	printf("Constructing TFS20L with I2C bus");

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

TFS20L::TFS20L(std::unique_ptr<UARTBus> bus) : Sensor(std::move(bus))
{
	// m_uartBus = std::move(bus);

	// if(!m_uartBus->CheckBusAvailability())
	// {
	// 	printf("UART bus not available!");
	// }
	// else
	// {
	// 	printf("UART bus available!");
	// }

	// m_indexes = {3, 2};
}

void TFS20L::ReadSensorI2C_()
{
	std::array<uint8_t,9> packet{};

	m_i2cBus->ReadByte(m_i2cAddress, packet, m_registerAddress);

	m_distance = SetupData(packet);
}

void TFS20L::ReadSensorUART_()
{
	
}

