#include "Implementation.h"
#include "Buses/II2CBus.h"


std::shared_ptr<II2CBus> CreateI2CImplementation(
    IOPin sda,
    IOPin scl)
{
#if defined(ESP_PLATFORM)
    return std::make_shared<ESP32I2CBus>(sda, scl);
#elif defined(STM32F767xx)
    return std::make_shared<STM32I2CBus>(sda, scl);
#endif
}


std::shared_ptr<IUARTBus> CreateUARTImplementation(
    IOPin sda,
    IOPin scl)
{
#if defined(ESP_PLATFORM)
    return std::make_shared<ESP32UARTBus>(sda, scl);
#elif defined(STM32F767xx)
    return std::make_shared<STM32UARTBus>(sda, scl);
#endif
}