#ifndef IUART_BUS_H_
#define IUART_BUS_H_

#include <vector>
#include <cstdint>

class IUARTBus
{
protected:
    virtual void UARTSetup() = 0;
    virtual bool CheckPortAvailability() = 0;
public:
    IUARTBus();
    virtual bool Read(std::vector<uint8_t>& packet) = 0;
    virtual bool ReadAfterCommand(std::vector<uint8_t>& packet, const std::vector<uint8_t>& command) = 0;
};


#endif