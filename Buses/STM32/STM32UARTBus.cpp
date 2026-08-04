#include "Buses/STM32/STM32Bus.h"
#include "Buses/STM32/STM32UARTBus.h"

constexpr uint32_t TIME_OUT_MS = 100;

std::unordered_map<USART_TypeDef*, STM32UARTBus*> STM32UARTBus::m_instanceMap{};

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

STM32UARTBus::~STM32UARTBus()
{
    if(m_instanceMap.find(m_uartPort.Instance) != m_instanceMap.end())
    {
        m_instanceMap.erase(m_uartPort.Instance);
    }
}

void STM32UARTBus::UARTSetup()
{
    const UARTSetupInfo& setupInfo = enable_uart_clock_map.at(m_uartPort.Instance);

    GPIO_InitTypeDef txInit{};
    txInit.Pin = get_tx().pin;
    txInit.Mode = GPIO_MODE_AF_OD;
    txInit.Pull = GPIO_PULLUP;
    txInit.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    txInit.Alternate = setupInfo.alternateFunction;

    HAL_GPIO_Init(get_tx().gpioPort, &txInit);

    GPIO_InitTypeDef rxInit{};
    rxInit.Pin = get_rx().pin;
    rxInit.Mode = GPIO_MODE_AF_OD;
    rxInit.Pull = GPIO_PULLUP;
    rxInit.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    rxInit.Alternate = setupInfo.alternateFunction;

    HAL_GPIO_Init(get_rx().gpioPort, &rxInit);

    m_uartPort.Init.BaudRate = 115200;
    m_uartPort.Init.WordLength = UART_WORDLENGTH_8B;
    m_uartPort.Init.StopBits = UART_STOPBITS_1;
    m_uartPort.Init.Parity = UART_PARITY_NONE;
    m_uartPort.Init.Mode = UART_MODE_TX_RX;
    m_uartPort.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    m_uartPort.Init.OverSampling = UART_OVERSAMPLING_16;

    HAL_UART_Init(&m_uartPort);
    setupInfo.setIRQPriority();
    setupInfo.enableIRQ();
}


bool STM32UARTBus::CheckPortAvailability()
{
    USART_TypeDef* tx_port = uart_pin_options_map.at(get_tx()).uartPort;
    USART_TypeDef* rx_port = uart_pin_options_map.at(get_rx()).uartPort;

    if(tx_port == rx_port && m_instanceMap.find(tx_port) == m_instanceMap.end())
    {
        m_uartPort.Instance = tx_port;
        m_instanceMap[tx_port] = this;
        enable_uart_clock_map.at(tx_port).enableUART();
        return true;
    }

    return false;
}


bool STM32UARTBus::Read(std::vector<uint8_t>& packet)
{
    m_waitingTask = xTaskGetCurrentTaskHandle();
    m_readComplete = false;


    HAL_StatusTypeDef status = HAL_UART_Receive_IT(
        &m_uartPort,
        packet.data(),
        static_cast<uint16_t>(packet.size())
    );

    if(status != HAL_OK)
    {
        m_waitingTask = nullptr;
        m_readComplete = false;
        return false;
    }

    
    auto notified{ulTaskNotifyTake(
        pdTRUE,
        pdMS_TO_TICKS(TIME_OUT_MS)
    )};

    bool success{notified > 0 && m_readComplete};

    m_waitingTask = nullptr;
    m_readComplete = false;

    if(success)
    {
        return true;
    }

    HAL_UART_AbortReceive_IT(&m_uartPort);
    
    return false;
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
        TIME_OUT_MS);

    if (status != HAL_OK)
    {
        return false;
    }

    m_waitingTask = xTaskGetCurrentTaskHandle();
    m_readComplete = false;

    status = HAL_UART_Receive_IT(
        &m_uartPort,
        packet.data(),
        static_cast<uint16_t>(packet.size())
    );

    if(status != HAL_OK)
    {
        m_waitingTask = nullptr;
        m_readComplete = false;
        return false;
    }

    
    auto notified{ulTaskNotifyTake(
        pdTRUE,
        pdMS_TO_TICKS(TIME_OUT_MS)
    )};

    bool success{notified > 0 && m_readComplete};

    m_waitingTask = nullptr;
    m_readComplete = false;

    if(success)
    {
        return true;
    }

    HAL_UART_AbortReceive_IT(&m_uartPort);

    return false;
}


GPIOPin STM32UARTBus::get_tx()
{
    return m_pinOne;
}


GPIOPin STM32UARTBus::get_rx()
{
    return m_pinTwo;
}

void STM32UARTBus::HandleUARTIRQ(USART_TypeDef* peripheralInstance)
{
    auto& instanceMap{STM32UARTBus::m_instanceMap};
    if(instanceMap.find(peripheralInstance) != instanceMap.end())
    {
        HAL_UART_IRQHandler(&instanceMap.at(peripheralInstance)->m_uartPort);
    }
}

void STM32UARTBus::HandleReadComplete(UART_HandleTypeDef* uartPort)
{
    if(STM32UARTBus::m_instanceMap.find(uartPort->Instance) != STM32UARTBus::m_instanceMap.end())
    {
        auto bus = STM32UARTBus::m_instanceMap.at(uartPort->Instance);
        bus->m_readComplete = true;

        if(bus->m_waitingTask)
        {
            BaseType_t woke{pdFALSE};
            vTaskNotifyGiveFromISR(bus->m_waitingTask, &woke);
            portYIELD_FROM_ISR(woke);
        }
    }
}

extern "C" void USART1_IRQHandler()
{
    STM32UARTBus::HandleUARTIRQ(USART1);
}

extern "C" void USART2_IRQHandler()
{
    STM32UARTBus::HandleUARTIRQ(USART2);
}

extern "C" void USART3_IRQHandler()
{
    STM32UARTBus::HandleUARTIRQ(USART3);
}

extern "C" void UART4_IRQHandler()
{
    STM32UARTBus::HandleUARTIRQ(UART4);
}

extern "C" void UART5_IRQHandler()
{
    STM32UARTBus::HandleUARTIRQ(UART5);
}

extern "C" void USART6_IRQHandler()
{
    STM32UARTBus::HandleUARTIRQ(USART6);
}

extern "C" void UART7_IRQHandler()
{
    STM32UARTBus::HandleUARTIRQ(UART7);
}

extern "C" void UART8_IRQHandler()
{
    STM32UARTBus::HandleUARTIRQ(UART8);
}

extern "C" void HAL_UART_RxCpltCallback(UART_HandleTypeDef* uartPort)
{
    STM32UARTBus::HandleReadComplete(uartPort);
}

extern "C" void HAL_UART_ErrorCallback(UART_HandleTypeDef* uartPort)
{
    printf("UART error: 0x%X\n", uartPort->ErrorCode);
}
