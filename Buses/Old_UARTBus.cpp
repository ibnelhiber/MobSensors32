#include "UARTBus.h"
#include "driver/gpio.h"
#include <cassert>

UARTBus::UARTBus(uint8_t tx, uint8_t rx) : Bus(tx, rx)
{
    if(!(CheckPinsAvailability(tx, rx) && CheckPortAvailability()))
    {
        std::exit(EXIT_FAILURE);
    }

    UARTSetup();
}

void UARTBus::UARTSetup()
{
uart_config_t cfg = {};

    cfg.baud_rate = 115200;
    cfg.data_bits = UART_DATA_8_BITS;
    cfg.parity = UART_PARITY_DISABLE;
    cfg.stop_bits = UART_STOP_BITS_1;
    cfg.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
    cfg.rx_flow_ctrl_thresh = 0;
    cfg.source_clk = UART_SCLK_DEFAULT;

    ESP_ERROR_CHECK(uart_param_config(*m_uartPort, &cfg));

    ESP_ERROR_CHECK(uart_set_pin(*m_uartPort,
                                 getTx(),
                                 getRx(),
                                 UART_PIN_NO_CHANGE,
                                 UART_PIN_NO_CHANGE));

    ESP_ERROR_CHECK(uart_driver_install(*m_uartPort,
                                        2048,   // RX Buffer size
                                        0,      // TX Buffer size
                                        0,    
                                        NULL,
                                        0));
}

uart_port_t UARTBus::getUARTPort() const 
{
    return *m_uartPort;
}

bool UARTBus::CheckPinsAvailability(uint8_t tx, uint8_t rx)
{
    assert(GPIO_IS_VALID_GPIO(tx));
    assert(GPIO_IS_VALID_GPIO(rx));
    
    int usedPinsAmount = static_cast<int>(m_usedPins.size());

    if(m_usedPins.find(tx) == m_usedPins.end())
    {
        m_usedPins.insert(tx);
    }
    else
    {
        std::cout << "TX pin: " << tx << " not available!" << std::endl;
    }
    if(m_usedPins.find(rx) == m_usedPins.end())
    {
        m_usedPins.insert(rx);
    }
    else
    {
        std::cout << "RX pin: " << rx << " not available!" << std::endl;
    }

    if(usedPinsAmount != static_cast<int>(m_usedPins.size()) + 2)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool UARTBus::CheckPortAvailability()
{
    for(int port = static_cast<int>(UART_NUM_0); port <= static_cast<int>(UART_NUM_MAX); port++)
    {
        uart_port_t uartPort = static_cast<uart_port_t>(port);

        if(uartPort == UART_NUM_MAX)
        {
            std::cout << "No UART Port available!" << std::endl;
            return false;
        }
        if(m_usedUARTPorts.find(uartPort) == m_usedUARTPorts.end())
        {
            m_uartPort = std::make_unique<uart_port_t>(uartPort);
            break;
        }
    }

    return true;
}


uint8_t UARTBus::getTx() const
{
    return m_pinOne;
}

uint8_t UARTBus::getRx() const
{
    return m_pinTwo;
}

