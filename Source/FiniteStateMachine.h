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
    int generateCheckingSequenceDist();
    int generateCheckingSequenceChar();
    void generateCharacterizingSequences(bool print);


private:
    int stateNumber;
    vector<Transition> trans;

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

    class CheckingDist{
    public:
        vector<int> sequence;
        vector<int> outputSequences;
        vector<int> outputStateSeq; //always has more than one element than checkingDist sequence. Like pattern in paper.
        vector<bool> isCheckedState;
        vector<bool> isCheckedTrans;
        bool isAllChecked();
        bool isAllTransChecked();
        void print();
        void addDistToChecking(FiniteStateMachine fsm, int& lastState);

    };
    CheckingDist checkingDist;

    class Characterizing{
    public:
        vector<string> sequences;
        vector<vector<string>> outputSequences;
        vector<vector<int>> currUncertainties;
        vector<string> currOutputSeq;
        vector<string> currInputSeq;
        vector<string> allInputSeq;
        //first element is unnecessary, use index + 1
        vector<vector<string>> allOutputStateSeq;
        vector<vector<string>> allOutputSeqTable;

        void printOutputTable(int stateNumber);
        //stop when outputSeq size n-1
        void makeCharSeqCheckTable(int stateNumber, bool print);
        void findCharacterizingSequences(int stateNumber, bool print);
    };
    Characterizing characterizing;

    class CheckingChar{
    public:
        vector<string> sequences;
        vector<string> outputSequences;
    };
    CheckingChar checkingChar;

    int produceUncertaintyDist(vector<vector<int>> pInputStates, bool print, vector<string> precedingOutSeq,
                               vector<string> precedingInpSeq);

    int produceUncertaintyChar(vector<int> pInputStates, bool print, vector<string> precedingOutSeq,
                               vector<string> precedingInpSeq);
    void findInOutStatesDist();

    vector<int> findUncheckedItem(vector<vector<int>> &inputs, vector<int> &tempOutputStates, string item);

    void takeToUncheckedItem(int &lastState, string item);

    int transVerify(int &lastState);


};
