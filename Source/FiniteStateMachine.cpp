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

                if (print == true) {

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

    allUncertainties.push_back(startInputStates);
    currUncertainties.push_back(startInputStates);

    vector<string> tempOutSeq;

    while (!currUncertainties.empty()) {

        for (auto it = currUncertainties.begin(); it != currUncertainties.end(); ) {
            for (int i = 0; i < stateNumber; ++i) {
                tempOutSeq.emplace_back(currOutputSeq[i] + "" );
                cout<< tempOutSeq[i]<<endl;
            }
            cout<<endl;

            produceUncertainty(*it, print, tempOutSeq);

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


void FiniteStateMachine::produceUncertainty(vector<vector<int>> tempInputStates, bool print, vector<string> outputSeqPreceding) {

    allUncertainties.push_back(tempInputStates);

    cout<< "Input State= ";
    if(print){
        for (auto &val: tempInputStates){
            for(auto &innerVal: val){
                cout<< to_string(innerVal) + ", ";
            }
        }
    }
    cout << endl;


    //terminal sartları

    //2)homogenus compenenta sahip
    for (int i = 0; i < stateNumber; ++i) {
        sort(tempInputStates[i].begin(), tempInputStates[i].end());
        for(auto it = tempInputStates[i].begin(); it != tempInputStates[i].end(); it++) {
            if (tempInputStates[i] == tempInputStates[i + 1]) {
                vector<vector<int>> temp;
                return ;
            }
        }
    }
    //3)trivial components
    bool isTrivial=true;
    for (int i = 0; i < stateNumber; ++i) {
        if(tempInputStates.size() != 1) {
            isTrivial = false;
            break;
        }
    }
    if(isTrivial)
        return ;

    for(int input =0 ; input < 2 ; input++) {

        int tempOutputState;
        int tempOutput;
        int flagOutput;

        string outputsFlags[stateNumber];
        for (int l = 0; l < stateNumber; ++l) {
            outputsFlags[l]=-1;
        }

        vector<int> listOfEmptyComponent;

        //generate empty uncertainty vector
        vector<vector<int>> uncertainty;
        vector<int> component;
        for (int i = 0; i < stateNumber; ++i) {
            uncertainty.push_back(component);
            listOfEmptyComponent.push_back(i);
        }

        int stringInput=0;

        for (int i = 0; i < stateNumber; ++i) {
            //tersten dolaşarak, pop_back ile daha rahat eleman silecegim
            for (int j = (int) tempInputStates[i].size() - 1; j >= 0; --j) {

                tie(tempOutputState, tempOutput) = step(tempInputStates[i][j], input, print);


                string temp = "";
                temp.append(outputSeqPreceding[stringInput++ % stateNumber]);
                temp.append(to_string(tempOutput));

                currOutputSeq.push_back(temp);

                if (j == (int) tempInputStates[i].size() -1) {//listede oluşan ilk output, o listede hangi outputların saklanacağını belirler
                    uncertainty[i].push_back(tempOutputState);
                    outputsFlags[i]=temp;
                    //boş component listesinden, bu componentı cıkart
                    listOfEmptyComponent.erase(remove(listOfEmptyComponent.begin(), listOfEmptyComponent.end(), i), listOfEmptyComponent.end());
                } else {
                    int k=0;
                    bool flag=false;
                    for (k = 0; k < stateNumber; ++k) {
                        if(outputsFlags[k].compare(temp) == 0) {
                            flag = true;
                            break;
                        }
                    }
                    if(flag) {
                        uncertainty[k].push_back(tempOutputState);
                    }
                    else{
                        int emptyComp = listOfEmptyComponent.back();
                        uncertainty[emptyComp].push_back(tempOutputState);
                        outputsFlags[emptyComp]=temp;
                        //boş component listesinden, bu componentı cıkart
                        listOfEmptyComponent.erase(remove(listOfEmptyComponent.begin(), listOfEmptyComponent.end(), emptyComp), listOfEmptyComponent.end());

                    }
                }
            }
        }
        currUncertainties.push_back(uncertainty);

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


