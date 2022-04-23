//
//  chip8.hpp
//  
//
//  Created by Ben Camburn on 6/8/20.
//

#ifndef chip8_hpp
#define chip8_hpp

#include <vector>
#include <string>
#include <map>

struct Chip8 {
    Chip8();
    void load(std::string filePath);
    void runCycle();

    uint8_t stackPointer;
    uint8_t delayTimer;
    uint8_t soundTimer;
    uint16_t programCounter;
    uint16_t iRegister;

    std::vector<uint8_t> memory;
    std::vector<uint8_t> vRegisters;
    std::vector<uint8_t> keys;
    std::vector<uint8_t> mainDisplay;
    std::vector<uint16_t> stack;

    // The system should check this every cycle to see if 
    // the CPU is request input from the keys. 
    bool checkKeys;
    void setKeys() noexcept;

    bool drawFlag; 

    bool halt; 
private: 
    std::map<uint8_t, uint8_t> hexDigitMap;
    int opCodeWatchDog;
    uint16_t previousopCode; 
};

#endif /* chip8_hpp */
