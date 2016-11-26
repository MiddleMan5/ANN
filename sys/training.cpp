#ifndef _training
#define _training

//training class
#include <fstream>
#include <sstream>
#include <Windows.h>

class TrainingData
{
public:
    TrainingData(const std::string filename, double normFactor);
    bool isEof(void) { return m_trainingDataFile.eof(); }
    void getTopology(std::vector<unsigned> &topology);

    // Returns the number of input values read from the file:
    unsigned getNextInputs(value_Container &inputVals);
    unsigned getTargetOutputs(value_Container &targetOutputVals);

private:
    std::ifstream m_trainingDataFile;
    double _normFactor;
};

void TrainingData::getTopology(std::vector<unsigned> &topology)
{
    std::string line;
    std::string label;

    getline(m_trainingDataFile, line);
    std::stringstream ss(line);
    ss >> label;
    if (this->isEof() || label.compare("topology:") != 0) {
        abort();
    }

    while (!ss.eof()) {
        unsigned n;
        ss >> n;
        topology.push_back(n);
    }

    return;
}

TrainingData::TrainingData(const std::string filename, double normFactor)
{
    m_trainingDataFile.open(filename.c_str());
    _normFactor = normFactor;
}

unsigned TrainingData::getNextInputs(value_Container &inputVals)
{
    inputVals.clear();

    std::string line;
    getline(m_trainingDataFile, line);
    std::stringstream ss(line);

    std::string label;
    ss>> label;
    if (label.compare("in:") == 0)
    {
        double oneValue;

        while (ss >> oneValue)
            inputVals.push_back(oneValue/_normFactor);
    }

    return inputVals.size();
}

unsigned TrainingData::getTargetOutputs(value_Container &targetOutputVals)
{
    targetOutputVals.clear();

    std::string line;
    getline(m_trainingDataFile, line);
    std::stringstream ss(line);

    std::string label;
    ss >> label;
    if (label.compare("out:") == 0)
    {
        double oneValue;

        while (ss >> oneValue)
            targetOutputVals.push_back(oneValue/_normFactor);
    }

    return targetOutputVals.size();
}


struct Connection{
	double weight;
	double deltaWeight;
};

#endif
