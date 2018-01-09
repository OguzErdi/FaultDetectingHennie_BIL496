#include <iostream>
#include "FsmHelper.h"

using namespace std;


int main(int argc, char* argv[])
{
    vector<FiniteStateMachine> fsmList;
    fsmList = FsmHelper::GetAllFsms();

    vector<FiniteStateMachine> fsmTestList;
    fsmTestList = FsmHelper::GetAllTestFsms();



    FsmHelper::makeCheck(10000);


}
