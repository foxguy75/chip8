//
//  chip8.hpp
//  
//
//  Created by Ben Camburn on 6/8/20.
//

#ifndef chip8_hpp
#define chip8_hpp

#include <array>
#include <string>
#include <stdio.h>
#include <stdint.h>
#include <map>
#include <Windows.h>

struct Chip8 {
    uint16_t programCounter;
    uint8_t stackPointer;
    uint16_t iRegister;
    std::array<uint16_t, 16> stack;
    std::array<uint8_t, 4096> memory;
    std::array<uint8_t, 16> vRegisters;

    std::map<int, COORD> registerDisplay;

    COORD prompt; 

    Chip8();
    void load(std::string filePath);
    void run();
    void clearDisplay();

    // Debug and Dev Console
    void initDevConsol();
    void updateDebugRegisterDisplay();
};

#endif /* chip8_hpp */
