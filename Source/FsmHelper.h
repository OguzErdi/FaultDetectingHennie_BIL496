#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include "FiniteStateMachine.h"


using namespace std;

class FsmHelper
{
public:
	bool static GetOneFsm();
	bool static GetOneTestFsm();
	vector<FiniteStateMachine> static GetAllFsms();
	vector<FiniteStateMachine> static GetAllTestFsms();

    void static makeCheck(int no);



private:
	FsmHelper();
	explicit FsmHelper(string file);
};
static vector<FiniteStateMachine> fsmList;
static vector<FiniteStateMachine> fsmTestList;
static list<int> inputs;
static list<int> inputsTest;
static ifstream dataFsmFile;
static ifstream dataTestFsmFile;
static vector<int>  mutants;


ostream &operator<<(ostream &output, const Transition &transition );

ostream &operator<<(ostream &output, const FiniteStateMachine &fsm);