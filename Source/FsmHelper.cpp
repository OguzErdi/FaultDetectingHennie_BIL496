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
    char tempC, inputSignal;
    int tempInt;
    int transNo;

    dataFsmFile >> tempC;
    if(dataFsmFile.eof())
        return false;
    dataFsmFile >> tempC;
    dataFsmFile >> tempC;
    dataFsmFile >> tempInt;
    dataFsmFile >> tempInt;
    inputs.push_back(tempInt);
    dataFsmFile >> transNo;
    inputs.push_back(transNo);

    int transValues = transNo * 4;

    for (int i = 0; i < transNo; ++i) {
        dataFsmFile >> tempInt;
        inputs.push_back(tempInt);
        dataFsmFile >> tempInt;
        inputs.push_back(tempInt);
        dataFsmFile >> inputSignal;

        //return the input signal from alphabet to number
        if(inputSignal == 'a')
            tempInt= 0;
        else if(inputSignal == 'b')
            tempInt= 1;
        else if(inputSignal == 'c')
            tempInt= 2;
        else if(inputSignal == 'd')
            tempInt= 3;
        else if(inputSignal == 'e')
            tempInt= 4;

        inputs.push_back(tempInt);

        dataFsmFile >> tempInt;
        inputs.push_back(tempInt);
    }


    return true;

}

vector <FiniteStateMachine> FsmHelper::GetAllFsms() {

    FsmHelper fsm;
    list<int> idle(0);

    //add one idle fsm for make index and FSM number equal
    idle.push_back(0);
    FiniteStateMachine idleFSM(idle);
    fsmList.push_back(idleFSM);

    while(GetOneFsm()) {
        FiniteStateMachine tempFSM(inputs);
        fsmList.push_back(tempFSM);
        inputs.clear();
    }

    dataFsmFile.close();
    return fsmList;
}


