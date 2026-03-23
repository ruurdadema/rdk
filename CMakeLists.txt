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

        # source/
        source/string/StringUtilities.cpp

        # lib/
        lib/natsort/strnatcmp.h
        lib/natsort/strnatcmp.c
)

target_include_directories(rdk INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
