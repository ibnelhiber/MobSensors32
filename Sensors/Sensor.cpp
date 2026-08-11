#include "Sensor.h"
#include <iostream>

Sensor::Sensor(std::shared_ptr<I2CBus> bus) : 
    m_i2cBus{bus},
    ReadSensor{[this](){ReadSensorI2C_();}}
{
}

Sensor::Sensor(std::unique_ptr<UARTBus> bus) :
    m_uartBus{std::move(bus)},
    ReadSensor{[this](){ReadSensorUART_();}}
{
}

Sensor::~Sensor()
{
	StopPolling();
}

void Sensor::PollSensor(const int delayPeriod)
{
	printf("Starting to poll sensor");
    if(!m_pollingTask)
    {
        m_pollingTask = std::make_unique<Task>(shared_from_this());
        m_pollingTask->StartTask(delayPeriod);
    }
    else
    {
        std::cout << "This sensor is already polling!" << std::endl;
    }
}

void Sensor::StopPolling()
{
    if (m_pollingTask) 
	{
		printf("Stopped Polling Sensor");
        m_pollingTask->EndTask();
		m_pollingTask = nullptr;
    }
}
