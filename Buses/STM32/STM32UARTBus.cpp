#include "Buses/STM32/STM32Bus.h"
#include "Buses/STM32/STM32UARTBus.h"

constexpr uint32_t TIME_OUT_MS = 100;

STM32UARTBus::STM32UARTBus(GPIOPin tx, GPIOPin rx) : STM32Bus(tx, rx)
{
    if(CheckPortAvailability())
    {
        UARTSetup();
    }
    else
    {
        printf("No port available!");
    }
}

void STM32UARTBus::UARTSetup()
{
    GPIO_InitTypeDef txInit{};
    txInit.Pin = get_tx().pin;
    txInit.Mode = GPIO_MODE_AF_OD;
    txInit.Pull = GPIO_PULLUP;
    txInit.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    txInit.Alternate = enable_uart_clock_map.at(m_uartPort.Instance).alternateFunction;

    HAL_GPIO_Init(get_tx().gpioPort, &txInit);

    GPIO_InitTypeDef rxInit{};
    rxInit.Pin = get_rx().pin;
    rxInit.Mode = GPIO_MODE_AF_OD;
    rxInit.Pull = GPIO_PULLUP;
    rxInit.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    rxInit.Alternate = enable_uart_clock_map.at(m_uartPort.Instance).alternateFunction;

    HAL_GPIO_Init(get_rx().gpioPort, &rxInit);

    m_uartPort.Init.BaudRate = 115200;
    m_uartPort.Init.WordLength = UART_WORDLENGTH_8B;
    m_uartPort.Init.StopBits = UART_STOPBITS_1;
    m_uartPort.Init.Parity = UART_PARITY_NONE;
    m_uartPort.Init.Mode = UART_MODE_TX_RX;
    m_uartPort.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    m_uartPort.Init.OverSampling = UART_OVERSAMPLING_16;

    HAL_UART_Init(&m_uartPort);
}


bool STM32UARTBus::CheckPortAvailability()
{
    USART_TypeDef* tx_port = uart_pin_options_map.at(get_tx()).uartPort;
    USART_TypeDef* rx_port = uart_pin_options_map.at(get_rx()).uartPort;

    if(tx_port == rx_port && m_usedUARTPorts.find(tx_port) == m_usedUARTPorts.end())
    {
        m_uartPort.Instance = tx_port;
        m_usedUARTPorts.insert(tx_port);
        enable_uart_clock_map.at(tx_port).enableUART();
        return true;
    }

    return false;
}


bool STM32UARTBus::Read(std::vector<uint8_t>& packet)
{
    HAL_StatusTypeDef status = HAL_UART_Receive(
        &m_uartPort,
        packet.data(),
        static_cast<uint16_t>(packet.size()),
        TIME_OUT_MS
    );

    return status == HAL_OK;
}



bool STM32UARTBus::ReadAfterCommand(
    std::vector<uint8_t>& packet,
    const std::vector<uint8_t>& command)
{
    HAL_StatusTypeDef status;

    status = HAL_UART_Transmit(
        &m_uartPort,
        command.data(),
        static_cast<uint16_t>(command.size()),
        TIME_OUT_MS
    );

    if (status != HAL_OK)
    {
        return false;
    }

    status = HAL_UART_Receive(
        &m_uartPort,
        packet.data(),
        static_cast<uint16_t>(packet.size()),
        TIME_OUT_MS
    );

    return status == HAL_OK;
}


GPIOPin STM32UARTBus::get_tx()
{
    return m_pinOne;
}


GPIOPin STM32UARTBus::get_rx()
{
    return m_pinTwo;
}
