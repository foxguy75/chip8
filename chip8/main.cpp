//
//  main.cpp
//  chip8
//
//  Created by Ben Camburn on 6/8/20.
//

#include <iostream>
#include "Chip8.hpp"

void main() {
    Chip8 chip{};
    chip.initDevConsol();

    chip.load("test1l.rom");
    chip.run();

    //std::cout << "chip program counter: ";
    //std::cout << std::hex << static_cast<int>(chip.programCounter);
    //std::cout << std::endl;

//    int temp = 0;
//    int temp2 = 0;
//
//    for (auto elem : chip.memory) {
//
//        std::cout << std::hex << static_cast<int>(elem);
//
//        if( temp2 < 1 )
//        {
//            temp2++;
//        }
//        else
//        {
//            std::cout << " " ;
//            temp2 = 0;
//        }
//
//        if( temp < 15 )
//        {
//            temp++;
//        }
//        else
//        {
//            temp = 0;
//            std::cout << std::endl;
//        }
//    }
    std::cout << "Program ended... " << std::endl << "Press enter to continue...";
    std::cout << std::endl;
    std::cin.ignore(); 
    return;
}
