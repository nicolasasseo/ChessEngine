
cmake_minimum_required(VERSION 3.15...3.31)
set(CMAKE_CXX_STANDARD 20)        # Change 11 to 14, 17, or 20 if needed
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(ChessAI
  VERSION 1.0
  LANGUAGES CXX)
# Add the executable target
add_executable(chessbin
    main.cpp
    initialize.cpp
    piece.cpp
    print.cpp
    moves.cpp
    eval.cpp
    eval_helper.cpp
    tree.cpp
    legal_moves.cpp
)
# Include directories for header files
target_include_directories(chessbin PRIVATE .)