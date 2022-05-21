cmake_minimum_required(VERSION 3.22.1)

project(DRG_Prgoject VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH $ENV{QTDIR})

find_package(Qt6 REQUIRED COMPONENTS 
    Core
    Quick
    Quickcontrols2
    Qml
    Sql
)

qt_standard_project_setup()

add_subdirectory(Client)
add_subdirectory(CRUD)
