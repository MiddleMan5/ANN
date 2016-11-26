#include "./sys/N_Network.cpp"

int main(){

    TrainingData trainData("./TrainingDataGenerator/trainingData_OUT.txt",normFactor);
    logData(0,0,0,TRUE);
    ShellExecute(NULL, NULL, "python","./Graphing/dataGraph.py",NULL,SW_HIDE);

    cout<<"Neural Network Training Program" << endl;
    vector<unsigned> topology;
    trainData.getTopology(topology);

    network xor_net(topology);

    value_Container inputValues, targetValues, resultValues;
    int trainingPass = 0;

    while (!trainData.isEof()){
        ++trainingPass;
        cout << endl << "Pass " << trainingPass << endl;

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
        cout << "Current error: " << normFactor * xor_net.getError() << endl;
        cout << "Average error: " << normFactor * xor_net.getRecentAverageError() << endl;
        //cout << "Percent error: " << normFactor * ??????? << endl;
        gotoxy(0,1);
        logData(trainingPass, xor_net.getSumOutputGradient(), xor_net.getRecentAverageError());

        if (trainingPass > 1000 && xor_net.getRecentAverageError() < .1/normFactor)
        {
            cout << endl << "Average Error Within Defined Acceptable Range -------> break" << endl;
            break;
        }
    }
    gotoxy(0,8);
    cout << endl << "Done" << endl << endl;


    if (topology[0] == 2)
    {
        cout << "TEST" << endl;
        cout << endl;

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

        cout << "/TEST" << endl;

        while(true){
          cout << endl << "Enter X: "<<endl;
          char xin;
          cin >> xin;
          cout << "Enter Y: "<< endl;
          char yin;
          cin >> yin;

          double xin_t = (double)(parse_digit(xin));
          double yin_t = (double)(parse_digit(yin));

          inputValues.clear();
          inputValues.push_back(xin_t/normFactor);
          inputValues.push_back(yin_t/normFactor);

          xor_net.feedForward(inputValues);
          xor_net.analyzeFeedback(resultValues);

          cout << xin_t << " Squared + " <<yin_t<<" Squared = ";
            for (unsigned i = 0; i < resultValues.size(); ++i)cout << abs(resultValues[i])*normFactor << " ";
        }
    }
}
