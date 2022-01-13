// a class defines a type that holds data (member variables) and behavior (member functions)

class DateClass
{
public:
    int m_year {};
    int m_month {};
    int m_day {};

    void print() // defines a member function named print()
    {
        std::cout << m_year << '/' << m_month << '/' << m_day;
    }
};

Just like members of a struct, members (variables and functions) of a class are accessed using the member selector operator (.):

#include <iostream>

class DateClass
{
public:
    int m_year {};
    int m_month {};
    int m_day {};

    void print()
    {
        std::cout << m_year << '/' << m_month << '/' << m_day;
    }
};

int main()
{
    DateClass today { 2020, 10, 14 };

    today.m_day = 16; // use member selection operator to select a member variable of the class
    today.print(); // use member selection operator to call a member function of the class

    return 0;
}
This prints:

2020/10/16
Note how similar this program is to the struct version we wrote above.

However, there are a few differences. In the DateStruct version of print() from the example above, we needed to pass the struct itself to the print() function as the first parameter. Otherwise, print() wouldn’t know what DateStruct we wanted to use. We then had to reference this parameter inside the function explicitly.

Member functions work slightly differently: All member function calls must be associated with an object of the class. When we call “today.print()”, we’re telling the compiler to call the print() member function, associated with the today object.

Now let’s take a look at the definition of the print member function again:

void print() // defines a member function named print()
{
    std::cout << m_year << '/' << m_month << '/' << m_day;
}
What do m_year, m_month, and m_day actually refer to? They refer to the associated object (as determined by the caller).

So when we call “today.print()”, the compiler interprets m_day as today.m_day, m_month as today.m_month, and m_year as today.m_year. If we called “tomorrow.print()”, m_day would refer to tomorrow.m_day instead.

In this way, the associated object is essentially implicitly passed to the member function. For this reason, it is often called the implicit object.

We’ll talk more about how the implicit object passing works in detail in a later lesson in this chapter.

The key point is that with non-member functions, we have to pass data to the function to work with. With member functions, we can assume we always have an implicit object of the class to work with!

Using the “m_” prefix for member variables helps distinguish member variables from function parameters or local variables inside member functions. This is useful for several reasons. First, when we see an assignment to a variable with the “m_” prefix, we know that we are changing the state of the class instance. Second, unlike function parameters or local variables, which are declared within the function, member variables are declared in the class definition. Consequently, if we want to know how a variable with the “m_” prefix is declared, we know that we should look in the class definition instead of within the function.

By convention, class names should begin with an upper-case letter.

Best practice

Name your classes starting with a capital letter.

Here’s another example of a class:

#include <iostream>
#include <string>

class Employee
{
public:
    std::string m_name {};
    int m_id {};
    double m_wage {};

    // Print employee information to the screen
    void print()
    {
        std::cout << "Name: " << m_name <<
                "  Id: " << m_id <<
                "  Wage: $" << m_wage << '\n';
    }
};

int main()
{
    // Declare two employees
    Employee alex { "Alex", 1, 25.00 };
    Employee joe { "Joe", 2, 22.25 };

    // Print out the employee information
    alex.print();
    joe.print();

    return 0;
}
This produces the output:

Name: Alex  Id: 1  Wage: $25
Name: Joe  Id: 2  Wage: $22.25
With normal non-member functions, a function can’t call a function that’s defined “below” it (without a forward declaration):

void x()
{
// You can't call y() from here unless the compiler has already seen a forward declaration for y()
}

void y()
{
}
With member functions, this limitation doesn’t apply:

class foo
{
public:
     void x() { y(); } // okay to call y() here, even though y() isn't defined until later in this class
     void y() { };
};
Member types

In addition to member variables and member functions, classes can have member types or nested types (including type aliases). In the following example, we’re creating a calculator where we can swiftly change the type of number it’s using if we ever need to.

#include <iostream>
#include <vector>

class Calculator
{
public:
    using number_type = int; // this is a nested type alias

    std::vector<number_type> m_resultHistory{};

    number_type add(number_type a, number_type b)
    {
        auto result{ a + b };

        m_resultHistory.push_back(result);

        return result;
    }
};

int main()
{
    Calculator calculator;

    std::cout << calculator.add(3, 4) << '\n'; // 7
    std::cout << calculator.add(99, 24) << '\n'; // 123

    for (Calculator::number_type result : calculator.m_resultHistory)
    {
        std::cout << result << '\n';
    }

    return 0;
}
Output

7
123
7
123
In such a context, the class name effectively acts like a namespace for the nested type. From inside the class, we only need reference number_type. From outside the class, we can access the type via Calculator::number_type.

When we decide that an int no longer fulfills our needs and we want to use a double, we only need to update the type alias, rather than having to replace every occurrence of int with double.

Type alias members make code easier to maintain and can reduce typing. Template classes, which we’ll cover later, often make use of type alias members. You’ve already seen this as std::vector::size_type, where size_type is an alias for an unsigned integer.

Up to now, we used a “_t” suffix for type aliases. For member type aliases, a “_type” or no suffix at all is more common.

Nested types cannot be forward declared. Generally, nested types should only be used when the nested type is used exclusively within that class. Note that since classes are types, it’s possible to nest classes inside other classes -- this is uncommon and is typically only done by advanced programmers.






// https://www.learncpp.com/cpp-tutorial/classes-and-class-members/
