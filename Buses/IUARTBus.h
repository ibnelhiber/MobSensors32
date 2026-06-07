#ifndef IUART_BUS_H_
#define IUART_BUS_H_


#include "Bus.h"



class IUARTBus
{
protected:
    virtual void UARTSetup() = 0;
    virtual bool CheckPortAvailability() = 0;

public:
    IUARTBus();
    virtual bool CheckBusAvailability() = 0;
    virtual bool ReadByte() = 0;
};






