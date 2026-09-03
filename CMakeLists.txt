cmake_minimum_required(VERSION 3.16)
project(AGI LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Crow CONFIG REQUIRED)
find_package(RapidJSON CONFIG REQUIRED)

add_library(agi_core
    src/Emotion.cpp
    src/EmotionData.cpp
)
target_include_directories(agi_core PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(agi_core SYSTEM PRIVATE ${RAPIDJSON_INCLUDE_DIRS})

add_executable(AGI src/AGI.cpp)
target_link_libraries(AGI PRIVATE agi_core Crow::Crow)
target_compile_definitions(AGI PRIVATE AGI_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

include(CTest)
if(BUILD_TESTING)
    add_executable(agi_tests tests/EmotionTests.cpp)
    target_link_libraries(agi_tests PRIVATE agi_core)
    target_compile_definitions(agi_tests PRIVATE AGI_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
    add_test(NAME agi_tests COMMAND agi_tests)
endif()
