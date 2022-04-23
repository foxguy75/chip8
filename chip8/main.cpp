//
//  main.cpp
//  chip8
//
//  Created by Ben Camburn on 6/8/20.
//

#include <iostream>
#include <chrono>
#include <thread>
#include <memory>
#include "Chip8.hpp"
#include <SDL.h>

int main(int argc, char** argv) {
    Chip8 chip;

    constexpr int screenWidth = 64 * 16;
    constexpr int screenHight = 32 * 16;
    constexpr int logicalWidth = 64;
    constexpr int logicalHight = 32;

    chip.load( "C:\\Users\\foxgu\\Downloads\\PONG" );

    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    }
    else
    {
        std::unique_ptr<SDL_Window, decltype( &SDL_DestroyWindow )> window( SDL_CreateWindow( "Test Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHight, SDL_WINDOW_SHOWN ), SDL_DestroyWindow );

        std::unique_ptr<SDL_Renderer, decltype( &SDL_DestroyRenderer )> renderer( SDL_CreateRenderer( window.get(), -1, SDL_RENDERER_ACCELERATED ), SDL_DestroyRenderer );

        SDL_RenderSetLogicalSize( renderer.get(), logicalWidth, logicalHight );

        std::unique_ptr<SDL_Texture, decltype( &SDL_DestroyTexture )> texture( SDL_CreateTexture( renderer.get(), SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, logicalWidth, logicalHight ), SDL_DestroyTexture );

        std::vector<uint32_t> pixels;
        pixels.reserve( 2048 );

        for( int i = 0; i < 2048; ++i )
        {
            pixels.push_back( ( 0x00FFFFFF * 0 ) | 0xFF000000 );
        }

        constexpr int arrayMath = 64 * sizeof( uint32_t );

        SDL_UpdateTexture( texture.get(), NULL, pixels.data(), arrayMath );
        SDL_RenderClear( renderer.get() );
        SDL_RenderCopy( renderer.get(), texture.get(), NULL, NULL );
        SDL_RenderPresent( renderer.get() );

        bool running = true; 
        while( !chip.halt )
        {
            chip.runCycle();

            if( chip.drawFlag )
            {
                for( uint32_t i = 0; i < pixels.size(); ++i )
                {
                    pixels[i] = ( 0x00FFFFFF * chip.mainDisplay[i] ) | 0xFF000000;
                }

                SDL_UpdateTexture( texture.get(), NULL, pixels.data(), arrayMath );
                SDL_RenderClear( renderer.get() );
                SDL_RenderCopy( renderer.get(), texture.get(), NULL, NULL );
                SDL_RenderPresent( renderer.get() );
                chip.drawFlag = false; 
            }
            std::this_thread::sleep_for( std::chrono::milliseconds( 1200 ) );
        }
        std::cout << "Program ended... \n" << "Press enter to continue...\n";
        std::cin.ignore();
    }

    SDL_Quit();


    return 0;
}
