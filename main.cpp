#include "./sys/N_Network.cpp"
#include <exception>
/*int main(){

vector<unsigned> topology;
  for(int i=3; i>0; --i)topology.push_back(i);
  network net(topology);

  vector<double> inputValues;
  net.feedForward(inputValues);

  vector<double> targetValues;
  net.feedBack(targetValues);

  vector<double> resultValues;
  net.analyzeFeedback(resultValues);

}*/
void showVectorVals(std::string label, t_vals &v)
{
    std::cout << label << " ";
    for (unsigned i = 0; i < v.size(); ++i)
        std::cout << v[i] << " ";

    std::cout << std::endl;
}

int main()
{
      int counter=0;
    TrainingData trainData("./trainsample/out_xor.txt");
    // TrainingData trainData("trainsample/out_and.txt");
    // TrainingData trainData("trainsample/out_or.txt");
    // TrainingData trainData("trainsample/out_no.txt");
    counter+=1;
cout<<"Success "<< counter <<endl;
    // e.g., { 3, 2, 1 }

    std::vector<unsigned> topology;
    trainData.getTopology(topology);

    network myNet(topology);
    counter+=1;
  cout<<"Success "<< counter <<endl;
    t_vals inputValues, targetValues, resultValues;
    int trainingPass = 0;

    while (!trainData.isEof())
    {
        ++trainingPass;
        std::cout << std::endl << "Pass " << trainingPass << std::endl;

        // Get new input data and feed it forward:
        if (trainData.getNextInputs(inputValues) != topology[0])
            break;

        showVectorVals("Inputs:", inputValues);
        myNet.feedForward(inputValues);

        // Collect the net's actual output results:
        myNet.analyzeFeedback(resultValues);
        showVectorVals("Outputs:", resultValues);

        // Train the net what the outputs should have been:
        trainData.getTargetOutputs(targetValues);
        showVectorVals("Targets:", targetValues);
        assert(targetValues.size() == topology.back());

        myNet.feedBack(targetValues);

        // Report how well the training is working, average over recent samples:
        std::cout << "Net current error: " << myNet.getError() << std::endl;
        std::cout << "Net recent average error: " << myNet.getRecentAverageError() << std::endl;

        if (trainingPass > 100 && myNet.getRecentAverageError() < 0.05)
        {
            std::cout << std::endl << "average error acceptable -> break" << std::endl;
            break;
        }
    }

    std::cout << std::endl << "Done" << std::endl;

    if (topology[0] == 2)
    {
        std::cout << "TEST" << std::endl;
        std::cout << std::endl;

        unsigned dblarr_test[4][2] = { {0,0}, {0,1}, {1,0}, {1,1} };

        for (unsigned i = 0; i < 4; ++i)
        {
            inputValues.clear();
            inputValues.push_back(dblarr_test[i][0]);
            inputValues.push_back(dblarr_test[i][1]);

            myNet.feedForward(inputValues);
            myNet.analyzeFeedback(resultValues);

            showVectorVals("Inputs:", inputValues);
            showVectorVals("Outputs:", resultValues);

            std::cout << std::endl;
        }

        std::cout << "/TEST" << std::endl;
    }
}
