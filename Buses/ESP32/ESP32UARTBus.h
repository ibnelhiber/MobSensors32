#ifndef ESP32UARTBus_H_
#define ESP32UARTBus_H_

#include "IUARTBus.h"
#include "ESP32Bus.h"
#include <memory>
#include <vector>
#include "driver/UART.h"
#include "esp_log.h"


class ESP32UARTBus : public IUARTBus, public ESP32Bus
{

private:
    inline static std::unordered_set<uart_port_t> m_usedUARTPorts{};
    uart_port_t m_uartPort;
    bool CheckPortAvailability() override;
    
    void UARTSetup() override;

    gpio_num_t get_tx();
    gpio_num_t get_rx();
public:
    ESP32UARTBus(gpio_num_t tx, gpio_num_t rx);
    bool Read(std::vector<uint8_t>& packet) override;
    bool ReadAfterCommand(std::vector<uint8_t>& packet, 
    const std::vector<uint8_t>& command) override;
};


#endif