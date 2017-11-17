//
// Created by ERDIERKMEN on 28.10.2017.
//

#include "Transition.h"



Transition::Transition(int _inputState, int _outputState, int _input, int _output) {
    inputState = _inputState;
    outputState = _outputState;
    input = _input;
    output = _output;
}


int Transition::getInputState() const {
    return inputState;
}

void Transition::setInputState(int inputState) {
    Transition::inputState = inputState;
}

int Transition::getOutputState() const {
    return outputState;
}

void Transition::setOutputState(int outpurState) {
    Transition::outputState = outpurState;
}

int Transition::getInput() const {
    return input;
}

void Transition::setInput(int input) {
    Transition::input = input;
}

int Transition::getOutput() const {
    return output;
}

void Transition::setOutput(int output) {
    Transition::output = output;
}
