

#include <cassert>
#include <iostream>

class Fraction
{
private:
	int m_numerator{};
	int m_denominator{};

public:
	Fraction(int numerator=0, int denominator=1)
		: m_numerator{numerator}, m_denominator{denominator}
	{
		assert(denominator != 0);
	}

	friend std::ostream& operator<<(std::ostream& out, const Fraction& f1);
};

std::ostream& operator<<(std::ostream& out, const Fraction& f1)
{
	out << f1.m_numerator << '/' << f1.m_denominator;
	return out;
}

int main()
{
	Fraction fiveThirds { 5, 3 };
	Fraction fCopy { fiveThirds };
	std::cout << fCopy << '\n';
}

// a copy constructor is a special type of constructor used to create a new object as a copy of an existing object


// if you do not provide a copy constructor for your classes then a default copy constructor is provided that initializes each member of the copy directly from the member of the class being copied



#include <cassert>
#include <iostream>

class Fraction
{
private:
	int m_numerator{};
	int m_denominator{};

public:
	Fraction(int numerator=0, int denominator=1)
		: m_numerator{numerator}, m_denominator{denominator}
	{
		assert(denominator != 0);
	}

	Fraction(const Fraction& fraction)
		: m_numerator{fraction.m_numerator}, m_denominator{fraction.m_denominator}
    {
			std::cout << "Copy constructor called\n";
    }

    friend std::ostream& operator<<(std::ostream& out, const Fraction& f1);
};

std::ostream& operator<<(std::ostream& out, const Fraction& f1)
{
	out << f1.m_numerator << '/' << f1.m_denominator;
	return out;
}

int main()
{
	Fraction fiveThirds { 5, 3 }; // Direct initialize a Fraction, calls Fraction(int, int) constructor
	Fraction fCopy { fiveThirds }; // Direct initialize -- with Fraction copy constructor
	std::cout << fCopy << '\n';
}


// member functions of a class can access the private members of parameters of the same class type



Preventing copies

// We can prevent copies of our classes from being made by making the copy constructor private


The copy constructor may be elided

Fraction fiveThirds { Fraction { 5, 3 } };

Note that in cases where elision is used, any statements in the body of the copy constructor are not executed, even if they would have produced side effects (like printing to the screen)!
