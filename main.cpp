/*Runtime program for testing and control of prototype ANN
----------------------------------------------------------------------------------------------------
	Created by Quinn Mikelson, inspired by references:
		Vinh Nguyen, Neural Net in C++; https://www.youtube.com/watch?v=KkwX7FkLfug
		https://github.com/GuillaumeBouchetEpitech and his project https://github.com/GuillaumeBouchetEpitech/NeuralNetwork_experiment
*/

#include "./sys/N_Network.cpp"

int main(){

    TrainingData trainData("./TrainingDataGenerator/trainingData_OUT.txt",normFactor);
    logData(0,0,0,TRUE);
    //ShellExecute(NULL, NULL, "python","./Graphing/dataGraph.py",NULL,SW_HIDE);
    processHandler();

    std::cout<<"Neural Network Training Program" << std::endl;
    std::vector<unsigned> topology;
    trainData.getTopology(topology);

    network xor_net(topology);

    value_Container inputValues, targetValues, resultValues;
    int trainingPass = 0;

    while (!trainData.isEof()){
        ++trainingPass;
        std::cout << std::endl << "Pass " << trainingPass << std::endl;

        // Get new input data and feed it forward:
        if (trainData.getNextInputs(inputValues) != topology[0])
            break;

        showVectorVals("Inputs:", inputValues);
        xor_net.feedForward(inputValues);

        // Collect the net's actual output results:
        xor_net.analyzeFeedback(resultValues);
        showVectorVals("Outputs:", resultValues);

        // Train the net on error delta:
        trainData.getTargetOutputs(targetValues);
        showVectorVals("Targets:", targetValues);
        assert(targetValues.size() == topology.back());

        xor_net.feedBack(targetValues);

        // Report how well the training is working, average over recent samples:
        std::cout << "Current error: " << normFactor * xor_net.getError() << std::endl;
        std::cout << "Average error: " << normFactor * xor_net.getRecentAverageError() << std::endl;
        //std::cout << "Percent error: " << normFactor * ??????? << std::endl;
        gotoxy(0,1);
        logData(trainingPass, xor_net.getSumOutputGradient(), xor_net.getRecentAverageError());

        if (trainingPass > 1000 && xor_net.getRecentAverageError() < .02/normFactor)
        {
            std::cout << std::endl << "Average Error Within Defined Acceptable Range -------> break" << std::endl;
            break;
        }
    }
    gotoxy(0,8);
    std::cout << std::endl << "Done" << std::endl << std::endl;


    if (topology[0] == 2)
    {
        std::cout << "TEST" << std::endl;
        std::cout << std::endl;

        double dblarr_test[8][2] = { {0/normFactor,3/normFactor}, {1/normFactor,1/normFactor}, {20/normFactor,16/normFactor}, {5/normFactor,2/normFactor},
                                     {0/normFactor,0/normFactor}, {81/normFactor,59/normFactor}, {20/normFactor,20/normFactor}, {32/normFactor,11/normFactor} };

        for (unsigned i = 0; i < 8; ++i)
        {
            inputValues.clear();
            inputValues.push_back(dblarr_test[i][0]);
            inputValues.push_back(dblarr_test[i][1]);

            xor_net.feedForward(inputValues);
            xor_net.analyzeFeedback(resultValues);

            showVectorVals("Inputs:", inputValues);
            showVectorVals("Outputs:", resultValues);
        }

        std::cout << "/TEST" << std::endl;

        while(true){
          std::cout << std::endl << "Enter X: "<<std::endl;
          char xin;
          std::cin >> xin;
          std::cout << "Enter Y: "<< std::endl;
          char yin;
          std::cin >> yin;

          double xin_t = (double)(parse_digit(xin));
          double yin_t = (double)(parse_digit(yin));

          inputValues.clear();
          inputValues.push_back(xin_t/normFactor);
          inputValues.push_back(yin_t/normFactor);

          xor_net.feedForward(inputValues);
          xor_net.analyzeFeedback(resultValues);

          std::cout << xin_t << " + " <<yin_t<<" = ";
            for (unsigned i = 0; i < resultValues.size(); ++i)std::cout << abs(resultValues[i])*normFactor << " ";
        }
    }
}
