cmake_minimum_required(VERSION 3.16)
project(AVLTree)

set(CMAKE_CXX_STANDARD 11)

add_executable(AVLTree main1.cpp library1.h library1.cpp AVLTree.hpp GameSystem.cpp GameSystem.hpp game_exceptions.hpp Group.hpp Node.hpp Player.hpp PlayerById.hpp)