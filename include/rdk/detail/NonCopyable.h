//
// Created by Ruurd Adema on 21/01/2022.
// Copyright (c) Owllab. All rights reserved.
//

#pragma once

class NonCopyable
{
public:
    NonCopyable (const NonCopyable&) = delete;
    const NonCopyable& operator= (const NonCopyable&) = delete;

protected:
    NonCopyable() = default;
    ~NonCopyable() = default;
};
