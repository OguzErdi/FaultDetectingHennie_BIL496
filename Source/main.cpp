#include <iostream>
#include "FsmHelper.h"

using namespace std;


int main(int argc, char* argv[])
{
    vector<FiniteStateMachine> fsmList;
    fsmList = FsmHelper::GetAllFsms();

    fsmList[0].distinguishSequence(true);

}
