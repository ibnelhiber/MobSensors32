#include "MockBus.h"

MockBus::MockBus()
{
}


bool MockBus::Read(std::vector<uint8_t>& packet)
{
   for(auto& byte : packet)
   {
        byte = 0x10;
   } 
   return true;
}


bool MockBus::ReadFromRegister(std::vector<uint8_t>& packet,
const int registerAddress)
{
   for(auto& byte : packet)
   {
        byte = 0x10;
   } 
   return true;
}


bool MockBus::ReadAfterCommand(std::vector<uint8_t>& packet, 
const std::vector<uint8_t>& command)
{
   for(auto& byte : packet)
   {
        byte = 0x10;
   } 
   return true;
}