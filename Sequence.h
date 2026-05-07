#pragma once

#include <functional>

#include "IEnumerable.h"

template <class T>
class Sequence : public IEnumerable<T>
{
public:
    virtual ~Sequence() = default;

    virtual const T &GetFirst() const = 0;
    virtual const T &GetLast() const = 0;
    virtual const T &Get(int index) const = 0;
    virtual int GetLength() const = 0;

    virtual Sequence<T> *GetSubsequence(int startIndex, int endIndex) const = 0;

    virtual Sequence<T> *Append(const T &item) = 0;
    virtual Sequence<T> *Prepend(const T &item) = 0;
    virtual Sequence<T> *InsertAt(const T &item, int index) = 0;
    virtual Sequence<T> *Concat(const Sequence<T> &other) = 0;

    virtual Sequence<T> *Map(const std::function<T(const T &)> &func) const = 0;
    virtual Sequence<T> *Where(const std::function<bool(const T &)> &predicate) const = 0;
    virtual T Reduce(const std::function<T(const T &, const T &)> &func, const T &initial) const = 0;

    virtual Sequence<T> *Clone() const = 0;

    virtual const T &operator[](int index) const = 0;
};