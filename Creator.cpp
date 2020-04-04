#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct Employee {
	int num;
	char name[24];
	double hours;
};

int main(int argc, char* argv[])
{
	ofstream out(argv[1], ios::binary);
	if (!out) {
		cout << "Cannot open file!" << endl;
		return 1;
	}
	int length = atoi(argv[2]);
	Employee emp;
	while (length) {
		cout << "Enter number, worker name and hours: ";
		cin >> emp.num >> emp.name >> emp.hours;
		out.write((char*) &emp, sizeof(Employee));
		length--;
	}
	out.close();
}