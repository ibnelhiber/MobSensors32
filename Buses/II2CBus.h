#ifndef II2C_H_
#define II2C_H_

#include <optional>
#include <array>
#include <cstdint>
#include <optional>
#include <unordered_set>
#include <vector>
#include <memory>


class II2CBus
{
protected:
    inline static std::unordered_set<uint8_t> m_usedAddresses{};
    virtual void I2CSetup() = 0;
    virtual bool CheckPortAvailability() = 0;

public:
    II2CBus();
    bool CheckAddressAvailability(const uint8_t address);
    virtual bool Read(const uint8_t address, std::vector<uint8_t>& packet) = 0;
    virtual bool ReadFromRegister(const uint8_t address, std::vector<uint8_t>& packet,
    const int registerAddress) = 0;
    virtual bool ReadAfterCommand(const uint8_t address, std::vector<uint8_t>& packet, 
    const std::vector<uint8_t>& command) = 0;
};

#endif