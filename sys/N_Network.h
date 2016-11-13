#ifndef _N_Network
#define _N_Network

class network{
int inputNodes, hiddenNodes, outputNodes;

//neurons
	double* inputNeurons;
	double* hiddenNeurons;
	double* outputNeurons;

	//weights
	double** wInputHidden;
	double** wHiddenOutput;

	//Friends
	//--------------------------------------------------------------------------------------------
	friend neuralNetworkTrainer;

	//public methods
	//--------------------------------------------------------------------------------------------

public:
  //constructor & destructor
  network(int numInput, int numHidden, int numOutput);
  ~network();

	//weight operations
	bool loadWeights(char* inputFilename);
	bool saveWeights(char* outputFilename);
	int* feedForwardPattern( double* pattern );
	double getSetAccuracy( std::vector<dataEntry*>& set );
	double getSetMSE( std::vector<dataEntry*>& set );

	//private methods
	//--------------------------------------------------------------------------------------------

private:

	void initializeWeights();
	inline double activationFunction( double x );
	inline int clampOutput( double x );
	void feedForward( double* pattern );

};
#endif
