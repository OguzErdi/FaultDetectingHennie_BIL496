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
    tuple<int, int> step(int inpState, int input, bool print);
    int distinguishSequence(bool print);
    int produceUncertainty(vector<vector<int>> pInputStates, bool print, vector<string> precedingOutSeq);


private:
    int stateNumber;
    int transitionNumber;
    vector<Transition> trans;

    vector<vector<vector<int>>> currUncertainties;
    vector<string> currOutputSeq;
    vector<string> currInputSeq;

};
