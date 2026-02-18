# Find Google Test
find_package(GTest)

if(NOT GTest_FOUND)
    message(STATUS "Google Test not found. Fetching from GitHub...")
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()

# Helper function to add tests
function(add_jsrl_test test_name)
    add_executable(${test_name} ${test_name}.cpp)
    target_link_libraries(${test_name}
        PRIVATE
            jsrl::jsrl
            GTest::gtest
            GTest::gtest_main
    )
    target_include_directories(${test_name}
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}
    )

    # Add the test to CTest
    add_test(NAME ${test_name} COMMAND ${test_name})

    # Set C++ standard for tests
    set_target_properties(${test_name} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
    )
endfunction()

# Add tests
add_jsrl_test(jsrl_general_number_test)
add_jsrl_test(jsrl_mod_test)
add_jsrl_test(jsrl_test)
add_jsrl_test(jsrlpp_test)

# Add format test only if C++20 or later is available
if(CMAKE_CXX_STANDARD GREATER_EQUAL 20)
    add_jsrl_test(jsrl_format_test)
    set_target_properties(jsrl_format_test PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
    )
endif()
