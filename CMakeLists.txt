cmake_minimum_required(VERSION 3.13)
project(implementation_netsim)

add_compile_options(-Wall -Wextra -Werror -Wpedantic -pedantic-errors -Wconversion)

set(CMAKE_CXX_STANDARD 17)

set(PROJECT_ID netsim)

#include_directories(
#        include
#)

#set(SOURCE_FILES
#
#       )

#add_executable(netsim ${SOURCE_FILES} main.cpp)

if(EXISTS ${PROJECT_SOURCE_DIR}/test)
    set(EXEC_TEST ${PROJECT_ID}__test)
    add_executable(${EXEC_TEST} ${SOURCE_FILES} ${SOURCES_FILES_TESTS} test/main_gtest.cpp)

    # == Uwzględnij pliki frameworku Google Testing Framework (GTF) ==

    # Przyjmij, że główny katalog z plikami frameworku GTF znajduje się
    # dwa katalogi wyżej względem katalogu projektu.
    set(GTEST_ROOT googletest-master)

    # Dodaj katalogi z plikami nagłówkowymi GTF.
    target_include_directories(${EXEC_TEST} PUBLIC
            ${GTEST_ROOT}/googlemock/include
            ${GTEST_ROOT}/googletest/include
            )

    add_subdirectory(${GTEST_ROOT} googletest-master)

    # Dołącz bibliotekę Google Mock.
    target_link_libraries(${EXEC_TEST} gmock)
endif()