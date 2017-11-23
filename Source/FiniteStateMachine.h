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
    int distinguishSequence(bool print);


private:
    int stateNumber;
    int produceUncertainty(vector<vector<int>> pInputStates,
                           bool print,
                           vector<string> precedingOutSeq,
                           vector<string> precedingInpSeq);
    void findInOutStates();

private:
    vector<Transition> trans;

    vector<vector<vector<int>>> currUncertainties;
    vector<string> currOutputSeq;
    vector<string> currInputSeq;

    class Distinguish{
    public:
        string sequence;
        vector<string> outputSequences;
        string initialStates;
        string outputStates;
        void print();

    };

    Distinguish distinguish;
};
