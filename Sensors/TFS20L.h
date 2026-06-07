#ifndef MAIN_SENSORS_TFS20L_H_
#define MAIN_SENSORS_TFS20L_H_

#include "Sensor.h"
#include "Buses/I2CBus.h"
#include "Tasks/Task.h"


class TFS20L : public Sensor
{
private:
    void ReadSensorI2C_() override;
    void ReadSensorUART_()  override;
    static constexpr uint8_t m_i2cAddress = 0x10;
    static constexpr uint8_t m_registerAddress = 0x00;

public:
    explicit TFS20L(std::shared_ptr<I2CBus> bus);
    explicit TFS20L(std::unique_ptr<UARTBus> bus);
};


#endif