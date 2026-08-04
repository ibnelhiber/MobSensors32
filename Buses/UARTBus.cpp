#include "UARTBus.h"
#include "Buses/Adaptation/Implementation.h"

UARTBus::UARTBus(IOPin tx, IOPin rx)
    : m_implementation{CreateUARTImplementation(tx, rx)}
{
}


bool UARTBus::Read(std::vector<uint8_t>& packet)
{
    if(m_implementation)
    {
        return m_implementation->Read(packet);
    }

    return false;   
}


bool UARTBus::ReadAfterCommand(std::vector<uint8_t>& packet, 
const std::vector<uint8_t>& command)
{
    if(m_implementation)
    {
        return m_implementation->ReadAfterCommand(packet, command);
    }

    return false;
}