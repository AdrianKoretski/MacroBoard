#include <iostream>
#include "SerialClass.h"
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <chrono>
#include <string>

void terminal();
void sendFKey(int f_key, int key);
void processInput(byte first_val, byte second_val);
bool isOffCooldown(byte first_val, byte second_val);

// https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
int inputToKey(byte first_val, byte second_val)
{
	return 0x41 + second_val;
}

int inputToFKey(byte first_val, byte second_val)
{
	return 0x7C + first_val;
}

const long long KEY_PRESS_COOLDOWN = 700;

Serial* SP;

const int WIDTH = 20;
const int HEIGHT = 7;

long long* last_press = new long long[HEIGHT * WIDTH];

void clearLastInput()
{
	for (int i = 0; i < HEIGHT * WIDTH; i++)
		last_press[i] = 0;
}

int main()
{
	clearLastInput();

	for (int i = 0; i < 256; i++)
	{
		SP = new Serial(("\\\\.\\COM" + std::to_string(i)).c_str());
		if (SP->IsConnected() && SP->Handshake())
		{
			std::cout << "Macro board connected" << std::endl;
			terminal();
			break;
		}
		else
		{
			delete(SP);
		}
	}
	std::cout << "Macro board disconnected" << std::endl;
}



void terminal()
{
	char incomingData[1] = "";
	int readResult = 0;

	while (SP->IsConnected())
	{
		readResult = SP->ReadData(incomingData, 1);
		if (readResult)
		{
			int first_val = *incomingData;
			do
			{
				readResult = SP->ReadData(incomingData, 1);
			} while (!readResult);
			processInput(first_val, *incomingData);
		}
	}
}

void processInput(byte first_val, byte second_val)
{
	first_val -= 15;
	second_val -= 27;

	if (isOffCooldown(first_val, second_val))
	{
		sendFKey(inputToKey(first_val, second_val), inputToFKey(first_val, second_val));
		std::cout << "F" << ((int)first_val + 12) << "+" << (char)(65 + (int)second_val) << std::endl;
	}
}

bool isOffCooldown(byte first_val, byte second_val)
{
	std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
	if (last_press[first_val * WIDTH + second_val] + KEY_PRESS_COOLDOWN > ms.count())
		return false;
	last_press[first_val * WIDTH + second_val] = ms.count();
	return true;
}

void sendFKey(int key, int f_key)
{
	INPUT f_input;
	f_input.type = INPUT_KEYBOARD;
	f_input.ki.wScan = MapVirtualKey(f_key, MAPVK_VK_TO_VSC);
	f_input.ki.time = 0;
	f_input.ki.dwExtraInfo = 0;
	f_input.ki.wVk = f_key;

	INPUT input;
	input.type = INPUT_KEYBOARD;
	input.ki.wScan = MapVirtualKey(key, MAPVK_VK_TO_VSC);
	input.ki.time = 0;
	input.ki.dwExtraInfo = 0;
	input.ki.wVk = key;

	f_input.ki.dwFlags = 0; // there is no KEYEVENTF_KEYDOWN
	SendInput(1, &f_input, sizeof(INPUT));

	input.ki.dwFlags = 0; // there is no KEYEVENTF_KEYDOWN
	SendInput(1, &input, sizeof(INPUT));

	input.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &input, sizeof(INPUT));

	f_input.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &f_input, sizeof(INPUT));
}