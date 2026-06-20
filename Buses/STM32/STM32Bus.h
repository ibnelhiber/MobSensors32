#ifndef STM32_BUS_H_
#define STM32_BUS_H_

#include <unordered_set>
#include <unordered_map>
#include <functional>
#include "Config.h"

class STM32Bus
{
protected:
    static std::unordered_set<GPIOPin, GPIOPinHash> m_usedPins;
    
    GPIOPin m_pinOne;
    GPIOPin m_pinTwo;

    bool CheckPinsAvailability(GPIOPin pinOne, GPIOPin pinTwo);
    virtual bool CheckPortAvailability() = 0;
    void EnableGPIOPort();


public:
STM32Bus(GPIOPin pinOne, GPIOPin pinTwo);


};





#endif