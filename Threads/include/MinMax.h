#include <iostream>
#include <windows.h>

struct MinMaxParams {
	int* arr = nullptr;
	int length = 0;
	int min = 0;
	int max = 0;
};

DWORD WINAPI get_min_max(void* param);
