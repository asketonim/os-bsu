#include <iostream>
#include <windows.h>

struct AverageParams {
	int* arr = nullptr;
	int length = 0;
	double average = 0;
};

DWORD WINAPI get_average(void* param);
