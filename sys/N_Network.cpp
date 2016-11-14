#ifndef _N_Network
#define _N_Network

#include <vector>
#include <iostream>

using namespace std;

class node{};

typedef vector<node> Layer;

class network{

	private:
		vector<Layer> _layer; //_layers[layerID][nodeID]

	public:
		network(const vector<unsigned> &topology);
		void feedForward(const vector<double> &inputValues) {};
		void feedBack(vector<double> &targetValues) {};
		void analyzeFeedback(vector<double> &resultValues) const {};
};

network::network(const vector<unsigned> &topology){
	unsigned stackSize = topology.size();
	for(unsigned layerID = 0; layerID < stackSize; ++layerID){
		_layer.push_back(Layer()); //create new layer stack

		for(unsigned nodeID = 0; nodeID <= topology[layerID]; ++nodeID){ //add specified number of nodes +1 bias node
			_layer.back().push_back(node());
			if(nodeID!=topology[layerID])cout<<"node: ["<<layerID<<","<<nodeID<<"] created"<<endl;
			else cout<<"Bias node: ["<<layerID <<","<<nodeID<<"] created."<<endl;
		}
	}
}
#endif
