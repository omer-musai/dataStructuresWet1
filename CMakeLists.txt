cmake_minimum_required(VERSION 3.16)
project(AVLTree)

set(CMAKE_CXX_STANDARD 11)

add_executable(AVLTree AVLTree.cpp AVLTree.hpp gameSystem.cpp gameSystem.hpp game_exceptions.hpp Group.hpp Node.hpp Player.hpp playerById.hpp)