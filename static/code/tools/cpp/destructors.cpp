#include <iostream>
#include <cassert>
#include <cstddef>

// a destructor is a special kind of class member function that is executed when an object of that class is destroyed

// the class destructor is automatically called when an object goes out of scope normally (or a dynamically allocated object is explicitly deleted using the delete keyword)

// destructors are used to clean up any resources held the the object (e.g. dynamic memory, file descriptors, etc.)


// a class can only have a single destructor and the destructor cannot take any argument and has no return type

// destructors may safely call other member functions since the object is not destroyed until after the destructor executes

class Array {
private:
  int *m_array{};
  int m_size{};

public:
  Array(int size) {
    assert(size > 0);

    m_array = new int[static_cast<std::size_t>(size)]{};
    m_size = size;
  }

  ~Array() {
    delete[] m_array;
  }
};


	~IntArray() // destructor
	{
		// Dynamically delete the array we allocated earlier
		delete[] m_array;
	}

	void setValue(int index, int value) { m_array[index] = value; }
	int getValue(int index) { return m_array[index]; }

	int getLength() { return m_length; }
};

int main()
{
	IntArray ar ( 10 ); // allocate 10 integers
	for (int count{ 0 }; count < ar.getLength(); ++count)
		ar.setValue(count, count+1);

	std::cout << "The value of element 5 is: " << ar.getValue(5) << '\n';

	return 0;
} // ar is destroyed here, so the ~IntArray() destructor function is called here
Tip

If you compile the above example and get the following error:

error: 'class IntArray' has pointer data members [-Werror=effc++]|
error:   but does not override 'IntArray(const IntArray&)' [-Werror=effc++]|
error:   or 'operator=(const IntArray&)' [-Werror=effc++]|
Then you can either remove the “-Weffc++” flag from your compile settings for this example, or you can add the following two lines to the class:

IntArray(const IntArray&) = delete;
IntArray& operator=(const IntArray&) = delete;
We’ll discuss what these do in 13.14 -- Converting constructors, explicit, and delete

This program produces the result:

The value of element 5 is: 6
On the first line, we instantiate a new IntArray class object called ar, and pass in a length of 10. This calls the constructor, which dynamically allocates memory for the array member. We must use dynamic allocation here because we do not know at compile time what the length of the array is (the caller decides that).

At the end of main(), ar goes out of scope. This causes the ~IntArray() destructor to be called, which deletes the array that we allocated in the constructor!

A reminder

In lesson 10.23 -- An introduction to std::vector, we note that parenthesis based initialization should be used when initializing an array/container/list class with a length (as opposed to a list of elements). For this reason, we initialize IntArray using IntArray ar ( 10 );.

Constructor and destructor timing

As mentioned previously, the constructor is called when an object is created, and the destructor is called when an object is destroyed. In the following example, we use cout statements inside the constructor and destructor to show this:

class Simple
{
private:
    int m_nID{};

public:
    Simple(int nID)
        : m_nID{ nID }
    {
        std::cout << "Constructing Simple " << nID << '\n';
    }

    ~Simple()
    {
        std::cout << "Destructing Simple" << m_nID << '\n';
    }

    int getID() { return m_nID; }
};

int main()
{
    // Allocate a Simple on the stack
    Simple simple{ 1 };
    std::cout << simple.getID() << '\n';

    // Allocate a Simple dynamically
    Simple* pSimple{ new Simple{ 2 } };

    std::cout << pSimple->getID() << '\n';

    // We allocated pSimple dynamically, so we have to delete it.
    delete pSimple;

    return 0;
} // simple goes out of scope here
This program produces the following result:

Constructing Simple 1
1
Constructing Simple 2
2
Destructing Simple 2
Destructing Simple 1
Note that “Simple 1” is destroyed after “Simple 2” because we deleted pSimple before the end of the function, whereas simple was not destroyed until the end of main().

Global variables are constructed before main() and destroyed after main().

RAII

RAII (Resource Acquisition Is Initialization) is a programming technique whereby resource use is tied to the lifetime of objects with automatic duration (e.g. non-dynamically allocated objects). In C++, RAII is implemented via classes with constructors and destructors. A resource (such as memory, a file or database handle, etc…) is typically acquired in the object’s constructor (though it can be acquired after the object is created if that makes sense). That resource can then be used while the object is alive. The resource is released in the destructor, when the object is destroyed. The primary advantage of RAII is that it helps prevent resource leaks (e.g. memory not being deallocated) as all resource-holding objects are cleaned up automatically.

The IntArray class at the top of this lesson is an example of a class that implements RAII -- allocation in the constructor, deallocation in the destructor. std::string and std::vector are examples of classes in the standard library that follow RAII -- dynamic memory is acquired on initialization, and cleaned up automatically on destruction.



Summary

As you can see, when constructors and destructors are used together, your classes can initialize and clean up after themselves without the programmer having to do any special work! This reduces the probability of making an error, and makes classes easier to use.

// https://www.learncpp.com/cpp-tutorial/destructors/
