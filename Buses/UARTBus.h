#ifndef UARTBus_H_
#define UARTBus_H_

#include <memory>
#include <cstdint>
#include "Buses/Adaptation/Adapt.h"
#include "IUARTBus.h"

class UARTBus
{

private:
    std::shared_ptr<II2CBus> m_implementation;
public:
    UARTBus(gpio_num_t tx, gpio_num_t rx);
    bool Read(std::vector<uint8_t>& packet) override;
    bool ReadFromRegister(std::vector<uint8_t>& packet,
    const int registerAddress) override;
    bool ReadAfterCommand(std::vector<uint8_t>& packet, 
    const std::vector<uint8_t>& command) override;
};


#endif