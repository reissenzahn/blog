#include <iostream>

// a friend function is a function that can access the private members of a class as though it were a member of that class

// a friend function may be either a normal function or a member function of another class

// it does not matter whether you declare the friend function in the private or public section of the class

class Accumulator
{
private:
    int m_value { 0 };

public:
    void add(int value) { m_value += value; }

    friend void reset(Accumulator& accumulator);
};

void reset(Accumulator& accumulator)
{
  accumulator.m_value = 0;
}

int main()
{
    Accumulator acc;
    
    acc.add(5);
    
    reset(acc);

    return 0;
}




// a function can be a friend of more than one class at the same time

class Humidity;

class Temperature
{
private:
  int m_temp {};

public:
  Temperature(int temp=0)
    : m_temp {temp}
  {
  }

  friend void printWeather(const Temperature& temperature, const Humidity& humidity);
};

class Humidity
{
private:
  int m_humidity {};

public:
  Humidity(int humidity=0)
    : m_humidity { humidity }
  {
  }

  friend void printWeather(const Temperature& temperature, const Humidity& humidity);
};

void printWeather(const Temperature& temperature, const Humidity& humidity)
{
  std::cout << "The temperature is " << temperature.m_temp << " and the humidity is " << humidity.m_humidity << '\n';
}

int main()
{
    Humidity hum(10);
    Temperature temp(12);

    printWeather(temp, hum);

    return 0;
}



Friend classes

// it is also possible to make an entire class a friend of another class which gives all of the members of the friend class access to the private members of the other class

#include <iostream>

class Storage
{
private:
    int m_nValue {};
    double m_dValue {};
public:
    Storage(int nValue, double dValue)
       : m_nValue { nValue }, m_dValue { dValue }
    {
    }

    // Make the Display class a friend of Storage
    friend class Display;
};

class Display
{
private:
    bool m_displayIntFirst;

public:
    Display(bool displayIntFirst)
         : m_displayIntFirst { displayIntFirst }
    {
    }

    void displayItem(const Storage& storage)
    {
        if (m_displayIntFirst)
            std::cout << storage.m_nValue << ' ' << storage.m_dValue << '\n';
        else // display double first
            std::cout << storage.m_dValue << ' ' << storage.m_nValue << '\n';
    }
};

int main()
{
    Storage storage(5, 6.7);
    Display display(false);

    display.displayItem(storage);

    return 0;
}


// Instead of making an entire class a friend, you can make a single member function a friend. This is done similarly to making a normal function a friend, except using the name of the member function with the className:: prefix included (e.g. Display::displayItem).

However, in actuality, this can be a little trickier than expected. Let’s convert the previous example to make Display::displayItem a friend member function. You might try something like this:

class Display; // forward declaration for class Display

class Storage
{
private:
	int m_nValue {};
	double m_dValue {};
public:
	Storage(int nValue, double dValue)
		: m_nValue { nValue }, m_dValue { dValue }
	{
	}

	// Make the Display::displayItem member function a friend of the Storage class
	friend void Display::displayItem(const Storage& storage); // error: Storage hasn't seen the full definition of class Display
};

class Display
{
private:
	bool m_displayIntFirst {};

public:
	Display(bool displayIntFirst)
		: m_displayIntFirst { displayIntFirst }
	{
	}

	void displayItem(const Storage& storage)
	{
		if (m_displayIntFirst)
			std::cout << storage.m_nValue << ' ' << storage.m_dValue << '\n';
		else // display double first
			std::cout << storage.m_dValue << ' ' << storage.m_nValue << '\n';
	}
};
However, it turns out this won’t work. In order to make a member function a friend, the compiler has to have seen the full definition for the class of the friend member function (not just a forward declaration). Since class Storage hasn’t seen the full definition for class Display yet, the compiler will error at the point where we try to make the member function a friend.

Fortunately, this is easily resolved simply by moving the definition of class Display before the definition of class Storage.

class Display
{
private:
	bool m_displayIntFirst {};

public:
	Display(bool displayIntFirst)
		: m_displayIntFirst { displayIntFirst }
	{
	}

	void displayItem(const Storage& storage) // error: compiler doesn't know what a Storage is
	{
		if (m_displayIntFirst)
			std::cout << storage.m_nValue << ' ' << storage.m_dValue << '\n';
		else // display double first
			std::cout << storage.m_dValue << ' ' << storage.m_nValue << '\n';
	}
};

class Storage
{
private:
	int m_nValue {};
	double m_dValue {};
public:
	Storage(int nValue, double dValue)
		: m_nValue { nValue }, m_dValue { dValue }
	{
	}

	// Make the Display::displayItem member function a friend of the Storage class
	friend void Display::displayItem(const Storage& storage); // okay now
};
However, we now have another problem. Because member function Display::displayItem() uses Storage as a reference parameter, and we just moved the definition of Storage below the definition of Display, the compiler will complain it doesn’t know what a Storage is. We can’t fix this one by rearranging the definition order, because then we’ll undo our previous fix.

Fortunately, this is also fixable in a couple of simple steps. First, we can add class Storage as a forward declaration. Second, we can move the definition of Display::displayItem() out of the class, after the full definition of Storage class.

Here’s what this looks like:

#include <iostream>

class Storage; // forward declaration for class Storage

class Display
{
private:
	bool m_displayIntFirst {};

public:
	Display(bool displayIntFirst)
		: m_displayIntFirst { displayIntFirst }
	{
	}

	void displayItem(const Storage& storage); // forward declaration above needed for this declaration line
};

class Storage // full definition of Storage class
{
private:
	int m_nValue {};
	double m_dValue {};
public:
	Storage(int nValue, double dValue)
		: m_nValue { nValue }, m_dValue { dValue }
	{
	}

	// Make the Display::displayItem member function a friend of the Storage class (requires seeing the full declaration of class Display, as above)
	friend void Display::displayItem(const Storage& storage);
};

// Now we can define Display::displayItem, which needs to have seen the full definition of class Storage
void Display::displayItem(const Storage& storage)
{
	if (m_displayIntFirst)
		std::cout << storage.m_nValue << ' ' << storage.m_dValue << '\n';
	else // display double first
		std::cout << storage.m_dValue << ' ' << storage.m_nValue << '\n';
}

int main()
{
    Storage storage(5, 6.7);
    Display display(false);

    display.displayItem(storage);

    return 0;
}
Now everything will compile properly: the forward declaration of class Storage is enough to satisfy the declaration of Display::displayItem(), the full definition of Display satisfies declaring Display::displayItem() as a friend of Storage, and the full definition of class Storage is enough to satisfy the definition of member function Display::displayItem(). If that’s a bit confusing, see the comments in the program above.

If this seems like a pain -- it is. Fortunately, this dance is only necessary because we’re trying to do everything in a single file. A better solution is to put each class definition in a separate header file, with the member function definitions in corresponding .cpp files. That way, all of the class definitions would have been visible immediately in the .cpp files, and no rearranging of classes or functions is necessary!

Summary

A friend function or class is a function or class that can access the private members of another class as though it were a member of that class. This allows the friend function or friend class to work intimately with the other class, without making the other class expose its private members (e.g. via access functions).

Friending is commonly used when defining overloaded operators (which we’ll cover in a later chapter), or less commonly, when two or more classes need to work together in an intimate way.

Note that making a specific member function a friend requires the full definition for the class of the member function to have been seen first.

// https://www.learncpp.com/cpp-tutorial/friend-functions-and-classes/
