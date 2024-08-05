//
// Created by Ruurd Adema on 16/03/2023.
// Copyright (c) Owllab. All rights reserved.
//

#pragma once

#include <atomic>

#include "rdk/detail/NonCopyable.h"
#include "rdk/detail/NonMoveable.h"

namespace rdk {

/**
 * Class which represents a process-wide unique id for a class.
 * @tparam ClassType The class type.
 * @tparam IdType The counter type.
 */
template<class ClassType, class IdType = int>
class ClassId {
  public:
    ClassId() : id_(counter_.fetch_add(1) + 1) {}

    RDK_DECLARE_NON_COPYABLE(ClassId)
    RDK_DECLARE_NON_MOVEABLE(ClassId)

    bool operator==(const ClassId& other) const {
        return id_ == other.id_;
    }

    bool operator!=(const ClassId& other) const {
        return id_ != other.id_;
    }

    /**
     * Get the id of this class instance.
     * @return The id of this class.
     */
    IdType get_id() const {
        return id_;
    }

    /**
     * Get the current counter value.
     * @return The current counter value.
     */
    static IdType get_counter() {
        return counter_;
    }

  private:
    /// The id of this class instance.
    IdType id_ {0};

    /// The counter is static, so it will be shared between all instances of this class.
    inline static std::atomic<IdType> counter_ {0};
};

}  // namespace rdk
