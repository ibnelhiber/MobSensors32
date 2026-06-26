#ifndef ESP32_BUS_H_
#define ESP32_BUS_H_

#include <unordered_set>
#include "driver/gpio.h"



class ESP32Bus
{

protected:
    // Inline allows initialize of static member
    inline static std::unordered_set<gpio_num_t> m_usedPins{};
    
    gpio_num_t m_pinOne;
    gpio_num_t m_pinTwo;

    bool CheckPinsAvailability(gpio_num_t pinOne, gpio_num_t pinTwo);
public:
    ESP32Bus(gpio_num_t pinOne, gpio_num_t pinTwo);
};



#endif