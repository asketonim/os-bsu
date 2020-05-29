#include <iostream>
#include <windows.h>

struct Params {
	int length;
	int index;
};

CRITICAL_SECTION csArray;
CRITICAL_SECTION csConsole;

HANDLE* stopEvents;
HANDLE* continueEvents;

bool TERMINATE;
int* initial_array;
int* thread_indices;
Params* param_array;


DWORD WINAPI marker(void* param);

int main() {
	TERMINATE = false;
	int length;
	std::cout << "Enter array length: ";
	std::cin >> length;

	initial_array = new int[length];
	for (int i = 0; i < length; i++)
		initial_array[i] = 0;

	int number_of_threads;
	std::cout << "Enter number of threads: ";
	std::cin >> number_of_threads;

	HANDLE* threads = new HANDLE[number_of_threads];

	stopEvents = new HANDLE[number_of_threads];
	continueEvents = new HANDLE[number_of_threads];

	thread_indices = new int[number_of_threads];
	param_array = new Params[number_of_threads];

	for (int i = 0; i < number_of_threads; i++) {
		stopEvents[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
		continueEvents[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
	}

	InitializeCriticalSection(&csConsole);
	InitializeCriticalSection(&csArray);
	for (int i = 0; i < number_of_threads; i++) {
		param_array[i].length = length;
		param_array[i].index = i + 1;
		threads[i] = CreateThread(NULL, 0, marker, &param_array[i], 0, NULL);
		if (!threads[i]) std::cout << "Not created!\n";
	}

	WaitForMultipleObjects(number_of_threads, stopEvents, TRUE, INFINITE);

	for (int i = 0; i < length; i++)
		std::cout << initial_array[i] << " ";
	std::cout << '\n';

	TERMINATE = true;

	std::cout << "Which thread do you want to terminate? (from 1 to " << number_of_threads << ")\n";
	int thread_to_terminate;
	do {
		std::cin >> thread_to_terminate;
		if (thread_to_terminate >= 1 && thread_to_terminate <= number_of_threads) break;
		else std::cout << thread_to_terminate << " - is not valid!\n" << "Enter valid number, please...\n";
	} while (true);

	SetEvent(continueEvents[thread_to_terminate - 1]);
	WaitForSingleObject(threads[thread_to_terminate - 1], INFINITE);


	for (int i = 0; i < length; i++)
		std::cout << initial_array[i] << " ";
	std::cout << '\n';

	for (int i = 0; i < number_of_threads; i++)
		SetEvent(continueEvents[i]);

	WaitForMultipleObjects(number_of_threads, threads, TRUE, INFINITE);

	DeleteCriticalSection(&csArray);
	DeleteCriticalSection(&csConsole);

	for (int i = 0; i < number_of_threads; i++) {
		CloseHandle(threads[i]);
		CloseHandle(stopEvents[i]);
		CloseHandle(continueEvents[i]);
	}

	std::cout << "Finishing...\n";
}
