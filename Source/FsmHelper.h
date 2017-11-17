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
        vector<FiniteStateMachine> static GetAllFsms();


private:
    FsmHelper();
    explicit FsmHelper(string file);
};
static vector<FiniteStateMachine> fsmList;
static list<int> inputs;
static ifstream dataFsmFile;

ostream &operator<<( ostream &output, const Transition &transition );

ostream &operator<<(ostream &output, const FiniteStateMachine &fsm);