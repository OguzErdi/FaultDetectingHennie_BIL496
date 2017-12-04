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
                currOutputSeq.emplace_back("");
            }
        }
        startInputStates.push_back(component);
    }


    currInputSeq.emplace_back("");

    currUncertainties.push_back(startInputStates);

    vector<string> preOutSeq;
    vector<string> preInpSeq;

    int hasDist;

    for (auto it = currUncertainties.begin(); it != currUncertainties.end();) {
        for (int i = 0; i < stateNumber; ++i) {
            preOutSeq.emplace_back(currOutputSeq[i] + "");
        }

        preInpSeq.emplace_back(currInputSeq[0] + "");


        hasDist = produceUncertainty(*it, print, preOutSeq, preInpSeq);

        if (hasDist)
            break;

        if (print) {
            cout << "press any key to continue" << endl;
            getchar();
        }

        //bu islem zaten iteratoru ilerletecek
        it = currUncertainties.erase(currUncertainties.begin());

        for (int i = 1; i <= stateNumber; ++i) {
            currOutputSeq.erase(currOutputSeq.begin());
        }

        currInputSeq.erase(currInputSeq.begin());


        preOutSeq.clear();
        preInpSeq.clear();
    }

    cout << "Outputs of Distinguish Sequence: ";
    for (auto it = currOutputSeq.end() - stateNumber; it != currOutputSeq.end(); ++it) {
        //distinguish.outputSequences.push_back(*it);
        if (it == currOutputSeq.end() - stateNumber)
            cout << *it << endl;
        else
            cout << "\t\t\t\t\t\t\t\t " + *it << endl;
    }

    findInOutStates();
    distinguish.print();
    generateCheckingSequence();
}


int FiniteStateMachine::produceUncertainty(vector<vector<int>> pInputStates,
                                           bool print,
                                           vector<string> precedingOutSeq,
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


        currUncertainties.push_back(uncertainty);
        //outputlar tek diziye sokluyor tekrar
        for (int i = 0; i < stateNumber; ++i) {
            for (int j = 0; j < (int) outputSeqs[i].size(); ++j) {
                currOutputSeq.push_back(outputSeqs[i][j]);

            }
        }

        preInputs.append(precedingInpSeq[0]);
        preInputs.append(to_string(input));
        currInputSeq.push_back(preInputs);


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


void FiniteStateMachine::findInOutStates() {

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

int FiniteStateMachine::generateCheckingSequence() {

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
            vector<int> toUncheckStateInputs;

            findUncheckedState(lastState, toUncheckStateInputs);

            for (int k = 0; k < toUncheckStateInputs.size(); ++k) {
                checking.sequence.push_back(toUncheckStateInputs[k]);
                int outputState;
                int output;
                tie(outputState, output) = step(lastState, toUncheckStateInputs[k], false);
                checking.outputStateSeq.push_back(outputState);
                checking.outputSequences.push_back(output);
            }
            //take next state
            lastState = checking.outputStateSeq.back();
        }
        //add one dist to checking sequence
        checking.addDistToChecking(*this, lastState);

    }

    checking.print();

    //add one more dist sequence to verify last output state
    checking.addDistToChecking(*this, lastState);

    checking.print();


    vector<Transition> uncheckedTrans;
    for (int j = 0; j < trans.size(); j++) {

        bool isUnchecked = true;
        for (int i = 0; i < distinguish.sequence.size(); i++) {

            //inputState ve inputu kontrol et
            if (trans[j].getInputState() == distinguish.initialStates[i] - '0' &&
                trans[j].getInput() == distinguish.sequence[i] - '0') {
                isUnchecked = false;
                break;
            }
        }
        if (isUnchecked)
            uncheckedTrans.push_back(trans[j]);

    }

    for (int i = 0; i < uncheckedTrans.size(); i++) {
        cout << uncheckedTrans[i] << endl;
    }

    return 0;
}


int FiniteStateMachine::findUncheckedState(int lastState, vector<int> &inputs) {
    vector<int> tempOutputs;
    vector<int> tempOutputStates;

    int outputState;
    int output;
    for (int input = 0; input < 2; input++) {
        tie(outputState, output) = step(lastState, input, false);
        tempOutputs.push_back(output);
        tempOutputStates.push_back(outputState);
        if (!checking.isCheckedState[outputState - 1]) {
            inputs.push_back(input);
            return true;
        }
    }
    for (int i = 0; i < tempOutputStates.size(); ++i) {
        inputs.push_back(tempOutputs[i]);
        if (!findUncheckedState(tempOutputStates[i], inputs))
            inputs.pop_back();
    }

    return 0;
}

void FiniteStateMachine::Checking::addDistToChecking(FiniteStateMachine fsm, int& lastState) {

    //add dist seq, output states and outputs to check seq for state A
    for (int i = 0; i < fsm.distinguish.sequence.size(); ++i) {
        //add dist seq
        sequence.push_back(fsm.distinguish.sequence[i] - '0');

    }
    //add output states
    for (int j = 0; j < fsm.distinguish.outputStateSeq[lastState-1].size(); ++j) {
        //add output states
        outputStateSeq.push_back(fsm.distinguish.outputStateSeq[lastState - 1][j] - '0');
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

    cout<<" ";
    for (int i = 0; i < sequence.size(); ++i) {
        cout<<sequence[i] << " ";
    }
    cout<<endl;
    for (int i = 0; i < outputStateSeq.size(); ++i) {
        cout<<outputStateSeq[i] << " ";
    }
    cout<<endl;
    cout<<" ";
    for (int i = 0; i < outputSequences.size(); ++i) {
        cout<<outputSequences[i] << " ";
    }
    cout<<endl;



}

bool FiniteStateMachine::Checking::isAllChecked() {
    bool temp = true;
    for (int i = 0; i < isCheckedState.size(); ++i) {
        temp = temp && isCheckedState[i];
    }
    return temp;
}

