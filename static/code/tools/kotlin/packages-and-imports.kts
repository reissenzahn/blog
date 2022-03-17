// the package specification should be at the top of the source field

// it is not required to match directories and packages and so source files can be placed arbitrarily in the file system

package my.demo

import kotlin.text.*

// ...
Copied!



A source file may start with a package declaration:

package org.example

fun printMessage() { /*...*/ }
class Message { /*...*/ }


All the contents, such as classes and functions, of the source file are included in this package. So, in the example above, the full name of printMessage() is org.example.printMessage, and the full name of Message is org.example.Message.

If the package is not specified, the contents of such a file belong to the default package with no name.

Default imports


// a number of packages are imported into every Kotlin file by default: kotlin.*, kotlin.collections.*, kotlin.io.*, etc.



Imports﻿
Apart from the default imports, each file may contain its own import directives.

You can import either a single name:

import org.example.Message // Message is now accessible without qualification
Copied!
or all the accessible contents of a scope: package, class, object, and so on:

import org.example.* // everything in 'org.example' becomes accessible
Copied!
If there is a name clash, you can disambiguate by using as keyword to locally rename the clashing entity:

import org.example.Message // Message is accessible
import org.test.Message as testMessage // testMessage stands for 'org.test.Message'
Copied!
The import keyword is not restricted to importing classes; you can also use it to import other declarations:

top-level functions and properties

functions and properties declared in object declarations

enum constants


Visibility of top-level declarations
If a top-level declaration is marked private, it is private to the file it's declared in (see Visibility modifiers).


