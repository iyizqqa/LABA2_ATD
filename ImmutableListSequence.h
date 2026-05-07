#pragma once

#include "ListSequence.h"

template <class T>
class ImmutableListSequence final : public ListSequence<T>
{
protected:
    ListSequence<T> *CloneListSequence() const override
    {
        return new ImmutableListSequence<T>(*this);
    }

    ListSequence<T> *Instance() override
    {
        return CloneListSequence();
    }

public:
    ImmutableListSequence() = default;

    ImmutableListSequence(const T *items, int count)
        : ListSequence<T>(items, count) {}

    explicit ImmutableListSequence(const LinkedList<T> &list)
        : ListSequence<T>(list) {}

    ImmutableListSequence(const ImmutableListSequence<T> &other)
        : ListSequence<T>(other) {}

    Sequence<T> *Clone() const override
    {
        return new ImmutableListSequence<T>(*this);
    }
};