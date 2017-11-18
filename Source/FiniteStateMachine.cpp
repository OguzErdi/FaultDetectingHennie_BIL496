#include "FiniteStateMachine.h"
FiniteStateMachine::FiniteStateMachine(list<int> inputs) {

    auto it = inputs.begin();//iterator for traverse inputs


    it++;
    stateNumber = *it;
    it++;
    transitionNumber = *it;

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
int FiniteStateMachine::distinguishSequence(bool print) {

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

    currUncertainties.push_back(startInputStates);

    vector<string> tempOutSeq;

    while (!currUncertainties.empty()) {

        for (auto it = currUncertainties.begin(); it != currUncertainties.end(); ) {
            for (int i = 0; i < stateNumber; ++i) {
                tempOutSeq.emplace_back(currOutputSeq[i] + "" );
                //cout<< tempOutSeq[i]<<endl;
            }
            cout<<endl;

            produceUncertainty(*it, print, tempOutSeq);

            if(print){
                cout<<"-------------------------------------"<<endl;
                cout<<"press any key to continue"<<endl;
                getchar();
            }


            //bu islem zaten iteratoru ilerletecek
            it = currUncertainties.erase(currUncertainties.begin());
            for (int i = 1; i <= stateNumber; ++i) {
                currOutputSeq.erase(currOutputSeq.begin());

            }
            tempOutSeq.clear();
        }


    }

    for (auto it = currOutputSeq.begin(); it != currOutputSeq.end(); it++) {
        cout<<*it + "" <<endl;
    }


}


void FiniteStateMachine::produceUncertainty(vector<vector<int>> pInputStates, bool print, vector<string> preceedingOutSeq) {

    if (print) {
        cout<< "Input State= ";
        for (int i = 0; i < stateNumber; ++i) {
            cout << "(";
            for (auto &val : pInputStates[i]) {
                cout << val << ", ";
            }
            cout << ")";
        }
        cout << endl;
    }


    //terminal sartları

    //2)homogenus compenenta sahip
    for (int i = 0; i < stateNumber; ++i) {
        for(auto it = pInputStates[i].begin(); it != pInputStates[i].end(); it++) {
            for (auto  itIn = it+1; itIn != pInputStates[i].end() ; itIn++) {
                if (itIn == it) {
                    cout << "it has homogenus component"<<endl;
                    return ;
                }
            }
        }
    }
    //3)trivial components
    bool isTrivial=true;
    for (int i = 0; i < stateNumber; ++i) {
        if(pInputStates[i].size() != 1) {
            isTrivial = false;
            break;
        }
    }
    if(isTrivial)
        return ;

    for(int input =0 ; input < 2 ; input++) {

        int outputState;
        int output;

        string outputsFlags[stateNumber];
        for (int l = 0; l < stateNumber; ++l) {
            outputsFlags[l]="empty";
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

        int stringInput=0;

        for (int i = 0; i < stateNumber; ++i) {
            for (int j = 0; j < (int) pInputStates[i].size(); ++j) {

                int inputState = pInputStates[i][j];
                tie(outputState, output) = step(inputState, input, false);

                string resultOutSeq;
                resultOutSeq.append(preceedingOutSeq[stringInput++ % stateNumber]);
                resultOutSeq.append(to_string(output));

                int emptyComponentNo = -1;
                int sameComponentNo=0;
                bool isSameComponent=false;
                for (int l = 0; l < stateNumber; ++l) {
                    if(outputsFlags[l] == resultOutSeq) {
                        isSameComponent = true;
                        sameComponentNo = l;
                    }
                    else if(outputsFlags[l] == "empty" && emptyComponentNo == -1)
                        emptyComponentNo = l;
                }

                if(isSameComponent) {
                    uncertainty[sameComponentNo].push_back(outputState);
                    outputSeqs[sameComponentNo].push_back(resultOutSeq);
                }
                else{
                    outputsFlags[emptyComponentNo]=resultOutSeq;
                    uncertainty[emptyComponentNo].push_back(outputState);
                    outputSeqs[emptyComponentNo].push_back(resultOutSeq);
                }
            }
        }
        currUncertainties.push_back(uncertainty);

        //outputlar tek diziye sokluyor tekrar
        for (int i = 0; i < stateNumber; ++i) {
            for (int j = 0; j < (int) outputSeqs[i].size(); ++j) {
                currOutputSeq.push_back( outputSeqs[i][j]);
            }
        }

        cout<< "(" << input << ") - Output State= ";
        if (print) {
            for (int i = 0; i < stateNumber; ++i) {
                cout << "(";
                for (auto &val : uncertainty[i]) {
                    cout << val << ", ";
                }
                cout << ")";
            }
            cout << endl;
        }
    }
    return ;

}


