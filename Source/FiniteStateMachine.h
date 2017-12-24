//
// Created by ERDIERKMEN on 24.10.2017.
//
#include <iostream>
#include <list>
#include <array>
#include <vector>
#include <queue>
#include <tuple>
#include "Transition.h"
#include <algorithm>

using namespace std;

class FiniteStateMachine {

public:
    explicit FiniteStateMachine(list<int> inputs);
    const vector<Transition> &getTrans() const;
    tuple<int, int> step(int inpState,
                         int input,
                         bool print);
    void generateDistinguishSequence(bool print);
    int generateCheckingSequence();
    void generateCharacterizingSequences(bool print);


private:
    class Distinguish{
    public:
        string sequence;
        vector<string> outputSequences;
        vector<string> outputStateSeq;
        string initialStates;
        string outputStates;
        vector<vector<vector<int>>> currUncertainties;
        vector<string> currOutputSeq;
        vector<string> currInputSeq;
        void print();

    };
    Distinguish distinguish;

    class Checking{
    public:
        vector<int> sequence;
        vector<int> outputSequences;
        vector<int> outputStateSeq; //always has more than one element than checking sequence. Like pattern in paper.
        vector<bool> isCheckedState;
        vector<bool> isCheckedTrans;
        bool isAllChecked();
        bool isAllTransChecked();
        void print();
        void addDistToChecking(FiniteStateMachine fsm, int& lastState);

    };
    Checking checking;

    class Characterizing{
    public:
        vector<string> sequence;
        vector<string> outputSequences;
        vector<string> outputStateSeq;
        string initialStates;
        string outputStates;
        vector<vector<vector<int>>> currUncertainties;
        vector<string> currOutputSeq;
        vector<string> currInputSeq;
        vector<string> allInputSeq;
        vector<vector<string>> alloutputSeqTable;

        void print();

    };
    Characterizing characterizing;

    int stateNumber;
    vector<Transition> trans;

    int produceUncertaintyDist(vector<vector<int>> pInputStates, bool print, vector<string> precedingOutSeq,
                               vector<string> precedingInpSeq);

    int produceUncertaintyChar(vector<vector<int>> pInputStates, bool print, vector<string> precedingOutSeq,
                               vector<string> precedingInpSeq);
    void findInOutStatesDist();
    void findInOutStatesChar();

    vector<int> findUncheckedItem(vector<vector<int>> &inputs, vector<int> &tempOutputStates, string item);

    void takeToUncheckedItem(int &lastState, string item);

    int transVerify(int &lastState);

};
