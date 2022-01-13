


#include <cassert>

template <typename T>
class Array
{
private:
    int m_length{};
    T* m_data{}; // changed type to T

public:

    Array(int length)
    {
        assert(length > 0);
        m_data = new T[length]{}; // allocated an array of objects of type T
        m_length = length;
    }

    Array(const Array&) = delete;
    Array& operator=(const Array&) = delete;

    ~Array()
    {
        delete[] m_data;
    }

    void erase()
    {
        delete[] m_data;
        m_data = nullptr;
        m_length = 0;
    }

    T& operator[](int index)
    {
        assert(index >= 0 && index < m_length);
        return m_data[index];
    }

    int getLength() const;
};

// member functions defined outside the class need their own template declaration
template <typename T>
int Array<T>::getLength() const
{
  return m_length;
}



Note that we’ve also defined the getLength() function outside of the class declaration. This isn’t necessary, but new programmers typically stumble when trying to do this for the first time due to the syntax, so an example is instructive. Each templated member function defined outside the class declaration needs its own template declaration. Also, note that the name of the templated array class is Array<T>, not Array -- Array would refer to a non-templated version of a class named Array, unless Array is used inside of the class. For example, the copy constructor and copy-assignment operator used Array rather than Array<T>. When the class name is used without template arguments inside of the class, the arguments are the same as the ones of the current instantiation.

Here’s a short example using the above templated array class:

#include <iostream>
#include "Array.h"

int main()
{
	Array<int> intArray { 12 };
	Array<double> doubleArray { 12 };

	for (int count{ 0 }; count < intArray.getLength(); ++count)
	{
		intArray[count] = count;
		doubleArray[count] = count + 0.5;
	}

	for (int count{ intArray.getLength() - 1 }; count >= 0; --count)
		std::cout << intArray[count] << '\t' << doubleArray[count] << '\n';

	return 0;
}
This example prints the following:

11     11.5
10     10.5
9       9.5
8       8.5
7       7.5
6       6.5
5       5.5
4       4.5
3       3.5
2       2.5
1       1.5
0       0.5
Template classes are instanced in the same way template functions are -- the compiler stencils out a copy upon demand, with the template parameter replaced by the actual data type the user needs, and then compiles the copy. If you don’t ever use a template class, the compiler won’t even compile it.

Template classes are ideal for implementing container classes, because it is highly desirable to have containers work across a wide variety of data types, and templates allow you to do so without duplicating code. Although the syntax is ugly, and the error messages can be cryptic, template classes are truly one of C++’s best and most useful features.

Template classes in the standard library

Now that we’ve covered template classes, you should understand what std::vector<int> means now -- std::vector is actually a template class, and int is the type parameter to the template! The standard library is full of predefined template classes available for your use. We’ll cover these in later chapters.

Splitting up template classes

A template is not a class or a function -- it is a stencil used to create classes or functions. As such, it does not work in quite the same way as normal functions or classes. In most cases, this isn’t much of a issue. However, there is one area that commonly causes problems for developers.

With non-template classes, the common procedure is to put the class definition in a header file, and the member function definitions in a similarly named code file. In this way, the source for the class is compiled as a separate project file. However, with templates, this does not work. Consider the following:

Array.h:

#ifndef ARRAY_H
#define ARRAY_H

#include <cassert>

template <typename T>
class Array
{
private:
    int m_length{};
    T* m_data{};

public:

    Array(int length)
    {
        assert(length > 0);
        m_data = new T[length]{};
        m_length = length;
    }

    Array(const Array&) = delete;
    Array& operator=(const Array&) = delete;

    ~Array()
    {
        delete[] m_data;
    }

    void erase()
    {
        delete[] m_data;

        m_data = nullptr;
        m_length = 0;
    }

    T& operator[](int index)
    {
        assert(index >= 0 && index < m_length);
        return m_data[index];
    }

    int getLength() const;
};

#endif
Array.cpp:

#include "Array.h"

template <typename T>
int Array<T>::getLength() const // note class name is Array<T>, not Array
{
  return m_length;
}
main.cpp:

#include "Array.h"

int main()
{
	Array<int> intArray(12);
	Array<double> doubleArray(12);

	for (int count{ 0 }; count < intArray.getLength(); ++count)
	{
		intArray[count] = count;
		doubleArray[count] = count + 0.5;
	}

	for (int count{ intArray.getLength() - 1 }; count >= 0; --count)
		std::cout << intArray[count] << '\t' << doubleArray[count] << '\n';

	return 0;
}
The above program will compile, but cause a linker error:

unresolved external symbol "public: int __thiscall Array<int>::getLength(void)" (?GetLength@?$Array@H@@QAEHXZ)
In order for the compiler to use a template, it must see both the template definition (not just a declaration) and the template type used to instantiate the template. Also remember that C++ compiles files individually. When the Array.h header is #included in main, the template class definition is copied into main.cpp. When the compiler sees that we need two template instances, Array<int>, and Array<double>, it will instantiate these, and compile them as part of main.cpp. However, when it gets around to compiling Array.cpp separately, it will have forgotten that we need an Array<int> and Array<double>, so that template function is never instantiated. Thus, we get a linker error, because the compiler can’t find a definition for Array<int>::getLength() or Array<double>::getLength().

There are quite a few ways to work around this.

The easiest way is to simply put all of your template class code in the header file (in this case, put the contents of Array.cpp into Array.h, below the class). In this way, when you #include the header, all of the template code will be in one place. The upside of this solution is that it is simple. The downside here is that if the template class is used in many places, you will end up with many local copies of the template class, which can increase your compile and link times (your linker should remove the duplicate definitions, so it shouldn’t bloat your executable). This is our preferred solution unless the compile or link times start to become a problem.

If you feel that putting the Array.cpp code into the Array.h header makes the header too long/messy, an alternative is to move the contents of Array.cpp to a new file named Array.inl (.inl stands for inline), and then include Array.inl at the bottom of the Array.h header (inside the header guard). That yields the same result as putting all the code in the header, but helps keep things a little more organized.

Tip

If you use the .inl method and then get a compiler error about duplicate definitions, your compiler is most likely compiling the .inl file as part of the project as if it were a code file. This results in the contents of the .inl getting compiled twice: once when your compiler compiles the .inl, and once when the .cpp file that includes the .inl gets compiled, and we run afoul of the one definition rule. If this happens, you’ll need to exclude the .inl file from being compiled as part of the build.

Excluding the .inl from the build can usually be done by right clicking on the .inl file in the project view, and then choosing properties. The setting will be somewhere in there. In Visual Studio, set “Exclude From Build” to “Yes”. In Code::Blocks, uncheck “Compile file” and “Link file”.

Other solutions involve #including .cpp files, but we don’t recommend these because of the non-standard usage of #include.

Another alternative is to use a three-file approach. The template class definition goes in the header. The template class member functions goes in the code file. Then you add a third file, which contains all of the instantiated classes you need:

templates.cpp:

// Ensure the full Array template definition can be seen
#include "Array.h"
#include "Array.cpp" // we're breaking best practices here, but only in this one place

// #include other .h and .cpp template definitions you need here

template class Array<int>; // Explicitly instantiate template Array<int>
template class Array<double>; // Explicitly instantiate template Array<double>

// instantiate other templates here
The “template class” command causes the compiler to explicitly instantiate the template class. In the above case, the compiler will stencil out both Array<int> and Array<double> inside of templates.cpp. Because templates.cpp is inside our project, this will then be compiled. These functions can then be linked to from elsewhere.

This method is more efficient, but requires maintaining the templates.cpp file for each program.