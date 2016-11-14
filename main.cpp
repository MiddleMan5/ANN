#include "./sys/N_Network.cpp"

int main(){

vector<unsigned> topology;
  for(int i=3; i>0; --i)topology.push_back(i);
  network net(topology);

  vector<double> inputValues;
  net.feedForward(inputValues);

  vector<double> targetValues;
  net.feedBack(targetValues);

  vector<double> resultValues;
  net.analyzeFeedback(resultValues);

}
