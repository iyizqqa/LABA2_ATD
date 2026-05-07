#pragma once

#include <iostream>
#include <stdexcept>
#include <string>

#include "DynamicArray.h"
#include "LinkedList.h"
#include "MutableArraySequence.h"
#include "ImmutableArraySequence.h"
#include "MutableListSequence.h"
#include "ImmutableListSequence.h"

class TestFailure : public std::runtime_error
{
public:
    explicit TestFailure(const std::string &message)
        : std::runtime_error(message) {}
};

inline void Assert(bool condition, const std::string &message)
{
    if (!condition)
    {
        throw TestFailure(message);
    }
}

template <class ExceptionType, class Func>
void AssertThrows(Func func, const std::string &message)
{
    try
    {
        func();
    }
    catch (const ExceptionType &)
    {
        return;
    }

    throw TestFailure(message);
}

//проверка что об-т содержит ожидаемые эл-ты
template <class T>
void AssertEnumerableEquals(
    const IEnumerable<T> &enumerable,
    const T *expected,
    int count,
    const std::string &message)
{
    IEnumerator<T> *iterator = enumerable.GetEnumerator();
    int index = 0;

    try
    {
        while (iterator->HasNext())
        {
            Assert(index < count, message + " (too many elements)");

            Assert(
                iterator->Next() == expected[index],
                message + " (element mismatch at index " + std::to_string(index) + ")");

            ++index;
        }

        delete iterator;
    }
    catch (...)
    {
        delete iterator;
        throw;
    }

    Assert(index == count, message + " (too few elements)");
}

//проверяет длину и сами эл-ты через итератор
template <class T>
void AssertSequenceEquals(
    const Sequence<T> &sequence,
    const T *expected,
    int count,
    const std::string &message)
{
    Assert(sequence.GetLength() == count, message + " (length mismatch)");
    AssertEnumerableEquals(sequence, expected, count, message);
}

//для мап
inline int Square(const int &value)
{
    return value * value;
}

//для вэрэ
inline bool IsEven(const int &value)
{
    return value % 2 == 0;
}

//для редюс
inline int Sum(const int &left, const int &right)
{
    return left + right;
}


//проверяет арр
inline void TestDynamicArray()
{
    DynamicArray<int> array(3);

    Assert(array.GetSize() == 3, "DynamicArray: size constructor failed");
    Assert(array.Get(0) == 0, "DynamicArray: default value failed");

    array.Set(1, 10);

    Assert(array.Get(1) == 10, "DynamicArray: Set/Get failed");

    array.EnsureCapacity(20);

    Assert(array.GetCapacity() >= 20, "DynamicArray: EnsureCapacity failed");
    Assert(array.Get(1) == 10, "DynamicArray: value lost after EnsureCapacity");

    array.Resize(4);
    array.Set(3, 30);

    int expected[] = {0, 10, 0, 30};

    AssertEnumerableEquals(array, expected, 4, "DynamicArray: iterator failed");

    array.Resize(1);

    Assert(array.GetSize() == 1, "DynamicArray: Resize down failed");

    AssertThrows<std::invalid_argument>(
        []()
        {
            DynamicArray<int> invalid(-1);
        },
        "DynamicArray: negative size must throw");

    AssertThrows<std::invalid_argument>(
        []()
        {
            DynamicArray<int> invalid(nullptr, 1);
        },
        "DynamicArray: null items must throw");

    AssertThrows<std::out_of_range>(
        [&array]()
        {
            array.Get(10);
        },
        "DynamicArray: invalid Get must throw");

    AssertThrows<std::out_of_range>(
        [&array]()
        {
            array.Set(-1, 1);
        },
        "DynamicArray: invalid Set must throw");
}

//для лист
inline void TestLinkedList()
{
    int items[] = {1, 2, 3};

    LinkedList<int> list(items, 3);

    Assert(list.GetLength() == 3, "LinkedList: constructor failed");
    Assert(list.GetFirst() == 1, "LinkedList: GetFirst failed");
    Assert(list.GetLast() == 3, "LinkedList: GetLast failed");

    list.Prepend(0);
    list.Append(4);
    list.InsertAt(99, 2);

    int expected[] = {0, 1, 99, 2, 3, 4};

    AssertEnumerableEquals(list, expected, 6, "LinkedList: iterator/mutations failed");

    LinkedList<int> *sub = list.GetSubList(1, 4);

    int expectedSub[] = {1, 99, 2, 3};

    AssertEnumerableEquals(*sub, expectedSub, 4, "LinkedList: GetSubList failed");

    delete sub;

    LinkedList<int> empty;

    AssertThrows<std::out_of_range>(
        [&empty]()
        {
            empty.GetFirst();
        },
        "LinkedList: empty GetFirst must throw");

    AssertThrows<std::out_of_range>(
        [&empty]()
        {
            empty.GetLast();
        },
        "LinkedList: empty GetLast must throw");

    AssertThrows<std::invalid_argument>(
        []()
        {
            LinkedList<int> invalid(nullptr, 1);
        },
        "LinkedList: null items must throw");

    AssertThrows<std::out_of_range>(
        [&list]()
        {
            list.Get(100);
        },
        "LinkedList: invalid Get must throw");

    AssertThrows<std::out_of_range>(
        [&list]()
        {
            list.GetSubList(4, 1);
        },
        "LinkedList: invalid sublist must throw");
}


inline void TestMutableArraySequence()
{
    int items[] = {1, 2, 3};

    MutableArraySequence<int> sequence(items, 3);

    Sequence<int> *same = sequence.Append(4);

    Assert(same == &sequence, "MutableArraySequence: Append must return this");

    sequence.Prepend(0);
    sequence.InsertAt(99, 2);

    int expected[] = {0, 1, 99, 2, 3, 4};

    AssertSequenceEquals(sequence, expected, 6, "MutableArraySequence: mutations failed");

    Sequence<int> *mapped = sequence.Map(Square);

    int expectedMap[] = {0, 1, 9801, 4, 9, 16};

    AssertSequenceEquals(*mapped, expectedMap, 6, "MutableArraySequence: Map failed");

    delete mapped;

    Sequence<int> *filtered = sequence.Where(IsEven);

    int expectedWhere[] = {0, 2, 4};

    AssertSequenceEquals(*filtered, expectedWhere, 3, "MutableArraySequence: Where failed");

    delete filtered;

    Assert(sequence.Reduce(Sum, 0) == 109, "MutableArraySequence: Reduce failed");

    Sequence<int> *sub = sequence.GetSubsequence(1, 3);

    int expectedSub[] = {1, 99, 2};

    AssertSequenceEquals(*sub, expectedSub, 3, "MutableArraySequence: GetSubsequence failed");

    delete sub;

    AssertThrows<std::out_of_range>(
        [&sequence]()
        {
            sequence.InsertAt(1, -1);
        },
        "MutableArraySequence: negative InsertAt must throw");

    const std::function<int(const int &)> emptyMap;

    AssertThrows<std::invalid_argument>(
        [&sequence, &emptyMap]()
        {
            Sequence<int> *result = sequence.Map(emptyMap);
            delete result;
        },
        "MutableArraySequence: null Map must throw");
}

inline void TestImmutableArraySequence()
{
    int items[] = {1, 2, 3};

    ImmutableArraySequence<int> sequence(items, 3);

    Sequence<int> *copy = sequence.Append(4);

    Assert(copy != &sequence, "ImmutableArraySequence: Append must return new object");

    int original[] = {1, 2, 3};
    int changed[] = {1, 2, 3, 4};

    AssertSequenceEquals(sequence, original, 3, "ImmutableArraySequence: source changed");
    AssertSequenceEquals(*copy, changed, 4, "ImmutableArraySequence: result incorrect");

    delete copy;

    Sequence<int> *inserted = sequence.InsertAt(99, 1);

    int insertedExpected[] = {1, 99, 2, 3};

    AssertSequenceEquals(sequence, original, 3, "ImmutableArraySequence: InsertAt changed source");
    AssertSequenceEquals(*inserted, insertedExpected, 4, "ImmutableArraySequence: InsertAt result incorrect");

    delete inserted;
}

inline void TestMutableListSequence()
{
    int items[] = {1, 2, 3};

    MutableListSequence<int> sequence(items, 3);

    Sequence<int> *same = sequence.Append(4);

    Assert(same == &sequence, "MutableListSequence: Append must return this");

    sequence.Prepend(0);
    sequence.InsertAt(99, 2);

    int expected[] = {0, 1, 99, 2, 3, 4};

    AssertSequenceEquals(sequence, expected, 6, "MutableListSequence: mutations failed");

    Sequence<int> *mapped = sequence.Map(Square);

    int expectedMap[] = {0, 1, 9801, 4, 9, 16};

    AssertSequenceEquals(*mapped, expectedMap, 6, "MutableListSequence: Map failed");

    delete mapped;

    Sequence<int> *filtered = sequence.Where(IsEven);

    int expectedWhere[] = {0, 2, 4};

    AssertSequenceEquals(*filtered, expectedWhere, 3, "MutableListSequence: Where failed");

    delete filtered;

    Assert(sequence.Reduce(Sum, 0) == 109, "MutableListSequence: Reduce failed");
}

inline void TestImmutableListSequence()
{
    int items[] = {1, 2, 3};

    ImmutableListSequence<int> sequence(items, 3);

    Sequence<int> *copy = sequence.Prepend(0);

    Assert(copy != &sequence, "ImmutableListSequence: Prepend must return new object");

    int original[] = {1, 2, 3};
    int changed[] = {0, 1, 2, 3};

    AssertSequenceEquals(sequence, original, 3, "ImmutableListSequence: source changed");
    AssertSequenceEquals(*copy, changed, 4, "ImmutableListSequence: result incorrect");

    delete copy;
}

inline void TestEmptySequences()
{
    MutableArraySequence<int> arraySequence;
    MutableListSequence<int> listSequence;

    AssertThrows<std::out_of_range>(
        [&arraySequence]()
        {
            arraySequence.GetFirst();
        },
        "ArraySequence: empty GetFirst must throw");

    AssertThrows<std::out_of_range>(
        [&arraySequence]()
        {
            arraySequence.GetLast();
        },
        "ArraySequence: empty GetLast must throw");

    AssertThrows<std::out_of_range>(
        [&listSequence]()
        {
            listSequence.GetFirst();
        },
        "ListSequence: empty GetFirst must throw");

    AssertThrows<std::out_of_range>(
        [&listSequence]()
        {
            listSequence.GetLast();
        },
        "ListSequence: empty GetLast must throw");

    Assert(arraySequence.Reduce(Sum, 10) == 10, "ArraySequence: Reduce on empty failed");
    Assert(listSequence.Reduce(Sum, 10) == 10, "ListSequence: Reduce on empty failed");
}

inline void TestIteratorReset()
{
    int items[] = {1, 2, 3};

    MutableArraySequence<int> arraySequence(items, 3);
    MutableListSequence<int> listSequence(items, 3);

    IEnumerator<int> *arrayIterator = arraySequence.GetEnumerator();

    Assert(arrayIterator->Next() == 1, "Array iterator: first Next failed");
    Assert(arrayIterator->Next() == 2, "Array iterator: second Next failed");

    arrayIterator->Reset();

    Assert(arrayIterator->Next() == 1, "Array iterator: Reset failed");

    delete arrayIterator;

    IEnumerator<int> *listIterator = listSequence.GetEnumerator();

    Assert(listIterator->Next() == 1, "List iterator: first Next failed");
    Assert(listIterator->Next() == 2, "List iterator: second Next failed");

    listIterator->Reset();

    Assert(listIterator->Next() == 1, "List iterator: Reset failed");

    delete listIterator;
}

inline void TestConcatSelf()
{
    int items[] = {1, 2, 3};

    MutableArraySequence<int> arraySequence(items, 3);
    arraySequence.Concat(arraySequence);

    int expectedArray[] = {1, 2, 3, 1, 2, 3};

    AssertSequenceEquals(arraySequence, expectedArray, 6, "MutableArraySequence: self concat failed");

    MutableListSequence<int> listSequence(items, 3);
    listSequence.Concat(listSequence);

    int expectedList[] = {1, 2, 3, 1, 2, 3};

    AssertSequenceEquals(listSequence, expectedList, 6, "MutableListSequence: self concat failed");
}

inline void RunAllTests()
{
    TestDynamicArray();
    TestLinkedList();
    TestMutableArraySequence();
    TestImmutableArraySequence();
    TestMutableListSequence();
    TestImmutableListSequence();
    TestEmptySequences();
    TestIteratorReset();
    TestConcatSelf();

    std::cout << "All tests passed successfully.\n";
}