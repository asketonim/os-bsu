#include <iostream>
#include <windows.h>


extern CRITICAL_SECTION csArray;

extern HANDLE* stopEvents;
extern HANDLE* continueEvents;

extern bool TERMINATE;

extern int initial_size;

extern int* initial_array;
extern int* thread_indices;


DWORD WINAPI marker(void* param) {
	int index = *(int*)param;
	WaitForSingleObject(continueEvents[index - 1], INFINITE);
	srand(index);
	int counter = 0;

	while (true) {
		ResetEvent(continueEvents[index - 1]);
		int randomized = rand();

		EnterCriticalSection(&csArray);
		while (true) {
			randomized %= initial_size;
			if (initial_array[randomized]) break;
			Sleep(5);
			initial_array[randomized] = index;
			Sleep(5);
			counter++;
			randomized = rand();
		}
		LeaveCriticalSection(&csArray);
		
		printf("thread number: %d | marked elements: %d | index of nonzero: %d\n", index, counter, randomized);

		SetEvent(stopEvents[index - 1]);
		WaitForSingleObject(continueEvents[index - 1], INFINITE);
		
		if (TERMINATE) {
			for (int j = 0; j < initial_size; j++)
				if (initial_array[j] == index) initial_array[j] = 0;
			ExitThread(0);
		}
	}

	return 0;
}
