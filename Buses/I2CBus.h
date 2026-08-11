#ifndef I2C_BUS_H_
#define I2C_BUS_H_

#include <memory>
#include <cstdint>
#include "Buses/Adaptation/Adapt.h"
#include "II2CBus.h"

class I2CBus
{
private:
    std::shared_ptr<II2CBus> m_implementation;
public:
    I2CBus(IOPin sda, IOPin scl);
    bool CheckBusAvailability(const uint8_t address);
    bool Read(const uint8_t address, std::vector<uint8_t>& packet);
    bool ReadFromRegister(const uint8_t address, std::vector<uint8_t>& packet,
    const int registerAddress);
    bool ReadAfterCommand(const uint8_t address, std::vector<uint8_t>& packet, 
    const std::vector<uint8_t>& command);
    bool Write(const uint8_t address, const std::vector<uint8_t>& command);
    bool WriteToRegister(const uint8_t address, const int registerAddress,
        std::vector<uint8_t>& command);
};

#endif