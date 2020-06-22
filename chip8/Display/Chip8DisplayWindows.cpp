#include "..\Chip8.hpp"
#include <iostream>

#if defined(_WIN64) || defined(WIN32)

void Chip8::initDevConsol()
{
	HANDLE hConOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi{};
	COORD testCord{};
	const char boxConerTopLeft = '\xd6';
	const char boxHorizontalLineThin = '\xc4';
	const char boxConerTopRight = '\xb7';
	const char boxVertical = '\xba';
	const char boxConerBottomLeft = '\xc8';
	const char boxConerBottomRight = '\xbc';
	const char boxHorizontalLineThicc = '\xcd';

	GetConsoleScreenBufferInfo(hConOut, &csbi);

	testCord.X = (csbi.srWindow.Right / 2) + (csbi.srWindow.Right / 4);
	testCord.Y = 0;

	SetConsoleCursorPosition(hConOut, testCord);

	std::cout << boxConerTopLeft << boxHorizontalLineThin << "Registers";

	GetConsoleScreenBufferInfo(hConOut, &csbi);

	for (int i = csbi.dwCursorPosition.X; i < csbi.srWindow.Right; i++)
	{
		std::cout << boxHorizontalLineThin;
	}

	std::cout << boxConerTopRight;

	for (int i = 1; i <= 4; i++)
	{
		testCord.X = (csbi.srWindow.Right / 2) + (csbi.srWindow.Right / 4);
		testCord.Y = i;

		SetConsoleCursorPosition(hConOut, testCord);

		std::cout << boxVertical;

		testCord.X = csbi.srWindow.Right;

		SetConsoleCursorPosition(hConOut, testCord);

		std::cout << boxVertical;
	}

	testCord.X = (csbi.srWindow.Right / 2) + (csbi.srWindow.Right / 4);
	testCord.Y++;

	SetConsoleCursorPosition(hConOut, testCord);

	std::cout << boxConerBottomLeft;

	GetConsoleScreenBufferInfo(hConOut, &csbi);

	for (int i = csbi.dwCursorPosition.X; i < csbi.srWindow.Right; i++)
	{
		std::cout << boxHorizontalLineThicc;
	}

	std::cout << boxConerBottomRight << std::endl;

	GetConsoleScreenBufferInfo(hConOut, &csbi);

	this->prompt = csbi.dwCursorPosition;

	testCord.X = (csbi.srWindow.Right / 2) + (csbi.srWindow.Right / 4);
	testCord.X += 2;
	testCord.Y = 1;

	SetConsoleCursorPosition(hConOut, testCord);

	for (int i = 0; i < 16; i++)
	{
		if (i % 4 == 0 && i != 0)
		{
			testCord.Y++;
			SetConsoleCursorPosition(hConOut, testCord);
		}
		std::cout << "V" << std::hex << i << ": ";

		GetConsoleScreenBufferInfo(hConOut, &csbi);

		this->registerDisplay[i] = csbi.dwCursorPosition;

		if (this->vRegisters[i] < 16)
		{
			std::cout << std::hex << "0" << std::hex << static_cast<int>(this->vRegisters[i]);
		}
		else
		{
			std::cout << std::hex << static_cast<int>(this->vRegisters[i]);
		}

		std::cout << " ";
			
		GetConsoleScreenBufferInfo(hConOut, &csbi);
	}

	SetConsoleCursorPosition(hConOut, this->prompt);
}

void Chip8::updateDebugRegisterDisplay()
{
	HANDLE hConOut = GetStdHandle(STD_OUTPUT_HANDLE);

	for (int i = 0; i < this->registerDisplay.size(); i++)
	{
		SetConsoleCursorPosition(hConOut, this->registerDisplay[i]);
	
		if (this->vRegisters[i] < 16)
		{
			std::cout << std::hex << "0" << std::hex << static_cast<int>(this->vRegisters[i]);
		}
		else
		{
			std::cout << std::hex << static_cast<int>(this->vRegisters[i]);
		}
	}

	SetConsoleCursorPosition(hConOut, this->prompt);
}
#endif // !_WIN defined
