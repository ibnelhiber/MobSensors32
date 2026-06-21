#include "II2CBus.h"


II2CBus::II2CBus()
{
}


bool II2CBus::CheckBusAvailability(const uint8_t address)
{
    if(m_usedAddresses.find(address) == m_usedAddresses.end())
    {
        m_usedAddresses.insert(address);
        return true;
    }

    return false;
}