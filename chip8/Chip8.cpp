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
    vRegisters{},
    registerDisplay{}
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
    // Add Test
    // Load V3 with 16
    this->memory[0x202] = 0x63;
    this->memory[0x203] = 0x0f;
    // Load v8 with 175
    this->memory[0x204] = 0x68;
    this->memory[0x205] = 0xaf;
    // Call the add function
    this->memory[0x206] = 0x83;
    this->memory[0x207] = 0x84;
    // Add Test
    // Load V3 with 16
    this->memory[0x208] = 0x64;
    this->memory[0x209] = 0x0f;
    // Load v8 with 255
    this->memory[0x20a] = 0x65;
    this->memory[0x20b] = 0xff;
    // Call the add function
    this->memory[0x20c] = 0x84;
    this->memory[0x20d] = 0x54;
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

        this->updateDebugRegisterDisplay();

        uint16_t opCode = this->memory[this->programCounter] << 8;
        this->programCounter++;
        opCode = opCode | this->memory[this->programCounter];
        this->programCounter++;
        std::cout << "OP Code: " << std::hex << opCode << std::endl;
                        
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
                        this->stackPointer--;
                        this->programCounter = stack[static_cast<uint16_t>(this->stackPointer)];
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
                break;
            // Skip next instruction if Vx == kk.
            case 0x3000:
                if (this->vRegisters[(opCode & 0x0f00) >> 8] == (opCode & 0x0ff))
                {
                    this->programCounter += 2;
                }
                break;
            // Skip next instruction if Vx != kk.
            case 0x4000:
                if (this->vRegisters[(opCode & 0x0f00) >> 8] != (opCode & 0x0ff))
                {
                    this->programCounter += 2;
                }
                break;
            // Skip next instruction if Vx == Vy.
            case 0x5000:
                if (this->vRegisters[(opCode & 0x0f00) >> 8] == this->vRegisters[0x00f0])
                {
                    this->programCounter += 2;
                }
                break;
            // Set Vx = kk.
            case 0x6000:
                this->vRegisters[(opCode & 0x0f00) >> 8] = (opCode & 0x00ff);
                break;
            // Set Vx = Vx + kk.
            case 0x7000:
                this->vRegisters[(opCode & 0x0f00) >> 8] += (opCode & 0x00ff);
                break;
            case 0x8000:
                switch (opCode & 0x000f)
                {
                // Set Vx = Vy.
                case 0x0000:
                    this->vRegisters[(opCode & 0x0f00) >> 8] = (opCode & 0x00f0 >> 4);
                    break;
                // Set Vx = Vx OR Vy.
                case 0x0001:
                    this->vRegisters[(opCode & 0x0f00) >> 8] = (this->vRegisters[(opCode & 0x0f00) >> 8] | this->vRegisters[(opCode & 0x00f0) >> 4]);
                    break;
                // Set Vx = Vx AND Vy.
                case 0x0002:
                    this->vRegisters[(opCode & 0x0f00) >> 8] = (this->vRegisters[(opCode & 0x0f00) >> 8] & this->vRegisters[(opCode & 0x00f0) >> 4]);
                    break;
                // Set Vx = Vx XOR Vy.
                case 0x0003:
                    this->vRegisters[(opCode & 0x0f00) >> 8] = (this->vRegisters[(opCode & 0x0f00) >> 8] ^ this->vRegisters[(opCode & 0x00f0) >> 4]);
                    break;
                // Set Vx = Vx + Vy, set VF = carry.
                case 0x0004:
                {
                    uint16_t sum = this->vRegisters[(opCode & 0x0f00) >> 8] + this->vRegisters[(opCode & 0x00f0) >> 4];

                    if (sum > 255)
                    {
                        this->vRegisters[0xf] = 1;
                    }
                    else
                    {
                        this->vRegisters[0xf] = 0;
                    }

                    this->vRegisters[(opCode & 0x0f00) >> 8] = this->vRegisters[(opCode & 0x0f00) >> 8] + this->vRegisters[(opCode & 0x00f0) >> 4];
                    break;
                }
                // Set Vx = Vx - Vy, set VF = NOT borrow.
                case 0x0005:
                {
                    if(this->vRegisters[(opCode & 0x0f00) >> 8] > this->vRegisters[(opCode & 0x00f0) >> 4])
                    {
                        
                        this->vRegisters[0xf] = 1;
                    }
                    else
                    {
                        this->vRegisters[0xf] = 0;
                    }

                    this->vRegisters[(opCode & 0x0f00) >> 8] = this->vRegisters[(opCode & 0x0f00) >> 8] - this->vRegisters[(opCode & 0x00f0) >> 4];
                    break;
                }
                default:
                    break;
                }
                break;
            default:
                running = false;
                break;
        }

        std::cout << "Press enter to continue...";

        std::cout << std::endl;
        std::cin.ignore();
    }
}
