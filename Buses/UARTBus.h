#ifndef UARTBus_H_
#define UARTBus_H_

#include <memory>
#include <cstdint>
#include <vector>
#include "Buses/Adaptation/Adapt.h"
#include "IUARTBus.h"

class UARTBus
{

private:
    std::shared_ptr<IUARTBus> m_implementation;
public:
    UARTBus(IOPin tx, IOPin rx);
    bool Read(std::vector<uint8_t>& packet);
    bool ReadAfterCommand(std::vector<uint8_t>& packet, 
    const std::vector<uint8_t>& command);
};


#endif