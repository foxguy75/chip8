//
//  chip8.cpp
//  
//
//  Created by Ben Camburn on 6/8/20.
//

#include <iostream>
#include <ctime>

#include "Chip8.hpp"

Chip8::Chip8()
:   programCounter{0x200},
    stackPointer{0x00},
    iRegister{0x00},
    stack{},
    memory{},
    vRegisters{}
{
}

void Chip8::load(std::string fileName)
{
    std::srand(std::time(nullptr)); // use current time as seed for random generator
    uint16_t random_variable = std::rand();
    // Call a subroutine
    this->memory[0x200] = 0x23;
    this->memory[0x201] = 0x5f;
    
    // Subroutine
    this->memory[0x35f] = 0x6b;
    this->memory[0x360] = random_variable;
    // Return
    this->memory[0x361] = 0x00;
    this->memory[0x362] = 0xEE;
    // check is value is 7
    this->memory[0x203] = 0x3b;
    this->memory[0x204] = 0x07;
    // if true
    this->memory[0x205] = 0x1f;
    this->memory[0x206] = 0xfe;

    return;
}

void Chip8::clearDisplay()
{
    std::cout << "Display Cleared" << std::endl;
}

void Chip8::run()
{
    bool running = true;
    
    while (running) {
        uint16_t opCode = this->memory[this->programCounter] << 8;
        this->programCounter++;
        opCode = opCode | this->memory[this->programCounter];
        this->programCounter++;
        std::cout << "OP Code: " << std::hex << opCode << std::endl;
        
        int tempCount = 0;
        for (auto elem : this->vRegisters) {
            std::cout << "V" << tempCount << ": " << std::hex << static_cast<int>(elem) << " ";
            if(tempCount % 8 == 0)
            {
                std::cout << std::endl;
            }
            tempCount++;
        }
        
        std::cout << std::endl;
        
        switch (opCode & 0xf000) {
            case 0x0000:
                switch (opCode & 0x00ff) {
                    // Clear the screen
                    case 0x00e0:
                        this->clearDisplay();
                        this->programCounter++;
                        break;
                    // Return from subroutine
                    case 0x00ee:
                    {
                        int8_t tempStackPointer = static_cast<int>(this->stackPointer);
                        int16_t temp = this->stack[tempStackPointer];
                        this->programCounter = this->stack[temp];
                        this->stackPointer--;
                        break;
                    }
                        
                    default:
                        running = false;
                        break;
                }
                break;
            // Jump to address
            case 0x1000:
                this->programCounter = (opCode & 0x0fff);
                break;
            // Call subroutine
            case 0x2000:
                this->stack[stackPointer] = this->programCounter;
                this->stackPointer++;
                this->programCounter = (opCode & 0x0fff);
                this->programCounter--;
                break;
            // Skip next instruction if Vx == kk.
            case 0x3000:
                if (this->vRegisters[(opCode & 0x0f00)] == (opCode & 0x0ff))
                {
                    this->programCounter += 2;
                }
                break;
            // Skip next instruction if Vx != kk.
            case 0x4000:
                if (this->vRegisters[(opCode & 0x0f00)] != (opCode & 0x0ff))
                {
                    this->programCounter += 2;
                }
                break;
            // Skip next instruction if Vx == Vy.
            case 0x5000:
                if (this->vRegisters[(opCode & 0x0f00)] == this->vRegisters[0x00f0])
                {
                    this->programCounter += 2;
                }
                break;
            // Set Vx = kk.
            case 0x6000:
                this->vRegisters[(opCode & 0x0f00) >> 8] = (opCode & 0x00ff);
                break;
                
            default:
                running = false;
                break;
        }
    }

}
