//
// Created by ERDIERKMEN on 28.10.2017.
//
#include <iostream>

using namespace std;

class Transition {
public:
    Transition(int _inputState, int _outputState, int _input, int _output);
    friend ostream &operator<<( ostream &output, const Transition &transition );
    int getInputState() const;
    void setInputState(int inputState);
    int getOutputState() const;
    void setOutputState(int outputState);
    int getInput() const;
    void setInput(int input);
    int getOutput() const;
    void setOutput(int output);

private:
    int inputState;
    int outputState;
    int input;
    int output;

};
