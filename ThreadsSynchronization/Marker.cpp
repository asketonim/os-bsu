#include <iostream>
#include <windows.h>

struct Params {
	int length;
	int index;
};

extern CRITICAL_SECTION csConsole;
extern CRITICAL_SECTION csArray;

extern HANDLE* stopEvents;
extern HANDLE* continueEvents;
extern bool TERMINATE;

extern int* initial_array;
extern int* thread_indices;
extern Params* param_array;


DWORD WINAPI marker(void* param) {
	Params params = *(Params*)param;
	srand(params.index);
	int counter = 0;
	bool stop_working;
	for (int i = 0; i < params.length; i++) {
		int randomized = rand();

		randomized %= params.length;
		stop_working = true;
		EnterCriticalSection(&csArray);
		if (initial_array[randomized] == 0) {
			Sleep(5);
			initial_array[randomized] = params.index;
			counter++;
			stop_working = false;
			Sleep(5);
		}
		LeaveCriticalSection(&csArray);
		// 
		if (stop_working) {
			EnterCriticalSection(&csConsole);
			std::cout << "thread number: " << params.index << " | marked elements: " << counter << "| index of nonzero: " << i << '\n';
			LeaveCriticalSection(&csConsole);
			SetEvent(stopEvents[params.index - 1]);
			WaitForSingleObject(continueEvents[params.index - 1], INFINITE);
		}
		//
		if (TERMINATE) {
			for (int j = 0; j < params.length; j++)
				if (initial_array[j] == params.index) initial_array[j] = 0;
			ExitThread(0);
		}
	}

	return 0;
}