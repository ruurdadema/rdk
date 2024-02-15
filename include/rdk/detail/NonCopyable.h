//
// Created by Ruurd Adema on 21/01/2022.
// Copyright (c) Owllab. All rights reserved.
//

#pragma once

/**
 * Shortcut for declaring a class non copyable.
 */
#define RDK_DECLARE_NON_COPYABLE(className)                                                                            \
    className (const className&) = delete;                                                                             \
    className& operator= (const className&) = delete;

namespace rdk
{

class NonCopyable
{
public:
    NonCopyable (const NonCopyable&) = delete;
    const NonCopyable& operator= (const NonCopyable&) = delete;

protected:
    NonCopyable() = default;
    ~NonCopyable() = default;
};

} // namespace rdk
