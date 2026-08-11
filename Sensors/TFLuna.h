#ifndef MAIN_SENSORS_TFLuna_H_
#define MAIN_SENSORS_TFLuna_H_

#include "Sensor.h"
#include "Buses/I2CBus.h"
#include "Tasks/Task.h"


class TFLuna : public Sensor
{
private:
    void ReadSensorI2C_() override;
    void ReadSensorUART_()  override;
    void WriteSensorI2C_(const std::vector<uint8_t>& command) override;
    void WriteSensorUART_(const std::vector<uint8_t>& command) override;
    float SetupData(std::vector<uint8_t> rawDataVector);
    static constexpr uint8_t m_i2cAddress = 0x10;
public:
    explicit TFLuna(std::shared_ptr<I2CBus> bus);
    explicit TFLuna(std::unique_ptr<UARTBus> bus);
    double get_distance();

};

#endif