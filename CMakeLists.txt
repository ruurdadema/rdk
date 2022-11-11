cmake_minimum_required(VERSION 3.15)

project(rdk)

set(CMAKE_CXX_STANDARD 17)

if (NOT CMAKE_BUILD_TYPE)
    # CMAKE_BUILD_TYPE has to be specified for Conan to produce valid output (ie. CONAN_INCLUDE_DIRS)
    set(CMAKE_BUILD_TYPE Debug)
    message(WARNING "CMAKE_BUILD_TYPE not specified, set it to 'Debug'")
endif ()

# RDK Library

add_library(rdk)
target_sources(rdk PUBLIC
        # include/
        include/rdk/stl/Generic.h
        include/rdk/stl/Vector.h
        include/rdk/string/StringUtilities.h
        include/rdk/support/Support.h
        include/rdk/util/ICallbackProvider.h
        include/rdk/util/AsyncExecutor.h
        include/rdk/util/Callback.h
        include/rdk/util/Subscription.h
        include/rdk/util/Result.h
        include/rdk/util/SharedSubscriberList.h
        include/rdk/util/CountedGuard.h
        include/rdk/util/TaskScheduler.h
        include/rdk/util/ScopedRollback.h
        include/rdk/util/Leak.h
        include/rdk/detail/NonCopyable.h
        include/rdk/events/ValuePublisher.h

        # source/
        source/string/StringUtilities.cpp

        # lib/
        lib/natsort/strnatcmp.h
        lib/natsort/strnatcmp.c
        )

target_include_directories(rdk PUBLIC include)

# Unit tests

add_executable(RdkTests
        source/test/CatchMain.cpp
        source/string/StringUtilities.test.cpp
        source/util/ScopedAtomicCounter.test.cpp
        source/util/CountedGuard.test.cpp
        )
target_include_directories(RdkTests PRIVATE lib)
target_compile_definitions(RdkTests PUBLIC CATCH_CONFIG_NO_CPP17_UNCAUGHT_EXCEPTIONS=1)

target_link_libraries(RdkTests rdk)
