cmake_minimum_required(VERSION 3.15)

project(rdk)

set(CMAKE_CXX_STANDARD 17)

# RDK Library

add_library(rdk INTERFACE)
target_sources(rdk INTERFACE
        # include/
        include/rdk/string/StringUtilities.h
        include/rdk/support/Support.h
        include/rdk/util/Subscription.h
        include/rdk/util/Result.h
        include/rdk/util/SubscriberList.h
        include/rdk/util/ScopedRollback.h
        include/rdk/util/Leak.h
        include/rdk/detail/NonCopyable.h
        include/rdk/detail/NonMoveable.h

        # source/
        source/string/StringUtilities.cpp

        # lib/
        lib/natsort/strnatcmp.h
        lib/natsort/strnatcmp.c
)

target_include_directories(rdk INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Unit tests

option(RDK_WITH_UNIT_TESTS "Enable RDK unit tests" ON)

if (RDK_WITH_UNIT_TESTS)
    file(GLOB_RECURSE TEST_SOURCES source/*.test.cpp)

    add_executable(RdkTests
            source/test/CatchMain.cpp
            ${TEST_SOURCES}
    )
    target_include_directories(RdkTests PRIVATE lib)
    target_compile_definitions(RdkTests PUBLIC CATCH_CONFIG_NO_CPP17_UNCAUGHT_EXCEPTIONS=1)
    target_link_libraries(RdkTests PUBLIC rdk)
endif ()
