
#include <Windows.h>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <time.h>
#include <vector>

/* ----------------Edits----------------
+ ANN now handles normFactor, plan to generate a header of input values to the ANN
+ TrainingDataGenerator needs to handle ANN initialization as well as optional modifications.
+ Condense #include statements
+ Encapsulate TrainingDataGenerator, plan to move to crossplatform method Generation
*/
using namespace std;

int maxVal = 50; //whole number interger, random value will be between -maxVal to maxVal;

//int normFactor; //maximum value of the preprogrammed function with given maxVal
//ANN Now handles normFactor

string topology = "2 10 1";

int main(){

//create new file (Wipe existing contents) for Training Data
ofstream file;
file.open("trainingData_OUT.txt");

//Seed rand() function with time()
srand(time(NULL));

//Generate Header data for ANN input
file << "topology: " << topology << " " << std::endl;

//arbitrary function generator for ANN training
//
	for (int i = 100000; i >= 0; --i){
		//Generate whole intergers for ANN testing between +- given maxVal
				int iNumX = maxVal - rand() % maxVal * 2;//MODULO BIAS!!! Replace with even distribution function
				int iNumY = maxVal - rand() % maxVal * 2;

		//Create output for two inputs
				int iNumZ = (iNumX)+(iNumY);
				file << "in: " << iNumX << " " << iNumY << endl << "out: " << iNumZ << endl;
	}
file.close();
return(1);
}

//Outdated method for parsing normalized intergers, replaced with normFactor
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
