#ifndef STM32I2CBus_H_
#define STM32I2CBus_H_

#include "Buses/II2CBus.h"
#include "STM32Bus.h"
#include <unordered_map>
#include <memory>


class STM32I2CBus : public II2CBus, public STM32Bus
{

private:
    static std::unordered_set<I2C_TypeDef*> m_usedI2CPorts;
    I2C_HandleTypeDef m_i2cPort{};
    bool CheckPortAvailability() override;
    
    void I2CSetup() override;

    GPIOPin get_sda();
    GPIOPin get_scl();
public:
    STM32I2CBus(GPIOPin sda, GPIOPin scl);
    bool Read(const uint8_t address, std::vector<uint8_t>& packet) override;
    bool ReadFromRegister(const uint8_t address, std::vector<uint8_t>& packet,
    const int registerAddress) override;
    bool ReadAfterCommand(const uint8_t address, std::vector<uint8_t>& packet, 
    const std::vector<uint8_t>& command) override;
    bool Write(const uint8_t address, const std::vector<uint8_t>& command);
    bool WriteToRegister(const uint8_t address, const int registerAddress,
        std::vector<uint8_t>& command);

};


#endif
