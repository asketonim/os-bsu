#include "MinMax.h"

DWORD WINAPI get_min_max(void* param) {
	MinMaxParams* params = (MinMaxParams*)param;
	params->min = params->arr[0];
	params->max = params->arr[0];
	for (int i = 1; i < params->length; i++) {
		if (params->arr[i] > params->max) {
			params->max = params->arr[i];
			Sleep(7);
		}
		else if (params->arr[i] < params->min) {
			params->min = params->arr[i];
			Sleep(7);
		}
	}
	std::cout << "Min: " << params->min << '\n';
	std::cout << "Max: " << params->max << '\n';
	return 0;
}
