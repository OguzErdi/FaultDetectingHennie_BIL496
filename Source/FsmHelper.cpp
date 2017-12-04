#include "FsmHelper.h"

ostream &operator<<( ostream &output, const Transition &transition )
{
    output << transition.getInputState();
    output << transition.getOutputState();
    output << transition.getInput();
    output << transition.getOutput();

    return output;
}

ostream &operator<<(ostream &output, const FiniteStateMachine &fsm)
{
    for (auto &val: fsm.getTrans())
        output<< val << " ";
    output<<endl;
    return output;
}

FsmHelper::FsmHelper()
{
    dataFsmFile.open("../FsmData/fsm.txt");
    if(dataFsmFile.is_open())
        cout << "Fsm.txt file is opened\n";
    else
        cout << "Fsm.txt file doesnt open\n";

}
FsmHelper::FsmHelper(string file)
{
    dataFsmFile.open(file);
    if(dataFsmFile.is_open())
        cout << file + " file is opened\n";
    else
        cout << file + " file doesnt open\n";
}

bool FsmHelper::GetOneFsm()
{
    char tempC;
    int tempInt;
    int transNo;

    dataFsmFile >> tempC;
    if(dataFsmFile.eof())
        return false;
    dataFsmFile >> tempC;
    dataFsmFile >> tempC;
    dataFsmFile >> tempInt;
    inputs.push_back(tempInt);
    dataFsmFile >> transNo;
    inputs.push_back(transNo);

    int transValues = transNo * 4;

    for (int i = 0; i < transValues; ++i) {
        dataFsmFile >> tempInt;
        inputs.push_back(tempInt);
    }

    return true;

}

vector <FiniteStateMachine> FsmHelper::GetAllFsms() {

    FsmHelper fsm;

    while(GetOneFsm()) {
        FiniteStateMachine tempFSM(inputs);
        fsmList.push_back(tempFSM);
        inputs.clear();
    }

    dataFsmFile.close();
    return fsmList;
}


