#include "port_reader.h"
#include "logger.h"
#include "temperature_calculator.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <csignal>
#include <atomic>

std::atomic<bool> running{true};

void signal_handler(int signal) {
    std::cout << "\nReceived signal " << signal << ", stopping..." << std::endl;
    running = false;
}

void process_temperature_data(const std::string& data) {
    static TemperatureCalculator calculator;
    
    float temperature;
    std::time_t timestamp;
    
    // Парсим данные
    size_t temp_pos = data.find("TEMP:");
    if (temp_pos == std::string::npos) return;
    
    size_t temp_start = temp_pos + 5;
    size_t temp_end = data.find(' ', temp_start);
    std::string temp_str = data.substr(temp_start, temp_end - temp_start);
    
    try {
        temperature = std::stof(temp_str);
        timestamp = std::time(nullptr); // Используем текущее время
        
        // Логируем измерение
        Logger::get_instance().log_measurement(timestamp, temperature);
        
        // Добавляем в калькулятор
        calculator.add_measurement(timestamp, temperature);
        
        std::cout << "Temperature: " << temperature << "°C at "
                  << std::ctime(&timestamp);
                  
    } catch (...) {
        std::cerr << "Failed to parse temperature from: " << data << std::endl;
    }
}

int main(int argc, char* argv[]) {
    // Установка обработчика сигналов
    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);
    
#ifdef _WIN32
    std::signal(SIGBREAK, signal_handler);
#endif
    
    std::string port_name;
    
    if (argc > 1) {
        port_name = argv[1];
    } else {
#ifdef _WIN32
        port_name = "COM3";
        std::cout << "Using default port: COM3" << std::endl;
        std::cout << "Usage: " << argv[0] << " <port_name>" << std::endl;
        std::cout << "Example: " << argv[0] << " COM3" << std::endl;
#else
        port_name = "/dev/ttyS0";
        std::cout << "Using default port: /dev/ttyS0" << std::endl;
        std::cout << "Usage: " << argv[0] << " <port_name>" << std::endl;
        std::cout << "Example: " << argv[0] << " /dev/ttyUSB0" << std::endl;
#endif
    }
    
    PortReader reader(port_name, 9600);
    reader.set_callback(process_temperature_data);
    
    if (!reader.start()) {
        std::cerr << "Failed to start port reader" << std::endl;
        return 1;
    }
    
    std::cout << "Temperature monitoring started. Press Ctrl+C to stop." << std::endl;
    std::cout << "Log files:" << std::endl;
    std::cout << "  - temperature_measurements.log (last 24 hours)" << std::endl;
    std::cout << "  - hourly_averages.log (last month)" << std::endl;
    std::cout << "  - daily_averages.log (current year)" << std::endl;
    
    // Главный цикл
    while (running) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    reader.stop();
    std::cout << "Program stopped." << std::endl;
    
    return 0;
}
