#include "I2CBus.h"
#include "Config.h"


I2CBus::I2CBus(IOPin sda, IOPin scl)
{

    #if defined(USE_ESP32)
        implementation = std::make_shared<ESP32I2CBus>(sda, scl);
    #elif defined(USE_STM32F7) || defined(USE_STM32F4)
        implementation = std::make_shared<STM32I2CBus>(sda, scl);
    #endif

}

bool I2CBus::ReadByte(const uint8_t address, std::array<uint8_t, 9>& packet, 
    std::optional<uint8_t> registerAddress) const
{
    if(implementation)
    {
        return implementation->ReadByte(address, packet, registerAddress);
    }

    return false;
}

bool I2CBus::CheckBusAvailability(const uint8_t address)
{
    if(implementation)
    {
        return implementation->CheckBusAvailability(address);
    }

    return false;
}
