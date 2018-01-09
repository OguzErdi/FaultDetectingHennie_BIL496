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
        cout << "fsm.txt file is opened\n";
    else
        cout << "fsm.txt file doesn't open\n";



    dataTestFsmFile.open("../FsmData/fsm_test.txt");
    if(dataTestFsmFile.is_open())
        cout << "fsm_test.txt file is opened\n";
    else
        cout << "fsm_test.txt file doesn't open\n";

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
    inputs.push_back(tempInt);
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
bool FsmHelper::GetOneTestFsm()
{
    char tempC, inputSignal;
    int tempInt;
    int transNo;

    dataTestFsmFile >> tempC;
    if(dataTestFsmFile.eof())
        return false;
    dataTestFsmFile >> tempC;
    dataTestFsmFile >> tempC;
    dataTestFsmFile >> tempInt;
    inputsTest.push_back(tempInt);
    dataTestFsmFile >> tempInt;
    inputsTest.push_back(tempInt);
    dataTestFsmFile >> transNo;
    inputsTest.push_back(transNo);

    int transValues = transNo * 4;

    for (int i = 0; i < transNo; ++i) {
        dataTestFsmFile >> tempInt;
        inputsTest.push_back(tempInt);
        dataTestFsmFile >> tempInt;
        inputsTest.push_back(tempInt);
        dataTestFsmFile >> inputSignal;

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

        inputsTest.push_back(tempInt);

        dataTestFsmFile >> tempInt;
        inputsTest.push_back(tempInt);
    }


    return true;

}

vector <FiniteStateMachine> FsmHelper::GetAllTestFsms() {

    list<int> idle(0);

    //add one idle fsm for make index and FSM number equal
    idle.push_back(0);
    FiniteStateMachine idleFSM(idle);
    fsmTestList.push_back(idleFSM);

    while(GetOneTestFsm()) {
        FiniteStateMachine tempFSM(inputsTest);
        fsmTestList.push_back(tempFSM);
        inputsTest.clear();
    }

    dataTestFsmFile.close();
    return fsmTestList;
}

void FsmHelper::makeCheck(int no) {

    bool flag=false;
    for (int i = 1; i <= no; ++i) {

        cout<<"FSM "<<i<< endl;
        fsmList[i].generateCheckingSequence();

        string checkingSequence;
        string checkingOutputSequences;

        if(!fsmList[i].checkingDist.sequence.empty()) {
            for (int j = 0; j < fsmList[i].checkingDist.sequence.size(); ++j) {
                checkingSequence.append(to_string(fsmList[i].checkingDist.sequence[j]));
            }
            for (int j = 0; j < fsmList[i].checkingDist.outputSequences.size(); ++j) {
                checkingOutputSequences.append(to_string(fsmList[i].checkingDist.outputSequences[j]));
            }


            string testOutputSeq;
            int inputState=1;
            FiniteStateMachine test = fsmTestList[i];
            for (int i = 0; i < checkingSequence.size(); ++i) {

                int output;
                int outputState;
                tie(outputState, output) = test.step(inputState, checkingSequence[i] - '0', false);

                testOutputSeq.append(to_string(output));
                inputState = outputState;
            }

            if(checkingOutputSequences != testOutputSeq ){
                flag = true;
            }
        }
        else{
            for (int j = 0; j < fsmList[i].checkingChar.sequences.size(); ++j) {
                checkingSequence = fsmList[i].checkingChar.sequences[j];
                checkingOutputSequences = fsmList[i].checkingChar.outputSequences[j];

                string testOutputSeq;
                int inputState=1;
                FiniteStateMachine test = fsmTestList[i];
                for (int i = 0; i < checkingSequence.size(); ++i) {

                    int output;
                    int outputState;
                    tie(outputState, output) = test.step(inputState, checkingSequence[i] - '0', false);

                    testOutputSeq.append(to_string(output));
                    inputState = outputState;
                }

                if(checkingOutputSequences != testOutputSeq ){
                    flag = true;
                    break;
                }
            }
        }

        if(flag)
        {
            mutants.push_back(fsmList[i].fsmNo);
            flag = false;
        }
    }


    for (int k = 0; k < mutants.size(); ++k) {
        cout<<mutants[k]<<endl;
    }
}


