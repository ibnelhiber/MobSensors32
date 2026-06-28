cmake_minimum_required(VERSION 3.5)

if(ESP_PLATFORM)
idf_component_register(
    SRCS
    "Tasks/Task.cpp"
    "Buses/I2CBus.cpp"  
    "Buses/II2CBus.cpp"
    "Buses/ESP32/ESP32Bus.cpp"
    "Buses/ESP32/ESP32I2CBus.cpp"
    "Buses/ESP32/ESP32UARTBus.cpp"
    "Buses/IUARTBus.cpp"
    "Buses/UARTBus.cpp"
    "Sensors/Sensor.cpp"
    "Sensors/TFLuna.cpp"
    "Buses/Adaptation/Implementation.cpp" 
    INCLUDE_DIRS
        "."
        "Tasks"
        "Buses"
        "Sensors"
    REQUIRES driver
)
endif()
