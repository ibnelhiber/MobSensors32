#include "STM32Bus.h"


STM32Bus::STM32Bus(GPIOPin pinOne, GPIOPin pinTwo)
{
    if(CheckPinsAvailability(pinOne, pinTwo))
    {
        m_pinOne = pinOne;
        m_pinTwo = pinTwo;
    }
    else
    {
        printf("SDA and SCL pins used are not available!");
    }
}


bool STM32Bus::CheckPinsAvailability(GPIOPin pinOne, GPIOPin pinTwo)
{
    int m_usedPinsAmount = static_cast<int>(m_usedPins.size());

    printf("Checking Pins Availability");


    if(m_usedPins.find(pinOne) == m_usedPins.end())
    {
        m_usedPins.insert(pinOne);
    }
    else
    {
        std::cout << "pinOne pin: " << pinOne << " not available!" << std::endl;
    }
    
    if(m_usedPins.find(pinTwo) == m_usedPins.end())
    {
        m_usedPins.insert(pinTwo);
    }
    else
    {
        std::cout << "pinTwo pin: " << pinTwo << " not available!" << std::endl;
    }

    if((m_usedPinsAmount + 2) != static_cast<int>(m_usedPins.size()))
    {
        return false;
    }
    else
    {
        return true;
    }
}

