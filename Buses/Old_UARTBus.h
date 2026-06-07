#ifndef MAIN_BUSES_UARTBUS_H_
#define MAIN_BUSES_UARTBUS_H_

#include <memory>
#include <unordered_set>
#include "driver/uart.h"

template<typename UARTPort>

class UARTBus : public Bus
{
private:
    static std::unordered_set<UARTPort> m_usedUARTPorts;
    std::unique_ptr<uart_port_t> m_uartPort;
    void UARTSetup();

    bool CheckPinsAvailability(GPIOPin pinOne, GPIOPin pinTwo) override;
    bool CheckPortAvailability() override;

    GPIOPin get_tx() const;
    GPIOPin get_rx() const;

public:
    UARTBus(uint8_t tx, uint8_t rx);
    bool CheckBusAvailability();
    uart_port_t getUARTPort() const;
};

#endif
