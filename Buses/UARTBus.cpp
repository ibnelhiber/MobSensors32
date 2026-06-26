#include "UARTBus.h"
#include "Buses/Adaptation/Implementation.h"

UARTBus::UARTBus(IOPin tx, IOPin rx)
    : m_implementation{CreateUARTImplementation(tx, rx)}
{
}

bool UARTBus::CheckBusAvailability()
{
    if(m_implementation)
    {
        return m_implementation->CheckBusAvailability();
    }

    return false;
}

bool UARTBus::Read(const uint8_t address, std::vector<uint8_t>& packet)
{
     if(m_implementation)
    {
        return m_implementation->Read(address, packet);
    }

    return false;   
}


bool UARTBus::ReadFromRegister(const uint8_t address, std::vector<uint8_t>& packet,
const int registerAddress)
{
    if(m_implementation)
    {
        return m_implementation->ReadFromRegister(address, packet, registerAddress);
    }

    return false;
}


bool UARTBus::ReadAfterCommand(const uint8_t address, std::vector<uint8_t>& packet, 
const std::vector<uint8_t>& command)
{
    if(m_implementation)
    {
        return m_implementation->ReadAfterCommand(address, packet, command);
    }

    return false;
}