#include "ESP32UARTBus.h"

static const char* TAG = "ESP32UARTBus";

ESP32UARTBus::ESP32UARTBus(gpio_num_t tx, gpio_num_t rx)
    : ESP32Bus(tx, rx)
{
    if (CheckPortAvailability())
    {
        UARTSetup();
    }
}

void ESP32UARTBus::UARTSetup()
{
    uart_config_t config{};
    config.baud_rate = 115200;
    config.data_bits = UART_DATA_8_BITS;
    config.parity    = UART_PARITY_DISABLE;
    config.stop_bits = UART_STOP_BITS_1;
    config.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
    config.source_clk = UART_SCLK_DEFAULT;

    uart_param_config(*m_uartPort, &config);

    uart_set_pin(
        *m_uartPort,
        get_tx(),
        get_rx(),
        UART_PIN_NO_CHANGE, // Don't set RTS pin
        UART_PIN_NO_CHANGE // Don't set CTS pin
    );

    uart_driver_install(
        *m_uartPort,
        1024,   // RX buffer size
        // No TX buffer needed, ESP32S3 buffer 128 bytes.
        // We only send out a maximum of 5 bytes at a time right now.
        0,      
        0,      // No need for queue, we will be polling.
        nullptr,
        0 // Use default interrupt level for UART
    );
}

bool ESP32UARTBus::CheckPortAvailability()
{
    for (int p = static_cast<int>(UART_NUM_0); p < static_cast<int>(UART_NUM_MAX); ++p)
    {
        uart_port_t port = static_cast<uart_port_t>(p);

        ESP_LOGI(TAG, "Checking UART Port Availability");

        if (m_usedUARTPorts.find(port) == m_usedUARTPorts.end())
        {
            m_uartPort = std::make_unique<uart_port_t>(port);
            m_usedUARTPorts.insert(port);
            return true;
        }
    }

    return false;
}

bool ESP32UARTBus::Read(std::vector<uint8_t>& packet)
{
    int bytesRead = uart_read_bytes(
        *m_uartPort,
        packet.data(),
        packet.size(),
        pdMS_TO_TICKS(100)
    );

    return bytesRead == static_cast<int>(packet.size());
}


bool ESP32UARTBus::ReadAfterCommand(
    std::vector<uint8_t>& packet,
    const std::vector<uint8_t>& command)
{
    int bytes_written = uart_write_bytes(
    *m_uartPort,
    reinterpret_cast<const char*>(command.data()), // Data is uint8_t so reintrept as char
    command.size()
    );

    bool write_status = bytesWritten == static_cast<int>(command.size());

    if (!write_status)
    {
        return false;
    }

    int bytesRead = uart_read_bytes(
    *m_uartPort,
    packet.data(),
    packet.size(),
    pdMS_TO_TICKS(100)
    );

    return bytesRead == static_cast<int>(packet.size());
}

gpio_num_t ESP32UARTBus::get_tx()
{
    return m_pinOne;
}

gpio_num_t ESP32UARTBus::get_rx()
{
    return m_pinTwo;
}