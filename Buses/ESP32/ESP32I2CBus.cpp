#include "ESP32I2CBus.h"

static const char* TAG = "ESP32I2CBus";


ESP32I2CBus::ESP32I2CBus(gpio_num_t sda, gpio_num_t scl) : ESP32Bus(sda, scl)
{
    if(CheckPortAvailability())
    {
        I2CSetup();
    }
    
}

bool ESP32I2CBus::ReadByte(const uint8_t address, std::array<uint8_t, 9>& packet,
            const std::optional<uint8_t> registerAddress) const
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    if(registerAddress != std::nullopt)
    {
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (address << 1) | I2C_MASTER_WRITE, true);
        i2c_master_write_byte(cmd, *registerAddress, true);
    }
    else
    {
        i2c_cmd_handle_t writeCmd = i2c_cmd_link_create();
        const uint8_t command[] = {0x5A, 0x05, 0x00, 0x01, 0x60};
        i2c_master_start(writeCmd);
        i2c_master_write_byte(writeCmd, (address << 1) | I2C_MASTER_WRITE, true);
        i2c_master_write(writeCmd, command, sizeof(command), true);
        i2c_master_stop(writeCmd);

        esp_err_t err = i2c_master_cmd_begin(*m_i2cPort.get(), writeCmd, pdMS_TO_TICKS(100));
        i2c_cmd_link_delete(writeCmd);
    }
    // Repeated start and read
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (address << 1) | I2C_MASTER_READ, true);
    i2c_master_read(cmd, packet.data(), packet.size() - 1, I2C_MASTER_ACK);
    i2c_master_read_byte(cmd, &packet[packet.size() - 1], I2C_MASTER_NACK);

    i2c_master_stop(cmd);

    esp_err_t err = i2c_master_cmd_begin(*m_i2cPort.get(), cmd, pdMS_TO_TICKS(100));
    i2c_cmd_link_delete(cmd);

    if(err != ESP_OK) return false;

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

bool ESP32I2CBus::CheckPortAvailability()
{
    for (int p = static_cast<int>(I2C_NUM_0); p < static_cast<int>(I2C_NUM_MAX); ++p)
    {
        i2c_port_t port = static_cast<i2c_port_t>(p);

        ESP_LOGI(TAG, "Checking Port Availability");

        if(m_usedI2CPorts.find(port) == m_usedI2CPorts.end())
        {
            m_i2cPort = std::make_unique<i2c_port_t>(port);
            m_usedI2CPorts.insert(port);
            return true;
        }
    }

    return false;
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

    i2c_param_config(*m_i2cPort, &conf);
    i2c_driver_install(*m_i2cPort, I2C_MODE_MASTER, 0, 0, 0);
}

