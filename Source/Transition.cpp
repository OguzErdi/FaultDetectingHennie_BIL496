//
// Created by ERDIERKMEN on 28.10.2017.
//

#include "Transition.h"



Transition::Transition(int pInputState, int pOutputState, int pInput, int pOutput) {
    inputState = pInputState;
    outputState = pOutputState;
    input = pInput;
    output = pOutput;
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
