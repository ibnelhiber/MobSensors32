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

elseif(NOT DEFINED MOB_SENSORS_PLATFORM)
    message(FATAL_ERROR "Unkown platform. Please set platform. Ex: python set_platform.py --platform
                            Possible arguments:
                            -USE_STM32F4
                            -USE_STM32F7")
else()
    project(MobBenewake32)

    add_library(MobBenewake32
        "Sensors/Sensor.cpp"
        "Sensors/TFLuna.cpp"
        "Buses/II2CBus.cpp"
        "Buses/I2CBus.cpp"
        "Buses/UARTBus.cpp"
        "Tasks/Task.cpp" 
        "Buses/ESP32/STM32I2CBus"
        "Buses/ESP32/ESP32Bus"
    )

    target_include_directories(MobBenewake32
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}
            include
    )

    if(MOB_SENSORS_PLATFORM STREQUAL "STM32F7")
        target_compile_definitions(${COMPONENT_LIB} PUBLIC USE_STM32F4)
    elseif(MOB_SENSORS_PLATFORM STREQUAL "STM32F4")
        target_compile_definitions(${COMPONENT_LIB} PUBLIC USE_STM32F4)
    endif()
endif()
