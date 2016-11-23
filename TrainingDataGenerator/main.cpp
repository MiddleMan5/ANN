
#include <Windows.h>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <time.h>
#include <vector>

using namespace std;

int stream_digits(std::ostream& output, int num, const std::string& delimiter = " ")
{
	static int counter;
	if(delimiter=="0."){
		if(num==0&&counter==0)output << "0.0 0.0 0.0 0.0";
		else if (num) {
				if(num<10&&counter==0)output << "0.0 0.0 0.0 ";
				else if(num<100&&counter==0)output << "0.0 0.0 ";
				else if(num<1000&&counter==0)output << "0.0 ";
				counter += 1;
	        stream_digits(output, num/10, delimiter);
	        output <<"0." << static_cast<char>('0' + (num % 10)) << " ";
	    }

	}
	else{
	if(num==0&&counter==0)output << "0 0" << delimiter;
	else if (num) {
			if(num<10&&counter==0)output << "0" << delimiter;
			else if(num<100&&counter==0)output << "" << delimiter;
			counter += 1;
        stream_digits(output, num/10, delimiter);
        output << static_cast<char>('0' + (num % 10)) << delimiter;
    }
	}
		int temp=counter;
		counter=0;
		return temp;
		// if(oldCounter>counter)stream_digits(output, num/10, delimiter);
		// oldCounter = counter;
		// counter = 0;

}


int main()
{
ofstream file;
	file.open("trainingData_OUT.txt");
	srand(time(NULL));
	int _i = 2;
	int _j = 6;
	int _k = 1;

	file << "topology: " << _i << " " << _j << " "<< _k << std::endl;
	for (int i = 10000; i >= 0; --i){

		int iNumX = 70 - rand() % 140;
		int iNumY = 70 - rand() % 140;

		float iNumZ = ((iNumX*iNumX)+(iNumY*iNumY));

		file << "in: " << (double)iNumX/10000 << " " << (double)iNumY/10000;
		//stream_digits(file, iNumX);
		file<<endl;
		//stream_digits(file, iNumY);
		file << "out: ";
		//stream_digits(file, iNumZ, "0.");
		file<<iNumZ/10000<<endl;
	}
	file.close();
	return(1);
}
