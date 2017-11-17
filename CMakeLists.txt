cmake_minimum_required(VERSION 3.8)
project(HennieFsmTesting)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES Source/main.cpp
        Source/FsmHelper.cpp Source/FsmHelper.h Source/FiniteStateMachine.cpp Source/FiniteStateMachine.h Source/Transition.cpp Source/Transition.h Source/Uncertain.cpp Source/Uncertain.h)
add_executable(HennieFsmTesting ${SOURCE_FILES})