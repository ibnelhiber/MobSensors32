#ifndef MockBus_H_
#define MockBus_H_

#include <memory>
#include <cstdint>
#include <vector>

class MockBus
{
public:
    MockBus();
    bool Read(std::vector<uint8_t>& packet);
    bool ReadFromRegister(std::vector<uint8_t>& packet,
    const int registerAddress);
    bool ReadAfterCommand(std::vector<uint8_t>& packet, 
    const std::vector<uint8_t>& command);
};


#endif