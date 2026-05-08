#include <iostream>

#include "Tests.h"
#include "Sequence.h"
#include "IEnumerator.h"
#include "MutableArraySequence.h"
#include "MutableListSequence.h"

void PrintSequence(const Sequence<int> &sequence)
{
    std::cout << "[";

    IEnumerator<int> *iterator = sequence.GetEnumerator();
    bool first = true;

    try
    {
        while (iterator->HasNext())
        {
            if (!first)
            {
                std::cout << ", ";
            }

            std::cout << iterator->Next();
            first = false;
        }

        delete iterator;
    }
    catch (...)
    {
        delete iterator;
        throw;
    }

    std::cout << "]\n";
}

void ArraySequenceMenu()
{
    MutableArraySequence<int> sequence;

    bool running = true;

    while (running)
    {
        std::cout << "\nMutableArraySequence menu\n"
                  << "1. Append\n"
                  << "2. Prepend\n"
                  << "3. InsertAt\n"
                  << "4. Print\n"
                  << "5. GetSubsequence\n"
                  << "6. Map x*x\n"
                  << "7. Where even\n"
                  << "8. Reduce sum\n"
                  << "0. Back\n"
                  << "Choose: ";

        int command;

        if (!(std::cin >> command))
        {
            return;
        }

        try
        {
            if (command == 1)
            {
                int value;

                std::cout << "Value: ";
                std::cin >> value;

                sequence.Append(value);
            }
            else if (command == 2)
            {
                int value;

                std::cout << "Value: ";
                std::cin >> value;

                sequence.Prepend(value);
            }
            else if (command == 3)
            {
                int value;
                int index;

                std::cout << "Value and index: ";
                std::cin >> value >> index;

                sequence.InsertAt(value, index);
            }
            else if (command == 4)
            {
                PrintSequence(sequence);
            }
            else if (command == 5)
            {
                int startIndex;
                int endIndex;

                std::cout << "Start and end index: ";
                std::cin >> startIndex >> endIndex;

                Sequence<int> *sub = sequence.GetSubsequence(startIndex, endIndex);

                PrintSequence(*sub);

                delete sub;
            }
            else if (command == 6)
            {
                Sequence<int> *mapped = sequence.Map([](const int &value)
                                                     { return value * value; });

                PrintSequence(*mapped);

                delete mapped;
            }
            else if (command == 7)
            {
                Sequence<int> *filtered = sequence.Where([](const int &value)
                                                         { return value % 2 == 0; });

                PrintSequence(*filtered);

                delete filtered;
            }
            else if (command == 8)
            {
                int result = sequence.Reduce(
                    [](const int &left, const int &right)
                    {
                        return left + right;
                    },
                    0);

                std::cout << result << "\n";
            }
            else if (command == 0)
            {
                running = false;
            }
            else
            {
                std::cout << "Unknown command\n";
            }
        }
        catch (const std::exception &error)
        {
            std::cout << "Error: " << error.what() << "\n";
        }
    }
}

void ListSequenceMenu()
{
    MutableListSequence<int> sequence;

    bool running = true;

    while (running)
    {
        std::cout << "\nMutableListSequence menu\n"
                  << "1. Append\n"
                  << "2. Prepend\n"
                  << "3. InsertAt\n"
                  << "4. Print\n"
                  << "5. GetSubsequence\n"
                  << "6. Map x*x\n"
                  << "7. Where even\n"
                  << "8. Reduce sum\n"
                  << "0. Back\n"
                  << "Choose: ";

        int command;

        if (!(std::cin >> command))
        {
            return;
        }

        try
        {
            if (command == 1)
            {
                int value;

                std::cout << "Value: ";
                std::cin >> value;

                sequence.Append(value);
            }
            else if (command == 2)
            {
                int value;

                std::cout << "Value: ";
                std::cin >> value;

                sequence.Prepend(value);
            }
            else if (command == 3)
            {
                int value;
                int index;

                std::cout << "Value and index: ";
                std::cin >> value >> index;

                sequence.InsertAt(value, index);
            }
            else if (command == 4)
            {
                PrintSequence(sequence);
            }
            else if (command == 5)
            {
                int startIndex;
                int endIndex;

                std::cout << "Start and end index: ";
                std::cin >> startIndex >> endIndex;

                Sequence<int> *sub = sequence.GetSubsequence(startIndex, endIndex);

                PrintSequence(*sub);

                delete sub;
            }
            else if (command == 6)
            {
                Sequence<int> *mapped = sequence.Map([](const int &value)
                                                     { return value * value; });

                PrintSequence(*mapped);

                delete mapped;
            }
            else if (command == 7)
            {
                Sequence<int> *filtered = sequence.Where([](const int &value)
                                                         { return value % 2 == 0; });

                PrintSequence(*filtered);

                delete filtered;
            }
            else if (command == 8)
            {
                int result = sequence.Reduce(
                    [](const int &left, const int &right)
                    {
                        return left + right;
                    },
                    0);

                std::cout << result << "\n";
            }
            else if (command == 0)
            {
                running = false;
            }
            else
            {
                std::cout << "Unknown command\n";
            }
        }
        catch (const std::exception &error)
        {
            std::cout << "Error: " << error.what() << "\n";
        }
    }
}

int main()
{
    bool running = true;

    while (running)
    {
        std::cout << "\nLab 2 UI\n"
                  << "1. Run tests\n"
                  << "2. Work with MutableArraySequence\n"
                  << "3. Work with MutableListSequence\n"
                  << "0. Exit\n"
                  << "Choose: ";

        int command;

        if (!(std::cin >> command))
        {
            return 0;
        }

        try
        {
            if (command == 1)
            {
                RunAllTests();
            }
            else if (command == 2)
            {
                ArraySequenceMenu();
            }
            else if (command == 3)
            {
                ListSequenceMenu();
            }
            else if (command == 0)
            {
                running = false;
            }
            else
            {
                std::cout << "Unknown command\n";
            }
        }
        catch (const std::exception &error)
        {
            std::cout << "Error: " << error.what() << "\n";
        }
    }

    return 0;
}