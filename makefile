cmake_minimum_required(VERSION 3.16)
project(INTERFACEQT VERSION 1.0.0 LANGUAGES CXX)

# Configuration C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Recherche de Qt
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Activation de MOC pour Qt
set(CMAKE_AUTOMOC ON)

# Fichiers sources
set(SOURCES
    main.cpp
)

# Création de l'exécutable
add_executable(INTERFACEQT ${SOURCES})

# Liaison avec Qt
target_link_libraries(INTERFACEQT 
    Qt6::Core
    Qt6::Widgets
)

