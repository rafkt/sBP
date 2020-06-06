cmake_minimum_required(VERSION 3.16)
project(simple_bs_predictor)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(~/include)


add_executable(simple_bs_predictor
        backwardsSearchBWT.cpp
        backwardsSearchBWT.h
        main.cpp
        predictor.cpp
        predictor.h
        subseqPredictor.cpp
        subseqPredictor.h
        suffixArray.cpp
        suffixArray.h)
