//
// Created by Ruurd Adema on 15/02/2024.
// Copyright (c) 2024 $ORGANIZATION_NAME. All rights reserved.
//

#pragma once

/**
 * Shortcut for declaring a class non-moveable.
 */
#define RDK_DECLARE_NON_MOVEABLE(className) \
    className(className&&) = delete; \
    className& operator=(className&&) = delete;

namespace rdk {

/**
 * Class which prevents subclasses from being moved.
 * Inherit from this class to make a class non-moveable.
 */
class NonMoveable {
  public:
    NonMoveable(NonMoveable&&) noexcept = delete;
    const NonMoveable& operator=(NonMoveable&&) noexcept = delete;

  protected:
    NonMoveable() = default;
    ~NonMoveable() = default;
};

}  // namespace rdk
