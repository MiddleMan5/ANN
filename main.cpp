#include "./sys/N_Network.cpp"
#include <exception>
#include <Windows.h>

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
        cout << v[i] << " ";

    cout << endl;

  }

int main(){
    TrainingData trainData("./TrainingDataGenerator/out_xor.txt");

    // e.g., { 3, 2, 1 }
    cout<<"XOR Neural Network Training Program" << endl;
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

        // Train the net what the outputs should have been:
        trainData.getTargetOutputs(targetValues);
        showVectorVals("Targets:", targetValues);
        assert(targetValues.size() == topology.back());

        xor_net.feedBack(targetValues);

        // Report how well the training is working, average over recent samples:
        cout << "Net current error: " << xor_net.getError() << endl;
        cout << "Net recent average error: " << xor_net.getRecentAverageError() << endl;
        gotoxy(0,1);

        if (trainingPass > 100 && xor_net.getRecentAverageError() < 0.005)
        {
            cout << endl << "average error acceptable -> break" << endl;
            break;
        }
    }
    gotoxy(0,8);
    cout << endl << "Done" << endl << endl;


    if (topology[0] == 2)
    {
        cout << "TEST" << endl;
        cout << endl;

        unsigned dblarr_test[4][2] = { {0,0}, {0,1}, {1,0}, {1,1} };

        for (unsigned i = 0; i < 4; ++i)
        {
            inputValues.clear();
            inputValues.push_back(dblarr_test[i][0]);
            inputValues.push_back(dblarr_test[i][1]);

            xor_net.feedForward(inputValues);
            xor_net.analyzeFeedback(resultValues);

            showVectorVals("Inputs:", inputValues);
            showVectorVals("Outputs:", resultValues);

            //display rounded output values for viability
            cout<<"Rounded: ";
            for (unsigned i = 0; i < resultValues.size(); ++i)cout << abs(round(resultValues[i])) << " ";
            cout << endl << endl;
        }

        cout << "/TEST" << endl;
    }
}
