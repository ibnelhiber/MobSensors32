#include "ESP32Bus.h"
#include "esp_log.h"


static const char* TAG = "ESP32I2CBus";


ESP32Bus::ESP32Bus(gpio_num_t pinOne, gpio_num_t pinTwo)
{
    if(CheckPinsAvailability(pinOne, pinTwo))
    {
        m_pinOne = pinOne;
        m_pinTwo = pinTwo;
    }
    else
    {
        uint8_t pinOneInt = static_cast<int>(pinOne);
        uint8_t pinTwoInt = static_cast<int>(pinTwo);
        ESP_LOGI(TAG, "Pins %d and %d are not available!", pinOneInt, pinTwoInt);
        abort();
    }
}

bool ESP32Bus::CheckPinsAvailability(gpio_num_t pinOne, gpio_num_t pinTwo)
{
    int m_usedPinsAmount = static_cast<int>(m_usedPins.size());

    ESP_LOGI(TAG, "Checking Pins Availability");


    if(m_usedPins.find(pinOne) == m_usedPins.end())
    {
        m_usedPins.insert(pinOne);
    }
    else
    {
        ESP_LOGI(TAG, "Pin One: %d not available!", m_pinOne);
    }
    
    if(m_usedPins.find(pinTwo) == m_usedPins.end())
    {
        m_usedPins.insert(pinTwo);
    }
    else
    {
        ESP_LOGI(TAG, "Pin Two: %d not available!", m_pinTwo);
    }

    if((m_usedPinsAmount + 2) != static_cast<int>(m_usedPins.size()))
    {
        return false;
    }
    else
    {
        return true;
    }
}