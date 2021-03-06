#include "FiniteStateMachine.h"

FiniteStateMachine::FiniteStateMachine(list<int> inputs) {

    auto it = inputs.begin();//iterator for traverse inputs
    if(*it != 0) {//if it is not idle

        fsmNo = *it;
        it++;
        stateNumber = *it;
        it++;
        it++;

        for (; it != inputs.end(); it++) {
            int tempInpState = *it;
            it++;
            int tempOutState = *it;
            it++;
            int tempInput = *it;
            inputNo = tempInput+1;
            it++;
            int tempOutput = *it;

            Transition tempTrans(tempInpState, tempOutState, tempInput, tempOutput);

            trans.push_back(tempTrans);
        }

    }
}

const vector<Transition> &FiniteStateMachine::getTrans() const {
    return trans;
}

tuple<int, int> FiniteStateMachine::step(int inpState, int input, bool print) {


    for (auto transIt = trans.begin(); transIt != trans.end(); transIt++) {
        if (transIt->getInputState() == inpState) {
            if (transIt->getInput() == input) {

                if (print) {

                    cout << "InputState\t| Input\t|OutputState\t| Output" << endl;
                    cout << "\t" << inpState
                         << "\t\t| " << input
                         << "\t\t| \t\t" << transIt->getOutputState()
                         << "\t\t| \t" << transIt->getOutput() << endl
                         << "--------------------------------------------" << endl;
                }
                return make_tuple(transIt->getOutputState(), transIt->getOutput());
            }
        }

    }
    return make_tuple(-1, -1);
}

void FiniteStateMachine::generateDistinguishSequence(bool print) {

    vector<vector<int>> startInputStates;

    for (int inputState = 1; inputState <= stateNumber; ++inputState) {
        vector<int> component;
        if (inputState == 1) {
            for (int i = 1; i <= stateNumber; ++i) {
                component.push_back(i);
                distinguish.currOutputSeq.emplace_back("");
            }
        }
        startInputStates.push_back(component);
    }


    distinguish.currInputSeq.emplace_back("");

    distinguish.currUncertainties.push_back(startInputStates);

    vector<string> preOutSeq;
    vector<string> preInpSeq;

    int hasDist;

    for (auto it = distinguish.currUncertainties.begin(); it != distinguish.currUncertainties.end();) {
        for (int i = 0; i < stateNumber; ++i) {
            preOutSeq.emplace_back(distinguish.currOutputSeq[i] + "");
        }

        preInpSeq.emplace_back(distinguish.currInputSeq[0] + "");


        hasDist = produceUncertaintyDist(*it, print, preOutSeq, preInpSeq);

        if (hasDist)
            break;

        if (print) {
            cout << "press any key to continue" << endl;
            getchar();
        }

        //bu islem zaten iteratoru ilerletecek
        it = distinguish.currUncertainties.erase(distinguish.currUncertainties.begin());

        for (int i = 1; i <= stateNumber; ++i) {
            distinguish.currOutputSeq.erase(distinguish.currOutputSeq.begin());
        }

        distinguish.currInputSeq.erase(distinguish.currInputSeq.begin());

        //if size n+1 stop
        if(distinguish.currInputSeq.back().size() == 10 )
            return;

        preOutSeq.clear();
        preInpSeq.clear();

    }

//    cout << "Outputs of Distinguish Sequence: ";
//    for (auto it = distinguish.currOutputSeq.end() - stateNumber; it != distinguish.currOutputSeq.end(); ++it) {
//        //distinguish.outputSequences.push_back(*it);
//        if (it == distinguish.currOutputSeq.end() - stateNumber)
//            cout << *it << endl;
//        else
//            cout << "\t\t\t\t\t\t\t\t " + *it << endl;
//    }

    findInOutStatesDist();
    distinguish.print();
}


int FiniteStateMachine::produceUncertaintyDist(vector<vector<int>> pInputStates, bool print, vector<string> precedingOutSeq,
                                               vector<string> precedingInpSeq) {

    if (print) {
        cout << "Input State= ";
        for (int i = 0; i < stateNumber; ++i) {
            cout << "(";
            for (auto &val : pInputStates[i]) {
                cout << val << ", ";
            }
            cout << ")";
        }
        cout << endl;
    }


    for (int input = 0; input < inputNo; input++) {


        string outputsFlags[stateNumber];
        for (int l = 0; l < stateNumber; ++l) {
            outputsFlags[l] = "empty";
        }


        //generate empty uncertainty vector
        vector<vector<int>> uncertainty;
        vector<int> component;
        for (int i = 0; i < stateNumber; ++i) {
            uncertainty.push_back(component);
        }

        //outputların, outputStateler ile aynı hizada olması icin aynı yapıda store edilmeli
        //generate empty output sequences
        vector<vector<string>> outputSeqs;
        vector<string> compOutputSeq;
        for (int i = 0; i < stateNumber; ++i) {
            outputSeqs.push_back(compOutputSeq);
        }
        //generate empty input sequences
        string preInputs;

        int stringInput = 0;

        for (int i = 0; i < stateNumber; ++i) {
            for (int j = 0; j < (int) pInputStates[i].size(); ++j) {

                int output;
                int outputState;
                int inputState = pInputStates[i][j];
                tie(outputState, output) = step(inputState, input, false);

                string resultOutSeq;
                resultOutSeq.append(precedingOutSeq[stringInput++ % stateNumber]);
                resultOutSeq.append(to_string(output));


                int emptyComponentNo = -1;
                int sameComponentNo = 0;
                bool isSameComponent = false;
                for (int l = 0; l < stateNumber; ++l) {
                    if (outputsFlags[l] == resultOutSeq) {
                        isSameComponent = true;
                        sameComponentNo = l;
                    } else if (outputsFlags[l] == "empty" && emptyComponentNo == -1)
                        emptyComponentNo = l;
                }

                if (isSameComponent) {
                    uncertainty[sameComponentNo].push_back(outputState);
                    outputSeqs[sameComponentNo].push_back(resultOutSeq);
                } else {
                    outputsFlags[emptyComponentNo] = resultOutSeq;
                    uncertainty[emptyComponentNo].push_back(outputState);
                    outputSeqs[emptyComponentNo].push_back(resultOutSeq);
                }
            }
        }


        if (print) {
            cout << "(" << input << ") - Output State= ";
            for (int i = 0; i < stateNumber; ++i) {
                cout << "(";
                for (auto &val : uncertainty[i]) {
                    cout << val << ", ";
                }
                cout << ")";
            }
            cout << endl;
        }

        //terminal sartları
        //2)homogeneous componenta sahip
        for (int i = 0; i < stateNumber; ++i) {
            for (auto it = uncertainty[i].begin(); it != uncertainty[i].end(); it++) {
                for (auto itIn = it + 1; itIn != uncertainty[i].end(); itIn++) {
                    if (itIn == it) {
                        cout << "it has homogeneous component" << endl;
                        return 0;
                    }
                }
            }
        }


        distinguish.currUncertainties.push_back(uncertainty);
        //outputlar tek diziye sokluyor tekrar
        for (int i = 0; i < stateNumber; ++i) {
            for (int j = 0; j < (int) outputSeqs[i].size(); ++j) {
                distinguish.currOutputSeq.push_back(outputSeqs[i][j]);

            }
        }

        preInputs.append(precedingInpSeq[0]);
        preInputs.append(to_string(input));
        distinguish.currInputSeq.push_back(preInputs);


        //3)trivial components
        bool isTrivial = true;
        for (int i = 0; i < stateNumber; ++i) {
            if (uncertainty[i].size() != 1) {
                isTrivial = false;
                break;
            }
        }
        if (isTrivial) {
            cout << "Distinguish Sequence" << endl;
//            cout << "Uncertainty= ";
//            for (int i = 0; i < stateNumber; ++i) {
//                cout << "(";
//                for (auto &val : uncertainty[i]) {
//                    cout << val << ", ";
//                }
//                cout << ")";
//            }
            cout << endl;

            distinguish.sequence.append(preInputs);
            return 1;
        }

    }

    return 0;
}

int FiniteStateMachine::generateCheckingSequenceDist() {

    //find distinguish sequence
    generateDistinguishSequence(false);

    if(!distinguish.sequence.empty()) {

        //initialize checked state list
        for (int l = 0; l < stateNumber; ++l) {
            checkingDist.isCheckedState.push_back(false);
        }
        //first put distinguish sequence for each state, start from 1(A) state
        //start from state A
        int lastState = 1;
        checkingDist.outputStateSeq.push_back(lastState);
        int i=0;
        while (!checkingDist.isAllChecked()) {

//            cout<<i<<endl;
//            i++;
//            if(i == 38)
//                cout<<"aa"<<endl;

            //if dist sequence already have added to check sequence for that state,
            //take the state to another unchecked state
            if (checkingDist.isCheckedState[lastState - 1]) {

                if ( takeToUncheckedItem(lastState, "state") == -1)
                    return -1;

            }
            //add one dist to checkingDist sequence
            checkingDist.addDistToChecking(*this, lastState);

        }

        //checkingDist.print();

        //add one more dist sequence to verify last output state
        checkingDist.addDistToChecking(*this, lastState);

        //checkingDist.print();

        //verify every state adding distinguish sequence in the end


        for (int i = 0; i < trans.size(); ++i) {
            checkingDist.isCheckedTrans.push_back(false);
        }

        while (!checkingDist.isAllTransChecked()) {
            transVerify(lastState);
        }


        cout<<endl;
        cout<<"Checking Sequence for Distinguish Sequence"<<endl<<endl;
        cout<<"inputs : \t";
        for (int i = 0; i < checkingDist.sequence.size(); ++i) {
            cout << checkingDist.sequence[i];
        }
        cout << endl;
        cout<<"outputs: \t";
        for (int i = 0; i < checkingDist.sequence.size(); ++i) {
            cout << checkingDist.outputSequences[i];
        }
        cout<<endl;
        cout<<endl;
    }

    return 0;
}

void FiniteStateMachine::findInOutStatesDist() {

    for (int inputState = 1; inputState <= stateNumber; ++inputState) {

        int outputState = -1;
        int output;
        string tempOutputSeq;
        vector<int> tempOutputStateSeq;
        int tempInputState = inputState;
        for (int distInp = 0; distInp < distinguish.sequence.size(); ++distInp) {

            tie(outputState, output) = step(tempInputState, distinguish.sequence[distInp] - '0', false);
            tempOutputSeq.append(to_string(output));
            tempOutputStateSeq.push_back(outputState);
            tempInputState = outputState;
        }
        distinguish.outputSequences.push_back(tempOutputSeq);
        distinguish.outputStateSeq.push_back(tempOutputStateSeq);
        distinguish.initialStates.append(to_string(inputState));
        distinguish.outputStates.append(to_string(outputState));

    }

}

vector<int> FiniteStateMachine::findUncheckedItem(vector<vector<int>> &inputs, vector<int> &tempOutputStates,
                                                  string item) {

    do {
        int outputState;
        int output;
        int tempInpState = tempOutputStates[0];
        tempOutputStates.erase(tempOutputStates.begin());
        vector<int> initialTempInp = inputs[0];
        inputs.erase(inputs.begin());
        for (int input = 0; input < inputNo; input++) {
            vector<int> oneSideInputs = initialTempInp;
            tie(outputState, output) = step(tempInpState, input, false);
            oneSideInputs.push_back(input);
            tempOutputStates.push_back(outputState);
            inputs.push_back(oneSideInputs);

//            cout<<inputs.size()<<endl;
//            cout<<"girdi: "<<endl;
//            for (int j = 0; j < inputs.back().size(); ++j) {
//                cout<<inputs.back()[j];
//            }
            cout<<endl;
            if (item == "state") {
                if (!checkingDist.isCheckedState[outputState - 1]) {
                    return oneSideInputs;
                }
            } else if (item == "trans") {
                for (int i = 0; i < checkingDist.isCheckedTrans.size(); ++i) {
                    if (checkingDist.isCheckedTrans[i] == false && trans[i].getInputState() == outputState) {
                        return oneSideInputs;
                    }
                }
            }
        }
    }while(true);
}

int FiniteStateMachine::takeToUncheckedItem(int &lastState, string item) {

    vector<vector<int>> inputs;
    vector<int> temp;
    inputs.push_back(temp);

    vector<int> tempOutputStates;
    vector<int> tempOutputs;
    tempOutputStates.push_back(lastState);

    vector<int> toUncheckStateInputs = findUncheckedItem(inputs, tempOutputStates, item);
    if(toUncheckStateInputs[0] == -1)
        return -1;

    for (int k = 0; k < toUncheckStateInputs.size(); ++k) {
        checkingDist.sequence.push_back(toUncheckStateInputs[k]);
        int outputState;
        int output;
        tie(outputState, output) = step(lastState, toUncheckStateInputs[k], false);
        checkingDist.outputStateSeq.push_back(outputState);
        checkingDist.outputSequences.push_back(output);
        lastState = outputState;
    }

    return 0;
}

int FiniteStateMachine::transVerify(int &lastState) {

    int outputState;
    int output;
    int currentCheckTrans;
    bool currFlag = false;
    for (currentCheckTrans = 0; currentCheckTrans < trans.size(); ++currentCheckTrans) {
        if (trans[currentCheckTrans].getInputState() == lastState &&
            checkingDist.isCheckedTrans[currentCheckTrans] == false) {
            checkingDist.isCheckedTrans[currentCheckTrans] = true;
            currFlag = true;
            break;
        }
    }
    //find another unchecked trans
    if (!currFlag) {
        takeToUncheckedItem(lastState, "trans");
//        transVerify(lastState);
        return 0;
    } else {
        checkingDist.sequence.push_back(trans[currentCheckTrans].getInput());
        tie(outputState, output) = step(lastState, trans[currentCheckTrans].getInput(), false);
        checkingDist.outputStateSeq.push_back(outputState);
        checkingDist.outputSequences.push_back(output);
        lastState = outputState;
    }

    //add one dist to checkingDist sequence
    checkingDist.addDistToChecking(*this, lastState);

//    checkingDist.print();

    //update sequences
    for (int j = 0; j < checkingDist.sequence.size(); ++j) {

//        if(checkingDist.sequence[j] == trans[currentCheckTrans].getInput() &&
//           checkingDist.outputStateSeq[j] == trans[currentCheckTrans].getInputState() &&
//           checkingDist.outputStateSeq[j+1] == 0) {
//
//            checkingDist.outputStateSeq[j + 1] = outputState;
//        }
        //look checkedTrans and put in to the sequence again
        if (checkingDist.outputStateSeq[j] != 0 && checkingDist.outputStateSeq[j + 1] == 0) {
            for (int i = 0; i < checkingDist.isCheckedTrans.size(); ++i) {

                if (checkingDist.isCheckedTrans[i] == true &&
                    checkingDist.sequence[j] == trans[i].getInput() &&
                    checkingDist.outputStateSeq[j] == trans[i].getInputState()) {

                    checkingDist.outputStateSeq[j + 1] = trans[i].getOutputState();
                }
            }
        }
    }
//    checkingDist.print();

    //update isCheckedTrans
    for (int iSeq = 0; iSeq < checkingDist.sequence.size(); ++iSeq) {
        if (checkingDist.outputStateSeq[iSeq] != 0 && checkingDist.outputStateSeq[iSeq + 1] != 0) {

            for (int jTrans = 0; jTrans < trans.size(); ++jTrans) {
                if (trans[jTrans].getInputState() == checkingDist.outputStateSeq[iSeq] &&
                    trans[jTrans].getInput() == checkingDist.sequence[iSeq] &&
                    checkingDist.isCheckedTrans[jTrans] == false) {
                    checkingDist.isCheckedTrans[jTrans] = true;
                    break;
                }
            }
        }
    }


}

void FiniteStateMachine::CheckingDist::addDistToChecking(FiniteStateMachine fsm, int &lastState) {

    //add dist seq, output states and outputs to check seq for state A
    for (int i = 0; i < fsm.distinguish.sequence.size(); ++i) {
        //add dist seq
        sequence.push_back(fsm.distinguish.sequence[i] - '0');

    }
    //add output states
    for (int j = 0; j < fsm.distinguish.outputStateSeq[lastState - 1].size(); ++j) {
        if (j == fsm.distinguish.outputStateSeq[lastState - 1].size() - 1) //if the last value of outputs
            //add output states
            outputStateSeq.push_back(fsm.distinguish.outputStateSeq[lastState - 1][j]);
        else
            outputStateSeq.push_back(0);
        //add outputs
        outputSequences.push_back(fsm.distinguish.outputSequences[lastState - 1][j] - '0');
    }

    //mark the current state
    isCheckedState[lastState - 1] = true;

    //take next state
    lastState = outputStateSeq.back();

}


void FiniteStateMachine::Distinguish::print() {
    cout << "Initial States:   " << initialStates << endl;
    cout << "\t\t\t\t\t|\n\t\t\t\t\t|\n";
    cout << "Distinguish Seq:   " << sequence << endl;
    cout << "\t\t\t\t\t|\n\t\t\t\t\t|\n\t\t\t\t\tv\n";
    cout << "Output States:    " << outputStates << endl;
    cout << "Output Seq:      --------\n";
    for (int i = 0; i < sequence.size(); ++i) {

        cout << "\t\t\t\t  ";
        for (int j = 0; j < outputSequences.size(); ++j) {
            cout << outputSequences[j][i];

        }
        cout << endl;
    }

    cout << "Output State Seq: --------\n";
    for (int i = 0; i < sequence.size(); ++i) {

        cout << "\t\t\t\t  ";
        for (int j = 0; j < outputStateSeq.size(); ++j) {
            cout << outputStateSeq[j][i];

        }
        cout << endl;
    }


}

void FiniteStateMachine::CheckingDist::print() {

    cout << "--------------------------------------" << endl;
    cout << " ";
    for (int i = 0; i < sequence.size(); ++i) {
        cout << sequence[i] << " ";
    }
    cout << endl;
    for (int i = 0; i < outputStateSeq.size(); ++i) {
        switch (outputStateSeq[i]) {
            case 0:
                cout << "  ";
                break;
            case 1:
                cout << "A ";
                break;
            case 2:
                cout << "B ";
                break;
            case 3:
                cout << "C ";
                break;
            case 4:
                cout << "D ";
                break;
        }
//        cout << outputStateSeq[i] << " ";
    }
    cout << endl;
    cout << " ";
    for (int i = 0; i < outputSequences.size(); ++i) {
        cout << outputSequences[i] << " ";
    }
    cout << endl;
    cout << "--------------------------------------" << endl;


}

bool FiniteStateMachine::CheckingDist::isAllChecked() {
    bool temp = true;
    for (int i = 0; i < isCheckedState.size(); ++i) {
        temp = temp && isCheckedState[i];
    }
    return temp;
}

bool FiniteStateMachine::CheckingDist::isAllTransChecked() {
    bool temp = true;
    for (int i = 0; i < isCheckedTrans.size(); ++i) {
        temp = temp && isCheckedTrans[i];
    }
    return temp;
}

void FiniteStateMachine::generateCharacterizingSequences(bool print) {

    vector<int> startInputStates;

    for (int inputState = 1; inputState <= stateNumber; ++inputState) {
        startInputStates.push_back(inputState);
        characterizing.currOutputSeq.emplace_back("");
    }


    characterizing.currInputSeq.emplace_back("");

    characterizing.currUncertainties.push_back(startInputStates);

    vector<string> preOutSeq;
    vector<string> preInpSeq;

    for (auto it = characterizing.currUncertainties.begin(); it != characterizing.currUncertainties.end();) {
        for (int i = 0; i < stateNumber; ++i) {
            preOutSeq.emplace_back(characterizing.currOutputSeq[i] + "");
        }
        preInpSeq.emplace_back(characterizing.currInputSeq[0] + "");


        produceUncertaintyChar(*it, print, preOutSeq, preInpSeq);

        if (print) {
            cout << "press any key to continue" << endl;
            getchar();
        }

        //bu islem zaten iteratoru ilerletecek
        it = characterizing.currUncertainties.erase(characterizing.currUncertainties.begin());
        for (int i = 1; i <= stateNumber; ++i) {
            characterizing.currOutputSeq.erase(characterizing.currOutputSeq.begin());
        }

        characterizing.currInputSeq.erase(characterizing.currInputSeq.begin());

        //stop condition, when input seq is n-1(n = stateNumber)
        if(!characterizing.currInputSeq.empty()) {
            auto inputSeqSize = (int) characterizing.currInputSeq.front().size();
            if (inputSeqSize == stateNumber)
                break;
        }
        characterizing.makeCharSeqCheckTable(stateNumber, print);

        preOutSeq.clear();
        preInpSeq.clear();
    }

    if(print)
        characterizing.printOutputTable(stateNumber);


    characterizing.findCharacterizingSequences(stateNumber, print);
}

int
FiniteStateMachine::produceUncertaintyChar(vector<int> pInputStates, bool print, vector<string> precedingOutSeq,
                                           vector<string> precedingInpSeq) {
    if (print) {
        cout << "Input State= ";
        cout << "(";
        for (auto &val : pInputStates) {
            cout << val << ", ";
        }
        cout << ")";
        cout << endl;
    }

    vector<int> outputStateSeq;
    for (int i = 0; i < stateNumber; ++i) {
        outputStateSeq.push_back(pInputStates[i]);
    }
    characterizing.allOutputStateSeq.push_back(outputStateSeq);


    for (int input = 0; input < inputNo; input++) {
        vector<int> uncertainty;

        //outputların, outputStateler ile aynı hizada olması icin aynı yapıda store edilmeli
        //generate empty output sequences
        vector<string> outputSeqs;
        //generate empty input sequences
        string preInputs;

        int stringInput = 0;

        for (int i = 0; i < pInputStates.size(); ++i) {

            int output;
            int outputState;
            int inputState = pInputStates[i];
            tie(outputState, output) = step(inputState, input, false);

            string resultOutSeq;
            resultOutSeq.append(precedingOutSeq[stringInput++ % stateNumber]);
            resultOutSeq.append(to_string(output));

            uncertainty.push_back(outputState);
            outputSeqs.push_back(resultOutSeq);
        }



        if (print) {
            cout << "(" << input << ") - Output State= ";
            cout << "(";
            for (auto &val : uncertainty) {
                cout << val << ", ";
            }
            cout << ")";
            cout << endl;
        }

        characterizing.currUncertainties.push_back(uncertainty);
        //outputlar tek diziye sokluyor tekrar
        for (int i = 0; i < stateNumber; ++i) {
            characterizing.currOutputSeq.push_back(outputSeqs[i]);
        }

        preInputs.append(precedingInpSeq[0]);
        preInputs.append(to_string(input));
        characterizing.currInputSeq.push_back(preInputs);
    }
    return 0;
}

void FiniteStateMachine::Characterizing::makeCharSeqCheckTable(int stateNumber, bool print) {

    //create table to check is there any char seq
    allInputSeq.push_back(currInputSeq[0]);
    vector<string> tempOutputSeq;
    for (int i = 0; i < stateNumber; ++i) {
        tempOutputSeq.push_back(currOutputSeq[i]);
    }

    allOutputSeqTable.push_back(tempOutputSeq);

    if(print) {
        //print table that checkingDist char seq
        cout << allInputSeq.back() << endl;
        cout << "-------" << endl;
        for (int j = 0; j < allOutputSeqTable.back().size(); ++j) {
            cout << allOutputSeqTable.back()[j] << endl;
        }
        cout << endl;
    }
}

void FiniteStateMachine::Characterizing::findCharacterizingSequences(int stateNumber, bool print) {

    vector<vector<int>> willSingleton;
    bool isSplitted;

    vector<int> Bi;
    for (int j = 1; j <= stateNumber; ++j) {
        Bi.push_back(j);
    }
    willSingleton.push_back(Bi);

    do {
        //try to reduce size to 1(refer to one unique state)for each element of willSingleton list
        isSplitted = false;
        vector<int> processing(willSingleton[0]);
        willSingleton.erase(willSingleton.begin());
        //component have just one state, dont process this component
        if (processing.size() != 1 && !isSplitted ) {
            //Compare first element and rest of the vector
            for (int indexInputSeq = 0; indexInputSeq < allInputSeq.size() && !isSplitted ; ++indexInputSeq) {
                //travers all input seq to find which is procudes different output
                for (int i = 1; i < processing.size() && !isSplitted; ++i) {
                    //[index of output seq produce by input seq][index of state]
                    string currentOutput = allOutputSeqTable[indexInputSeq][processing[0] - 1];
                    string comparedOutput = allOutputSeqTable[indexInputSeq][processing[i] - 1];
                    //we find the characterizing sequence
                    if (currentOutput != comparedOutput) {
                        //add to W set
                        sequences.push_back(allInputSeq[indexInputSeq]);
                        bool flagSp = false;
                        //and split the component by output results
                        vector<int> sameOutputComp;
                        for (int k = 0; k < processing.size(); k++) {
                            string currentOutputSp = allOutputSeqTable[indexInputSeq][processing[k] - 1];
                            for (int l = 0; l < processing.size(); ++l) {
                                string comparedOutputSp = allOutputSeqTable[indexInputSeq][processing[l] - 1];
                                if (currentOutputSp == comparedOutputSp) {
                                    sameOutputComp.push_back(processing[l]);
                                    k++;
                                    flagSp = true;
                                }
                            }
                            if(flagSp)
                                k--;
                            flagSp = false;
                            willSingleton.push_back(sameOutputComp);
                            sameOutputComp.clear();
                        }
                        isSplitted = true;
                        break;
                    }
                }
            }
        }
        if(!isSplitted)
            willSingleton.push_back(processing);

    } while (willSingleton.size() != stateNumber);


    //delete all duplicates
    sort( sequences.begin(), sequences.end() );
    sequences.erase( unique( sequences.begin(), sequences.end() ), sequences.end() );

    //generate all output seq for char seq
    for (int n = 0; n < sequences.size(); ++n) {
        for (int i = 0; i < allInputSeq.size(); ++i) {
            if(sequences[n] == allInputSeq[i] ){
               outputSequences.push_back(allOutputSeqTable[i]);
            }
        }
    }


    cout<< "Characterizing Sequences"<<endl;

    cout<<"     ";
    for (int m = 0; m < sequences.size(); ++m) {
        for (int i = 0; i < allInputSeq.size(); ++i) {
            if (sequences[m] == allInputSeq[i])
                cout << allInputSeq[i] + "\t";
        }
    }
    cout<<endl;
    cout<<"     ";
    for (int i = 0; i < sequences.size(); ++i) {
        cout << "---";
    }
    cout << "" << endl;
    for (int y = 0; y < stateNumber; ++y) {
        switch (y+1) {
            case 1:
                cout << "A    ";
                break;
            case 2:
                cout << "B    ";
                break;
            case 3:
                cout << "C    ";
                break;
            case 4:
                cout << "D    ";
                break;
        }
        for (int m = 0; m < sequences.size(); ++m) {
            for (int i = 0; i < allInputSeq.size(); ++i) {
                if (sequences[m] == allInputSeq[i]) {

                    cout << allOutputSeqTable[i][y] + "\t";

                }
            }
        }

        cout << endl;
    }
    cout<< endl;
}


int FiniteStateMachine::generateCheckingSequenceChar() {

    generateCharacterizingSequences(false);

    vector<string> pathForState;
    vector<string> pathForStateOutputSeq;


    string initialState = "1";


    //generate seq that reach from state A to desired state
    for (int state = 1; state <= stateNumber; ++state) {
        if(state == 1 ){
            pathForState.push_back("");
            pathForStateOutputSeq.push_back("");
        }
        else{
            for (int i = 0; i < characterizing.allInputSeq.size(); ++i) {
                if(state == characterizing.allOutputStateSeq[i+1][0]) {
                    pathForState.push_back(characterizing.allInputSeq[i]);
                    pathForStateOutputSeq.push_back(characterizing.allOutputSeqTable[i][0]);
                    break;
                }
            }
        }

    }



    //check for states
    for (int state = 1; state <= stateNumber; ++state) {
        for (int charSeq = 0; charSeq < characterizing.sequences.size() ; ++charSeq) {
            string oneCheckingSeq;
            string oneCheckingOutputSeq;

            oneCheckingSeq.append(pathForState[state-1] + characterizing.sequences[charSeq]);
            oneCheckingOutputSeq.append(pathForStateOutputSeq[state-1] + characterizing.outputSequences[charSeq][state-1]);

            checkingChar.sequences.push_back(oneCheckingSeq);
            checkingChar.outputSequences.push_back(oneCheckingOutputSeq);
        }
    }

    //check for transition
    for (int j = 0; j < trans.size(); ++j) {
        for (int charSeq = 0; charSeq < characterizing.sequences.size() ; ++charSeq) {

            string oneCheckingSeq;
            string oneCheckingOutputSeq;
            int state = trans[j].getInputState();

            oneCheckingSeq.append(pathForState[state-1] + to_string(trans[j].getInput()) + characterizing.sequences[charSeq]);
            oneCheckingOutputSeq.append(pathForStateOutputSeq[state-1] + to_string(trans[j].getOutput()) + characterizing.outputSequences[charSeq][trans[j].getOutputState()-1]);

            checkingChar.sequences.push_back(oneCheckingSeq);
            checkingChar.outputSequences.push_back(oneCheckingOutputSeq);
        }
    }

    cout<<"Checking Sequences for Characteristic Sequences"<<endl;
    for (int k = 0; k < checkingChar.sequences.size(); ++k) {
        cout<<"inputs: "<<"\t"<< checkingChar.sequences[k]<<endl;
        cout<<"outputs:"<<"\t"<< checkingChar.outputSequences[k]<<endl;
        cout<<endl;
    }
    cout<<endl;


    return 0;
}

int FiniteStateMachine::generateCheckingSequence() {


    int x = generateCheckingSequenceDist();

    if(checkingDist.sequence.empty() || x != 0){
        generateCheckingSequenceChar();
    }

    return 0;
}

void FiniteStateMachine::Characterizing::printOutputTable(int stateNumber) {

    cout<<"Output table"<<endl;
    for (int i = 0; i < allInputSeq.size(); ++i) {
        cout << allInputSeq[i] + "\t";
    }
    cout << "" << endl;
    for (int i = 0; i < allInputSeq.size(); ++i) {
        cout << "---";
    }
    cout << "" << endl;
    for (int i = 0; i < stateNumber; ++i) {
        for (int m = 0; m < allOutputSeqTable.size(); ++m) {
            cout <<allOutputSeqTable[m][i]+"\t";
        }
        cout<<endl;
    }
    cout << "" << endl;
    for (int i = 0; i < stateNumber; ++i) {
        for (int m = 1; m < allOutputStateSeq.size(); ++m) {
//            cout <<allOutputStateSeq[m][i]+"\t";

            switch (allOutputStateSeq[m][i]) {
                case 0:
                    cout << "  ";
                    break;
                case 1:
                    cout << "A ";
                    break;
                case 2:
                    cout << "B ";
                    break;
                case 3:
                    cout << "C ";
                    break;
                case 4:
                    cout << "D ";
                    break;
            }
            cout<<"\t";
        }
        cout<<endl;
    }
}
