cmake_minimum_required(VERSION 3.5)

if(ESP_PLATFORM)
idf_component_register(
    SRCS
    "Tasks/Task.cpp"
    "Buses/I2CBus.cpp"  
    "Buses/II2CBus.cpp"
    "Buses/ESP32/ESP32Bus.cpp"
    "Buses/ESP32/ESP32I2CBus.cpp"
    "Sensors/Sensor.cpp"
    "Sensors/TFLuna.cpp" 
    INCLUDE_DIRS
        "."
        "Tasks"
        "Buses"
        "Sensors"
    REQUIRES driver
)
target_compile_definitions(${COMPONENT_LIB} PUBLIC USE_ESP32)

endif()
