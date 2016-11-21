
#include <Windows.h>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <fstream>

int main()
{
ofstream file;
file.open("trainingData_OUT.txt");

	file << "topology: 4 6 6" << std::endl;
	for (int i = 10000; i >= 0; --i)
	{
		double x = (int)(5 * (rand() / double(RAND_MAX)));
		double y = (int)(5 * (rand() / double(RAND_MAX)));

		double z = (x*x)+(y*y);

	//	int t = !n1 ^ !n2; // should be 0 xor 1

		file << "in: " << x/100 << " " << y/100 << std::endl;
		file << "out: " << z/100  << std::endl;
	}
	file.close();
	return(1);
}
