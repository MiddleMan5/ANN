/*Classes and Methods largely based on: https://github.com/GuillaumeBouchetEpitech/NeuralNetwork_experiment/blob/master/src/main.cpp
----------------------------------------------------------------------------------------------------
	Created by Quinn Mikelson, inspired by references:
		Vinh Nguyen, Neural Net in C++; https://www.youtube.com/watch?v=KkwX7FkLfug
		https://github.com/GuillaumeBouchetEpitech and his project https://github.com/GuillaumeBouchetEpitech/NeuralNetwork_experiment
*/

#ifndef _N_Network
#define _N_Network

using namespace std;

double normFactor = 1000;

#include <vector>
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <time.h>
typedef std::vector<double> value_Container; //vector meant for data of type == input

#include "./training.cpp" //must be formatted like this
typedef vector<Connection> Connections; //


class node;
typedef vector<node> Layer;

class node{
public:

node(unsigned outputQuantity, unsigned _selfIndex);
	inline void setOutputValue(double value) { _outputValue = value; }
	inline double getOutputValue(void) const { return _outputValue; }
	       void feedForward(const Layer &previousLayer);
				 void calculateOutputGradients(double targetValues);
				 void calculateHiddenGradients(const Layer &nextLayer);
				 void updateInputWeights(Layer &previousLayer);
				 double getGradient(void) const {return _gradient; }

private:
	static double transferFunction(double x);
	static double transferFunctionDerivative(double x);
	static double randomWeight(void) { return .5*( rand() / double(RAND_MAX) ); }
	       double sumDOW(const Layer &nextLayer);

	static double eta; //Net Learning Rate [0.0 --> 1.0] 0-slow learning, .2-moderate, 1.0-reckless
	static double alpha; //multiplier to weight change (momentum) [0.0 --> n] 0-no momentum, .5-moderate momentum
	static int    timeStep;
	static double decreaseConstant;
	double long weightTotal;
	static double hyperConstant;
	static double theta;
	       double      _outputValue;
				 Connections _outputWeights;
				 unsigned    _selfIndex;
				 double      _gradient;

};

double node::eta = .4; //learning rate
double node::alpha = .6; //momentum
int    node::timeStep=0;
double node::decreaseConstant=0;
double node::theta=0;
double node::hyperConstant = 1;

void gotoxy ( int column, int line ){
  COORD coord;
  coord.X = column;
  coord.Y = line;
  SetConsoleCursorPosition(
    GetStdHandle( STD_OUTPUT_HANDLE ),
    coord
  );
}

void showVectorVals(string label, value_Container &v){
    cout << label << " ";
    for (unsigned i = 0; i < v.size(); ++i)
        cout << normFactor*v[i] << " ";

    cout << endl;

  }

int parse_digit(char digit) {
    return digit - '0';
}

int logData (int pass,double gradient, double error, bool clear=FALSE) {
  ofstream dataLog;
if(clear){ dataLog.open("./Graphing/dataLog.csv", std::ofstream::trunc); dataLog.close(); }
  else {
    dataLog.open ("./Graphing/dataLog.csv", ios::app);
    dataLog << pass << "," << gradient << "," << error << "\n";
    cout.flush();
    dataLog.close();
    return 0;
  }
}

node::node(unsigned outputQuantity, unsigned selfIndex)
	: _selfIndex(selfIndex)
{
	for(unsigned i = 0; i < outputQuantity; ++i){
		_outputWeights.push_back(Connection());
		_outputWeights.back().weight = randomWeight(); //eventually replace connetion as a class with constructor to assign random weight
	}
}

void node::updateInputWeights(Layer &previousLayer){
timeStep+=1;
	for(unsigned n=0; n < previousLayer.size(); n++){

		node &node = previousLayer[n];
		eta = eta;
		double deltaWeightOld = node._outputWeights[_selfIndex].deltaWeight;
		double deltaWeightNew = eta *node.getOutputValue() * _gradient
					//add momentum as fraction of previous delta weight
					+ alpha * deltaWeightOld;

			node._outputWeights[_selfIndex].deltaWeight = deltaWeightNew;
			node._outputWeights[_selfIndex].weight += deltaWeightNew;
	}
}

double node::sumDOW(const Layer &nextLayer){
	double sum = 0.0;
  //weightTotal=0.0;
	for(unsigned n=0; n < nextLayer.size() - 1; ++n){
		sum += _outputWeights[n].weight * nextLayer[n]._gradient;
		weightTotal+=hyperConstant*_outputWeights[n].weight;
	}
	return (sum);
}

void node::feedForward(const Layer &previousLayer){
	double sum = 0.0;

	for(unsigned n=0; n < previousLayer.size()-1; ++n){
		sum += previousLayer[n].getOutputValue() * previousLayer[n]._outputWeights[_selfIndex].weight;
	}
		_outputValue = node::transferFunction(sum);
}

void node::calculateHiddenGradients(const Layer &nextLayer){
	double dow = sumDOW(nextLayer);
	_gradient = dow * node::transferFunctionDerivative(_outputValue);
}

void node::calculateOutputGradients(double targetValues){
	double delta = (targetValues - _outputValue);//*(1+(timeStep*hyperConstant));
	_gradient = delta * node::transferFunctionDerivative(_outputValue);
}

double node::transferFunction(double x){
	//tanh - output(-1.0 --> 1.0)
	return tanh(x);
}

double node::transferFunctionDerivative(double x){
	//tanh derivative
return (1-(tanh(x)*tanh(x)));
}

typedef vector<node> Layer;

class network{

public:
	network(const vector<unsigned> &topology);
	void feedForward(const value_Container &inputValues);
	void feedBack(value_Container &targetValues);
	void analyzeFeedback(value_Container &resultValues) const;
// create node based on last test run, take in vector input data, modify, and return specified output
	//void createChildNode(value_Container)
public: // error
  double getError(void) const { return _error; }
  double getRecentAverageError(void) const { return _recentAverageError; }
	double getSumOutputGradient(void) const {return _sumOutputGradient; }

private:
	vector<Layer> _layers; //_layers[layerID][nodeID]
	double _error;
	double _recentAverageError;
	static double _recentAverageSmoothingFactor;
	double _sumOutputGradient;
};

network::network(const vector<unsigned> &topology)
		:_error(0.0),
		 _recentAverageError(0.0)
{

	assert( !topology.empty() );
	srand(time(NULL));
	unsigned stackSize = topology.size();

	for(unsigned i = 0; i < stackSize; ++i){
		unsigned nodeQuantity = topology[i];

		assert( nodeQuantity > 0);

		_layers.push_back(Layer()); //create new layer stack

		Layer &newLayer = _layers.back();
		bool is_lastLayer = (i == (stackSize - 1));
		unsigned outputQuantity = ((is_lastLayer) ? (0) : topology[i+1]);


		for(unsigned j = 0; j < (nodeQuantity + 1); ++j){ //add specified number of nodes +1 bias node
			newLayer.push_back( node(outputQuantity, j) );
			// if(i!=topology[i])cout<<"node: ["<<i<<","<<j<<"] created"<<endl;
			// else cout<<"Bias node: ["<<i<<","<<j<<"] created."<<endl;
		}
		node &biasNode = newLayer.back();
		biasNode.setOutputValue(1); //set bias node to constant output
	}
}

void network::analyzeFeedback(value_Container &resultValues) const {
	resultValues.clear();
	for(unsigned n=0; n < _layers.back().size() - 1; ++n){
		resultValues.push_back(_layers.back()[n].getOutputValue());
	}
}

double network::_recentAverageSmoothingFactor = 200.0; // Number of training samples to average over

void network::feedBack(value_Container &targetValues){
	//Calculate total net error (RMS of output node errors)
		Layer &outputLayer= _layers.back();
		_error = 0.0;

		for(unsigned n=0; n < outputLayer.size()-1; ++n){
			double delta = targetValues[n] - outputLayer[n].getOutputValue();
			_error += delta * delta;
		}
		_error /= outputLayer.size()-1;
		_error= sqrt(_error); //RMS

	//implement recent average measurement
	_recentAverageError = (_recentAverageError * _recentAverageSmoothingFactor + _error)
												/ (_recentAverageSmoothingFactor + 1.0);
	//Calculate output layer gradient
_sumOutputGradient = 0;
	for(unsigned n=0; n < outputLayer.size() - 1; ++n){
		outputLayer[n].calculateOutputGradients(targetValues[n]);
	_sumOutputGradient += outputLayer[n].getGradient();
	}
	//Calculate hidden layer gradient
	for(unsigned i = _layers.size() - 2; i > 0; --i){
		Layer &hiddenLayer = _layers[i];
		Layer &nextLayer = _layers[i + 1];

	for(unsigned a=0; a < hiddenLayer.size(); ++a)
		hiddenLayer[a].calculateHiddenGradients(nextLayer);
	}

	//Update connection weights from output to first hidden layer
	for(unsigned layerID = _layers.size() - 1; layerID > 0; --layerID){
		Layer &currentLayer = _layers[layerID];
		Layer &previousLayer = _layers[layerID-1];

		for(unsigned n=0; n < currentLayer.size() -1; ++n){
			currentLayer[n].updateInputWeights(previousLayer);
		}
	}
}

void network::feedForward(const value_Container &inputValues){
	assert(inputValues.size() == _layers[0].size() - 1); //assert error handling for inputs==number of nodes -1 bias node

	//assign(latch) input values into input nodes
	for(unsigned input=0; input< inputValues.size(); ++input){
		_layers[0][input].setOutputValue(inputValues[input]);
	}

	for(unsigned i=1; i < _layers.size(); ++i){
		Layer &previousLayer = _layers[i-1];
		Layer &currentLayer = _layers[i];
		for(unsigned n=0; n < currentLayer.size() - 1; ++n){
			currentLayer[n].feedForward(previousLayer);
		}
	}
}




#endif
