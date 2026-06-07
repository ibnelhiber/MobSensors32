#ifndef STM32I2CBus_H_
#define STM32I2CBus_H_

#include "II2CBus.h"
#include "STM32Bus.h"
#include <unordered_map>

std::unordered_map<uint8_t, I2C_TypeDef*> i2cPortMap = 
{
    {1, I2C1},
    {2, I2C2},
    {3, I2C3},
    {4, I2C4}
}

class STM32I2CBus : public II2CBus, public STM32Bus
{

private:
    static std::unordered_set<I2C_TypeDef*> m_usedI2CPorts;
    std::unique_ptr<I2C_HandleTypeDef> m_i2cPort{
    std::make_unique<I2C_HandleTypeDef>()};
    bool CheckPortAvailability() override;
    
    void I2CSetup() override;

    GPIOPin get_sda();
    GPIOPin get_scl();

public:
    STM32I2CBus(GPIOPin sda, GPIOPin scl);
    bool ReadByte_(uint8_t address const, std::array<uint8_t, 9>& packet,
            std::optional<uint8_t> registerAddress) const override;

};


#endif