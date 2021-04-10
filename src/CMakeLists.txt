project("${CMAKE_PROJECT_NAME}")

file(GLOB_RECURSE S_SRCS "*.cpp")
file(GLOB_RECURSE S_HEADERS "*.hpp")

add_executable(${PROJECT_NAME} ${S_SRCS} ${S_HEADERS})

add_test(NAME ${PROJECT_NAME}
         COMMAND ${PROJECT_NAME})
