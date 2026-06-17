#include "STM32Bus.h"
#include <iostream>


STM32Bus::STM32Bus(GPIOPin pinOne, GPIOPin pinTwo)
{
    if(CheckPinsAvailability(pinOne, pinTwo))
    {
        m_pinOne = pinOne;
        m_pinTwo = pinTwo;

        EnableGPIOPort();
    }
    else
    {
        printf("SDA and SCL pins used are not available!");
    }
}

void STM32Bus::EnableGPIOPort()
{
    if(m_pinOne.gpioPort == m_pinTwo.gpioPort)
    {
        enable_clock_map.at(m_pinOne.gpioPort);
    }
    else
    {
        enable_clock_map.at(m_pinTwo.gpioPort);
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

