#include "I2CBus.h"
#include "Buses/Adaptation/Implementation.h"

I2CBus::I2CBus(IOPin sda, IOPin scl) : m_implementation{CreateI2CImplementation(sda, scl)}
{}


bool I2CBus::CheckBusAvailability(const uint8_t address)
{
    if(m_implementation)
    {
        return m_implementation->CheckAddressAvailability(address);
    }

    return false;
}

bool I2CBus::Read(const uint8_t address, std::vector<uint8_t>& packet)
{
     if(m_implementation)
    {
        return m_implementation->Read(address, packet);
    }

    return false;   
}


bool I2CBus::ReadFromRegister(const uint8_t address, std::vector<uint8_t>& packet,
const int registerAddress)
{
    if(m_implementation)
    {
        return m_implementation->ReadFromRegister(address, packet, registerAddress);
    }

    return false;
}


bool I2CBus::ReadAfterCommand(const uint8_t address, std::vector<uint8_t>& packet, 
const std::vector<uint8_t>& command)
{
    if(m_implementation)
    {
        return m_implementation->ReadAfterCommand(address, packet, command);
    }

    return false;
}

bool I2CBus::Write(const uint8_t address, const std::vector<uint8_t>& command)
{
    if(m_implementation)
    {
        return m_implementation->Write(address, command);
    }

    return false;
}

bool I2CBus::WriteToRegister(const uint8_t address, const int registerAddress,
        std::vector<uint8_t>& command)
{
    if(m_implementation)
    {
        return m_implementation->WriteToRegister(address, registerAddress, command);
    }

    return false;
}
