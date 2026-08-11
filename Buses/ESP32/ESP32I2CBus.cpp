#include "ESP32I2CBus.h"

static const char* TAG = "ESP32I2CBus";


ESP32I2CBus::ESP32I2CBus(gpio_num_t sda, gpio_num_t scl) : ESP32Bus(sda, scl)
{
    if(CheckPortAvailability())
    {
        I2CSetup();
    }
    
}

void ESP32I2CBus::I2CSetup()
{
    i2c_config_t conf{};
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = get_sda();
    conf.scl_io_num = get_scl();
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = 100000;

    i2c_param_config(m_i2cPort, &conf);
    i2c_driver_install(m_i2cPort, I2C_MODE_MASTER, 0, 0, 0);
}

bool ESP32I2CBus::CheckPortAvailability()
{
    for (int p = static_cast<int>(I2C_NUM_0); p < static_cast<int>(I2C_NUM_MAX); ++p)
    {
        i2c_port_t port = static_cast<i2c_port_t>(p);

        ESP_LOGI(TAG, "Checking Port Availability");

        if(m_usedI2CPorts.find(port) == m_usedI2CPorts.end())
        {
            m_i2cPort = port;
            m_usedI2CPorts.insert(port);
            return true;
        }
    }

    return false;
}

bool ESP32I2CBus::Read(const uint8_t address, std::vector<uint8_t>& packet)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();

    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (address << 1) | I2C_MASTER_READ, true);
    i2c_master_read(cmd, packet.data(), packet.size() - 1, I2C_MASTER_ACK);
    i2c_master_read_byte(cmd, &packet[packet.size() - 1], I2C_MASTER_NACK);
    i2c_master_stop(cmd);

    esp_err_t err = i2c_master_cmd_begin(m_i2cPort, cmd, pdMS_TO_TICKS(100));

    i2c_cmd_link_delete(cmd);

    if(err != ESP_OK) return false;
    return true;
}

bool ESP32I2CBus::ReadFromRegister(const uint8_t address, std::vector<uint8_t>& packet,
const int registerAddress)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();

    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (address << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, registerAddress, true);

    // Repeated start and read
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (address << 1) | I2C_MASTER_READ, true);
    i2c_master_read(cmd, packet.data(), packet.size() - 1, I2C_MASTER_ACK);
    i2c_master_read_byte(cmd, &packet[packet.size() - 1], I2C_MASTER_NACK);
    i2c_master_stop(cmd);

    esp_err_t err = i2c_master_cmd_begin(m_i2cPort, cmd, pdMS_TO_TICKS(100));

    i2c_cmd_link_delete(cmd);

    if(err != ESP_OK) return false;
    return true;
}

bool ESP32I2CBus::ReadAfterCommand(const uint8_t address, std::vector<uint8_t>& packet, 
const std::vector<uint8_t>& command)
{
    if (packet.empty() || command.empty())
    {
        return false;
    }

    i2c_cmd_handle_t writeCmd = i2c_cmd_link_create();

    i2c_master_start(writeCmd);
    i2c_master_write_byte(writeCmd, (address << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write(writeCmd, command.data(), command.size(), true);
    i2c_master_stop(writeCmd);

    esp_err_t err = i2c_master_cmd_begin(m_i2cPort, writeCmd, pdMS_TO_TICKS(100));
    i2c_cmd_link_delete(writeCmd);

    if (err != ESP_OK)
    {
        return false;
    }

    i2c_cmd_handle_t readCmd = i2c_cmd_link_create();

    i2c_master_start(readCmd);
    i2c_master_write_byte(readCmd, (address << 1) | I2C_MASTER_READ, true);

    if (packet.size() > 1)
    {
        i2c_master_read(readCmd, packet.data(), packet.size() - 1, I2C_MASTER_ACK);
    }

    i2c_master_read_byte(readCmd, &packet[packet.size() - 1], I2C_MASTER_NACK);
    i2c_master_stop(readCmd);

    err = i2c_master_cmd_begin(m_i2cPort, readCmd, pdMS_TO_TICKS(100));
    i2c_cmd_link_delete(readCmd);

    return err == ESP_OK;
}

bool STM32I2CBus::Write(const uint8_t address, const std::vector<uint8_t>& command)
{
    if (packet.empty() || command.empty())
    {
        return false;
    }

    i2c_cmd_handle_t cmdLink = i2c_cmd_link_create();

    i2c_master_start(cmdLink);
    i2c_master_write_byte(cmdLink, (address << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write(cmdLink, command.data(), command.size(), true);
    i2c_master_stop(cmdLink);

    esp_err_t err = i2c_master_cmd_begin(m_i2cPort, cmdLink, pdMS_TO_TICKS(100));
    i2c_cmd_link_delete(cmdLink);

    if (err != ESP_OK)
    {
        return false;
    }

    return true;
}

bool STM32I2CBus::WriteToRegister(const uint8_t address, const int registerAddress,
        std::vector<uint8_t>& command)
{
    if (packet.empty() || command.empty())
    {
        return false;
    }

    i2c_cmd_handle_t cmdLink = i2c_cmd_link_create();

    i2c_master_start(cmdLink);
    i2c_master_write_byte(cmdLink, (address << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmdLink, (registerAddress << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write(cmdLink, command.data(), command.size(), true);
    i2c_master_stop(cmdLink);

    esp_err_t err = i2c_master_cmd_begin(m_i2cPort, cmdLink, pdMS_TO_TICKS(100));
    i2c_cmd_link_delete(cmdLink);

    if (err != ESP_OK)
    {
        return false;
    }

    return true;
}


gpio_num_t ESP32I2CBus::get_sda()
{
    return m_pinOne;
}

gpio_num_t ESP32I2CBus::get_scl()
{
    return m_pinTwo;
}

