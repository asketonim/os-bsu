#include "MinMax.h"
#include "Average.h"

int main() {
	int length;
	std::cout << "Enter array length: ";
	std::cin >> length;

	int* array = new int[length];
	std::cout << "Enter array elements (" << length << " items): ";
	for (int i = 0; i < length; i++)
		std::cin >> array[i];

	HANDLE hThread;
	DWORD IDThread;

	AverageParams param;
	param.arr = array;
	param.length = length;

	// thread for average
	hThread = CreateThread(NULL, 0, get_average, &param, 0, &IDThread);
	if (hThread) {
		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread);
	}
	else {
		std::cout << "Thread is not created!";
		exit(0);
	}

	MinMaxParams param_min_max;
	param_min_max.arr = array;
	param_min_max.length = length;

	// thread for min and max 
	hThread = CreateThread(NULL, 0, get_min_max, &param_min_max, 0, &IDThread);
	if (hThread) {
		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread);
	}
	else {
		std::cout << "Thread is not created!";
		exit(0);
	}

	std::cout << "Initial array:\n";
	for (int i = 0; i < length; i++)
		std::cout << array[i] << " ";
	std::cout << "\nArray after replacing min and max items with average item:\n";
	for (int i = 0; i < length; i++) {
		if (array[i] == param_min_max.max || array[i] == param_min_max.min)
			array[i] = param.average;
		std::cout << array[i] << " ";
	}
}
