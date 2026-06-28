#pragma once

#include <memory>
#include "Buses/Adaptation/Adapt.h"

#if defined(ESP_PLATFORM)
    #include "Buses/ESP32/ESP32I2CBus.h"
    #include "Buses/ESP32/ESP32UARTBus.h"
#elif defined(STM32F767xx)
    #include "Buses/STM32/STM32I2CBus.h"
    #include "Buses/STM32/STM32UARTBus.h"
#endif

class II2CBus;
class IUARTBus;

std::shared_ptr<II2CBus> CreateI2CImplementation(
    IOPin sda,
    IOPin scl);

std::shared_ptr<IUARTBus> CreateUARTImplementation(
    IOPin sda,
    IOPin scl);