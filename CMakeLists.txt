cmake_minimum_required(VERSION 3.29)
project(DataStructuresHW1)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(DataStructuresHW1
    AVLTree.cpp
    AvLTree.h
    dspotify25b1.cpp
    dspotify25b1.h
    main25b1.cpp
    PlayList.cpp
    PlayList.h
    song.cpp
    song.h
    wet1util.h)
