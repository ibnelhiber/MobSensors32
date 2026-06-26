



#ifndef MAIN_SENSORS_TFMini_H_
#define MAIN_SENSORS_TFMini_H_

#include "Sensor.h"
#include "Buses/I2CBus.h"
#include "Tasks/Task.h"


class TFMini : public Sensor
{
private:
    void ReadSensorI2C_() override;
    void ReadSensorUART_()  override;
    static constexpr uint8_t m_i2cAddress = 0x10;
public:
    explicit TFMini(std::shared_ptr<I2CBus> bus);
    explicit TFMini(std::unique_ptr<UARTBus> bus);
};






#endif