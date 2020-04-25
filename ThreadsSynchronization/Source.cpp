#include <iostream>
#include <windows.h>

DWORD WINAPI maker(LPVOID param) {
	int index = *(int*)param;
	std::cout << index << '\n';
	return 0;
}

int main() {
	int length;
	std::cout << "Enter array length: ";
	std::cin >> length;

	int* array = new int[length];
	for (int i = 0; i < length; i++)
		array[i] = 0;

	int number_of_threads;
	std::cout << "Enter number of threads: ";
	std::cin >> number_of_threads;

	HANDLE hThread;
	DWORD IDThread;

	for (int i = 0; i < number_of_threads; i++) {
		hThread = CreateThread(NULL, 0, maker, &i, 0, &IDThread);
		if (hThread) {
			WaitForSingleObject(hThread, INFINITY);
			CloseHandle(hThread);
		}
		else std::cout << "no";
	}
}