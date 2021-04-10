project("${CMAKE_PROJECT_NAME}-test")

file(GLOB_RECURSE S_SRCS ../src/Cloud/*.cpp ../src/Experiment/*.cpp ../src/Instance/*.cpp ../src/Logger/*.cpp ../src/Utility/*.cpp)
file(GLOB_RECURSE S_HEADERS ../src/Cloud/*.hpp ../src/Experiment/*.hpp ../src/Instance/*.hpp ../src/Logger/*.hpp ../src/Utility/*.hpp)
file(GLOB_RECURSE T_SRCS tests/*.cpp)
file(GLOB_RECURSE T_HEADERS tests/*.hpp)

add_executable(${PROJECT_NAME} ${T_SRCS} ${T_HEADERS} ${S_SRCS} ${S_HEADERS})

include(gtest.cmake)

include_directories("${GTEST_SOURCE_DIR}/googletest/include"
                    "${GTEST_SOURCE_DIR}/googlemock/include"
                    "${TEST_DIR}")

target_link_libraries(${PROJECT_NAME}
    libgtest
    libgtest_main
    libgmock
)

add_test(NAME ${PROJECT_NAME}
         COMMAND ${PROJECT_NAME})
