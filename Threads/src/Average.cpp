#include "Average.h"

DWORD WINAPI get_average(void* param) {
	AverageParams* params = (AverageParams*)param;
	for (int i = 0; i < params->length; i++) {
		params->average += params->arr[i];
		Sleep(12);
	}
	params->average /= params->length;
	std::cout << "Average: " << params->average << '\n';
	params->average = round(params->average);
	std::cout << "Average after rounding: " << params->average << '\n';
	return 0;
}
