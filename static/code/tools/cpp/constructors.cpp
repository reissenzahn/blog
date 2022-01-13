#include <iostream>

// a constructor is a special kind of class member function that is automatically called when a object of that class is created

// constructors are typically used to initialize member variables or to do any setup steps necessary for the class to be used

// constructors have the same name as the class and have no return type


// a constructor that takes no parameters (or has parameters that all have default values) is called a default constructor

// the default constructor is called if no user-provided initialization values are provided


class Fraction
{
private:
  int m_numerator{};
  int m_denominator{};

public:
  Fraction()
  {
      m_numerator = 0;
      m_denominator = 1;
  }

  Fraction(int numerator, int denominator=1)
  {
    assert(denominator != 0);
    m_numerator = numerator;
    m_denominator = denominator;
  }

  int getNumerator()
  {
    return m_numerator;
  }

  int getDenominator()
  {
    return m_denominator;
  }
  
  double getValue()
  {
    return static_cast<double>(m_numerator) / m_denominator;
  }

  void print()
  {
    std::cout << m_numerator << '/' << m_denominator << '\n';
  }
};

int main()
{
  Frac frac;
  Fraction frac{};

  frac.print();  // 0/1


  Fraction fiveThirds{ 5, 3 }; // List initialization, calls Fraction(int, int)
  Fraction threeQuarters(3, 4); // Direct initialization, also calls Fraction(int, int)

  Fraction six{ 6 }; // calls Fraction(int, int) constructor, second parameter uses default value of 1
  
  
  
  return 0;
}


// you can define as many constructors as you want as long as each has a unique signature (number and type of parameters).


Default values for constructors work exactly the same way as with any other functions, so in the above case where we call six{ 6 }, the Fraction(int, int) function is called with the second parameter defaulted to value 1.



Copy initialization using equals with classes

Much like with fundamental variables, it’s also possible to initialize classes using copy initialization:

Fraction six = Fraction{ 6 }; // Copy initialize a Fraction, will call Fraction(6, 1)
Fraction seven = 7; // Copy initialize a Fraction.  The compiler will try to find a way to convert 7 to a Fraction, which will invoke the Fraction(7, 1) constructor.
However, we recommend you avoid this form of initialization with classes, as it may be less efficient. Although direct-initialization, list-initialization, and copy-initialization all work identically with fundamental types, copy-initialization does not work the same with classes (though the end-result is often the same). We’ll explore the differences in more detail in a future chapter.

Reducing your constructors

In the above two-constructor declaration of the Fraction class, the default constructor is actually somewhat redundant. We could simplify this class as follows:

#include <cassert>

class Fraction
{
private:
    int m_numerator {};
    int m_denominator {};

public:
    // Default constructor
    Fraction(int numerator=0, int denominator=1)
    {
        assert(denominator != 0);

        m_numerator = numerator;
        m_denominator = denominator;
    }

    int getNumerator() { return m_numerator; }
    int getDenominator() { return m_denominator; }
    double getValue() { return static_cast<double>(m_numerator) / m_denominator; }
};
Although this constructor is still a default constructor, it has now been defined in a way that it can accept one or two user-provided values as well.

Fraction zero; // will call Fraction(0, 1)
Fraction zero{}; // will call Fraction(0, 1)
Fraction six{ 6 }; // will call Fraction(6, 1)
Fraction fiveThirds{ 5, 3 }; // will call Fraction(5, 3)
When implementing your constructors, consider how you might keep the number of constructors down through smart defaulting of values.

A reminder about default parameters

The rules around defining and calling functions that have default parameters (described in lesson 8.12 -- Default arguments) apply to constructors too. To recap, when defining a function with default parameters, all default parameters must follow any non-default parameters, i.e. there cannot be non-defaulted parameters after a defaulted parameter.

This may produce unexpected results for classes that have multiple default parameters of different types. Consider:

class Something
{
public:
	// Default constructor
	Something(int n = 0, double d = 1.2) // allows us to construct a Something(int, double), Something(int), or Something()
	{
	}
};

int main()
{
	Something s1 { 1, 2.4 }; // calls Something(int, double)
	Something s2 { 1 }; // calls Something(int, double)
	Something s3 {}; // calls Something(int, double)

	Something s4 { 2.4 }; // will not compile, as there's no constructor to handle Something(double)

	return 0;
}
With s4, we’ve attempted to construct a Something by providing only a double. This won’t compile, as the rules for how arguments match with default parameters won’t allow us to skip a non-rightmost parameter (in this case, the leftmost int parameter).

If we want to be able to construct a Something with only a double, we’ll need to add a second (non-default) constructor:

class Something
{
public:
	// Default constructor
	Something(int n = 0, double d = 1.2) // allows us to construct a Something(int, double), Something(int), or Something()
	{
	}

	Something(double d)
	{
	}
};

int main()
{
	Something s1 { 1, 2.4 }; // calls Something(int, double)
	Something s2 { 1 }; // calls Something(int, double)
	Something s3 {}; // calls Something(int, double)

	Something s4 { 2.4 }; // calls Something(double)

	return 0;
}
An implicitly generated default constructor

If your class has no constructors, C++ will automatically generate a public default constructor for you. This is sometimes called an implicit constructor (or implicitly generated constructor).

Consider the following class:

class Date
{
private:
    int m_year{ 1900 };
    int m_month{ 1 };
    int m_day{ 1 };

    // No user-provided constructors, the compiler generates a default constructor.
};

int main()
{
    Date date{};

    return 0;
}
The Date class has no constructors. Therefore, the compiler will generate a default constructor that allows us to create a Date object without arguments.

When the generated default constructor is called, members will still be initialized if they have non-static member initializers.

If your class has any other constructors, the implicitly generated constructor will not be provided. For example:

class Date
{
private:
    int m_year{ 1900 };
    int m_month{ 1 };
    int m_day{ 1 };

public:
    Date(int year, int month, int day) // normal non-default constructor
    {
        m_year = year;
        m_month = month;
        m_day = day;
    }

    // No implicit constructor provided because we already defined our own constructor
};

int main()
{
    Date date{}; // error: Can't instantiate object because default constructor doesn't exist and the compiler won't generate one
    Date today{ 2020, 1, 19 }; // today is initialized to Jan 19th, 2020

    return 0;
}
If your class has another constructor and you want to allow default construction, you can either add default arguments to every parameter of a constructor with parameters, or explicitly define a default constructor.

There’s a third option as well: you can use the default keyword to tell the compiler to create a default constructor for us anyway:

class Date
{
private:
    int m_year{ 1900 };
    int m_month{ 1 };
    int m_day{ 1 };

public:
    // Tell the compiler to create a default constructor, even if
    // there are other user-provided constructors.
    Date() = default;

    Date(int year, int month, int day) // normal non-default constructor
    {
        m_year = year;
        m_month = month;
        m_day = day;
    }
};

int main()
{
    Date date{}; // date is initialized to Jan 1st, 1900
    Date today{ 2020, 10, 14 }; // today is initialized to Oct 14th, 2020

    return 0;
}
Using = default is longer than writing a constructor with an empty body, but expresses better what your intentions are (To create a default constructor), and it’s safer, because it can zero-initialize members even if they have not been initialized at their declaration. = default also works for other special constructors, which we’ll talk about in the future.

Best practice

If you have constructors in your class and need a default constructor that does nothing (e.g. because all your members are initialized using non-static member initialization), use = default.

Classes containing classes

A class may contain other classes as member variables. By default, when the outer class is constructed, the member variables will have their default constructors called. This happens before the body of the constructor executes.

This can be demonstrated thusly:

#include <iostream>

class A
{
public:
    A() { std::cout << "A\n"; }
};

class B
{
private:
    A m_a; // B contains A as a member variable

public:
    B() { std::cout << "B\n"; }
};

int main()
{
    B b;
    return 0;
}
This prints:

A
B
When variable b is constructed, the B() constructor is called. Before the body of the constructor executes, m_a is initialized, calling the class A default constructor. This prints “A”. Then control returns back to the B constructor, and the body of the B constructor executes.

This makes sense when you think about it, as the B() constructor may want to use variable m_a -- so m_a had better be initialized first!

The difference to the last example in the previous section is that m_a is a class-type. class-type members get initialized even if we don’t explicitly initialize them.

In the next lesson, we’ll talk about how to initialize these class member variables.

Constructor notes

Many new programmers are confused about whether constructors create the objects or not. They do not -- the compiler sets up the memory allocation for the object prior to the constructor call.

Constructors actually serve two purposes. First, constructors determine who is allowed to create an object. That is, an object of a class can only be created if a matching constructor can be found.

Second, constructors can be used to initialize objects. Whether the constructor actually does an initialization is up to the programmer. It’s syntactically valid to have a constructor that does no initialization at all (the constructor still serves the purpose of allowing the object to be created, as per the above).

However, much like it is a best practice to initialize all local variables, it’s also a best practice to initialize all member variables on creation of the object. This can be done via a constructor or via non-static member initialization.

Best practice

Always initialize all member variables in your objects.

Finally, constructors are only intended to be used for initialization when the object is created. You should not try to call a constructor to re-initialize an existing object. While it may compile, the results will not be what you intended (instead, the compiler will create a temporary object and then discard it).


// https://www.learncpp.com/cpp-tutorial/constructors/