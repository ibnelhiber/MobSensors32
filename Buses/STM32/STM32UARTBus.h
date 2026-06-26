#ifndef STM32UARTBus_H_
#define STM32UARTBus_H_

#include "Buses/IUARTBus.h"
#include "STM32Bus.h"
#include <unordered_map>
#include <memory>


class STM32UARTBus : public IUARTBus, public STM32Bus
{

private:
    static std::unordered_set<USART_TypeDef*> m_usedI2CPorts;
    std::unique_ptr<UART_HandleTypeDef> m_uartPort{
        std::make_unique<UART_HandleTypeDef>()};
    bool CheckPortAvailability() override;
    
    void UARTSetup() override;

    GPIOPin get_tx();
    GPIOPin get_rx();

public:
    STM32UARTBus(GPIOPin tx, GPIOPin rx);
    bool Read(std::vector<uint8_t>& packet) override;
    bool ReadFromRegister(std::vector<uint8_t>& packet, const int registerAddress) override;
    bool ReadAfterCommand(std::vector<uint8_t>& packet, const std::vector<uint8_t>& command) override;
};


#endif