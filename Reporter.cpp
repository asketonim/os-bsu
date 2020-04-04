#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

struct Employee {
	int num;
	char name[24];
	double hours;
};

int main(int argc, char* argv[])
{
	ifstream in(argv[1], ios::binary);
	ofstream report_file(argv[2], ios::out);
	const double hourly_wage = atof(argv[3]);
	const int length = atoi(argv[4]);

	Employee* emps = new Employee[length];

	report_file << argv[1] << " - report file:\n";

	Employee emp_temp;
	for (int i = 0; i < length; i++)
		in.read((char*)&emps[i], sizeof(Employee));

	for (int i = 0; i < length; i++) {
		emp_temp = emps[0];
		int ind = 0;
		for (int j = 0; j < length - i; j++)
			if (emp_temp.num < emps[j].num) {
				emp_temp = emps[j];
				ind = j;
			}

		emps[ind] = emps[length - i - 1];
		emps[length - i - 1] = emp_temp;
	}

	for (int i = 0; i < length; i++)
		report_file << emps[i].num << " " << emps[i].name << " " << emps[i].hours << " " << hourly_wage * emps[i].hours << '\n';

	report_file.close();
	in.close();
	return 0;
}
