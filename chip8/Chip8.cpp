//
//  chip8.cpp
//  
//
//  Created by Ben Camburn on 6/8/20.
//


#include <iostream>
#include <ctime>
#include <fstream>

#include "Chip8.hpp"

Chip8::Chip8()
:   programCounter{0x200},
    stackPointer{0x00},
    iRegister{0x00},
    stack(16),
    memory(4096),
    vRegisters(16),
    mainDisplay(2048), 
    keys(16)
{
    checkKeys = false;
    drawFlag = false; 
    halt = false;

    // Sprit for 0
    memory[0x000] = 0xf0;
    memory[0x001] = 0x90;
    memory[0x002] = 0x90;
    memory[0x003] = 0x90;
    memory[0x004] = 0xF0;

    // Sprit for 1
    memory[0x005] = 0x20;
    memory[0x006] = 0x60;
    memory[0x007] = 0x20;
    memory[0x008] = 0x20;
    memory[0x009] = 0x70;

    // Sprit for 2
    memory[0x00A] = 0x20;
    memory[0x00B] = 0x60;
    memory[0x00C] = 0x20;
    memory[0x00D] = 0x20;
    memory[0x00E] = 0x70;

    // Sprit for 3
    memory[0x00F] = 0xf0;
    memory[0x010] = 0x10;
    memory[0x011] = 0xf0;
    memory[0x012] = 0x10;
    memory[0x013] = 0xf0;

    // Sprit for 4
    memory[0x014] = 0x90;
    memory[0x015] = 0x90;
    memory[0x016] = 0xf0;
    memory[0x017] = 0x10;
    memory[0x018] = 0x10;

    // Sprit for 5
    memory[0x019] = 0xF0;
    memory[0x01A] = 0x80;
    memory[0x01B] = 0xF0;
    memory[0x01C] = 0x10;
    memory[0x01D] = 0xF0;

    // Sprit for 6
    memory[0x01E] = 0xF0;
    memory[0x01F] = 0x80;
    memory[0x020] = 0xF0;
    memory[0x021] = 0x90;
    memory[0x022] = 0xF0;

    // Sprit for 7
    memory[0x023] = 0xF0;
    memory[0x024] = 0x10;
    memory[0x025] = 0x20;
    memory[0x026] = 0x40;
    memory[0x027] = 0x40;

    // Sprit for 8
    memory[0x028] = 0xF0;
    memory[0x029] = 0x90;
    memory[0x02A] = 0xF0;
    memory[0x02B] = 0x90;
    memory[0x02C] = 0xF0;

    // Sprit for 9
    memory[0x02D] = 0xF0;
    memory[0x02E] = 0x90;
    memory[0x02F] = 0xF0;
    memory[0x030] = 0x10;
    memory[0x031] = 0xF0;

    // Sprit for A
    memory[0x032] = 0xF0;
    memory[0x033] = 0x90;
    memory[0x034] = 0xF0;
    memory[0x035] = 0x90;
    memory[0x036] = 0x90;

    // Sprit for B
    memory[0x037] = 0xE0;
    memory[0x038] = 0x90;
    memory[0x039] = 0xE0;
    memory[0x03A] = 0x90;
    memory[0x03B] = 0xE0;

    // Sprit for C
    memory[0x03C] = 0xF0;
    memory[0x03D] = 0x80;
    memory[0x03E] = 0x80;
    memory[0x03F] = 0x80;
    memory[0x040] = 0xF0;

    // Sprit for D
    memory[0x041] = 0xE0;
    memory[0x042] = 0x90;
    memory[0x043] = 0x90;
    memory[0x044] = 0x90;
    memory[0x045] = 0xE0;

    // Sprit for E
    memory[0x046] = 0xF0;
    memory[0x047] = 0x80;
    memory[0x048] = 0xF0;
    memory[0x049] = 0x80;
    memory[0x04A] = 0xF0;

    // Sprit for F
    memory[0x04B] = 0xF0;
    memory[0x04C] = 0x80;
    memory[0x04D] = 0xF0;
    memory[0x04E] = 0x80;
    memory[0x04F] = 0x80;

    hexDigitMap[0x00] = 0x000; 
    hexDigitMap[0x01] = 0x005;
    hexDigitMap[0x02] = 0x00A;
    hexDigitMap[0x03] = 0x00F;
    hexDigitMap[0x04] = 0x014;
    hexDigitMap[0x05] = 0x019;
    hexDigitMap[0x06] = 0x01E;
    hexDigitMap[0x07] = 0x023;
    hexDigitMap[0x08] = 0x028;
    hexDigitMap[0x09] = 0x02D;
    hexDigitMap[0x0A] = 0x032;
    hexDigitMap[0x0B] = 0x037;
    hexDigitMap[0x0C] = 0x03C;
    hexDigitMap[0x0D] = 0x041;
    hexDigitMap[0x0E] = 0x046;
    hexDigitMap[0x0F] = 0x04B;
}

void Chip8::load(std::string fileName)
{
    std::ifstream romFile;
    romFile.open( fileName.c_str(), std::ios::binary );

    if( romFile.is_open() )
    {
        int initalMemoryAddress = 0x200;

        while( !romFile.eof() )
        {
            memory[initalMemoryAddress] = romFile.get();
            initalMemoryAddress++;
        }

        romFile.close();
    }

    //std::srand(std::time(nullptr)); // use current time as seed for random generator
    //uint16_t random_variable = std::rand();
    //// Call a subroutine
    //memory[0x200] = 0x23;
    //memory[0x201] = 0x5f;
    //
    //// Subroutine
    //memory[0x35f] = 0x6b;
    //memory[0x360] = random_variable;
    //// Return
    //memory[0x361] = 0x00;
    //memory[0x362] = 0xEE;
    //// Add Test
    //// Load V3 with 16
    //memory[0x202] = 0x63;
    //memory[0x203] = 0x0f;
    //// Load v8 with 175
    //memory[0x204] = 0x68;
    //memory[0x205] = 0xaf;
    //// Call the add function
    //memory[0x206] = 0x83;
    //memory[0x207] = 0x84;
    //// Add Test
    //// Load V3 with 16
    //memory[0x208] = 0x64;
    //memory[0x209] = 0x0f;
    //// Load v8 with 255
    //memory[0x20a] = 0x65;
    //memory[0x20b] = 0xff;
    //// Call the add function
    //memory[0x20c] = 0x84;
    //memory[0x20d] = 0x54;
    return;
}

void Chip8::runCycle()
{
    uint16_t opCode = memory[programCounter] << 8;
    ++programCounter;
    opCode = opCode | memory[programCounter];
    ++programCounter;
    std::cout << "OP Code: " << std::hex << opCode << std::endl;

    if( opCode == previousopCode )
    {
        ++opCodeWatchDog;
    }
    else
    {
        opCodeWatchDog = 0;
    }

    previousopCode = opCode;
    
    if( opCodeWatchDog > 10 )
    {
        halt = true; 
        std::cout << "Watch Dog Triggered!! \n";
    }
    else 
    {
        switch( opCode & 0xf000 ) {
        case 0x0000:
            switch( opCode & 0x00ff ) {
                // Clear the screen
            case 0x00e0:
                for( uint8_t& pixel : mainDisplay )
                {
                    pixel = ( 0x00FFFFFF * 0 ) | 0xFF000000;
                }
                ++programCounter;
                drawFlag = true;
                break;
                // Return from subroutine
            case 0x00ee:
            {
                --stackPointer;
                programCounter = stack[stackPointer];
                break;
            }
            default:
                halt = true;
                break;
            }
            break;
            // Jump to address
        case 0x1000:
            programCounter = ( opCode & 0x0fff );
            break;
            // Call subroutine
        case 0x2000:
            stack[stackPointer] = programCounter;
            ++stackPointer;
            programCounter = ( opCode & 0x0fff );
            break;
            // Skip next instruction if Vx == kk.
        case 0x3000:
            if( vRegisters[( opCode & 0x0f00 ) >> 8] == ( opCode & 0x0ff ) )
            {
                programCounter += 2;
            }
            break;
            // Skip next instruction if Vx != kk.
        case 0x4000:
            if( vRegisters[( opCode & 0x0f00 ) >> 8] != ( opCode & 0x0ff ) )
            {
                programCounter += 2;
            }
            break;
            // Skip next instruction if Vx == Vy.
        case 0x5000:
            if( vRegisters[( opCode & 0x0f00 ) >> 8] == vRegisters[0x00f0 >> 4] )
            {
                programCounter += 2;
            }
            break;
            // Set Vx = kk.
        case 0x6000:
            vRegisters[( opCode & 0x0f00 ) >> 8] = ( opCode & 0x00ff );
            break;
            // Set Vx = Vx + kk.
        case 0x7000:
            vRegisters[( opCode & 0x0f00 ) >> 8] += ( opCode & 0x00ff );
            break;
        case 0x8000:
            switch( opCode & 0x000f )
            {
                // Set Vx = Vy.
            case 0x0000:
                vRegisters[( opCode & 0x0f00 ) >> 8] = vRegisters[( opCode & 0x00f0 ) >> 4];
                break;
                // Set Vx = Vx OR Vy.
            case 0x0001:
                vRegisters[( opCode & 0x0f00 ) >> 8] = ( vRegisters[( opCode & 0x0f00 ) >> 8] | vRegisters[( opCode & 0x00f0 ) >> 4] );
                break;
                // Set Vx = Vx AND Vy.
            case 0x0002:
                vRegisters[( opCode & 0x0f00 ) >> 8] = ( vRegisters[( opCode & 0x0f00 ) >> 8] & vRegisters[( opCode & 0x00f0 ) >> 4] );
                break;
                // Set Vx = Vx XOR Vy.
            case 0x0003:
                vRegisters[( opCode & 0x0f00 ) >> 8] = ( vRegisters[( opCode & 0x0f00 ) >> 8] ^ vRegisters[( opCode & 0x00f0 ) >> 4] );
                break;
                // Set Vx = Vx + Vy, set VF = carry.
            case 0x0004:
            {
                uint16_t sum = vRegisters[( opCode & 0x0f00 ) >> 8] + vRegisters[( opCode & 0x00f0 ) >> 4];

                if( sum > 255 )
                {
                    vRegisters[0xf] = 1;
                }
                else
                {
                    vRegisters[0xf] = 0;
                }

                vRegisters[( opCode & 0x0f00 ) >> 8] = vRegisters[( opCode & 0x0f00 ) >> 8] + vRegisters[( opCode & 0x00f0 ) >> 4];
                break;
            }
            // Set Vx = Vx - Vy, set VF = NOT borrow.
            case 0x0005:
            {
                if( vRegisters[( opCode & 0x0f00 ) >> 8] > vRegisters[( opCode & 0x00f0 ) >> 4] )
                {

                    vRegisters[0xf] = 1;
                }
                else
                {
                    vRegisters[0xf] = 0;
                }

                vRegisters[( opCode & 0x0f00 ) >> 8] = vRegisters[( opCode & 0x0f00 ) >> 8] - vRegisters[( opCode & 0x00f0 ) >> 4];
                break;
            }
            // Set Vx = Vx SHR 1.
            case 0x0006:
            {
                if( ( vRegisters[( opCode & 0x0f00 ) >> 8] & 0x0001 ) == 1 )
                {
                    vRegisters[0xf] = 1;
                }
                else
                {
                    vRegisters[0xf] = 0;
                }

                vRegisters[( opCode & 0x0f00 ) >> 8] = vRegisters[( opCode & 0x0f00 ) >> 8] / 2;
                break;
            }
            // Set Vx = Vy - Vx, set VF = NOT borrow.
            case 0x0007:
            {
                if( vRegisters[( opCode & 0x00f0 ) >> 4] > vRegisters[( opCode & 0x0f00 ) >> 8] )
                {

                    vRegisters[0xf] = 1;
                }
                else
                {
                    vRegisters[0xf] = 0;
                }

                vRegisters[( opCode & 0x0f00 ) >> 8] = vRegisters[( opCode & 0x00f0 ) >> 4] - vRegisters[( opCode & 0x0f00 ) >> 8];
                break;
            }
            // Set Vx = Vx SHL 1.
            case 0x000E:
            {
                if( ( vRegisters[( opCode & 0x0f00 ) >> 8] & 0x1000 ) == 1 )
                {
                    vRegisters[0xf] = 1;
                }
                else
                {
                    vRegisters[0xf] = 0;
                }

                vRegisters[( opCode & 0x0f00 ) >> 8] = vRegisters[( opCode & 0x0f00 ) >> 8] * 2;

                break;
            }
            default:
                break;
            }
            break;
            // Skip next instruction if Vx != Vy.
        case 0x9000:
        {
            if( vRegisters[( opCode & 0x0f00 ) >> 8] != vRegisters[( opCode & 0x00f0 ) >> 4] )
            {
                stackPointer += 2;
            }
            break;
        }
        // Set I = nnn.
        case 0xA000:
            iRegister = ( opCode & 0x0fff );
            break;
            // Jump to location nnn + V0.
        case 0xB000:
            programCounter = ( opCode & 0x0fff ) + vRegisters[0x0];
            break;
            // Set Vx = random byte AND kk.
        case 0xC000:
        {
            std::srand( std::time( nullptr ) ); // use current time as seed for random generator
            uint16_t randomVariable = std::rand() % 256;
            vRegisters[( opCode & 0x0f00 )] = randomVariable && ( opCode & 0x00ff );
            break;
        }
        // DRW Vx, Vy, nibble
        case 0xD000:
        {
            drawFlag = true;
            uint8_t xCoord = vRegisters[( opCode & 0x0f00 ) >> 8];
            uint8_t yCoord = vRegisters[( opCode & 0x00f0 ) >> 4];
            uint8_t hight = ( opCode & 0x000f );
            uint8_t yConst = 64;

            vRegisters[0xf] = 0;
            for( uint8_t ybyte = 0; ybyte < hight; ++ybyte )
            {
                uint8_t spriteByte = memory[iRegister + ybyte];
                for( uint8_t xbyte = 0; xbyte < 8; ++xbyte )
                {
                    if( ( spriteByte & ( 0x80 >> xbyte ) ) != 0 )
                    {
                        if( mainDisplay[( xCoord + xbyte ) + ( ( yCoord + ybyte ) * yConst )] == 1 )
                        {
                            vRegisters[0xf] = 1;
                        }
                        mainDisplay[( xCoord + xbyte ) + ( ( yCoord + ybyte ) * yConst )] ^= 1;
                    }
                }
            }
            break;
        }
        case 0xE000:
        {
            switch( opCode & 0x00ff )
            {
                //SKP Vx
            case 0x009E:
            {
                if( keys[vRegisters[( opCode & 0x0f00 ) >> 8]] == 1 )
                {
                    programCounter += 2;
                }
                break;
            }
            case 0x00A1:
            {
                if( keys[vRegisters[( opCode & 0x0f00 ) >> 8]] == 0 )
                {
                    programCounter += 2;
                }
                break;
            }
            default:
                break;
            }
            break;
        }
        case 0xF000:
        {
            switch( opCode & 0x00ff )
            {
            case 0x0007:
            {
                vRegisters[( opCode & 0x0f00 ) >> 8] = delayTimer;
                break;
            }
            case 0x000A:
            {
                checkKeys = true;
                std::cout << "Needs implemented... \n";
                break;
            }
            case 0x0015:
            {
                delayTimer = vRegisters[( opCode & 0x0f00 ) >> 8];
                break;
            }
            case 0x0018:
            {
                soundTimer = vRegisters[( opCode & 0x0f00 ) >> 8];
                break;
            }
            case 0x001E:
            {
                iRegister = iRegister + vRegisters[( opCode & 0x0f00 ) >> 8];
                break;
            }
            case 0x0029:
            {
                iRegister = hexDigitMap[( opCode & 0x0f00 ) >> 8];
                break;
            }
            case 0x0033:
            {
                uint8_t valueToStore = vRegisters[( opCode & 0x0f00 ) >> 8];
                int hundreds = valueToStore / 100;
                int tens = valueToStore % 100 / 10;
                int ones = valueToStore % 10;
                memory[iRegister] = hundreds;
                memory[iRegister + 1] = tens;
                memory[iRegister + 2] = ones;
                break;
            }
            case 0x0055:
            {
                uint8_t stopReg = ( opCode & 0x0f00 ) >> 8;
                for( uint8_t i = 0; i <= stopReg; ++i )
                {
                    memory[iRegister + i] = vRegisters[i];
                }
                break;
            }
            case 0x0065:
            {
                uint8_t stopReg = ( opCode & 0x0f00 ) >> 8;
                for( uint8_t i = 0; i <= stopReg; ++i )
                {
                    vRegisters[i] = memory[iRegister + i];
                }
                break;
            }
            break;
            }
            break;
        }
        default:
            break;
        }
    }

    if( delayTimer > 0 )
    {
        --delayTimer;
    }

    if( soundTimer > 0 )
    {
        // Implment Sound here...
        --soundTimer;
    }

    return;
}

void Chip8::setKeys() noexcept
{
    std::cout << "Not Implement!! \n";
}
