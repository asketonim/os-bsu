#include <iostream>
#include <windows.h>

CRITICAL_SECTION cs;
HANDLE globalEvent;
HANDLE* stopEvents;
HANDLE* continueEvents;
bool TERMINATE;
int* array;


struct Params {
	int length;
	int index;
};

DWORD WINAPI marker(void* param) {
	WaitForSingleObject(globalEvent, INFINITE);

	Params* params = (Params*)param;
	srand(params->index);
	int counter = 0;
	for (int i = 0; i < params->length; i++) {
		int randomized = rand();
		randomized /= params->length;
		if (!array[randomized] || !TERMINATE) {
			Sleep(5);
			EnterCriticalSection(&cs);
			array[randomized] = params->index - 1;
			counter++;
			LeaveCriticalSection(&cs);
			Sleep(5);
		}
		else {
			std::cout << "here " << params->index - 1 << " " << counter << " " << i << '\n';
			SetEvent(stopEvents[params->index - 1]);
			WaitForSingleObject(continueEvents[params->index - 1], INFINITE);
			ResetEvent(stopEvents[params->index - 1]);
			ResetEvent(continueEvents[params->index - 1]);
			if (TERMINATE) {
				for (int i = 0; i < params->length; i++)
					array[i] = 0;
				SetEvent(stopEvents[params->index - 1]);
				ExitThread(0);
			}
		}
	}

	return 0;
}

int main() {
	TERMINATE = true;
	int length;
	std::cout << "Enter array length: ";
	std::cin >> length;

	array = new int[length];
	for (int i = 0; i < length; i++)
		array[i] = 0;

	int number_of_threads;
	std::cout << "Enter number of threads: ";
	std::cin >> number_of_threads;

	HANDLE hThread;
	HANDLE hEvent;
	DWORD IDThread;

	HANDLE* threads = new HANDLE[number_of_threads];

	stopEvents = new HANDLE[number_of_threads];
	continueEvents = new HANDLE[number_of_threads];

	for (int i = 0; i < number_of_threads; i++) {
		stopEvents[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
		continueEvents[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
	}

	for (int i = 0; i < number_of_threads; i++) {
		Params param;
		param.length = length;
		param.index = i + 1;
		threads[i] = CreateThread(NULL, 0, marker, &param, 0, &IDThread);
		if (!threads[i]) std::cout << "Not created!\n";
	}

	globalEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	InitializeCriticalSection(&cs);
	SetEvent(globalEvent);

	WaitForMultipleObjects(number_of_threads, stopEvents, true, INFINITE);

	for (int i = 0; i < number_of_threads; i++)
		std::cout << array[i] << " ";
	std::cout << '\n';


	std::cout << "Which thread do you want to terminate? (from 0 to" << number_of_threads - 1 << ")";
	int thread_to_terminate;
	std::cin >> thread_to_terminate;

	SetEvent(continueEvents[thread_to_terminate]);
	WaitForSingleObject(threads[thread_to_terminate], INFINITE);


	for (int i = 0; i < number_of_threads; i++)
		std::cout << array[i] << " ";
	std::cout << '\n';

	TERMINATE = false;
	
	for (int i = 0; i < number_of_threads; i++)
		SetEvent(continueEvents[i]);

	CloseHandle(globalEvent);

	for (int i = 0; i < number_of_threads; i++) {
		CloseHandle(threads[i]);
		CloseHandle(stopEvents[i]);
		CloseHandle(continueEvents[i]);
	}

	std::cout << "Finishing...\n";
}