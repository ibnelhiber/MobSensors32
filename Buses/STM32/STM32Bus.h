#ifndef STM32_BUS_H_
#define STM32_BUS_H_

#include <unordered_set>
#include <unordered_map>
#include <functional>
#include "Config.h"


typedef struct
{
    GPIO_TypeDef* gpioPort;
    uint16_t pin;

    // Equality operator required for hashing
    bool operator==(const GPIOPin& pin) = default;

} GPIOPin;

struct GPIOPinHash
{
std::size_t operator()(const GPIOPin& gpioPin) const
{
    // std::hash is callable. Argument is what to create a hash for
    std::size_t portHashVal = std::hash<GPIO_TypeDef*>(gpioPin.gpioPort);
    std::size_t pinHashVal = std::hash<uint16_t>(gpioPin.pin)

    //Avoid collisions by not making flipped values equivalent
    return portHashVal ^ (pinHashVal << 1);
}

};


class STM32Bus
{

protected:
    static std::unordered_set<GPIOPin, GPIOPinHash> m_usedPins{};
    
    GPIOPin m_pinOne;
    GPIOPin m_pinTwo;

    bool CheckPinsAvailability(GPIOPin pinOne, GPIOPin pinTwo);
    virtual bool CheckPortAvailability() = 0;
    void EnableGPIOPort();


public:
STM32Bus(GPIOPin pinOne, GPIOPin pinTwo);


};





#endif