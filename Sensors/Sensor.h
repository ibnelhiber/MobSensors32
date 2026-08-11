/*
 * Sensor.h
 *
 *  Created on: Jan 22, 2026
 *      Author: moham
 */
#ifndef SENSORS_SENSOR_H_
#define SENSORS_SENSOR_H_

#include <stdint.h>
#include <cstdint>
#include <array>
#include <optional>
#include "Buses/UARTBus.h"
#include "Buses/I2CBus.h"
#include "Tasks/Task.h"
#include <memory>
#include <functional>

typedef struct
{
    uint8_t highIndex;
    uint8_t lowIndex;

} distanceIndexes;

class Sensor : public std::enable_shared_from_this<Sensor>
{
protected:
    std::unique_ptr<Task> m_pollingTask;

    float m_distance;

    distanceIndexes m_indexes;
    std::shared_ptr<I2CBus> m_i2cBus;
    std::unique_ptr<UARTBus> m_uartBus;

    virtual void ReadSensorI2C_() = 0;
    virtual void ReadSensorUART_() = 0;
    virtual void WriteSensorI2C_(const std::vector<uint8_t>& command) = 0;
    virtual void WriteSensorUART_(const std::vector<uint8_t>& command) = 0;

public:
    explicit Sensor(std::shared_ptr<I2CBus> bus);
    explicit Sensor(std::unique_ptr<UARTBus> bus);
    ~Sensor();
    std::function<void(void)> ReadSensor;
    std::function<void(const std::vector<uint8_t>& command)> WriteSensor;
    void PollSensor(const int delayPeriod);
    void StopPolling();
};





#endif /* MAIN_HEADERS_SENSOR_H_ */
