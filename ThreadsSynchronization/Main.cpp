#include <iostream>
#include <windows.h>
using namespace std;


CRITICAL_SECTION csArray;

HANDLE* stopEvents;
HANDLE* continueEvents;

bool TERMINATE;

int initial_size;

int* initial_array;
int* thread_indices;

DWORD WINAPI marker(void* param);


int main() {
	TERMINATE = false;
	cout << "Enter array length: ";
	cin >> initial_size;

	initial_array = new int[initial_size];
	for (int i = 0; i < initial_size; i++)
		initial_array[i] = 0;

	int number_of_threads;
	cout << "Enter number of threads: ";
	cin >> number_of_threads;

	HANDLE* threads = new HANDLE[number_of_threads];

	stopEvents = new HANDLE[number_of_threads];
	continueEvents = new HANDLE[number_of_threads];

	thread_indices = new int[number_of_threads];

	for (int i = 0; i < number_of_threads; i++) {
		stopEvents[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
		continueEvents[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
	}

	for (int i = 0; i < number_of_threads; i++) {
		thread_indices[i] = i + 1;
		threads[i] = CreateThread(NULL, 0, marker, &thread_indices[i], 0, NULL);
		if (!threads[i]) cout << "Not created!\n";
	}

	InitializeCriticalSection(&csArray);
	int have_threads = number_of_threads;
	while (have_threads) {

		for (int i = 0; i < number_of_threads; i++) {
			if (thread_indices[i] != -1) ResetEvent(stopEvents[i]);
			SetEvent(continueEvents[i]);
		}

		WaitForMultipleObjects(number_of_threads, stopEvents, TRUE, INFINITE);

		cout << "\nInitial array: ";
		for (int i = 0; i < initial_size; i++)
			cout << initial_array[i] << " ";

		cout << "\nAvailable threads: ";
		for (int i = 0; i < number_of_threads; i++) {
			if (thread_indices[i] != -1) cout << thread_indices[i] << " ";
		}

		cout << "\nWhich thread do you want to terminate?\n";
		int thread_to_terminate;
		do {
			cin >> thread_to_terminate;

			if (thread_to_terminate <= number_of_threads && thread_to_terminate > 0 && thread_indices[thread_to_terminate - 1] != -1) break;
			cout << '\n' << thread_to_terminate << " doesn't exist!" << "\nPlease, enter existing thread: ";
		} while (true);
		cout << '\n';

		TERMINATE = true;
	
		SetEvent(continueEvents[thread_to_terminate - 1]);
		WaitForSingleObject(threads[thread_to_terminate - 1], INFINITE);
		CloseHandle(threads[thread_to_terminate - 1]);
		thread_indices[thread_to_terminate - 1] = -1;
		have_threads--;

		TERMINATE = false;
	}

	cout << "Result array: ";
	for (int i = 0; i < initial_size; i++)
		cout << initial_array[i] << " ";
	cout << '\n';

	for (int i = 0; i < number_of_threads; i++) {
		CloseHandle(stopEvents[i]);
		CloseHandle(continueEvents[i]);
	}

	cout << "Finishing...\n";
}
