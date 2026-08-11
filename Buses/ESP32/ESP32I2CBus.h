#ifndef ESP32I2CBus_H_
#define ESP32I2CBus_H_

#include "II2CBus.h"
#include "ESP32Bus.h"
#include <memory>
#include "driver/i2c.h"
#include "esp_log.h"


class ESP32I2CBus : public II2CBus, public ESP32Bus
{

private:
    inline static std::unordered_set<i2c_port_t> m_usedI2CPorts{};
    i2c_port_t m_i2cPort;
    bool CheckPortAvailability() override;
    
    void I2CSetup() override;

    gpio_num_t get_sda();
    gpio_num_t get_scl();
public:
    ESP32I2CBus(gpio_num_t sda, gpio_num_t scl);
    bool Read(const uint8_t address, std::vector<uint8_t>& packet) override;
    bool ReadFromRegister(const uint8_t address, std::vector<uint8_t>& packet,
    const int registerAddress) override;
    bool ReadAfterCommand(const uint8_t address, std::vector<uint8_t>& packet, 
    const std::vector<uint8_t>& command) override;
    bool Write(const uint8_t address, const std::vector<uint8_t>& command);
    bool WriteToRegister(const uint8_t address, const int registerAddress,
        std::vector<uint8_t>& command);

};


#endif