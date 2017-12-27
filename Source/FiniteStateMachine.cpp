#include "FiniteStateMachine.h"

FiniteStateMachine::FiniteStateMachine(list<int> inputs) {

    auto it = inputs.begin();//iterator for traverse inputs


    stateNumber = *it;
    it++;
    it++;

    for (; it != inputs.end(); it++) {
        int tempInpState = *it;
        it++;
        int tempOutState = *it;
        it++;
        int tempInput = *it;
        it++;
        int tempOutput = *it;

        Transition tempTrans(tempInpState, tempOutState, tempInput, tempOutput);

        trans.push_back(tempTrans);
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


        preOutSeq.clear();
        preInpSeq.clear();
    }

    cout << "Outputs of Distinguish Sequence: ";
    for (auto it = distinguish.currOutputSeq.end() - stateNumber; it != distinguish.currOutputSeq.end(); ++it) {
        //distinguish.outputSequences.push_back(*it);
        if (it == distinguish.currOutputSeq.end() - stateNumber)
            cout << *it << endl;
        else
            cout << "\t\t\t\t\t\t\t\t " + *it << endl;
    }

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


    for (int input = 0; input < 2; input++) {


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
            cout << "it has trivial component. We have found Distinguish Sequence!" << endl;
            cout << "Uncertainty= ";
            for (int i = 0; i < stateNumber; ++i) {
                cout << "(";
                for (auto &val : uncertainty[i]) {
                    cout << val << ", ";
                }
                cout << ")";
            }
            cout << endl;

            distinguish.sequence.append(preInputs);
            return 1;
        }

    }

    return 0;
}

int FiniteStateMachine::generateCheckingSequence() {

    //find distinguish sequence
    generateDistinguishSequence(false);


    //initialize checked state list
    for (int l = 0; l < stateNumber; ++l) {
        checking.isCheckedState.push_back(false);
    }
    //first put distinguish sequence for each state, start from 1(A) state
    //start from state A
    int lastState = 1;
    checking.outputStateSeq.push_back(lastState);
    while (!checking.isAllChecked()) {

        //if dist sequence already have added to check sequence for that state,
        //take the state to another unchecked state
        if (checking.isCheckedState[lastState - 1]) {

            takeToUncheckedItem(lastState, "state");

        }
        //add one dist to checking sequence
        checking.addDistToChecking(*this, lastState);

    }

    checking.print();

    //add one more dist sequence to verify last output state
    checking.addDistToChecking(*this, lastState);

    checking.print();

    //verify every state adding distinguish sequence in the end


    for (int i = 0; i < trans.size(); ++i) {
        checking.isCheckedTrans.push_back(false);
    }

    while (!checking.isAllTransChecked()) {
        transVerify(lastState);
    }



//    for (int i = 0; i < isCheckedTrans.size(); ++i) {
//        checking
//    }


    return 0;
}

void FiniteStateMachine::findInOutStatesDist() {

    for (int inputState = 1; inputState <= stateNumber; ++inputState) {

        int outputState = -1;
        int output;
        string tempOutputSeq;
        string tempOutputStateSeq;
        int tempInputState = inputState;
        for (int distInp = 0; distInp < distinguish.sequence.size(); ++distInp) {

            tie(outputState, output) = step(tempInputState, distinguish.sequence[distInp] - '0', false);
            tempOutputSeq.append(to_string(output));
            tempOutputStateSeq.append(to_string(outputState));
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

    int outputState;
    int output;
    int tempInpState = tempOutputStates[0];
    tempOutputStates.erase(tempOutputStates.begin());
    vector<int> initialTempInp = inputs[0];
    inputs.erase(inputs.begin());
    for (int input = 0; input < 2; input++) {
        vector<int> oneSideInputs = initialTempInp;
        tie(outputState, output) = step(tempInpState, input, false);
        oneSideInputs.push_back(input);
        tempOutputStates.push_back(outputState);
        inputs.push_back(oneSideInputs);

        if (item == "state") {
            if (!checking.isCheckedState[outputState - 1]) {
                return oneSideInputs;
            }
        } else if (item == "trans") {
            for (int i = 0; i < checking.isCheckedTrans.size(); ++i) {
                if (checking.isCheckedTrans[i] == false && trans[i].getInputState() == outputState) {
                    return oneSideInputs;
                }
            }
        }
    }

    return findUncheckedItem(inputs, tempOutputStates, item);
}

void FiniteStateMachine::takeToUncheckedItem(int &lastState, string item) {

    vector<vector<int>> inputs;
    vector<int> temp;
    inputs.push_back(temp);

    vector<int> tempOutputStates;
    vector<int> tempOutputs;
    tempOutputStates.push_back(lastState);

    vector<int> toUncheckStateInputs = findUncheckedItem(inputs, tempOutputStates, item);

    for (int k = 0; k < toUncheckStateInputs.size(); ++k) {
        checking.sequence.push_back(toUncheckStateInputs[k]);
        int outputState;
        int output;
        tie(outputState, output) = step(lastState, toUncheckStateInputs[k], false);
        checking.outputStateSeq.push_back(outputState);
        checking.outputSequences.push_back(output);
        lastState = outputState;
    }

}

int FiniteStateMachine::transVerify(int &lastState) {

    int outputState;
    int output;
    int currentCheckTrans;
    bool currFlag = false;
    for (currentCheckTrans = 0; currentCheckTrans < trans.size(); ++currentCheckTrans) {
        if (trans[currentCheckTrans].getInputState() == lastState &&
            checking.isCheckedTrans[currentCheckTrans] == false) {
            checking.isCheckedTrans[currentCheckTrans] = true;
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
        checking.sequence.push_back(trans[currentCheckTrans].getInput());
        tie(outputState, output) = step(lastState, trans[currentCheckTrans].getInput(), false);
        checking.outputStateSeq.push_back(outputState);
        checking.outputSequences.push_back(output);
        lastState = outputState;
    }

    //add one dist to checking sequence
    checking.addDistToChecking(*this, lastState);

    checking.print();

    //update sequences
    for (int j = 0; j < checking.sequence.size(); ++j) {

//        if(checking.sequence[j] == trans[currentCheckTrans].getInput() &&
//           checking.outputStateSeq[j] == trans[currentCheckTrans].getInputState() &&
//           checking.outputStateSeq[j+1] == 0) {
//
//            checking.outputStateSeq[j + 1] = outputState;
//        }
        //look checkedTrans and put in to the sequence again
        if (checking.outputStateSeq[j] != 0 && checking.outputStateSeq[j + 1] == 0) {
            for (int i = 0; i < checking.isCheckedTrans.size(); ++i) {

                if (checking.isCheckedTrans[i] == true &&
                    checking.sequence[j] == trans[i].getInput() &&
                    checking.outputStateSeq[j] == trans[i].getInputState()) {

                    checking.outputStateSeq[j + 1] = trans[i].getOutputState();
                }
            }
        }
    }
    checking.print();

    //update isCheckedTrans
    for (int iSeq = 0; iSeq < checking.sequence.size(); ++iSeq) {
        if (checking.outputStateSeq[iSeq] != 0 && checking.outputStateSeq[iSeq + 1] != 0) {

            for (int jTrans = 0; jTrans < trans.size(); ++jTrans) {
                if (trans[jTrans].getInputState() == checking.outputStateSeq[iSeq] &&
                    trans[jTrans].getInput() == checking.sequence[iSeq] &&
                    checking.isCheckedTrans[jTrans] == false) {
                    checking.isCheckedTrans[jTrans] = true;
                    break;
                }
            }
        }
    }


}

void FiniteStateMachine::Checking::addDistToChecking(FiniteStateMachine fsm, int &lastState) {

    //add dist seq, output states and outputs to check seq for state A
    for (int i = 0; i < fsm.distinguish.sequence.size(); ++i) {
        //add dist seq
        sequence.push_back(fsm.distinguish.sequence[i] - '0');

    }
    //add output states
    for (int j = 0; j < fsm.distinguish.outputStateSeq[lastState - 1].size(); ++j) {
        if (j == fsm.distinguish.outputStateSeq[lastState - 1].size() - 1) //not the end one
            //add output states
            outputStateSeq.push_back(fsm.distinguish.outputStateSeq[lastState - 1][j] - '0');
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

void FiniteStateMachine::Checking::print() {

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

bool FiniteStateMachine::Checking::isAllChecked() {
    bool temp = true;
    for (int i = 0; i < isCheckedState.size(); ++i) {
        temp = temp && isCheckedState[i];
    }
    return temp;
}

bool FiniteStateMachine::Checking::isAllTransChecked() {
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
        for (int i = 0; i < stateNumber; ++i)
            preOutSeq.emplace_back(characterizing.currOutputSeq[i] + "");

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


    for (int input = 0; input < 2; input++) {
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
        for (int i = 0; i < stateNumber; ++i)
            characterizing.currOutputSeq.push_back(outputSeqs[i]);

        preInputs.append(precedingInpSeq[0]);
        preInputs.append(to_string(input));
        characterizing.currInputSeq.push_back(preInputs);
    }
    return 0;
}

void FiniteStateMachine::findInOutStatesChar() {
    for (int inputState = 1; inputState <= stateNumber; ++inputState) {

        int outputState = -1;
        int output;
        string tempOutputSeq;
        string tempOutputStateSeq;
        int tempInputState = inputState;
        for (int distInp = 0; distInp < distinguish.sequence.size(); ++distInp) {

            tie(outputState, output) = step(tempInputState, distinguish.sequence[distInp] - '0', false);
            tempOutputSeq.append(to_string(output));
            tempOutputStateSeq.append(to_string(outputState));
            tempInputState = outputState;
        }
        characterizing.outputSequences.push_back(tempOutputSeq);
        characterizing.outputStateSeq.push_back(tempOutputStateSeq);
        characterizing.initialStates.append(to_string(inputState));
        characterizing.outputStates.append(to_string(outputState));

    }

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
        //print table that checking char seq
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

    if(print) {
        cout<< "Characterizing Sequences"<<endl;
        for (int m = 0; m < sequences.size(); ++m) {
            cout << sequences[m] << endl;
        }
    }

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
}
