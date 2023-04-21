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
bool isOffCooldown();
void remapInput(byte* first_val, byte* second_val);

// https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
int inputToKey(byte first_val, byte second_val)
{
	return 0x41 + first_val;
}

int inputToFKey(byte first_val, byte second_val)
{
	return 0x7C + second_val;
}

const long long KEY_PRESS_COOLDOWN = 700;

Serial* SP;

const int WIDTH = 20;
const int HEIGHT = 7;

byte first_val_mapping[] = { 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 12, 13, 14, 15, 16, 17 };
byte second_val_mapping[] = { 0, 2, 1, 3, 5, 4, 7, 6 };

long long last_press = 0;

int main()
{
	for (int i = 0; i < 256; i++)
	{
		SP = new Serial(("\\\\.\\COM" + std::to_string(i)).c_str());
		if (SP->IsConnected() && SP->Handshake())
		{
			std::cout << "Macro board connected" << std::endl;
			Sleep(500);
			char* c = new char[4]();
			c[0] = 1 * 400 % 256;
			c[1] = 1 * 400 / 256;
			c[2] = 1;
			c[3] = '\n';
			SP->WriteData(c, 4);
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
	if (isOffCooldown())
	{
		remapInput(&first_val, &second_val);
		sendFKey(inputToKey(first_val, second_val), inputToFKey(first_val, second_val));
		std::cout << "F" << ((int)second_val + 12) << "+" << (char)(65 + (int)first_val) << std::endl;
	}
}

void remapInput(byte* first_val, byte* second_val)
{
	*first_val -= 2;
	*second_val -= 44;
	*first_val = first_val_mapping[(int)*first_val];
	*second_val = second_val_mapping[(int)*second_val];
}

bool isOffCooldown()
{
	std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
	if (last_press + KEY_PRESS_COOLDOWN > ms.count())
		return false;
	last_press = ms.count();
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