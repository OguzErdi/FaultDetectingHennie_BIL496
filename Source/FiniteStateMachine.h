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
    int generateCheckingSequence(bool print);


private:
    class Distinguish{
    public:
        string sequence;
        vector<string> outputSequences;
        vector<string> outputStateSeq;
        string initialStates;
        string outputStates;
        void print();

    };
    Distinguish distinguish;
    class Checking{
    public:
        vector<int> sequence;
        vector<int> outputSequences;
        vector<int> outputStateSeq; //always has more than one element than checking sequence. Like pattern in paper.
        vector<bool> isCheckedState;
        bool isAllChecked();
        void print();

    };

    Checking checking;


    int stateNumber;
    vector<Transition> trans;
    vector<vector<vector<int>>> currUncertainties;
    vector<string> currOutputSeq;
    vector<string> currInputSeq;

    int produceUncertainty(vector<vector<int>> pInputStates,
                           bool print,
                           vector<string> precedingOutSeq,
                           vector<string> precedingInpSeq);
    void findInOutStates();
};
