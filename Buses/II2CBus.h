#ifndef II2C_H_
#define II2C_H_

#include <optional>
#include <array>
#include <cstdint>
#include <optional>
#include <unordered_set>


class II2CBus
{
protected:
    inline static std::unordered_set<uint8_t> m_usedAddresses{};
    virtual void I2CSetup() = 0;
    virtual bool CheckPortAvailability() = 0;

public:
    II2CBus();
    bool CheckBusAvailability(const uint8_t address);
    virtual bool ReadByte(const uint8_t address, std::array<uint8_t, 9>& packet,
        std::optional<uint8_t> registerAddress = std::nullopt) const = 0;
};

#endif