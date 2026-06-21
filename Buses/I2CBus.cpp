#include "I2CBus.h"
#include "Buses/Adaptation/Implementation.h"

I2CBus::I2CBus(IOPin sda, IOPin scl) : m_implementation{CreateI2CImplementation(sda, scl)}
{

}

bool I2CBus::ReadByte(const uint8_t address, std::array<uint8_t, 9>& packet, 
    std::optional<uint8_t> registerAddress) const
{
    if(m_implementation)
    {
        return m_implementation->ReadByte(address, packet, registerAddress);
    }

    return false;
}

bool I2CBus::CheckBusAvailability(const uint8_t address)
{
    if(m_implementation)
    {
        return m_implementation->CheckBusAvailability(address);
    }

    return false;
}
