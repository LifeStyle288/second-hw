cmake_minimum_required(VERSION 3.2)

if (DEFINED ENV{GITHUB_RUN_NUMBER})
    project(allocator VERSION 0.0.$ENV{GITHUB_RUN_NUMBER})
    # find_package(Boost COMPONENTS unit_test_framework REQUIRED)
else ()
    project(allocator)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

add_executable(allocator_cli main.cpp)
add_library(factorial factorial.cpp)

target_link_libraries(allocator_cli PRIVATE
    factorial
)

target_include_directories(factorial
    PRIVATE "${CMAKE_BINARY_DIR}"
)

if (MSVC)
    target_compile_options(allocator_cli PRIVATE
        /W4
    )
    target_compile_options(factorial PRIVATE
        /W4
    )
else ()
    target_compile_options(allocator_cli PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(factorial PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()



install(TARGETS allocator_cli RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)
