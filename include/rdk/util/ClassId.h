//
// Created by Ruurd Adema on 16/03/2023.
// Copyright (c) Owllab. All rights reserved.
//

#pragma once

#include "rdk/support/Support.h"

namespace rdk
{

/**
 * Class which represents a process-wide unique id for a class.
 * @tparam ClassType The class type.
 * @tparam IdType The counter type.
 */
template <class ClassType, class IdType = int>
class ClassId
{
public:
    ClassId() : mId (sCounter.fetch_add (1) + 1) {}

    RDK_DECLARE_NON_COPYABLE (ClassId)
    RDK_DECLARE_NON_MOVEABLE (ClassId)

    bool operator== (const ClassId& other) const
    {
        return mId == other.mId;
    }

    bool operator!= (const ClassId& other) const
    {
        return mId != other.mId;
    }

    /**
     * Get the id of this class instance.
     * @return The id of this class.
     */
    IdType getId() const
    {
        return mId;
    }

    /**
     * Get the current counter value.
     * @return The current counter value.
     */
    static IdType getCounter()
    {
        return sCounter;
    }

private:
    /// The id of this class instance.
    IdType mId { 0 };

    /// The counter is static, so it will be shared between all instances of this class.
    inline static std::atomic<IdType> sCounter { 0 };
};

} // namespace rdk
