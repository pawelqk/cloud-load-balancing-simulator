project("${CMAKE_PROJECT_NAME}")

file(GLOB S_SRCS "*.cpp")
file(GLOB S_HEADERS "*.hpp")

add_executable(${PROJECT_NAME} ${S_SRCS} ${S_HEADERS})

include_directories(
    "${SRC_DIR}"
)

add_test(NAME ${PROJECT_NAME}
         COMMAND ${PROJECT_NAME})
