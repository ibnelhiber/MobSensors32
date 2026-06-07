#ifndef I2C_BUS_H_
#define I2C_BUS_H_

#include <memory>
#include <cstdint>
#include "Config.h"
#include "II2CBus.h"

class I2CBus
{
private:
    std::shared_ptr<II2CBus> implementation;

public:
    I2CBus(IOPin sda, IOPin scl);
    bool CheckBusAvailability(const uint8_t address);
    bool ReadByte(const uint8_t address, std::array<uint8_t, 9>& packet,
        std::optional<uint8_t> registerAddress = std::nullopt) const;
};

#endif