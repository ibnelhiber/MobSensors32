#ifndef STM32UARTBus_H_
#define STM32UARTBus_H_

#include "Buses/IUARTBus.h"
#include "STM32Bus.h"
#include <unordered_map>
#include <unordered_set>
#include <memory>

class STM32UARTBus : public IUARTBus, public STM32Bus
{

private:
    UART_HandleTypeDef m_uartPort{};
    bool CheckPortAvailability() override;
    TaskHandle_t m_waitingTask{nullptr};
    volatile bool m_readComplete{false};
    
    void UARTSetup() override;

    GPIOPin get_tx();
    GPIOPin get_rx();
     
public:
    STM32UARTBus(GPIOPin tx, GPIOPin rx);
    ~STM32UARTBus();
    bool Read(std::vector<uint8_t>& packet) override;
    bool ReadAfterCommand(std::vector<uint8_t>& packet, const std::vector<uint8_t>& command) override;
    static void HandleUARTIRQ(USART_TypeDef* peripheralInstance);
    static void HandleReadComplete(UART_HandleTypeDef* uartPort);
    static std::unordered_map<USART_TypeDef*, STM32UARTBus*> m_instanceMap;

};


#endif
