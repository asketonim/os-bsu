#define _CRT_SECURE_NO_WARNINGS

#include <fstream>
#include <iostream>
#include <string>
#include <windows.h>
#include <conio.h>
#include <stdio.h>
using namespace std;


struct Employee {
	int num;
	char name[24];
	double hours;
};


int main() {

	char creator_name[24];
	cout << "Enter file name: ";
	cin >> creator_name;

	char length[8];
	cout << "Enter number of employees: ";
	cin >> length;

	const char path_creator[] = "D:\\My files\\University\\OS\\labs\\lab1\\Creator\\x64\\Debug\\Creator.exe";

	char cmnd_line_creator[100];
	sprintf(cmnd_line_creator, "%s %s %s", path_creator, creator_name, length);

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si = { sizeof(STARTUPINFO) };

	if (CreateProcess(NULL, cmnd_line_creator, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}
	else {
		cout << "Failed to create process!";
		exit(0);
	}

	ifstream creator_file(creator_name, ios::binary);

	Employee emp;
	while (creator_file.read((char*)&emp, sizeof(Employee)))
		cout << emp.num << " " << emp.name << " " << emp.hours << '\n';
	
	creator_file.close();


	char report_name[24];
	cout << "\nEnter report file name: ";
	cin >> report_name;

	char hourly_wage[8];
	cout << "Enter hourly wage: ";
	cin >> hourly_wage;

	char cmnd_line_reporter[100];
	const char path_reporter[] = "D:\\My files\\University\\OS\\labs\\lab1\\Reporter\\Debug\\Reporter.exe";

	sprintf(cmnd_line_reporter, "%s %s %s %s %s", path_reporter, creator_name, report_name, hourly_wage, length);

	if (CreateProcess(NULL, cmnd_line_reporter, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}
	else {
		cout << "Error while creating proccess";
		exit(0);
	}

	ifstream report_file(report_name);

	string report_item;

	cout << '\n';
	while (getline(report_file, report_item))
		cout << report_item << '\n';

	report_file.close();
}