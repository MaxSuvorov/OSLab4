cmake_minimum_required(VERSION 3.10)
project(TemperatureMonitor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Для Windows
if(WIN32)
    add_definitions(-DWIN32_LEAN_AND_MEAN)
endif()

# Основная программа
add_executable(temperature_monitor
    main_program/main.cpp
    main_program/logger.cpp
    main_program/port_reader.cpp
    main_program/temperature_calculator.cpp
)

# Симулятор устройства
add_executable(device_simulator
    device_simulator/device_simulation.cpp
    device_simulator/device_simulation_main.cpp
)

# Настройка для Windows
if(WIN32)
    target_link_libraries(temperature_monitor kernel32)
    target_link_libraries(device_simulator kernel32)
endif()