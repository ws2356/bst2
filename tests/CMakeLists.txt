cmake_minimum_required(VERSION 3.0.0)

add_executable(tests tests.cc)
target_link_libraries(tests bst)

# add_custom_command()
# BUILD_TESTING
add_test(NAME test COMMAND tests)