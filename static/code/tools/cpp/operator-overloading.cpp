



// operators are implemented as functions and so function overloading allows us to overload these operator functions

int x { 2 };
int y { 3 };

std::cout << x + y << '\n';

when you see the expression x + y, you can translate this in your head to the function call operator+(x, y)


When evaluating an expression containing an operator, the compiler uses the following rules:

If all of the operands are fundamental data types, the compiler will call a built-in routine if one exists. If one does not exist, the compiler will produce a compiler error.
If any of the operands are user data types (e.g. one of your classes, or an enum type), the compiler looks to see whether the type has a matching overloaded operator function that it can call. If it can’t find one, it will try to convert one or more of the user-defined type operands into fundamental data types so it can use a matching built-in operator (via an overloaded typecast, which we’ll cover later in this chapter). If that fails, then it will produce a compile error.


What are the limitations on operator overloading?

almost any existing operator can be overloaded excluding: conditional (?:), sizeof, scope (::), member selector (.), member pointer selector (.*), typeid, casting operators

at least one of the operands in an overloaded operator must be a user-defined type

it is not possible to change the number of operands an operator supports and all operators keep their default precedence and associativity





---


Using a friend function to overload an operator is convenient because it gives you direct access to the internal members of the classes you’re operating on. In the initial Cents example above, our friend function version of operator+ accessed member variable m_cents directly.

However, if you don’t need that access, you can write your overloaded operators as normal functions. Note that the Cents class above contains an access function (getCents()) that allows us to get at m_cents without having to have direct access to private members. Because of this, we can write our overloaded operator+ as a non-friend:

#include <iostream>

class Cents
{
private:
  int m_cents{};

public:
  Cents(int cents)
    : m_cents{ cents }
  {}

  int getCents() const { return m_cents; }
};

// note: this function is not a member function nor a friend function!
Cents operator+(const Cents& c1, const Cents& c2)
{
  // use the Cents constructor and operator+(int, int)
  // we don't need direct access to private members here
  return Cents{ c1.getCents() + c2.getCents() };
}

int main()
{
  Cents cents1{ 6 };
  Cents cents2{ 8 };
  Cents centsSum{ cents1 + cents2 };
  std::cout << "I have " << centsSum.getCents() << " cents.\n";

  return 0;
}
Because the normal and friend functions work almost identically (they just have different levels of access to private members), we generally won’t differentiate them. The one difference is that the friend function declaration inside the class serves as a prototype as well. With the normal function version, you’ll have to provide your own function prototype.

Cents.h:

#ifndef CENTS_H
#define CENTS_H

class Cents
{
private:
  int m_cents{};

public:
  Cents(int cents)
    : m_cents{ cents }
  {}

  int getCents() const { return m_cents; }
};

// Need to explicitly provide prototype for operator+ so uses of operator+ in other files know this overload exists
Cents operator+(const Cents& c1, const Cents& c2);

#endif
Cents.cpp:

#include "Cents.h"

// note: this function is not a member function nor a friend function!
Cents operator+(const Cents& c1, const Cents& c2)
{
  // use the Cents constructor and operator+(int, int)
  // we don't need direct access to private members here
  return { c1.getCents() + c2.getCents() };
}
main.cpp:

#include "Cents.h"
#include <iostream>

int main()
{
  Cents cents1{ 6 };
  Cents cents2{ 8 };
  Cents centsSum{ cents1 + cents2 }; // without the prototype in Cents.h, this would fail to compile
  std::cout << "I have " << centsSum.getCents() << " cents.\n";

  return 0;
}
In general, a normal function should be preferred over a friend function if it’s possible to do so with the existing member functions available (the less functions touching your classes’s internals, the better). However, don’t add additional access functions just to overload an operator as a normal function instead of a friend function!

Best practice

Prefer overloading operators as normal functions instead of friends if it’s possible to do so without adding additional functions.


---


Some of the most commonly used operators in C++ are the arithmetic operators -- that is, the plus operator (+), minus operator (-), multiplication operator (*), and division operator (/). Note that all of the arithmetic operators are binary operators -- meaning they take two operands -- one on each side of the operator. All four of these operators are overloaded in the exact same way.

It turns out that there are three different ways to overload operators: the member function way, the friend function way, and the normal function way. In this lesson, we’ll cover the friend function way (because it’s more intuitive for most binary operators). Next lesson, we’ll discuss the normal function way. Finally, in a later lesson in this chapter, we’ll cover the member function way. And, of course, we’ll also summarize when to use each in more detail.

Overloading operators using friend functions

Consider the following trivial class:

class Cents
{
private:
	int m_cents {};

public:
	Cents(int cents) { m_cents = cents; }
	int getCents() const { return m_cents; }
};
The following example shows how to overload operator plus (+) in order to add two “Cents” objects together:

#include <iostream>

class Cents
{
private:
	int m_cents {};

public:
	Cents(int cents) { m_cents = cents; }

	// add Cents + Cents using a friend function
	friend Cents operator+(const Cents &c1, const Cents &c2);

	int getCents() const { return m_cents; }
};

// note: this function is not a member function!
Cents operator+(const Cents &c1, const Cents &c2)
{
	// use the Cents constructor and operator+(int, int)
	// we can access m_cents directly because this is a friend function
	return Cents(c1.m_cents + c2.m_cents);
}

int main()
{
	Cents cents1{ 6 };
	Cents cents2{ 8 };
	Cents centsSum{ cents1 + cents2 };
	std::cout << "I have " << centsSum.getCents() << " cents.\n";

	return 0;
}
This produces the result:

I have 14 cents.
Overloading the plus operator (+) is as simple as declaring a function named operator+, giving it two parameters of the type of the operands we want to add, picking an appropriate return type, and then writing the function.

In the case of our Cents object, implementing our operator+() function is very simple. First, the parameter types: in this version of operator+, we are going to add two Cents objects together, so our function will take two objects of type Cents. Second, the return type: our operator+ is going to return a result of type Cents, so that’s our return type.

Finally, implementation: to add two Cents objects together, we really need to add the m_cents member from each Cents object. Because our overloaded operator+() function is a friend of the class, we can access the m_cents member of our parameters directly. Also, because m_cents is an integer, and C++ knows how to add integers together using the built-in version of the plus operator that works with integer operands, we can simply use the + operator to do the adding.

Overloading the subtraction operator (-) is simple as well:

#include <iostream>

class Cents
{
private:
	int m_cents {};

public:
	Cents(int cents) { m_cents = cents; }

	// add Cents + Cents using a friend function
	friend Cents operator+(const Cents &c1, const Cents &c2);

	// subtract Cents - Cents using a friend function
	friend Cents operator-(const Cents &c1, const Cents &c2);

	int getCents() const { return m_cents; }
};

// note: this function is not a member function!
Cents operator+(const Cents &c1, const Cents &c2)
{
	// use the Cents constructor and operator+(int, int)
	// we can access m_cents directly because this is a friend function
	return Cents(c1.m_cents + c2.m_cents);
}

// note: this function is not a member function!
Cents operator-(const Cents &c1, const Cents &c2)
{
	// use the Cents constructor and operator-(int, int)
	// we can access m_cents directly because this is a friend function
	return Cents(c1.m_cents - c2.m_cents);
}

int main()
{
	Cents cents1{ 6 };
	Cents cents2{ 2 };
	Cents centsSum{ cents1 - cents2 };
	std::cout << "I have " << centsSum.getCents() << " cents.\n";

	return 0;
}
Overloading the multiplication operator (*) and the division operator (/) is as easy as defining functions for operator* and operator/ respectively.

Friend functions can be defined inside the class

Even though friend functions are not members of the class, they can still be defined inside the class if desired:

#include <iostream>

class Cents
{
private:
	int m_cents {};

public:
	Cents(int cents) { m_cents = cents; }

	// add Cents + Cents using a friend function
        // This function is not considered a member of the class, even though the definition is inside the class
	friend Cents operator+(const Cents &c1, const Cents &c2)
	{
		// use the Cents constructor and operator+(int, int)
		// we can access m_cents directly because this is a friend function
		return Cents(c1.m_cents + c2.m_cents);
	}

	int getCents() const { return m_cents; }
};

int main()
{
	Cents cents1{ 6 };
	Cents cents2{ 8 };
	Cents centsSum{ cents1 + cents2 };
	std::cout << "I have " << centsSum.getCents() << " cents.\n";

	return 0;
}
We generally don’t recommend this, as non-trivial function definitions are better kept in a separate .cpp file, outside of the class definition. However, we will use this pattern in future tutorials to keep the examples concise.

Overloading operators for operands of different types

Often it is the case that you want your overloaded operators to work with operands that are different types. For example, if we have Cents(4), we may want to add the integer 6 to this to produce the result Cents(10).

When C++ evaluates the expression x + y, x becomes the first parameter, and y becomes the second parameter. When x and y have the same type, it does not matter if you add x + y or y + x -- either way, the same version of operator+ gets called. However, when the operands have different types, x + y does not call the same function as y + x.

For example, Cents(4) + 6 would call operator+(Cents, int), and 6 + Cents(4) would call operator+(int, Cents). Consequently, whenever we overload binary operators for operands of different types, we actually need to write two functions -- one for each case. Here is an example of that:

#include <iostream>

class Cents
{
private:
	int m_cents {};

public:
	Cents(int cents) { m_cents = cents; }

	// add Cents + int using a friend function
	friend Cents operator+(const Cents &c1, int value);

	// add int + Cents using a friend function
	friend Cents operator+(int value, const Cents &c1);


	int getCents() const { return m_cents; }
};

// note: this function is not a member function!
Cents operator+(const Cents &c1, int value)
{
	// use the Cents constructor and operator+(int, int)
	// we can access m_cents directly because this is a friend function
	return { c1.m_cents + value };
}

// note: this function is not a member function!
Cents operator+(int value, const Cents &c1)
{
	// use the Cents constructor and operator+(int, int)
	// we can access m_cents directly because this is a friend function
	return { c1.m_cents + value };
}

int main()
{
	Cents c1{ Cents{ 4 } + 6 };
	Cents c2{ 6 + Cents{ 4 } };

	std::cout << "I have " << c1.getCents() << " cents.\n";
	std::cout << "I have " << c2.getCents() << " cents.\n";

	return 0;
}
Note that both overloaded functions have the same implementation -- that’s because they do the same thing, they just take their parameters in a different order.

Another example

Let’s take a look at another example:

#include <iostream>

class MinMax
{
private:
	int m_min {}; // The min value seen so far
	int m_max {}; // The max value seen so far

public:
	MinMax(int min, int max)
	{
		m_min = min;
		m_max = max;
	}

	int getMin() const { return m_min; }
	int getMax() const { return m_max; }

	friend MinMax operator+(const MinMax &m1, const MinMax &m2);
	friend MinMax operator+(const MinMax &m, int value);
	friend MinMax operator+(int value, const MinMax &m);
};

MinMax operator+(const MinMax &m1, const MinMax &m2)
{
	// Get the minimum value seen in m1 and m2
	int min{ m1.m_min < m2.m_min ? m1.m_min : m2.m_min };

	// Get the maximum value seen in m1 and m2
	int max{ m1.m_max > m2.m_max ? m1.m_max : m2.m_max };

	return { min, max };
}

MinMax operator+(const MinMax &m, int value)
{
	// Get the minimum value seen in m and value
	int min{ m.m_min < value ? m.m_min : value };

	// Get the maximum value seen in m and value
	int max{ m.m_max > value ? m.m_max : value };

	return { min, max };
}

MinMax operator+(int value, const MinMax &m)
{
	// call operator+(MinMax, int)
	return { m + value };
}

int main()
{
	MinMax m1{ 10, 15 };
	MinMax m2{ 8, 11 };
	MinMax m3{ 3, 12 };

	MinMax mFinal{ m1 + m2 + 5 + 8 + m3 + 16 };

	std::cout << "Result: (" << mFinal.getMin() << ", " <<
		mFinal.getMax() << ")\n";

	return 0;
}
The MinMax class keeps track of the minimum and maximum values that it has seen so far. We have overloaded the + operator 3 times, so that we can add two MinMax objects together, or add integers to MinMax objects.

This example produces the result:

Result: (3, 16)
which you will note is the minimum and maximum values that we added to mFinal.

Let’s talk a little bit more about how “MinMax mFinal = m1 + m2 + 5 + 8 + m3 + 16” evaluates. Remember that operator+ has higher precedence than operator=, and operator+ evaluates from left to right, so m1 + m2 evaluate first. This becomes a call to operator+(m1, m2), which produces the return value MinMax(8, 15). Then MinMax(8, 15) + 5 evaluates next. This becomes a call to operator+(MinMax(8, 15), 5), which produces return value MinMax(5, 15). Then MinMax(5, 15) + 8 evaluates in the same way to produce MinMax(5, 15). Then MinMax(5, 15) + m3 evaluates to produce MinMax(3, 15). And finally, MinMax(3, 15) + 16 evaluates to MinMax(3, 16). This final result is then assigned to mFinal.

In other words, this expression evaluates as “MinMax mFinal = (((((m1 + m2) + 5) + 8) + m3) + 16)”, with each successive operation returning a MinMax object that becomes the left-hand operand for the following operator.

Implementing operators using other operators

In the above example, note that we defined operator+(int, MinMax) by calling operator+(MinMax, int) (which produces the same result). This allows us to reduce the implementation of operator+(int, MinMax) to a single line, making our code easier to maintain by minimizing redundancy and making the function simpler to understand.

It is often possible to define overloaded operators by calling other overloaded operators. You should do so if and when doing so produces simpler code. In cases where the implementation is trivial (e.g. a single line) it’s often not worth doing this, as the added indirection of an additional function call is more complicated than just implementing the function directly.

