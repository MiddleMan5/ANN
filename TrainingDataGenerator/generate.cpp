/*Data generation program meant to create data input files for an ANN (Artificial Nodal Network)
----------------------------------------------------------------------------------------------------
	Created by Quinn Mikelson, inspired by references:
		Vinh Nguyen, Neural Net in C++; https://www.youtube.com/watch?v=KkwX7FkLfug
		https://github.com/GuillaumeBouchetEpitech and his project https://github.com/GuillaumeBouchetEpitech/NeuralNetwork_experiment
*/
#include <Windows.h>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <time.h>
#include <vector>
#include <random>

/* ----------------Edits----------------
+ ANN now handles normFactor, plan to generate a header of input values to the ANN
+ TrainingDataGenerator needs to handle ANN initialization as well as optional modifications.
+ Condense #include statements
+ Encapsulate TrainingDataGenerator, plan to move to cross-platform method Generation
*/

using namespace std;
int maxVal = 70; //whole number interger, random value will be between -maxVal to maxVal;
int normFactor = maxVal*2; //maximum value of the preprogrammed function with given maxVal
string topology = "( 2 10 1 )"; //Create a ANN of two inputs(inputX, inputY), 1 hiddenLayer of 10 nodes, and 1 output (inputZ)
int iterations = 10000;
//Generate Header data for ANN input
void createHeader(std::ostream& header, std::string _topology, int _normFactor, int _iterations){
	header << "HEADER[ " << "topology" << _topology << " " << normFactor << " " << _iterations << " ]" << endl;
}

int main(){

	std::random_device randGen; //creates an object to generate psuedo-random numbers; from <random>
	std::mt19937 gen(randGen());
	//create new file (Wipe existing contents) for Training Data
	ofstream file;
	file.open("trainingData_OUT.txt");
	createHeader(file, topology, normFactor, iterations);

	//--------------**arbitrary** ---------------
	// function generator for ANN training
	//Generate whole intergers for ANN testing between +- given maxVal
	std::uniform_int_distribution<> dis(-maxVal, maxVal);
		for (int i = iterations; i >= 0; --i){
			int inputX = dis(gen);
			int inputY = dis(gen);
			//Create output for two inputs
					int inputZ = (inputX)+(inputY);
					file << "in: " << inputX << " " << inputY << endl << "out: " << inputZ << endl;
		}
	file.close();
	return(1);
}




//Outdated method for parsing normalized intergers, replaced with normFactor
//Ripped from StackOverflow discussion here: http://stackoverflow.com/a/4616084
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
