

1.3 Using the Typescript compiler

```bash
# install TypeScript compiler
$ npm install -g typescript

# 
$ tsc -v

# create tsconfig.json for new project
$ tsc --init

# 
$ vim tsconfig.json
{
  "compilerOptions": {
    "baseUrl": "src",
    "outDir": "./dist",
    "noEmitOnError": true,
    "target": "es5",
    "watch": true
  }
}
```


BASIC TYPE ANNOTATIONS

```ts
// types are provided for the corresponding JavaScript primitives
let text: string = 'hello';
let count: number = 1;
let huge: : bigint = 100n;
let flag: boolean = true;
let unique: symbol = Symbol('unique');

// any indicates a variable can hold values of various types (default type if the compiler cannot infer a type)
let various: any = 'hello';
various = 1;

// the never type is assigned to a function that never returns
function forever() {
  while (true) {}
}

// void is used to indicate that a function does not return a value
function say(text: string) {
  console.log(text);
}

 unknown—A counterpart of any, but no operations are permitted on an unknown
without first asserting or narrowing it to a more specific type

// TypeScript can infer the type of a variable from the context
let msg = 'hello';


// literals can also be used as types in which case the variable can only be assigned to the value of its type
let lucky: 13
lucky = 14  // error

NOTE TypeScript includes other types that are used in interactions with the web browser, such as HTMLElement and Document.



2.1.2 Types in function declarations
TypeScript functions and function expressions are similar to JavaScript functions, but
you can explicitly declare the types of arguments and return values.
 Let’s start by writing a JavaScript function (with no type annotations) that calculates tax. The function in the next listing has three parameters and will calculate tax
based on the state, income, and number of dependents. For each dependent, the person is entitled to a $500 or $300 tax deduction, depending on the state the person
lives in.
Type widening
If you declare a variable without initializing it with a specific value, TypeScript uses
the internal types null or undefined, which are converted to any. This is called type
widening.
The value of the following variable would be undefined.
let productId;
productId = null;
productId = undefined;
The Typescript compiler applies type widening and assigns the type any to null and
undefined values. Hence, the type of the productId variable is any.
It’s worth mentioning that the Typescript compiler supports a --strictNullCheck
option that prohibits the assignment of null to variables with known types. In the
following code snippet, the type of productId is number, and the second and third
lines won’t compile if you turn on --strictNullCheck:
let productId = 123;
productId = null; // compiler error
productId = undefined; // compiler error
The --strictNullCheck option also helps in catching potentially undefined values.
For example, a function may return an object with an optional property, and your code
might wrongly assume that this property is there and try to apply a function on it.
22 CHAPTER 2 Basic and custom types
function calcTax(state, income, dependents) {
if (state === 'NY') {
return income * 0.06 - dependents * 500;
} else if (state === 'NJ') {
return income * 0.05 - dependents * 300;
}
}
Suppose a person with an income of $50,000 lives in the state of New Jersey and has
two dependents. Let’s invoke calcTax():
let tax = calcTax('NJ', 50000, 2);
The tax variable gets the value of 1,900, which is correct. Even though calcTax()
didn’t declare any types for the function parameters, we guessed how to call this function based on the parameter names.
 What if we didn’t guess it right? Let’s invoke it the wrong way, passing a string
value for the number of dependents:
let tax = calcTax('NJ', 50000, 'two');
You won’t know there’s a problem until you invoke this function. The tax variable will
have a NaN value (not a number). A bug sneaked in just because you couldn’t explicitly
specify the types of the parameters, and the compiler couldn’t infer the types of the
function arguments.
 The next listing shows a TypeScript version of this function, using type annotations
for the function arguments and return value.
function calcTax(state: string, income: number, dependents: number) : number {
if (state === 'NY'){
return income * 0.06 - dependents * 500;
} else if (state ==='NJ'){
return income * 0.05 - dependents * 300;
}
}
Now there’s no way to make the same mistake and pass a string value for the number
of dependents:
let tax: number = calcTax('NJ', 50000, 'two');
Listing 2.4 Calculating tax in JavaScript
Listing 2.5 Calculating tax in TypeScript
The function arguments have no type annotations.
Calculates the New York tax
Calculates the New Jersey tax
The function arguments
and its return value
have type annotations.
Declaring variables with types 23
The Typescript compiler will display an error: “Argument of type string is not assignable to parameter of type number.” Moreover, the return value of the function is
declared as number, which stops you from making another mistake and assigning the
result of the tax calculation to a non-numeric variable:
let tax: string = calcTax('NJ', 50000, 'two');
The compiler will catch this, producing the error “The type ‘number’ is not assignable
to type ‘string’: var tax: string.” This kind of type-checking during compilation can
save you a lot of time on any project.
2.1.3 The union type
Unions allow you to express that a value can be one of several types. You can declare a
custom type based on two or more existing types. For example, you can declare a variable of a type that can accept either a string value or a number (the vertical bar means
union):
let padding: string | number;
Although the padding variable can store the value of either of the two specified types,
at any given time it can be only of one type—either a string or a number.
 TypeScript supports the type any, but the preceding declaration provides some
benefits compared to the declaration let padding: any. Listing 2.6 shows one a code
sample from the TypeScript documentation (see http://mng.bz/5742). This function
can add the left padding to the provided string. The padding can be specified either
as a string that will prepend the provided value or as a number of spaces that should
prepend the provided string.
Fixing the calcTax() function
This section has JavaScript and TypeScript versions of the calcTax() function, but
they only process two states: NY and NJ. Invoking either of these functions for any
other state will return undefined at runtime.
The Typescript compiler won’t warn you that the function in listing 2.5 is poorly written
and may return undefined, but the TypeScript syntax allows you to warn the person
who reads this code that the function in listing 2.5 may return not only a number but
also an undefined value, if you invoke it with any other state but NY or NJ. You
should change this function signature to declare such a use case as follows:
function calcTax(state: string, income: number, dependents: number) :
➥ number | undefined
24 CHAPTER 2 Basic and custom types
function padLeft(value: string, padding: any ): string {
if (typeof padding === "number") {
return Array(padding + 1).join(" ") + value;
}
if (typeof padding === "string") {
return padding + value;
}
throw new Error(`Expected string or number, got '${padding}'.`);
}
The following listing illustrates the use of padLeft():
console.log( padLeft("Hello world", 4));
console.log( padLeft("Hello world", "John says "));
console.log( padLeft("Hello world", true));
TIP In section 2.2.4, we’ll explain the structural type system implemented in
TypeScript. In short, an object created using the object literal syntax (the syntax with curly braces) can be used where an object of a class (such as Person)
is expected if the object literal has the same properties as Person. Because of
this, if (person instanceof Person) may give you a false negative if the
variable person points at an object that was not created by the constructor of
the class Person.
Listing 2.6 padLeft with the any type
Listing 2.7 Invoking the padLeft function
Provides the string and the padding of type any
For a numeric argument,
generates spaces
 For a string, uses concatenation
If the second argument is
neither a string nor a
number, throws an error
Returns “Hello world”
Returns “John says
Hello world”
Runtime error
Type guards typeof and instanceof
An attempt to apply conditional statements to refine a variable’s type is called type
narrowing. In the if statement in listing 2.6, we used the typeof type guard to narrow the type of a variable that can store more than one TypeScript type. We used
typeof to find out the actual type of padding at runtime.
Similarly, the instanceof type guard is used with custom types (with constructors),
as will be explained in section 2.2. The instanceof guard allows you to check the
actual object type at runtime:
if (person instanceof Person) {...}
The difference between typeof and instanceof is that the former is used with the
built-in TypeScript types and the latter with the custom ones.
Declaring variables with types 25
If we now change the type of padding to the union of string and number (as shown in
the following listing), the compiler will report an error if you try to invoke padLeft()
providing anything other than string or number. This will also eliminate the need to
throw an exception.
function padLeft(value: string, padding: string | number ): string {
if (typeof padding === "number") {
return Array(padding + 1).join(" ") + value;
}
if (typeof padding === "string") {
return padding + value;
}
}
Now invoking padLeft() with the wrong type for the second argument (such as true)
returns a compilation error:
console.log( padLeft("Hello world", true)); // compilation error
TIP If you need to declare a variable that can hold values of more than one
type, don’t use the type any; use a union such as let padding: string | number.
Another choice is to declare two separate variables: let paddingStr: string;
let paddingNum: number;.
Let’s modify the code in listing 2.8 to illustrate the type never by adding an else
clause to the if statement. This next listing shows how the type checker will infer the
never type for an impossible value.
function padLeft(value: string, padding: string | number ): string {
if (typeof padding === "number") {
return Array(padding + 1).join(" ") + value;
}
if (typeof padding === "string") {
return padding + value;
}
else {
return padding;
}
}
Since we declared in the function signature that the padding argument can be either
string or number, any other value for padding is impossible. In other words, the else
case is not possible, and the type checker will infer the type never for the padding variable in the else clause. You can see it for yourself by copying the code from listing 2.9
into the TypeScript Playground and hovering the mouse over the padding variable.
Listing 2.8 padLeft with the union type
Listing 2.9 The never type of an impossible value
Allows only a string or
a number as a second
argument
This else block is
never executed.
26 CHAPTER 2 Basic and custom types
NOTE Another benefit of using the union type is that IDEs have an autocomplete feature that will prompt you with allowed argument types, so you won’t
even have the chance to make such a mistake.
Compare the code of the padLeft() functions in listings 2.6 and 2.9. What are the
main benefits of using the string | number union versus the any type for the second
argument? If you use the union, the Typescript compiler will prevent incorrect invocations of padLeft() by reporting an error at compile time.
 We just used a union of primitive types (string and number), but in the next section you’ll see how to declare unions of custom types.
2.2 Defining custom types
TypeScript allows you to create custom types with the type keyword, by declaring a
class or an interface, or by declaring an enum (covered in chapter 4). Let’s get familiar
with the type keyword first.
2.2.1 Using the type keyword
The type keyword allows you to declare a new type or a type alias for an existing type.
Let’s say your app deals with patients who are represented by their name, height,
and weight. Both height and weight are numbers, but to improve the readability of
your code, you can create aliases hinting at the units in which the height and weight
are measured.
type Foot = number;
type Pound = number;
You can create a new Patient type and use the preceding aliases in its declaration.
type Patient = {
name: string;
height: Foot;
weight: Pound;
}
Declarations of type aliases don’t generate code in the compiled JavaScript. In TypeScript, declaring and initializing a variable of type Patient can look like the following.
let patient: Patient = {
name: 'Joe Smith',
height: 5,
weight: 100
}
Listing 2.10 Declaring alias types Foot and Pound
Listing 2.11 Declaring a new type that uses aliases
Listing 2.12 Declaring and initializing a type’s properties
Declares the Patient type
Uses the type alias Foot
Uses the type alias Pound
We create an instance using
the object literal notation.
Defining custom types 27
What if, while initializing the patient variable, you forget to specify the value of one
of the properties, such as weight?
let patient: Patient = {
name: 'Joe Smith',
height: 5
}
TypeScript will complain:
"Type '{ name: string; height: number; }' is not assignable to type 'Patient'.
Property 'weight' is missing in type '{ name: string; height: number; }'."
If you want to declare some of the properties as optional, you must add a question
mark to their names. In the following type declaration, providing the value for the
weight property is optional, and there won’t be any errors.
type Patient = {
name: string;
height: Height;
weight?: Weight;
}
let patient: Patient = {
name: 'Joe Smith',
height: 5
}
TIP You can use the question mark to define optional properties in classes or
interfaces as well. You’ll get familiar with TypeScript classes and interfaces
later in this section.
You can also use the type keyword to declare a type alias for a function signature.
Imagine you’re writing a framework that should allow you to create form controls and
assign validator functions to them. A validator function must have a specific signature—
it must accept an object of type FormControl and return either an object describing the
errors of the form control value, or null if the value is valid. You can declare a new
ValidatorFn type as follows:
type ValidatorFn =
(c: FormControl) => { [key: string]: any }| null
Here, { [key: string]: any } means an object that can have properties of any type,
but the key has to be either of type string or convertable to a string.
Listing 2.13 Forgetting to add the weight property
Listing 2.14 Declaring optional properties
The weight property
is optional.
The patient variable is
initialized without the weight.
28 CHAPTER 2 Basic and custom types
 The constructor of the FormControl can have a parameter for the validator function, and it can use the custom ValidatorFn type as follows:
class FormControl {
constructor (initialValue: string, validator: ValidatorFn | null) {...};
}
TIP In the appendix, you can see the syntax for declaring optional function
parameters in JavaScript. The preceding code snippet shows you a way of
declaring an optional parameter using the TypeScript union type.
2.2.2 Using classes as custom types
We assume you’re familiar with the JavaScript classes covered in the appendix. In this
section we’ll start showing you additional features that TypeScript brings to JavaScript
classes. In chapter 3, we’ll continue covering classes in more detail.
 JavaScript doesn’t offer any syntax for declaring class properties, but TypeScript
does. In figure 2.2, on the left, you can see how we declared and instantiated a Person
class that has three properties. The right side of figure 2.2 shows the ES6 version of
this code produced by the Typescript compiler.
As you can see, there are no properties in the JavaScript version of the Person class.
Also, since the Person class didn’t declare a constructor, we had to initialize its properties after instantiation. A constructor is a special function that’s executed once when
the instance of a class is created.
 Declaring a constructor with three arguments would allow you to instantiate the
Person class and initialize its properties in one line. In TypeScript you can provide
type annotations for a constructor’s arguments, but there’s more.
 TypeScript offers the access level qualifiers public, private, and protected (covered in chapter 3), and if you use any of them with the constructor arguments, the
Figure 2.2 The Person class compiled into JavaScript (ES6)
Defining custom types 29
Typescript compiler will generate the code for adding these arguments as properties
in the generated JavaScript object (see figure 2.3).
 Now the code of the TypeScript class (on the left) is more concise, and the generated
JavaScript code creates three properties in the constructor. Note line 6 in figure 2.3 on
the left. We declared the constant without specifying its type, but we could rewrite this
line explicitly specifying the type of p as follows:
const p: Person = new Person("John", "Smith", 25);
This would be an unnecessary use of an explicit type annotation. Since you declare a
constant and immediately initialize it with an object of a known type (Person), the
TypeScript type checker can easily infer and assign the type to the constant p. The
generated JavaScript code will look the same regardless of whether you specify the
type of p or not. You can see how to instantiate the class Person without explicitly
declaring its type by following this link to the TypeScript Playground: http://mng
.bz/zlV1. Hover the mouse pointer over the variable p—its type is Person.
TIP In figure 2.3, we used the public access level with each constructor argument in the TypeScript class, which simply means that the generated corresponding properties can be accessed from any code located both inside and
outside of the class.
When you declare properties of a class, you can also mark them as readonly. Such
properties can be initialized either at the declaration point or in the class constructor,
and their values can’t be changed afterwards. The readonly qualifier is similar to the
const keyword, but the latter can’t be used with class properties.
 In chapter 8, we’ll start developing a blockchain app, and any blockchain consists
of blocks with immutable properties. That app will include a Block class, and a fragment of it follows.
Figure 2.3 The Person class with a constructor
30 CHAPTER 2 Basic and custom types
class Block {
readonly nonce: number;
readonly hash: string;
constructor (
readonly index: number,
readonly previousHash: string,
readonly timestamp: number,
readonly data: string
) {
const { nonce, hash } = this.mine();
this.nonce = nonce;
this.hash = hash;
}
// The rest of the code is omitted for brevity
}
The Block class includes six readonly properties. Note that we don’t need to explicitly declare class properties for constructor arguments that have readonly, private,
protected, or public qualifiers as we would in other object-oriented languages. In
listing 2.15, two class properties are declared explicitly and four implicitly.
2.2.3 Using interfaces as custom types
Many object-oriented languages include a syntax construct called interface, which is
used to enforce the implementation of specified properties or methods on an object.
JavaScript doesn’t support interfaces, but TypeScript does. In this section, we’ll show
you how to use interfaces to declare custom types, and in chapter 3, you’ll see how to
use interfaces to ensure that a class implements the specified members.
 TypeScript includes the keywords interface and implements to support interfaces, but interfaces aren’t compiled into JavaScript code. They just help you to avoid
using wrong types during development. Let’s get familiar with using the interface
keyword to declare a custom type.
 Suppose you want to write a function that can save information about people in
some storage. This function should take an object that represents a person, and you
want to ensure that it has specific properties of specific types. You can declare a
Person interface as follows.
interface Person {
firstName: string;
lastName: string;
age: number;
}
Listing 2.15 The properties of the Block class
Listing 2.16 Declaring a custom type using an interface
This property is initialized
in the constructor.
The value for this property is
provided to the constructor
during instantiation.
Uses destructuring to extract the
values from the object returned
by the mine() method
Defining custom types 31
The script on left side of figure 2.2 declares a similar custom Person type but using the
class keyword. What’s the difference? If you declare a custom type as class, you can
use it as a value, meaning you can instantiate it using the new keyword, as shown in figures 2.2 and 2.3.
 Also, if you use the class keyword in the TypeScript code, it’ll have the corresponding code in generated JavaScript (a function in ES5, and a class in ES6). If you
use the interface keyword, there won’t be any corresponding code in JavaScript, as
seen in figure 2.4.
There is no mention of the interface on the right side of figure 2.4, and the JavaScript
is more concise, which is good for any deployable code. But during development, the
compiler will check that the object given as an argument to the savePerson() function includes all the properties declared in the Person interface.
 We encourage you to experiment with the code snippet from figure 2.4 by visiting
TypeScript Playground at the following link: http://mng.bz/MOpB. For example,
remove the lastName property defined in line 13, and the TypeScript type checker will
immediately underline the variable p with a red line. Hover your mouse pointer over
the variable p, and you’ll see the following error message:
Type '{ firstName: string; age: number; }' is not assignable to type 'Person'.
Property 'lastName' is missing in type '{ firstName: string; age: number; }'.
Keep experimenting. Try to access person.lastName inside savePerson(). If the Person interface won’t declare the lastName property, TypeScript will give you a compiler
error, but the JavaScript code would just crash at runtime.
 Try another experiment: remove the Person type annotation in line 11. The code
is still valid and no errors are reported in line 17. Why does TypeScript allow you to
invoke the savePerson() function with an argument that wasn’t explicitly assigned
Figure 2.4 The custom Person type as an interface
32 CHAPTER 2 Basic and custom types
the type Person? The reason is that TypeScript uses a structural type system, which
means that if two different types include the same members, the types are considered
compatible. We’ll discuss the structural type system in more detail in the next section.
2.2.4 Structural vs. nominal type systems
A primitive type has just a name (such as number), whereas a more complex type like
an object or class has a name and some structure, represented by properties (for
example, a Customer class likely has name and address properties).
 How would you know if two types are the same or not? In Java (which uses a nominal type system), two types are the same if they have the same names declared in the
same namespace (a.k.a. packages). In a nominal type system, if you declare a variable
of type Person, you can assign to it only an object of type Person or its descendant. In
Which keyword to use: type, interface, or class?
We’ve shown you that a custom type can be declared using the keywords type,
class, or interface. Which of these keywords should you use for declaring a custom type like Person?
If the custom type doesn’t need to be used for instantiating objects at runtime, use
interface or type; otherwise use class. In other words, use class for creating a
custom type if it should be used to represent a value.
For example, if you declare a Person interface and there’s a function that takes an
argument of type Person, you can’t apply the operator instanceof to the argument:
interface Person {
name: string;
}
function getP(p: Person){
if (p instanceof Person){ // compile error
}
}
The type checker will complain that Person only refers to a type but is being used as
a value here.
If you are declaring a custom type just for the additional safety offered by TypeScript’s
type checker, use type or interface. Neither interfaces nor types declared with the
type keyword have representations in the emitted JavaScript code, which makes the
runtime code smaller (bytewise). If you use classes for declaring types, they will have
a footprint in the generated JavaScript.
Defining a custom type with the type keyword offers the same features as interface plus some extras. For example, you can’t use types declared as interfaces in
unions or intersections. Also, in chapter 5, you’ll learn about conditional types, which
can’t be declared using interfaces. 
Defining custom types 33
Java, the last line in the following listing won’t compile, because the names of the
classes are not the same, even though they have the same structure.
class Person {
String name;
}
class Customer {
String name;
}
Customer cust = new Person();
But TypeScript and some other languages use a structural type system. The following listing shows the preceding code snippet rewritten in TypeScript.
class Person {
name: string;
}
class Customer {
name: string;
}
const cust: Customer = new Person();
This code doesn’t report any errors because TypeScript uses a structural type system,
and since both the Person and Customer classes have the same structure, it’s OK to
assign an instance of one class to a variable of another.
 Moreover, you can use object literals to create objects and assign them to classtyped variables or constants, as long as the shape of the object literal is the same. The
following listing will compile without errors.
class Person {
name: String;
}
class Customer {
name: String;
}
const cust: Customer = { name: 'Mary' };
const pers: Person = { name: 'John' };
Listing 2.17 A Java code snippet
Listing 2.18 A TypeScript code snippet
Listing 2.19 Compatible types
Declares the Person
class (think “type”)
Declares the Customer class
Syntax error: the names of the classes
on the left and right are not the same.
Declares the Person
class (think “type”)
Declares the Customer class
No errors: the type
structures are the same.
34 CHAPTER 2 Basic and custom types
TIP The access-level modifiers affect type compatibility. For example, if we
declared the name property of the Person class as private, the code in listing
2.19 wouldn’t compile.
Our classes didn’t define any methods, but if both of them defined a method with the
same signature (name, arguments, and return type), they would again be compatible.
 What if the structure of Person and Customer are not exactly the same? Let’s add
an age property to the Person class.
class Person {
name: String;
age: number;
}
class Customer {
name: String;
}
const cust: Customer = new Person(); // still no errors
Still no errors! TypeScript sees that Person and Customer have the same shape (something in common). We just wanted to use the constant of type Customer (it has the
property name) to point at the object of type Person, which also has the property name.
 What can you do with the object represented by the cust variable? You can write
something like cust.name='John'. The instance of Person has the name property, so
the compiler doesn’t complain.
NOTE Because we can assign an object of type Person to a variable of type
Customer, we can say that the Person type is assignable to the Customer type.
Take a look at this code in TypeScript Playground: http://mng.bz/adQm. Click Ctrlspace after the dot in cust. and you’ll see that only the name property is available,
even though the Person class has also the age property.
 In listing 2.20, the Person class had more properties than Customer, and the code
compiled without errors. Would the code in the next listing compile if the Customer
class had more properties than Person?
class Person {
name: string;
}
class Customer {
name: string;
age: number;
}
const cust: Customer = new Person();
Listing 2.20 When classes are not the same
Listing 2.21 The instance has more properties than a reference variable
We’ve added this property.
The types don’t match.
Defining custom types 35
The code in listing 2.21 wouldn’t compile because the cust reference variable would
point to a Person object that wouldn’t even allocate memory for the age property, and
an assignment like cust.age = 29 wouldn’t be possible. This time, the Person type is
not assignable to the Customer type.
TIP We’ll come back to TypeScript’s structural typing in section 4.2 when we
discuss generics.
2.2.5 Unions of custom types
In the previous section, we introduced union types that allow you to declare that a
variable can have one of the listed types. For example, in listing 2.8 we specified that
the function argument padding could be either a string or a number. This is an example of a union that included primitive types.
 Let’s look at declaring a union of custom types. Imagine an app that can perform
various actions in response to the user’s activity. Each action is represented by a class
with a different name. Each action must have a type and optionally may carry a payload, such as a search query. The following listing includes declarations for three
action classes and a union SearchActions type.
export class SearchAction {
actionType = "SEARCH";
constructor(readonly payload: {searchQuery: string}) {}
}
export class SearchSuccessAction {
actionType = "SEARCH_SUCCESS";
constructor(public payload: {searchResults: string[]}) {}
}
export class SearchFailedAction {
actionType = "SEARCH_FAILED";
}
export type SearchActions = SearchAction | SearchSuccessAction |
➥ SearchFailedAction;
TIP The code in listing 2.22 needs improvement, because just stating that
each action must have a property describing its type is more of a JavaScript
style of programming. In TypeScript, such a statement can be enforced programmatically, and we’ll do that in section 3.2.1.
Discriminated unions include type members that have a common property—the discriminant. Depending on the value of the discriminant, you may want to perform different
actions.
Listing 2.22 Using a union to represent actions in the actions.ts file
A class with an
action type and
payload
A class with an action type
but without a payload
A union type declaration
36 CHAPTER 2 Basic and custom types
 The union shown in listing 2.22 is an example of a discriminated union because
each member has an actionType discriminant. Let’s create another discriminated
union of two types, Rectangle and Circle.
interface Rectangle {
kind: "rectangle";
width: number;
height: number;
}
interface Circle {
kind: "circle";
radius: number;
}
type Shape = Rectangle | Circle;
The Shape type is a discriminated union, and both Rectangle and Circle have a common property, kind. Depending on the value in the kind property, we can calculate
the area of the Shape differently.
function area(shape: Shape): number {
switch (shape.kind) {
case "rectangle": return shape.height * shape.width;
case "circle": return Math.PI * shape.radius ** 2;
}
}
const myRectangle: Rectangle = { kind: "rectangle", width: 10, height: 20 };
console.log(`Rectangle's area is ${area(myRectangle)}`);
const myCircle: Circle = { kind: "circle", radius: 10};
console.log(`Circle's area is ${area(myCircle)}`);
You can run this code sample in the Playground at http://mng.bz/gVev.
Listing 2.23 Using a union with a discriminant to tell shapes apart
Listing 2.24 Using a discriminated union
The discriminant
The union
Switches on the discriminator’s value
Applies the formula
for rectangles
Applies the formula for circles
The in type guard
The in type guard acts as a narrowing expression for types. For example, if you have
a function that can take an argument of a union type, you can check the actual type
given during function invocation.
The following code shows two interfaces with different properties. The foo() function
can take an object A or B as an argument. Using the in type guard, the foo() function
can check if the provided object has a specific property before using it.
The any and unknown types, and user-defined type guards 37
2.3 The any and unknown types, and user-defined type guards
At the beginning of this chapter, we mentioned the types any and unknown. In this section, we’ll show you the difference between them. You’ll also see how to write a custom type guard, in addition to typeof, instanceof, and in.
 Declaring a variable of any type allows you to assign it a value of any type. It’s like
writing in JavaScript where you don’t specify a type. Similarly, trying to access a nonexistent property on an object of any type may give unexpected results at runtime.
 The unknown type was introduced in TypeScript 3.0. If you declare a variable of
type unknown, the compiler will force you to narrow its type down before accessing its
properties, sparing you from potential runtime surprises.
 To illustrate the difference between any and unknown, let’s assume that we declared
the Person type on the frontend, and it’s populated with data coming from the backend in JSON format. To turn the JSON string into an object, we’ll use the method
JSON.parse(), which returns any.
type Person = {
address: string;
}
let person1: any;
person1 = JSON.parse('{ "adress": "25 Broadway" }');
console.log(person1.address);
The last line will print undefined because we misspelled “address” in the JSON string.
The parse method returns a JavaScript object that has an adress property, but not
address on person1. To experience this issue, you need to run this code.
Listing 2.25 Using the any type
interface A { a: number };
interface B { b: string };
function foo(x: A | B) {
if ("a" in x) {
return x.a;
}
return x.b;
}
The property that you check has to be a string, such as "a".
Checks for a specific
property using in
Declares a type alias
Declares a variable of type any
Parses the JSON string
This prints undefined.
38 CHAPTER 2 Basic and custom types
 Now let’s see how the same use case works with a variable of type unknown.
let person2: unknown;
person2 = JSON.parse('{ "adress": "25 Broadway" }');
console.log(person2.address);
This time the last line won’t even compile because we tried to use the person2 variable
of unknown type without narrowing its type down.
 TypeScript allows you to write user-defined type guards that can check if an object
is of a particular type. This would be a function that returns something like “thisFunctionArg is SomeType.” Let’s write an isPerson() type guard which assumes that
if the object under test has an address property, it’s a person.
const isPerson = (object: any): object is Person => "address" in object;
This type guard returns true if the given object has an address property. You can
apply this guard as in the following listing.
if (isPerson(person2)) {
console.log(person2.address);
} else {
console.log("person2 is not a Person");
}
This code has no compilation errors, and it works as expected unless the isPerson()
guard gets a falsy object as an argument. For example, passing null to isPerson() will
result in a runtime error in the expression "address" in object.
 The following listing shows a safer version of the isPerson() guard. The doublebang operator !! will ensure that the given object is truthy.
const isPerson = (object: any): object is Person => !!object && "address"
➥ in object;
You can try this code in the TypeScript Playground at http://mng.bz/eDaV.
Listing 2.26 The compiler’s error with the type unknown
Listing 2.27 The first version of the isPerson type guard
Listing 2.28 Applying the isPerson type guard
Listing 2.29 The isPerson type guard
Declares a variable of type unknown
Attempting to use a variable
of the unknown type results
in a compilation error.
Applies the type guard
Safely accesses the
address property
A mini project 39
 In this example, we assumed that the existence of the address property is enough
to identify the Person type, but in some cases checking just one property is not
enough. For example, the Organization or Pet classes may also have address properties. You may need to check several properties to determine if an object matches a specific type.
 A simpler solution is to declare your own discriminator property that will identify
this type as a person:
type Person = {
discriminator: 'person';
address: string;
}
Then your custom type guard could look like this:
const isPerson = (object: any): object is Person => !!object &&
➥ object.discriminator === 'person';
OK, we’ve covered just enough of the TypeScript syntax related to types. It’s time to
apply the theory in practice.
2.4 A mini project
If you’re the type of person who prefers learning by doing, we can offer you a little
assignment, followed by a solution. We won’t provide detailed explanations about the
solution—the description of the assignment should make it clear enough.
 Write a program with two custom types, Dog and Fish, which are declared using
classes. Each of these types must have a name property. The Dog class should have a
sayHello(): string method, and the Fish class should have a dive(howDeep: number): string method.
 Declare a new Pet type as a union of Dog and Fish. Write a talkToPet(pet: Pet):
string function that will use type guards and will either invoke the sayHello()
method on the Dog instance or print the message “Fish cannot talk, sorry.”
 Invoke talkToPet() three times providing the object Dog first, then Fish, and
finally an object that is neither Dog nor Fish.
 Our solution is shown in the following listing.
class Dog {
constructor(readonly name: string) { };
sayHello(): string {
return 'Dog says hello!';
}
}
class Fish {
Listing 2.30 The solution
Declares a custom Dog type
Declares a custom Fish type
40 CHAPTER 2 Basic and custom types
constructor(readonly name: string) { };
dive(howDeep: number): string {
return `Diving ${howDeep} feet`;
}
}
type Pet = Dog | Fish;
function talkToPet(pet: Pet): string | undefined {
if (pet instanceof Dog) {
return pet.sayHello();
} else if (pet instanceof Fish) {
return 'Fish cannot talk, sorry.';
}
}
const myDog = new Dog('Sammy');
const myFish = new Fish('Marry');
console.log(talkToPet(myDog));
console.log(talkToPet(myFish));
talkToPet({ name: 'John' });
You can see this script in action on CodePen at http://mng.bz/pyjK.
Summary
 Although declaring variable types forces developers to write more code, their
productivity increases in the long run.
 TypeScript offers a number of type annotations, but you can declare custom
types as needed.
 You can create new types by declaring a union of existing ones.
 You can declare custom types using the type, interface, and class keywords.
In chapter 4, you’ll see yet another way of declaring custom types with the enum
keyword.
 TypeScript uses a structural type system as opposed to languages like Java or C#,
which use a nominal type system.
Creates a union of Dog and Fish
Uses a type guard
Creates an instance of a Dog
Creates an instance of a Fish
Invokes talkToPet(),
passing a Pet
This won’t compile—wrong parameter type
41
Object-oriented
 programming with
 classes and interfaces
In chapter 2, we introduced classes and interfaces for creating custom types. In this
chapter, we’ll continue learning about classes and interfaces from the objectoriented programming (OOP) perspective. OOP is a programming style where your
programs focus on handling objects rather than on composing actions (think
functions). Of course, some of those functions would create objects as well, but in
OOP, objects are the center of the universe.
This chapter covers
 How class inheritance works
 Why and when to use abstract classes
 How interfaces can force a class to have methods
with known signatures without worrying about
implementation details
 What “programming to interfaces” means
42 CHAPTER 3 Object-oriented programming with classes and interfaces
 Developers who work with object-oriented languages use interfaces as a way to
enforce certain APIs on classes. Also, you can often hear the phrase “program to interfaces” in the conversations of programmers. In this chapter, we’ll explain what that
means. In short, this chapter is a whirlwind tour of OOP using TypeScript.
3.1 Working with classes
Let’s recap what you learned about TypeScript classes in chapter 2:
 You can declare classes with properties, which in other object-oriented languages are called member variables.
 As in JavaScript, classes may declare constructors, which are invoked once
during instantiation.
 The Typescript compiler converts classes into JavaScript constructor functions if
ES5 is specified as the compilation target syntax. If ES6 or later is specified as
the target, TypeScript classes will be compiled into JavaScript classes.
 If a class constructor defines arguments that use such keywords as readonly,
public, protected, or private, TypeScript creates class properties for each of
the arguments.
But there’s more to classes. In this chapter, we’ll cover class inheritance, what the
abstract classes are for, and what the public, protected, and private access modifiers
are for.
3.1.1 Getting familiar with class inheritance
In real life, every person inherits some features from their parents. Similarly, in the
TypeScript world, you can create a new class based on an existing one. For example,
you can create a Person class with some properties, and then create an Employee class
that will inherit all the properties of Person as well as declare additional ones. Inheritance is one of the main features of any object-oriented language, and the extends
keyword declares that one class inherits from another.
 Figure 3.1 is a screenshot from the TypeScript Playground (http://mng.bz/O9Yw).
Note that we didn’t use explicit types in declaring the properties of the Person class. We
initialized the firstName and lastName properties with empty strings and age with 0.
The Typescript compiler will infer the types based on the initial values.
TIP In the TypeScript Playground’s Config menu, the strictPropertyInitialization compiler option is on. With this option, if the class properties are not initialized either when declared or in the class’s constructor, the
compiler reports an error.
Line 7 in figure 3.1 shows how you can declare an Employee class that extends the
Person class and declares an additional department property. In line 11 we create an
instance of the Employee class.
Working with classes 43
This screenshot was taken after we entered empl. followed by Ctrl-space on line 13.
TypeScript’s static analyzer recognizes that the Employee type is inherited from Person, so it suggests the properties defined in both the Person and Employee classes.
 In our example, the Employee class is a subclass of Person. Accordingly, the Person
class is a superclass of Employee. You can also say that the Person class is an ancestor, and
that Employee is a descendant of Person.
NOTE Under the hood, JavaScript supports prototypal object-based inheritance, where one object can be assigned to another object as its prototype—
this happens at runtime. Once TypeScript code that uses inheritance is compiled, the generated JavaScript code uses the syntax of prototypal inheritance.
In addition to properties, a class can include methods—this is how we call functions
declared inside classes. And if a method is declared in a superclass, it will be inherited
by the subclass unless the method was declared with the private access qualifier,
which we’ll discuss a bit later.
 The next version of the Person class is shown in figure 3.2, and it includes the sayHello() method. (You can find this code in the Playground at http://mng.bz/YeNz.)
As you can see in line 18, TypeScript’s static analyzer included this method in the autocomplete dropdown.
 You may be wondering, “Is there any way to control which properties and methods
of a class are accessible from other scripts?” The answer is yes—this is what the private, protected, and public keywords are for.
Figure 3.1 Class inheritance in TypeScript
44 CHAPTER 3 Object-oriented programming with classes and interfaces
3.1.2 Access modifiers public, private, protected
TypeScript includes the public, protected, and private keywords to control access
to the members of a class (properties or methods).
 public—Class members marked as public can be accessed from the internal
class methods as well as from external scripts. This is the default access, so if you
place the keyword public in front of a property or method of the Person class
shown in figure 3.2, the accessibility of these class members won’t change.
 protected—Class members marked as protected can be accessed either from
the internal class code or from class descendants.
 private—The private class members are visible only within the class.
NOTE If you know languages like Java or C#, you may be familiar with restricting the access level with the private and protected keywords. TypeScript is a
superset of JavaScript, which doesn’t support the private keyword, so the
keywords private and protected (as well as public) are removed during
code compilation. The resulting JavaScript won’t include these keywords, so
you can consider them just a convenience during development.
Figure 3.2 The sayHello() method from a superclass is visible.
Working with classes 45
Figure 3.3 illustrates the protected and private access level modifiers. In line 15, we
can access the protected ancestor’s sayHello() method because we do this from the
descendant. But when we clicked Ctrl-space after this. in line 20, the age variable is
not shown in the autocomplete list because it’s declared as private and can be
accessed only within the Person class.
 This code sample shows that the subclass can’t access the private member of the
superclass (try it in the Playground at http://mng.bz/07gJ). Here, only a method
from the Person class can access private members from this class.
 Although protected class members are accessible from the descendant’s code, they
are not accessible on the class instance. For example, the following code won’t compile and will give you the error, “Property ‘sayHello’ is protected and only accessible
within class ‘Person’ and its subclasses.”
const empl = new Employee();
empl.sayHello(); // error
Let’s look at another example of the Person class that has a constructor, two public
properties, and one private property, as shown in figure 3.4 (or in the Playground at
Figure 3.3 The age private property is not visible
46 CHAPTER 3 Object-oriented programming with classes and interfaces
http://mng.bz/KEgX). This is a verbose version of the class declaration because we
explicitly declared three properties of the class. The constructor in the Person class
performs the tedious job of assigning values from its arguments to the respective
properties of this class.
 Now let’s declare a more concise version of the Person class, as shown in figure 3.5
(or in the Playground at http://mng.bz/9w9j). By using access qualifiers with the constructor’s arguments, we can instruct the Typescript compiler to create class properties
having the same names as the constructor’s arguments. The compiler will autogenerate
JavaScript code that assigns the values given to the constructor to class properties.
 In line 7 of figure 3.5, we create an instance of the Person class, passing the initial
property values to its constructor, which will assign these values to the respective
object’s properties. In line 9 we wanted to print the values of the object’s firstName
and age properties, but the latter became marked with a red squiggly line because age
is private.
 Compare figures 3.4. and 3.5. In figure 3.4, the Person class explicitly declares three
properties, which we initialize in the constructor. In figure 3.5, the Person class has no
explicit declarations of properties and no explicit initialization in the constructor.
Figure 3.4 A verbose version of the Person class
Figure 3.5 Using access qualifiers with the constructor’s arguments
Working with classes 47
So what’s better—explicit or implicit declaration of class properties? There are arguments for both programming styles. Explicit declaration and initialization of class
properties may increase the readability of the code, whereas implicit declaration
makes the code of the TypeScript class more concise. But it doesn’t have to be an
either-or decision. For example, you can declare public properties explicitly and private and protected ones implicitly. We generally use implicit declarations unless the
property initialization involves some logic.
3.1.3 Static variables and a singleton example
In the ES6 version of JavaScript, when a property has to be shared by each instance of
a class, we can declare it as static (see section A.9.3 in the appendix). Being a superset of JavaScript, TypeScript also supports the static keyword. In this section we’ll
look at a basic example, and then we’ll implement the singleton design pattern with
the help of a static property and a private constructor.
 Suppose a group of gangsters is on a mission (no worries—it’s just a game). We
need to monitor the total number of bullets they have left. Every time a gangster
shoots, this value has to be decreased by one. The total number of bullets should be
known to each gangster.
class Gangsta {
static totalBullets = 100;
shoot(){
Gangsta.totalBullets--;
console.log(`Bullets left: ${Gangsta.totalBullets}`);
}
}
const g1 = new Gangsta();
g1.shoot();
const g2 = new Gangsta();
g2.shoot();
After running the code shown in listing 3.1 (it’s in the Playground at http://mng.bz/
j5Ya), the browser console will print the following:
Bullets left: 99
Bullets left: 98
Both instances of the Gangsta class share the same totalBullets variable. That’s why
no matter which gangster shoots, the shared totalBullets variable is updated.
Listing 3.1 A gangster with a static property
Declares and initializes a static variable
Updates the number of
bullets after each shot
Creates a new instance
of the Gangsta
This gangster shoots once.
48 CHAPTER 3 Object-oriented programming with classes and interfaces
 Note that in the shoot() method we didn’t write this.totalBullets because this
is not an instance variable. You access static class members by prepending their
names with the class name, like Gangsta.totalBullets.
NOTE Static class members are not shared by subclasses. If you create a SuperGangsta class that subclasses Gangsta, it will get its own copy of the totalBullets property. We’ve provided an example in the Playground at http://
mng.bz/WO8g.
TIP Often we have several methods that perform similar actions. For example, we may need to write a dozen functions for validating user input in different UI fields. Instead of having separate functions, you can group them in a
class with a dozen static methods.
Now let’s consider another example. Imagine you need to store important data in
memory representing the current state of the app. Various scripts can access this storage, but you want to make sure that only one such object can be created for the entire
app, also known as a single source of truth. Singleton is a popular design pattern that
restricts the instantiation of a class to only one object.
 How do you create a class that you can instantiate only once? It’s a trivial task in
any object-oriented language that supports the private access qualifier. Basically, you
need to write a class that won’t allow the new keyword to be used, because with new you
can create as many instances as you want. The idea is simple—if a class has a private
constructor, the new operator will fail.
 How can you create even a single instance of such a class? If the class constructor is
private, you can access it only within the class, and as the author of this class, you’ll
responsibly create it only once by invoking the new operator from the class method.
 But can you invoke a method on a class that hasn’t been instantiated? You can do it
by making the class method static, so it doesn’t belong to any particular object
instance and belongs to the class.
 Listing 3.2 shows our implementation of the singleton design pattern in an
AppState class, which has a counter property. Let’s assume that the counter represents our app state, and it may be updated by multiple scripts in the app. This single
instance of AppState should be the only place that stores the value of counter. Any
script that needs to know the latest value of counter will get it from this AppState
instance.
class AppState {
counter = 0;
private static instanceRef: AppState;
private constructor() { }
Listing 3.2 A singleton class
This property represents
the app state. This property stores the
reference to the single
instance of AppState.
A private constructor
prevents using the new
operator with AppState.
Working with classes 49
static getInstance(): AppState {
if (AppState.instanceRef === undefined) {
AppState.instanceRef = new AppState();
}
return AppState.instanceRef;
}
}
// const appState = new AppState(); // error because of the private
➥ constructor
const appState1 = AppState.getInstance();
const appState2 = AppState.getInstance();
appState1.counter++;
appState1.counter++;
appState2.counter++;
appState2.counter++;
console.log(appState1.counter);
console.log(appState2.counter);
The AppState class has a private constructor, which means that no other script
can instantiate it using the new statement. It’s perfectly fine to invoke such a constructor from within the AppState class, and we do this in the static getInstance()
method. This is the only way we can invoke a method in the absence of the class
instance. Both console.log() invocations will print 4 because there is only one
instance of AppState. You can see this code sample in the Playground at http://mng
.bz/8zKK.
3.1.4 The super() method and the super keyword
Let’s continue looking into class inheritance. In figure 3.3 in line 15, we invoked the
sayHello() method, which was declared in the superclass. What if both the superclass
and subclass have methods with the same names? What if both have constructors? Can
we control which method is executed? If both the superclass and the subclass have
constructors, the one from the subclass must invoke the constructor of the superclass
using the super() method.
class Person {
constructor(public firstName: string,
public lastName: string,
private age: number) {}
}
class Employee extends Person {
Listing 3.3 Invoking the constructor of the super class
This is the only method to
get an instance of AppState.
Instantiates the AppState
object if it doesn’t exist yet
This variable gets a reference
to the AppState instance.
Modifies the counter (we use
two reference variables)
Prints the value of the counter
(we use two reference variables)
The constructor of the
Person superclass
The Employee subclass
50 CHAPTER 3 Object-oriented programming with classes and interfaces
constructor (firstName: string, lastName: string,
age: number, public department: string) {
super(firstName, lastName, age);
}
}
const empl = new Employee('Joe', 'Smith', 29, 'Accounting');
NOTE We discuss using super() and super in JavaScript in the appendix, section A.9.2. In this section, we’ll provide a similar example, but in TypeScript.
Both classes define constructors, and we must ensure that each of them is invoked with
the correct parameters. The constructor of the Employee class is automatically invoked
when we use the new operator, but we’ll have to manually invoke the constructor of the
Person superclass. The Employee class has a constructor with four arguments, but only
one of them—department—is needed to construct the Employee object. The other
three parameters are needed for constructing the Person object, and we pass them
over to Person by invoking the super() method with three arguments. You can play
with this code in the TypeScript Playground at http://mng.bz/E14q.
 Now let’s consider the situation where both the superclass and subclass have methods with the same names. If a method in a subclass wants to invoke a method with the
same name that’s defined in the superclass, it needs to use the super keyword instead
of this when referencing the superclass method.
 Let’s say a Person class has a sellStock() method that connects to a stock
exchange and sells the specified number of shares of the given stock. In the Employee
class we’d like to reuse this functionality, but every time employees sell stocks, they
must report it to a compliance department in the firm.
 We can declare a sellStock() method on the Employee class, and this method will
call sellStock() on Person, and then its own method reportToCompliance(), as
shown in the following listing.
class Person {
constructor(public firstName: string,
public lastName: string,
private age: number) { }
sellStock(symbol: string, numberOfShares: number) {
console.log(`Selling ${numberOfShares} of ${symbol}`);
}
}
class Employee extends Person {
Listing 3.4 Using the super keyword
The constructor
of the Employee
subclass Invokes the constructor
of the superclass
Instantiates
the subclass
The sellStock() method
in the ancestor
Working with classes 51
constructor (firstName: string, lastName: string,
age: number, public department: string) {
super(firstName, lastName, age);
}
sellStock(symbol: string, shares: number) {
super.sellStock(symbol, shares);
this.reportToCompliance(symbol, shares);
}
private reportToCompliance(symbol: string, shares: number) {
console.log(`${this.lastName} from ${this.department} sold ${shares}
➥ shares of ${symbol}`);
}
}
const empl = new Employee('Joe', 'Smith', 29, 'Accounting');
empl.sellStock('IBM', 100);
Note that we declared the reportToCompliance() method as private because we
want it to be called only by the internal method of the Employee class and never from
an external script. You can run this program in the Playground at http://mng.bz/
NeOE, and the browser will print the following in its console:
Selling 100 of IBM
Smith from Accounting sold 100 shares of IBM
With the help of the super keyword, we reused functionality from the method
declared in the superclass and added new functionality as well.
3.1.5 Abstract classes
If you add the abstract keyword to the class declaration, it can’t be instantiated. An
abstract class may include both methods that are implemented as well as abstract ones
that are only declared.
 Why would you want to create a class that can’t be instantiated? You may want to
delegate the implementation of some methods to its subclasses, and you want to make
sure these methods will have specific signatures.
 Let’s look at how abstract classes can be used. Suppose a company has employees
and contractors, and we need to design classes to represent the workers of this company. Any worker’s object should support the following methods:
 constructor(name: string)
 changeAddress(newAddress: string)
 giveDayOff()
 promote(percent: number)
 increasePay(percent: number)
Invokes the constructor
of the ancestor
The sellStock() method
in the descendant
Invokes sellStock()
on the ancestor
A private reportToCompliance() method
Invokes sellStock() on
the Employee object
52 CHAPTER 3 Object-oriented programming with classes and interfaces
In this scenario, “promote” means giving one day off and raising the salary by the
specified percent. The increasePay() method should raise the yearly salary for
employees but increase the hourly rate for contractors. How we implement the methods is irrelevant, but a method should just have one console.log() statement.
 Let’s work on this assignment. We’ll need to create the classes Employee and Contractor, which should have some common functionality. For example, changing
addresses and giving days off should work the same way for contractors and employees, but increasing their pay requires different implementations for these categories
of workers.
 Here’s the plan: we’ll create an abstractPerson class with two descendants:
Employee and Contractor. The Person class will implement the methods changeAddress(), giveDayOff(), and promote(). This class will also include a declaration of
the abstract method increasePay(), which will be implemented (differently!) in the
subclasses of Person, as shown in the following listing.
abstract class Person {
constructor(public name: string) { };
changeAddress(newAddress: string ) {
console.log(`Changing address to ${newAddress}`);
}
giveDayOff() { 2((CO5-3))
console.log(`Giving a day off to ${this.name}`);
}
promote(percent: number) {
this.giveDayOff();
this.increasePay(percent);
}
abstract increasePay(percent: number): void;
}
TIP If you don’t want to allow the giveDayOff() method to be invoked from
external scripts, add private to its declaration. If you want to allow it to be
invoked only from the Person class and its descendants, make this method
protected.
Note that you are allowed to write a line that looks like it invokes the abstract method.
But since the class is abstract, it can’t be “instantiated,” and there’s no way that the
abstract (unimplemented) method will actually be executed. If you want to create a
descendant of the abstract class that can be instantiated, you must implement all the
abstract methods of the ancestor.
Listing 3.5 The abstract Person class
Declares an abstract class
Declares and
implements a
method
“Invokes” the abstract method
Declares an abstract method
Working with classes 53
 The following listing shows how we implemented the classes Employee and
Constructor.
class Employee extends Person {
increasePay(percent: number) {
console.log(`Increasing the salary of ${this.name} by ${percent}%`);
}
}
class Contractor extends Person {
increasePay(percent: number) {
console.log(`Increasing the hourly rate of ${this.name} by
➥ ${percent}%`);
}
}
In section 2.2.4, we used the term assignable while discussing listing 2.20. When we
have class A extends class B, this means that class B is more general and class
A is more specific (for example, it adds more properties).
 A more specific type is assignable to a more general one. That’s why you can
declare a variable of type Person and assign to it an Employee or Contractor object, as
you’ll see in listing 3.7.
 Let’s create an array of workers with one employee and one contractor, and then
iterate through this array invoking the promote() method on each object.
const workers: Person[] = [];
workers[0] = new Employee('John');
workers[1] = new Contractor('Mary');
workers.forEach(worker => worker.promote(5));
The workers array is of type Person, which allows us to store the instances of descendant objects there as well.
TIP Because the descendants of Person don’t declare their own constructors,
the constructor of the ancestor will be invoked automatically when we
instantiate Employee and Contractor. If any of the descendants declared its
own constructor, we’d have to use super() to ensure that the constructor of
Person was invoked.
You can run this code sample in the TypeScript Playground at http://mng.bz/DNvy,
and the browser console will show the following output:
Listing 3.6 Descendants of the Person class
Listing 3.7 Running the promotion campaign
Implements the increasePay()
method for employees
Implements the increasePay()
method for contractors
Declares an array of
the superclass type
Invokes promote()
on each object
54 CHAPTER 3 Object-oriented programming with classes and interfaces
Giving a day off to John
Increasing the salary of John by 5%
Giving a day off to Mary
Increasing the hourly rate of Mary by 5%
The code in listing 3.7 gives the impression that we iterate through the objects of type
Person, invoking Person.promote(). But some of the objects can be of type Employee
while others are instances of Contractor. The actual type of the object is evaluated
only at runtime, which explains why the correct implementation of increasePay() is
invoked on each object. This is an example of polymorphism—a feature that each
object-oriented language supports.
In section 10.6.1, you’ll see another example of using an abstract class for handling
WebSocket messages in a blockchain app.
3.1.6 Method overloading
Object-oriented programming languages like Java and C# support method overloading,
which means that a class can declare more than one method with the same name but with
different arguments. For example, you can write two versions of the calculateTax()
method—one with two arguments, such as the person’s income and number of dependents, and the other with one argument of type Customer that has all the required data
about the person.
 In strongly typed languages, the ability to overload methods, specifying the types of
arguments and a return value, is important because you can’t just invoke a class
method and provide an argument of an arbitrary type, and the number of arguments
can be different. TypeScript, however, is a sugar coating for JavaScript, which allows
you to invoke a function while passing more or fewer arguments than the function signature declares. JavaScript won’t complain, and it doesn’t need to support function
overloading. Of course, you may get a runtime error if the method doesn’t properly
handle the provided object, but this would happen at runtime. TypeScript offers a syntax for explicitly declaring each allowed overloaded method signature, eliminating
the runtime surprises.
Protected constructors
In section 3.1.3, we declared a private constructor to create a singleton—a class
that can be instantiated only once. There’s some use for protected constructors as
well. Say you need to declare a class that can’t be instantiated, but its subclasses
can. You could declare a protected constructor in the superclass and invoke it using
super() from the subclass constructor.
This mimics one of the features of abstract classes. But a class with a protected constructor won’t let you declare abstract methods unless the class itself is declared as
abstract.
Working with classes 55
 Let’s see if the following code works.
class ProductService {
getProducts() {
console.log(`Getting all products`);
}
getProducts(id: number) { // error
console.log(`Getting the product info for ${id}`);
}
}
const prodService = new ProductService();
prodService.getProducts(123);
prodService.getProducts();
The Typescript compiler will give you an error, “Duplicate function implementation,”
for the second getProduct() declaration, as shown in figure 3.6.
 The syntax in the TypeScript code (on the left) is wrong, but the JavaScript syntax
on the right is perfectly fine. The first version of the getProducts method (line 4) was
replaced with the second one (line 7), so during runtime the JavaScript version of this
script has only one version of getProducts(id).
 Let’s ignore the compiler’s errors and try to run the generated JavaScript in the TypeScript Playground. The browser console will print the messages only from the getProducts(id) method, even though we wanted to invoke different versions of this method:
Getting the product info for 123
Getting the product info for undefined
In the compiled JavaScript, the method (or function) can have only one body that can
account for all allowed method parameters. Still, TypeScript offers the syntax to specify
Listing 3.8 An erroneous attempt of method overloading
The getProducts() method
without arguments
The getProducts() method
with one argument
Figure 3.6 Erroneous TypeScript but valid JavaScript
56 CHAPTER 3 Object-oriented programming with classes and interfaces
method overloading. It comes down to declaring all allowed method signatures without
implementing these methods, followed by one implemented method.
class ProductService {
getProducts(): void;
getProducts(id: number): void;
getProducts(id?: number) {
if (typeof id === 'number') {
console.log(`Getting the product info for ${id}`);
} else {
console.log(`Getting all products`);
}
}
}
const prodService = new ProductService();
prodService.getProducts(123);
prodService.getProducts();
Note the question mark after the id argument in the implemented method. This
question mark declares this argument as optional. If we didn’t make this argument
optional, the compiler would give us the error “Overload signature is not compatible
with function implementation.” In our code sample, this means that if we declared a
no-argument getProducts() method signature, the method implementation should
allow us to invoke this function without arguments.
TIP Omitting the first two declarations in listing 3.9 wouldn’t change this
program’s behavior. These lines just help IDEs provide better autocomplete
options for the getProducts() function.
Try this code sample in the TypeScript Playground at http://mng.bz/lozj. Note that
the generated JavaScript has just one getProducts() function, as shown in figure 3.7.
Listing 3.9 Correct syntax for method overloading
Declares the allowed
method signature
Implements
the method
Figure 3.7 Proper syntax for overloading the method
Working with classes 57
Similarly, you can overload a method signature to indicate that it can not only have
different arguments but return values of different types. Listing 3.10 shows a script
with an overloaded getProducts() method, which can be invoked in two ways:
 Provide the product description and return an array of type Product.
 Provide the product id and return a single object of type Product.
interface Product {
id: number;
description: string;
}
class ProductService {
getProducts(description: string): Product[];
getProducts(id: number): Product;
getProducts(product: number | string): Product[] | Product{
if (typeof product === "number") {
console.log(`Getting the product info for id ${product}`);
return { id: product, description: 'great product' };
} else if (typeof product === "string") {
console.log(`Getting product with description ${product}`);
return [{ id: 123, description: 'blue jeans' },
{ id: 789, description: 'blue jeans' }];
} else {
return { id: -1,
description: 'Error: getProducts() accept only number or
➥ string as args' };
}
}
}
const prodService = new ProductService();
console.log(prodService.getProducts(123));
console.log(prodService.getProducts('blue jeans'));
You can see and run the preceding code sample in the TypeScript Playground at
http://mng.bz/BYov. The output in the browser console is shown in figure 3.8.
 Now, let’s experiment with the code in listing 3.10. If you comment out the two
lines that declare signatures of getProducts(), the program still works. You can
invoke this method providing either the number or string as an argument, and this
method will return either one Product or an array of them.
 The question is, why even declare overloaded signatures if you can simply implement a single method using unions in argument types and return values? Method
overloading helps the Typescript compiler to properly map the provided argument
types to the types of the return values. When the overloaded method signatures are
Listing 3.10 Different arguments and return types
Defines the Product type
The first overloaded
signature of getProducts()
The second overloaded
signature of getProducts()
Implementation
of getProducts()
Sees if the
method was
invoked with
product id
Sees if the method was invoked
with product description
58 CHAPTER 3 Object-oriented programming with classes and interfaces
declared, the TypeScript static analyzer will properly suggest possible ways of invoking
the overloaded method. Figure 3.9 (a screenshot from VS Code) shows the first
prompt (note 1/2) for invoking the getProducts() method.
Figure 3.10 shows the second prompt (note 2/2) for invoking the getProducts()
method, with a different parameter and return type.
If we commented out the declarations of the getProducts() signatures, the prompt
wouldn’t be as easy to reason about. It would be difficult to understand which argument type results in returning a value of which type, as shown in figure 3.11.
getProducts(123)
returns a single object.
getProducts('blue jeans')
returns an array.
Figure 3.8 Overloading with different returns
Figure 3.9 Prompting the first method signature
Figure 3.10 Prompting the second method signature
Figure 3.11 The prompt without overloading
Working with classes 59
You may argue that the benefits offered by TypeScript’s method overloading are not too
convincing, and we agree that it may be easier to just declare two methods with different
names, such as getProduct() and getProducts(), without unionizing their argument
and return types. This is true except for one use case: overloading constructors.
 In TypeScript classes, there’s only one name you can give to a constructor, and that’s
constructor. If you want to create a class with several constructors having different signatures, you may want to use the syntax for overloading, as shown in listing 3.11.
class Product {
id: number;
description: string;
constructor();
constructor(id: number);
constructor(id: number, description: string);
constructor(id?: number, description?: string) {
// Constructor implementation goes here )
}
}
NOTE Because we wanted to allow a no-argument constructor in the preceding
listing, we made all arguments optional in the constructor implementation.
But again, overloading constructors is not the only way to initialize an object’s properties. For example, you could declare a single interface to represent all possible parameters for a constructor. The following listing declares an interface with all optional
properties and a class with one constructor that takes one optional argument.
interface ProductProperties {
id?: number;
description?: string;
}
class Product {
id: number;
description: string;
constructor(properties?: ProductProperties ) {
// Constructor implementation goes here
}
}
To summarize, use common sense when overloading a method or a constructor in
TypeScript. Although overloading provides multiple ways of invoking a method, the
logic may quickly become difficult to reason about. In our daily TypeScript work, we
rarely use overloading.
Listing 3.11 Overloading constructors
Listing 3.12 A single constructor with an optional argument
A no-argument
constructor declaration
A one-argument
constructor declaration A two-argument
constructor declaration
An implementation of constructor
handling all possible arguments
A ProductProperties interface
with two optional properties
A class constructor with
an optional argument of
type ProductProperties
60 CHAPTER 3 Object-oriented programming with classes and interfaces
3.2 Working with interfaces
In chapter 2, we used TypeScript interfaces only for declaring custom types, and we
came up with a general rule: if you need a custom type that includes a constructor, use
a class; otherwise use an interface. In this section, we’ll show you how to use TypeScript
interfaces to ensure that a class implements a specific API.
3.2.1 Enforcing the contract
An interface can declare not only properties but also methods (no implementations
though). A class declaration can then include the implements keyword followed by
the name of the interface. In other words, while an interface just contains method signatures, a class can contain their implementations.
 Say you own a Toyota Camry. It has thousands of parts that perform various
actions, but as a driver you just need to know how to use a handful of controls—how
to start and stop the engine, how to accelerate and brake, how to turn on the radio,
and so on. All these controls together can be considered a public interface offered to
you by the designers of the Toyota Camry.
 Now imagine that you have to rent a car, and they give you a Ford Taurus, which
you’ve never driven before. Will you know how to drive it? Yes, because it has a familiar interface: a key for starting the engine, acceleration and brake pedals, and so on.
When you rent a car, you can even request a car that has a specific interface, such as
an automatic transmission.
 Let’s model some of the car interface using TypeScript’s syntax. The following listing shows a MotorVehicle interface that declares five methods.
interface MotorVehicle {
startEngine(): boolean;
stopEngine(): boolean;
brake(): boolean;
accelerate(speed: number): void;
honk(howLong: number): void;
}
Note that none of the methods of MotorVehicle are implemented. Now we can
declare a Car class that will implement all the methods declared in the MotorVehicle
interface. By using the keyword implements, we declare that a class implements a certain interface:
class Car implements MotorVehicle {
}
This simple class declaration won’t compile. It will give you the error “Class Car incorrectly implements interface MotorVehicle.” When you declare that a class implements
Listing 3.13 The MotorVehicle interface
Declares a method
signature that should be
implemented by a class
Working with interfaces 61
some interface, you must implement each and every method declared in the interface. In other words, the preceding code snippet states “I swear that the class Car will
implement the API declared in the interface MotorVehicle.” The following listing
shows a simplified implementation of this interface in the Car class.
class Car implements MotorVehicle {
startEngine(): boolean {
return true;
}
stopEngine(): boolean{
return true;
}
brake(): boolean {
return true;
}
accelerate(speed: number): void;
console.log(`Driving faster`);
}
honk(howLong: number): void {
console.log(`Beep beep yeah!`);
}
}
const car = new Car();
car.startEngine();
Note that we didn’t explicitly declare the type of the car constant—this is an example
of type inference. We could explicitly declare the type of car as follows (although it’s
not required):
const car: Car = new Car();
We could also declare the car constant of type MotorVehicle, because our Car class
implements this custom type:
const car: MotorVehicle = new Car();
What’s the difference between these two declarations of the car constant? Let’s say a
Car class implements eight methods: five of them come from the MotorVehicle interface and the others are some arbitrary methods. If the car constant is of type Car, you
can invoke all eight methods on the instance of the object represented by car. But if
the type of car is MotorVehicle, only the five methods declared in this interface can
be invoked using the car constant.
 We can say that an interface enforces a specific contract. In our example, this means
that we force the Car class to implement each of the five methods declared in the
MotorVehicle interface, or else the code won’t compile.
Listing 3.14 A class that implements MotorVehicle
Implements the methods
from the interface
Instantiates the Car class
Uses the Car’s API to start the engine
62 CHAPTER 3 Object-oriented programming with classes and interfaces
 Now let’s design an interface for James Bond’s car. Yes, for agent 007. This special
car should be able to fly, and swim as well. Not a problem. Let’s declare a couple of
interfaces first.
interface Flyable {
fly(howHigh: number);
land();
}
interface Swimmable {
swim(howFar: number);
}
A class can implement more than one interface, so let’s make sure that our class
implements these two interfaces.
class Car implements MotorVehicle, Flyable, Swimmable {
// Implement all the methods from three
// interfaces here
}
Actually, making every car flyable and swimmable is not a good idea, so let’s not modify the Car class from listing 3.14. Let’s instead use class inheritance and create a
SecretServiceCar class that extends Car and adds more features.
class SecretServiceCar extends Car implements Flyable, Swimmable {
// Implement all the methods from two
// interfaces here
}
By implementing all the methods declared in Flyable and Swimmable, our SecretServiceCar turns a regular motor vehicle into a flyable and swimmable object. The
Car class continues to represent a regular auto with the functionality defined in the
MotorVehicle interface.
3.2.2 Extending interfaces
As you saw in the previous section, combining classes and interfaces brings flexibility
to code design. Let’s consider yet another option—extending an interface.
 In the prior example, when the requirement to develop the secret service car came
in, we already had the MotorVehicle interface and the Car class that implemented this
Listing 3.15 Flyable and Swimmable interfaces
Listing 3.16 A car with three interfaces
Listing 3.17 A class that extends and implements
Working with interfaces 63
interface. In listing 3.17, the SecretServiceCar class inherited from Car and implemented two additional interfaces.
 But when you design a special car for the secret service, you may want to implement all the methods in the MotorVehicle interface differently as well, so you may
want to declare the SecretServiceCar class as follows.
class SecretServiceCar implements MotorVehicle, Flyable, Swimmable {
// Implement all the methods from three interfaces here
}
On the other hand, our flyable object is a motor vehicle as well, so we could declare
the Flyable interface as follows.
interface Flyable extends MotorVehicle{
fly(howHigh: number);
land();
}
Now, if a class includes implements Flyable in its declaration, it must implement the
five methods declared in the MotorVehicle interface (see listing 3.13) as well as two
methods from Flyable (listing 3.19)—seven methods in total. Our SecretServiceCar
class must implement these seven methods plus one from Swimmable.
class SecretServiceCar implements Flyable, Swimmable {
startEngine(): boolean {
return true;
};
stopEngine(): boolean{
return true;
};
brake(): boolean {
return true;
};
accelerate(speed: number) {
console.log(`Driving faster`);
}
honk(howLong: number): void {
console.log(`Beep beep yeah!`);
}
Listing 3.18 A class that implements three interfaces
Listing 3.19 Extending an interface
Listing 3.20 A class that implements Flyable and Swimmable
One interface extends another
Declares a method signature
to be implemented in a class
Implements the method
from MotorVehicle
64 CHAPTER 3 Object-oriented programming with classes and interfaces
fly(howHigh: number) {
console.log(`Flying ${howHigh} feet high`);
}
land() { 2((CO16-7))
console.log(`Landing. Fasten your belts.`);
}
swim(howFar: number) {
console.log(`Swimming ${howFar} feet`);
}
}
TIP Even if Swimmable also extended MotorVehicle, the Typescript compiler
wouldn’t complain.
Declaring interfaces that include method signatures improves code readability
because any interface describes a well-defined set of features that can be implemented
by one or more concrete classes.
 However, the interface won’t tell you exactly how a class will implement it. Developers who practice the object-oriented approach in programming have a mantra:
“Program to interfaces, not implementations.” In the next section, you’ll see what this
means.
3.2.3 Programming to interfaces
To understand the meaning and benefits of programming to interfaces, let’s consider
a case where this technique wasn’t used. Imagine you had to write code that would
retrieve information about all products or one product from a data source.
 You know how to write classes, so you could start implementing them right away. You
could define a custom Product type and a ProductService class with two methods.
class Product {
id: number;
description: string;
}
class ProductService {
getProducts(): Product[]{
// the code for getting products
// from a real data source should go here
return [];
}
getProductById(id: number): Product { 3((CO17-4))
// the code for getting products
// from a real data source should go here
Listing 3.21 Programming to implementations
Implements the
method from Flyable
Implements the method
from Swimmable
A custom Product type
A concrete implementation
of ProductService
An implemented method
Working with interfaces 65
return { id: 123, description: 'Good product' };
}
}
Then, in multiple places in your app, you could instantiate ProductService and use
its methods:
const productService = new ProductService();
const products = productService.getProducts();
That was easy, wasn’t it? You proudly commit this code to the source code repository,
but your manager says that the backend guys are delaying implementing the server
that was supposed to provide data for your ProductService. He asks you to create
another class, MockProductService, with the same API that could return hardcoded
product data.
 No problem. You write another implementation of the product service.
class MockProductService {
getProducts(): Product[]{
// the code for getting hard-coded
// products goes here
return [];
}
getProductById(id: number): Product {
return { id: 456, description: 'Not a real product' };
}
}
TIP You might need to create MockProductService not only because the
backend guys are running late, but also for use in unit tests, in which you
don’t use real services.
You’ve created two concrete implementations of the product service. Hopefully you
didn’t make mistakes while declaring methods in MockProductService. They need to
be exactly the same as in ProductService or you may break the code that uses MockProductService.
 We don’t like the word hopefully in the previous paragraph. We already know that
an interface allows us to enforce a contract upon a class—upon MockProductService
in this case. But we didn’t declare any interface here!
Listing 3.22 Another implementation of the product service
A concrete implementation of MockProductService
An implemented
method
66 CHAPTER 3 Object-oriented programming with classes and interfaces
 It sounds weird, but in TypeScript you can declare a class that implements another
class. The better (not the best) approach would be to start writing MockProductService
as follows:
class MockProductService implements ProductService {
// implementation goes here
}
TypeScript is smart enough to understand that if you use a class name after the word
implement, you want to use it as an interface and enforce the implementation of all
public methods of ProductService. This way, there’s no chance that you’ll forget to
implement something, or make a mistake in the signature of getProducts() or getProductById(). Your code won’t compile until you properly implement these methods in the MockProductService class.
 But the best approach is to program to interfaces from the very beginning. When
you got the requirement to write ProductService with two methods, you should have
started by declaring an interface with these methods, without worrying about their
implementation.
 Let’s call this interface IProductService and declare two method signatures
there. Then declare a ProductService class that implements this interface.
interface Product {
id: number;
description: string;
}
interface IProductService {
getProducts(): Product[];
getProductById(id: number): Product
}
class ProductService implements IProductService {
getProducts(): Product[]{
// the code for getting products
// from a real data source goes here
return [];
}
getProductById(id: number): Product {
// the code for getting a product by id goes here
return { id: 123, description: 'Good product' };
}
}
Listing 3.23 Programming to an interface
Declares a custom type
using an interface
Declares an API as an interface
Implements the interface
Working with interfaces 67
Declaring an API as an interface shows that you spent time thinking about the
required functionality, and only afterwards took care of the concrete implementation.
Now, if a new class, such as MockProductService, has to be implemented, you’d start it
like this:
class MockProductService implements IProductService {
// Another concrete implementation of the
// interface methods goes here
}
Have you noticed that the custom Product type is implemented differently in listings
3.21 and 3.23? Use the interface keyword instead of class if you don’t need to instantiate this custom type (for example, Product), and the JavaScript footprint will be
smaller. Try the code from these two listings in the TypeScript Playground and compare
the generated JavaScript. The version where Product is an interface is shorter.
TIP We named the interface IProductService, starting with a capital I,
whereas the class name was ProductService. Some people prefer using the
suffix Impl for concrete implementations, such as ProductServiceImpl, and
to simply name the interface ProductService.
Another good example of programming to interfaces is factory functions, which implement some business logic and then return the proper instance of the object. If we had
to write a factory function that returned either ProductService or MockProductService, we’d use the interface as its return type.
function getProductService(isProduction: boolean): IProductService {
if (isProduction) {
return new ProductService();
} else {
return new MockProductService();
}
}
const productService: IProductService;
...
const isProd = true;
const productService: IProductService = getProductService(isProd);
const products = productService.getProducts();
In this example, we used the isProd constant with a hardcoded value of true. In realworld apps, this value would be obtained from a property file or environment variable.
Listing 3.24 A factory function
A factory function that uses
the interface as a return type
A constant of the
interface type
In a real-world app, this
wouldn’t be hardcoded.
Gets the proper instance
of the product service
Invokes the method on
the product service
68 CHAPTER 3 Object-oriented programming with classes and interfaces
By changing this property from true to false, we can change the behavior of the app
at runtime.
 Although the actual type of the returned object will be either ProductService or
MockProductService, we used an IProductService abstraction in the function signature. This makes our factory function more flexible and easily expandable: if in the
future we need to modify this function’s body to return an object of the type AnotherProductService, we can just make sure that this new class declaration includes
implements IProductService, and the code that uses our factory function will compile without any additional changes. Program to interfaces!
TIP Chapter 16 has a sidebar titled “Once again about programming to interfaces,” and there you’ll see another use case where programming to interfaces
could prevent a runtime bug.
NOTE In section 11.5, while explaining the specifics of using dependency
injection with the Angular framework, we’ll come back to the idea of programming to abstractions. There you’ll see that TypeScript interfaces can’t be
used, but you could use abstract classes instead.
Summary
 You can create a class using another one as a base. We call this class inheritance.
 A subclass can use public or protected properties of a superclass.
 If a class property is declared as private, it can be used only within that class.
 You can create a class that can only be instantiated once by using a private
constructor.
 If methods with the same signatures exist in the superclass and a subclass, we
call it method overriding. Class constructors can be overridden as well. The super
keyword and the super() method allow a subclass to invoke the class members
of a superclass.
 You can declare several signatures for a method, and this is called method
overloading.
 Interfaces can include method signatures but can’t contain their implementations.
 You can inherit one interface from another.
 While implementing a class, see if any methods can be declared in a separate
interface. Then your class will have to implement that interface. This approach
provides a clean way to separate the declaration of functionality from the
implementation.
69
Using enums and generics
In chapter 2, we introduced unions, which allow you to create a custom type by
combining several existing types. In this chapter, you’ll learn how to use enums—a
way to create a new type based on a limited set of values.
 We’ll also introduce generics, which allow you to place type constraints on class
members, function parameters, or their return types.
4.1 Using enums
Enumerations (a.k.a. enums) allow you to create limited sets of named constants
that have something in common. Such constants can be numbers or strings.
This chapter covers
 The benefits of using enums
 The syntax for numeric and string enums
 What generic types are for
 How to write classes, interfaces, and functions
that support generics
70 CHAPTER 4 Using enums and generics
4.1.1 Numeric enums
A week has seven days, and you can assign the numbers 1 to 7 to represent them. But
what’s the first day of the week?
 According to ISO 8601, the standard on data elements and interchange formats,
Monday is the first day of the week, which doesn’t stop such countries as USA, Canada, and Australia from considering Sunday to be the first day of the week. Also, what
if someone assigned the number 8 to the variable that stores the day? We don’t want
this to happen, and using day names instead of numbers makes our code more readable. Using just the numbers 1 to 7 to represent days may not be a good idea. On the
other hand, using numbers to store days is more efficient than using names. We want
readability, the ability to restrict values to a limited set, and efficiency in storing data.
This is where enums can help.
 TypeScript has an enum keyword that can define a limited set of constants. We can
declare a new type for weekdays as follows.
enum Weekdays {
Monday = 1,
Tuesday = 2,
Wednesday = 3,
Thursday = 4,
Friday = 5,
Saturday = 6,
Sunday = 7
}
This listing defines a new type, Weekdays, that has a limited number of values. We initialized each enum member with a numeric value, and days of the week can be
referred to using dot notation:
let dayOff = Weekdays.Tuesday;
The value of the variable dayOff is now 2, but if you typed the preceding line in your
IDE or in TypeScript Playground, you’d be prompted with the possible values, as
shown in figure 4.1.
Listing 4.1 Defining weekdays using enum
Figure 4.1 Autocomplete with enums
Using enums 71
Using the members of the Weekdays enum stops you from making a mistake and
assigning a wrong value (such as 8) to the variable dayOff. Well, strictly speaking,
nothing stops you from ignoring this enum and writing dayOff = 8, but this would be
a misdemeanor.
 In listing 4.1, we could have initialized only Monday with 1, and the rest of the days
values would be assigned using auto-increment: Tuesday would be initialized with 2,
Wednesday with 3, and so on.
enum Weekdays {
Monday = 1,
Tuesday,
Wednesday,
Thursday,
Friday,
Saturday,
Sunday
}
By default, enums are zero-based, so if we didn’t initialize the Monday member with 1,
its value would be 0.
In some cases, you won’t care which numeric values are assigned to the enum members—
the following convertTemperature() function illustrates this. It converts the temperature
Listing 4.2 The enum with auto-increment values
Reversing numeric enums
If you know the value of a numeric enum, you can find the name of that enum member. For example, you might have a function that returns the weekday number, and
you’d like to print its name. By using this value as an index, you can retrieve the name
of the day.
enum Weekdays {
Monday = 1,
Tuesday,
Wednesday,
Thursday,
Friday,
Saturday,
Sunday
}
console.log(Weekdays[3]);
In the last line of the preceding code, we retrieve the name of day 3. It will print
“Wednesday” on the console.
Declares a numeric enum
Gets the name of the
member that’s equal to 3
72 CHAPTER 4 Using enums and generics
from Fahrenheit to Celsius or vice versa. In this version of convertTemperature() we won’t
use enums, but we’ll then rewrite it with them.
function convertTemperature(temp: number, fromTo: string): number {
return ('FtoC' === fromTo) ?
(temp - 32) * 5.0/9.0:
temp * 9.0 / 5.0 + 32;
}
console.log(`70F is ${convertTemperature(70, 'FtoC')}C`);
console.log(`21C is ${convertTemperature(21, 'CtoF')}F`);
console.log(`35C is ${convertTemperature(35, 'ABCD')}F`);
The function in listing 4.3 converts the value from Celsius to Fahrenheit if you pass
any value for fromTo except FtoC. In the last line, we purposely provided the erroneous value ABCD as the fromTo parameter, and the function still converted the temperature from Celsius to Fahrenheit. You can see it in action on CodePen at http://mng
.bz/JzaK. Attempts to invoke a function with erroneous values should be caught by the
compiler, and this is what TypeScript enums are for.
 In the following listing, we declare a Direction enum that restricts the allowed
constants to either FtoC or CtoF and nothing else. We also changed the type of the
fromTo parameter from string to Direction.
enum Direction {
FtoC,
CtoF
}
function convertTemperature(temp: number, fromTo: Direction): number {
return (Direction.FtoC === fromTo) ?
(temp - 32) * 5.0/9.0:
temp * 9.0 / 5.0 + 32;
}
console.log(`70F is ${convertTemperature(70, Direction.FtoC)}C`);
console.log(`21C is ${convertTemperature(21, Direction.CtoF)}F`);
Since the type of the second parameter of the function is Direction, we must invoke
this function providing one of this enum’s members, such as Direction.CtoF. We’re
Listing 4.3 Converting temperature without enums
Listing 4.4 Converting temperature with enums
This function takes two parameters:
temperature and conversion direction.
Converts from
Fahrenheit to Celsius
Converts from
Celsius to Fahrenheit
Converts 70
degrees Fahrenheit
Converts 21
degrees Celsius
Invokes the function with a meaningless fromTo
Declares the Direction enum
The type of the second
parameter is Direction.
Invokes the function using
the enum members
Using enums 73
not interested in the numeric value of this member. The purpose of this enum is just
to provide a limited set of constants: CtoF and FtoC. The IDE will prompt you with two
possible values for the second parameter, and you won’t have a chance for a typo.
TIP Using the Direction type for the second argument doesn’t prevent
another misdemeanor—invoking this function like convertTemperature
(50.0, 99).
Enum members are initialized with values (either explicitly or implicitly). All the
examples in this section had enum members initialized with numbers, but TypeScript
allows you to create enums with string values, and we’ll see such examples next.
4.1.2 String enums
In some cases, you may want to declare a limited set of string constants, and for this
you can use string enums—enums whose members are initialized with string values.
Say you’re programming a computer game where the player can move in four
directions.
enum Direction {
Up = "UP",
Down = "DOWN",
Left = "LEFT",
Right = "RIGHT",
}
When you declare a string enum, you must initialize each member. You might ask,
“Why not just use a numeric enum here so TypeScript could automatically initialize its
members with any numbers?” The reason is that in some cases you want to give meaningful values to enum members. For example, you’ll need to debug the program, and
instead of seeing that the last move was 0, you’ll see that the last move was UP.
 The next question you might ask is “Why declare the Direction enum if I can just
declare four string constants with the values UP, DOWN, LEFT, and RIGHT?” You can, but
suppose we have a function with the following signature:
move(where: string)
A developer could make a mistake (or a typo) and invoke this function as
move("North"). But North is not a valid direction, so it’s safer to declare this function
using the Direction enum:
move(where: Direction)
Listing 4.5 Declaring a string enum
Initializes the enum member
with a string value
74 CHAPTER 4 Using enums and generics
As you can see in figure 4.2, we made a mistake and provided the string “North” in
line 15. The compile-time error would read “Argument of type ‘“North”’ is not assignable to the parameter of type ‘Direction’.” In line 18, which uses the Direction enum,
the IDE offers you a selection of valid enum members so there’s no way you can provide the wrong argument.
 You can use the union type as an alternative to enums. For example, the signature
of the move() function could look like this:
function move(direction: 'Up' | 'Down' | 'Left' | 'Right') { }
move('North'); // compilation error
Another alternative to enums is defining a custom type:
type Direction = 'Up' | 'Down' | 'Left' | 'Right';
function move(direction: Direction) {}
move('North'); // compilation error
Now let’s imagine that you need to keep track of the app’s state changes. The user can
initiate a limited number of actions in each of the app’s views, and you want to log the
actions taken in the Products view. Initially the app tries to load products, and this
action can either succeed or fail. The user can also search for products. To represent
the states of the Products view, you might declare a string enum.
Wrong argument type is
caught by the compiler
Autocomplete
prevents mistakes
Figure 4.2 Catching erroneous function invocations
Using enums 75
enum ProductsActionTypes {
Search = 'Products Search',
Load = 'Products Load All',
LoadFailure = 'Products Load All Failure',
LoadSuccess = 'Products Load All Success'
}
// If the function that loads products fails...
console.log(ProductsActionTypes.LoadFailure);
When the user clicks on the button to load products, you can log the value of the
ProductsActionTypes.Load member, which will log the text "Products Load All". If
the products were not loaded successfully, you can log the value of ProductsActionTypes
.LoadFailure, which will log the text "Products Load All Failure".
NOTE Some state management frameworks (such as Redux) require the app
to emit actions when the app’s state should change. We can declare a string
enum as in listing 4.6 and emit the actions ProductsActionTypes.Load,
ProductsActionTypes.LoadSuccess, and so on.
String enums can easily be mapped to string values coming from a server or database
(order status, user role, and so on) with no additional coding, providing you with all the
benefits of strong typing. We’ll illustrate this at the end of this chapter in listing 4.17.
NOTE String enums are not reversible—you can’t find a member’s name if
you know its value.
4.1.3 Using const enums
If you use the keyword const while declaring an enum, its values will be inlined and no
JavaScript will be generated.
 Let’s compare the generated JavaScript of enum and const enum. The left side of
figure 4.3 shows an enum declared without const, and the right side shows the generated JavaScript. For illustration purposes, in the last line, we print the next move.
Listing 4.6 Declaring a string enum for monitoring actions
Initializes the Search member
Initializes the
Load member
Initializes the
LoadFailure member
Initializes the
LoadSuccess member
Prints “Products
Load All Failure”
Figure 4.3 An enum without the const keyword
76 CHAPTER 4 Using enums and generics
Now, in figure 4.4, we’ll add the keyword const on the first line before enum and compare the generated JavaScript (on the right) with that in figure 4.3.
 As you see, no JavaScript code for enum Direction was generated in figure 4.4. But
the value of the enum member from the TypeScript code (Direction.Down) was
inlined in the JavaScript.
TIP The earlier sidebar, “Reversing numeric enums,” includes a listing where
we reversed the third member of the enum: Weekdays[3]. This isn’t possible
with const enum as they are not represented in the generated JavaScript code.
Using const with enum results in more concise JavaScript, but keep in mind that
because there is no JavaScript code to represent your enum, you may run into some
limitations. For example, you won’t be able to retrieve a numeric enum member’s
name with its value.
 Overall, using enums increases the readability of your programs.
4.2 Using generics
You know that TypeScript has built-in types, and you can create custom ones as well.
But there’s more to it. Strange as it may sound, types can be parameterized—you can
provide a type (not the value) as a parameter.
 It’s easy to declare a function that takes parameters of specific concrete types, such
as a number and a string:
function calctTax(income: number, state: string){...}
But TypeScript generics allow you to write functions that can work with a variety of
types. In other words, you can declare a function that works with a generic type, and
the concrete type can be specified later by the caller of the function.
 In TypeScript, you can write generic functions, classes, or interfaces. A generic
type can be represented by an arbitrary letter (or letters), such as T in Array<T>, and
when you declare a specific array, you provide a concrete type in angle brackets, such
as number:
let lotteryNumbers: Array<number>;
Figure 4.4 An enum with the const keyword
Using generics 77
In this section, you’ll learn how to use generic code written by someone else, as well as
how to create your own classes, interfaces, and functions that can work with generic
types.
4.2.1 Understanding generics
A generic is a piece of code that can handle values of multiple types, which are specified when this code is used (during a function invocation or class instantiation).
 Let’s consider TypeScript arrays, which can be declared as follows:
 Specify the type of the array element followed by []:
const someValues: number[];
 Use the generic Array followed by the type parameter in angle brackets:
const someValues: Array<number>;
If all elements of the array have the same type, both declarations are equivalent but
the first syntax is simpler to read. With the second syntax, the angle brackets represent
a type parameter. You can instantiate this Array like any other while restricting the
type of allowed values (to number in our example). A bit later, we’ll show you when
declaring an array with a generic type is the better approach.
 The next code snippet creates an array that will initially have ten objects of type
Person, and the inferred type of the people variable is Person[].
class Person{ }
const people = new Array<Person>(10);
TypeScript arrays can hold objects of any type, but if you decide to use the generic
Array type, you must specify which value types are allowed in the array, such as
Array<Person>. By doing this, you place a constraint on this instance of the array. If
you tried to add an object of a different type to this array, the Typescript compiler
would generate an error. In another piece of code, you could use the array with a different type parameter, such as Array<Customer>.
 The following listing declares a Person class, its descendant Employee, and an
Animal class. Then it instantiates each class and tries to store all these objects in the
workers array using the generic array notation with the type parameter Array<Person>.
class Person {
name: string;
}
class Employee extends Person {
department: number;
}
class Animal {
Listing 4.7 Using a generic type
Declares the Person class
Declares a subclass of Person
Declares the Animal class
78 CHAPTER 4 Using enums and generics
breed: string;
}
const workers: Array<Person> = [];
workers[0] = new Person();
workers[1] = new Employee();
workers[2] = new Animal(); // compile-time error
In listing 4.7, the last line won’t compile because the workers array was declared with
the type parameter Person, and Animal is not a Person. But the Employee class
extends Person and is considered a subtype of Person, so you can use the Employee
subtype anywhere the supertype Person is allowed.
 So, by using a generic workers array with the parameter <Person>, we announce
our plans to store only instances of the class Person or objects of compatible types
there. An attempt to store an instance of the class Animal (as it was defined in listing
4.7) in the same array will result in the following compile-time error: “Type Animal is
not assignable to type Person. Property name is missing in type Animal.” In other words,
using TypeScript generics helps you avoid errors related to using the wrong types.
TIP We used const (and not let) to declare the workers identifier in listing 4.7
because its value never changes. Adding new objects to the workers array
doesn’t change the address of the array in memory, so the value of the workers
identifier remains the same (i.e., immutable).
If you’re familiar with generics in Java or C#, you may get the feeling that you understand TypeScript generics as well. There is a caveat, though. Whereas Java and C# use
a nominal type system, TypeScript uses a structural one, as was explained in section 2.2.4.
Declares and initializes a generic
array with a concrete parameter
Adds objects to the array
Generic variance
The term generic variance relates to the rules for using subtypes and supertypes in
any particular place in your program. For example, in Java, arrays are covariant, which
means that you can use Employee[] (the subtype) where the array Person[] (the
supertype) is allowed.
Because TypeScript supports structural typing, you can use either an Employee or
any other object literal that’s compatible with the type Person where the Person type
is expected. In other words, generic variance applies to objects that are structurally
the same. Given the importance of anonymous types in JavaScript, an understanding
of this is important for the optimal use of generics in Typescript.
To see if type A can be used where type B is expected, read about structural subtyping
in the TypeScript documentation at http://mng.bz/wla2.
Using generics 79
 In a nominal type system, types are checked against their names, but in a structural
system they’re defined by their structure. In languages with a nominal type system, the
following line would always result in an error:
let person: Person = new Animal();
With a structural type system, as long as the structures of the types are similar, you may
get away with assigning an object of one type to a variable of another. Let’s add a name
property to the Animal class.
class Person {
name: string;
}
class Employee extends Person {
department: number;
}
class Animal {
name: string;
breed: string;
}
const workers: Array<Person> = [];
workers[0] = new Person();
workers[1] = new Employee();
workers[2] = new Animal(); // no errors
Now the Typescript compiler doesn’t complain about assigning an Animal object to a
variable of type Person. The variable of type Person expects an object that has a name
property, and the Animal object has one! This is not to say that Person and Animal
represent the same types, but these types are compatible.
 Moreover, you don’t even have to create a new instance of the Person, Employee,
or Animal classes—you can use the syntax of object literals instead. Adding the following line to listing 4.8 is perfectly fine, because the structure of the object literal is compatible with the structure of type Person:
workers[3] = { name: "Mary" };
On the other hand, trying to assign a Person object to a variable of type Animal will
result in a compilation error:
const worker: Animal = new Person(); // compilation error
Listing 4.8 Generics and the structural type system
The only additional line
compared to listing 4.7
80 CHAPTER 4 Using enums and generics
The error message would read “Property breed is missing in type Person,” and that
makes sense, because if you declare a worker variable of type Animal but create an
instance of the Person object that has no breed property, you wouldn’t be able to
write worker.breed. Hence the compile-time error.
NOTE The previous sentence may irritate savvy JavaScript developers who are
accustomed to adding object properties like worker.breed without thinking
twice. If the breed property doesn’t exist on the worker object, the JavaScript
engine would simply create it, right? This works in dynamically typed code, but
if you decide to use the benefits of static typing, you have to play by the rules.
Generics can be used in a variety of scenarios. For example, you can create a function
that takes values of various types, but during its invocation you must specify a concrete
type. To be able to use generic types with a class, an interface, or a function, the creator of this class, interface, or function has to write them in a special way to support
generic types.
Open TypeScript’s type definition file (lib.d.ts) from the TypeScript GitHub repository at http://mng.bz/qXvJ and you’ll see the declaration of the Array interface, as
shown in figure 4.5.
 The <T> in line 1008 is a placeholder for a concrete type that must be provided by
the application developer during the array’s declaration, as we did in listing 4.8. TypeScript requires you to declare a type parameter with Array, and whenever you add
new elements to this array, the compiler will check that their type matches the type
used in the declaration.
When to use generic arrays
We started this section by showing two different ways of declaring an array of numbers. Let’s take another example:
const values1: string[] = ["Mary", "Joe"];
const values2: Array<string> = ["Mary", "Joe"];
When all elements of the array have the same type, you can use the syntax used to
declare values1—it’s easier to read and write. But if an array can store elements of
different types, you can use generics to restrict the types allowed in the array.
For example, you may declare an array that allows only strings and numbers. In the
following code snippet, the line that declares values3 will result in a compilation
error because Boolean values are not allowed in this array.
const values3: Array<string | number> = ["Mary", 123, true]; // error
const values4: Array<string | number> = ["Joe", 123, 567]; // no errors
Using generics 81
In listing 4.8, we used the concrete type <Person> as a replacement for the generic
parameter represented by the letter <T>:
const workers: Array<Person>;
But because generics aren’t supported in JavaScript, you won’t see them in the code
generated by the compiler—generics (and any other types) are erased. Using type
parameters is just a safety net for developers at compile time.
 You can see more generic T types in lines 1022 and 1026 in figure 4.5. When
generic types are specified with function arguments, no angle brackets are needed—
you’ll see this syntax in listing 4.9.
 There’s no T type in TypeScript. The T here means the push() and pop() methods
let you push or pop objects of the type provided during the array declaration. For
example, in the following code snippet, we declared an array using the type Person as
a replacement for T, and that’s why we can use the instance of Person as the argument
of the push() method:
const workers: Array<Person>;
workers.push(new Person());
NOTE The letter T stands for type, which is intuitive, but any letter or word
can be used when declaring a generic type. In a map, developers often use the
letters K for key and V for value.
Figure 4.5 A fragment of lib.d.ts describing the Array API
82 CHAPTER 4 Using enums and generics
Seeing the T type in the API of the Array interface tells us that its creator enabled support for generics. Even if you’re not planning to create your own generic types, it’s
really important that you understand the syntax of generics when reading someone
else’s code or TypeScript documentation.
4.2.2 Creating your own generic types
You can create your own generic classes, interfaces, or functions. In this section, we’ll
create a generic interface, but the explanations are applicable to creating generic
classes as well.
 Let’s say you have a Rectangle class and need to add the capability of comparing
the sizes of two rectangles. If you didn’t adopt the concept of programming to interfaces (introduced in section 3.2.3), you’d simply add a compareRectangles() method
to the Rectangle class.
 But armed with the concept of programming to interfaces, you think differently:
“Today, I need to compare rectangles, and tomorrow they’ll ask me to compare other
objects. I’ll play it smart and declare an interface with a compareTo() function. Then
the Rectangle class, and any other class in the future, can implement this interface.
The algorithms for comparing rectangles will be different than comparing, say,
triangles, but at least they’ll have something in common, and the method signature of
compareTo() will look the same.”
 So you have an object of some type, and it needs a compareTo() method that will
compare this object to another object of the same type. If this object is bigger than the
other, compareTo() will return a positive number; if this object is smaller, it returns
negative; if they are equal, it returns 0.
 If you weren’t familiar with generic types, you’d define this interface as follows:
interface Comparator {
compareTo(value: any): number;
}
The compareTo() method can take any object as its argument, and the class that
implements Comparator must include the appropriate comparison algorithm (such as
for comparing the sizes of rectangles).
 The following listing shows a partial implementation of Rectangle and Triangle
classes that use the Comparator interface.
interface Comparator {
compareTo(value: any): number;
}
class Rectangle implements Comparator {
compareTo(value: any): number {
Listing 4.9 Using the interface without a generic type
The compareTo() method takes
one parameter of type any.
Implementation of
compareTo() in Rectangle
Using generics 83
// the algorithm for comparing rectangles goes here
}
}
class Triangle implements Comparator {
compareTo(value: any): number {
// the algorithm for comparing triangles goes here
}
}
If the developer had a good night’s sleep and a cup of double espresso, they’d create
instances of two rectangles and then pass one of them to the compareTo() method of
another:
rectangle1.compareTo(rectangle2);
But what if the coffee machine wasn’t working that morning? Our developer could
make a mistake and try to compare a rectangle to a triangle.
rectangle1.compareTo(triangle1);
Triangle fits the any type of the compareTo() parameter, and the preceding code can
result in a runtime error. To catch such an error at compile time, we could use a
generic type (instead of any) to place a constraint on which types can be given to the
compareTo() method:
interface Comparator<T> {
compareTo(value: T): number;
}
It’s important that both the interface and the method use the generic type represented by the same letter T. Now the Rectangle and Triangle classes can implement
Comparator, specifying concrete types in angle brackets:
interface Comparator <T> {
compareTo(value: T): number;
}
class Rectangle implements Comparator<Rectangle> {
compareTo(value: Rectangle): number {
// the algorithm of comparing rectangles goes here
}
}
Listing 4.10 Using the interface with a generic type
Implementation of
compareTo() in Triangle
Declares a generic interface that
takes one type as a parameter
The compareTo() method takes one
parameter of the generic type.
In class Rectangle, compareTo()
has a parameter of type Rectangle.
84 CHAPTER 4 Using enums and generics
class Triangle implements Comparator<Triangle> {
compareTo(value: Triangle): number {
// the algorithm of comparing triangles goes here
}
}
Let’s say our developer tries to make the same mistake:
rectangle1.compareTo(triangle1);
The TypeScript code analyzer will underline triangle1 with a red squiggly line,
reporting an error: “Argument of type ‘Triangle’ is not assignable to parameter of type
‘Rectangle’.” As you can see, using generic types lowers the dependency of your code
quality on the coffee machine.
 Listing 4.11 shows a working example that declares a Comparator<T> interface that
declares a compareTo() method. This code shows how this interface can be used for
comparing rectangles as well as programmers. Our algorithms are simple:
 If the area of the first rectangle (width multiplied by height) is bigger than that
of the second, the first rectangle is bigger. Two rectangles may have the same
areas.
 If the salary of the first programmer is higher than that of the second, the first
one is richer. Two programmers may have the same salaries.
interface Comparator<T> {
compareTo(value: T): number;
}
class Rectangle implements Comparator<Rectangle> {
constructor(private width: number, private height: number){};
compareTo(value: Rectangle): number {
return this.width * this.height - value.width * value.height;
}
}
const rect1:Rectangle = new Rectangle(2,5);
const rect2: Rectangle = new Rectangle(2,3);
rect1.compareTo(rect2) > 0 ? console.log("rect1 is bigger"):
rect1.compareTo(rect2) == 0 ? console.log("rectangles are equal") :
➥ console.log("rect1 is smaller");
Listing 4.11 A working example that uses a generic interface
In class Triangle, compareTo() has
a parameter of type Triangle.
Declares a generic
Comparator interface
Creates a class that
implements Comparator
for the Rectangle type Implements the
method for
comparing
rectangles
Compares rectangles
(the type T is erased and
replaced with Rectangle)
Using generics 85
class Programmer implements Comparator<Programmer> {
constructor(public name: string, private salary: number){};
compareTo(value: Programmer): number{
return this.salary - value.salary;
}
}
const prog1:Programmer = new Programmer("John",20000);
const prog2: Programmer = new Programmer("Alex",30000);
prog1.compareTo(prog2) > 0 ? console.log(`${prog1.name} is richer`):
prog1.compareTo(prog2) == 0?
console.log(`${prog1.name} and ${prog1.name} earn the
➥ same amounts`) :
console.log(`${prog1.name} is poorer`);
Running the script in listing 4.11 prints the following:
rect1 is bigger
John is poorer
You can see this program in action on CodePen here: http://mng.bz/7zqe.
Creates a class that
implements Comparator
for the Programmer type Implements the method for
comparing programmers
Compares programmers (the
type T is erased and replaced
with Programmer)
Default values of generic types
To use a generic type, you have to provide a concrete type. The following code won’t
compile because we didn’t specify a concrete type parameter while using the type A:
class A <T> {
value: T;
}
class B extends A { // Compile error
}
Adding the type any while using the class A would fix this error:
class B extends A <any> {
}
Another way to fix that error would be to specify a default parameter while declaring
the generic type. The following code snippet will compile without any errors:
class A < T = any > { // declaring default parameter type
value: T;
}
class B extends A { // No errors
}
86 CHAPTER 4 Using enums and generics
In this section, we’ve created a generic Comparator<T> interface. Now let’s see how we
can create a generic function.
4.2.3 Creating generic functions
We all know how to write a function that can take arguments of concrete types and
return a value of a concrete type. This time around, we’ll write a generic function that
can take parameters of multiple types.
 But first, let’s consider a not-so-good solution where a function can take a parameter of the type any and return a value of the same type. The function in the following
listing can log objects of different types and return the data that was logged.
function printMe(content: any): any {
console.log(content);
return content;
}
const a = printMe("Hello");
class Person{
constructor(public name: string) { }
}
const b = printMe(new Person("Joe"));
This function works for various types of arguments, but TypeScript doesn’t remember
the argument type the printMe() function was invoked with. If you hover your mouse
over the variables a and b in your IDE, the TypeScript static analyzer will report the
types of both variables as any.
 If we care to know which types of arguments were used in the invocation of
printMe(), we’d need to rewrite it as a generic function. The following listing shows
the syntax for providing the generic type <T> for the function, its parameter, and the
return value.
Listing 4.12 A function with the type any
(continued)
Instead of any, you can specify another dummy type:
class A < T = {} >
With this technique, you won’t need to specify a generic parameter when you use
generic classes. In figure 13.10 you’ll see how the React.FC type from the React
library uses these default types.
Of course, if you create your own generic types and can provide parameter defaults
that make some business sense (not just any), do that by all means.
Declares a function
expression with any
Invokes printMe() with
the string argument
Declares a custom Person type
Invokes printMe() with an
argument of type Person
Using generics 87
function printMe<T> (content: T): T {
console.log(content);
return content;
}
const a = printMe("Hello");
class Person{
constructor(public name: string) { }
}
const b = printMe(new Person("Joe"));
In this version of the function, we declare the generic type for the function as <T> and
the type of the parameter and return value as T. Now the types are preserved, and the
type of the constant a is string and the type of b is Person. If later on in the script you
need to use a and b, the TypeScript static analyzer (and compiler) will perform the
proper type checking.
NOTE By using the same letter T for the function argument type and return
type, we place a constraint ensuring that no matter what concrete type is used
during invocation, the return type of this function will be the same.
Similarly, you could use generics in fat arrow function expressions. The code sample
from listing 4.13 could be rewritten as follows.
const printMe = <T> (content: T): T => {
console.log(content);
return content;
}
const a = printMe("Hello");
class Person{
constructor(public name: string) { }
}
const b = printMe(new Person("Joe"));
You could also invoke these functions specifying the types explicitly in angle brackets:
const a = printMe<string>("Hello");
const b = printMe<Person>(new Person("Joe"));
But using explicit types is not required here, as the Typescript compiler will infer the
type of a as string and b as Person.
Listing 4.13 A generic function
Listing 4.14 Using generic types in fat arrow functions
Uses the type T for the function,
param, and return value
Invokes printMe() with
a string argument
Invokes printMe() with an
argument of type Person
The signature of this fat arrow
function starts with <T>.
88 CHAPTER 4 Using enums and generics
 The preceding code snippet may not look too convincing—it seems redundant to
use <string> if it’s clear that “Hello” is a string. But this may not always be the case, as
you’ll see in listing 4.17.
 Let’s write another little script that will provide more illustration of using generics
in a class and function. The following listing declares a class that can represent a pair:
a key and a value. Both key and value can by represented by multiple types, so we
smell generics here.
class Pair<K, V> {
key: K;
value: V;
}
When you write a piece of code that introduces generic type parameters represented
by letters (such as K and V), you can declare variables using these letters as if they were
built-in TypeScript types. When you declare (and compile) a concrete Pair with specific types for K and V, the K and V will be erased and replaced with the declared types.
 Let’s write a more concise version of the Pair class, which has a constructor and
automatically created key and values properties:
class Pair<K, V> {
constructor(public key: K, public value: V) {}
}
Now let’s write a generic function that can compare generic pairs. Is your head spinning yet? The function in the following listing declares two generic types that are also
represented by K and V.
function compare <K,V> (pair1: Pair<K,V>, pair2: Pair<K,V>): boolean {
return pair1.key === pair2.key &&
pair1.value === pair2.value;
}
During the invocation of the compare() function you’re allowed to specify two concrete types, which should be the same as the types provided for its parameters—the
Pair objects.
 Listing 4.17 shows a working script that uses the generic Pair class as well as the
generic compare() function. First we create and compare two Pair instances that use
the number type for the key and string type for values. Then we compare another two
Pair instances that use the string type for both key and value.
Listing 4.15 The generic Pair class
Listing 4.16 A generic compare function
Declares a class with two
parameterized types Declares a property
of generic type K
Declares a property of generic type V
Declares the
generic function Compares keys and values of the pairs
Using generics 89
class Pair<K, V> {
constructor(public key: K, public value: V) {}
}
function compare <K,V> (pair1: Pair<K,V>, pair2: Pair<K,V>): boolean {
return pair1.key === pair2.key &&
pair1.value === pair2.value;
}
let p1: Pair<number, string> = new Pair(1, "Apple");
let p2 = new Pair(1, "Orange");
// Comparing apples to oranges
console.log(compare<number, string>(p1, p2));
let p3 = new Pair("first", "Apple");
let p4 = new Pair("first", "Apple");
// Comparing apples to apples
console.log(compare(p3, p4));
Please note that in the first invocation of compare(), we explicitly specified the concrete parameters, and in the second invocation we didn’t:
compare<number, string>(p1, p2)
compare(p3, p4)
The first line is easier to reason about, as we can see what kinds of pairs p1 and p2 are.
Besides, if you make a mistake and specify the wrong types, the compiler will catch it
right away:
compare<string, string>(p1, p2) //compile error
You can see this code in action in the Playground at http://mng.bz/m454.
 The next script shows another example of the generic function. This time we’ll
map the members of a string enumeration to the user roles returned by a function.
Imagine that an authorization mechanism returns one of the following user roles:
admin or manager. We want to use string enums and map the user roles to the corresponding members of this enum.
 First, we’ll declare a custom User type:
interface User {
name: string;
role: UserRole;
}
Listing 4.17 Using compare() and Pair
Creates the first
<number, string> pair
Creates the second
<number, string> pair
using type inference Compares the pairs
(prints “false”)
Creates the first <string, string> pair
Creates the second <string, string> pair
Compares the pairs (prints “true”)
90 CHAPTER 4 Using enums and generics
Then we’ll create a string enum that lists a limited set of constants that can be used as
user roles.
enum UserRole {
Administrator = 'admin',
Manager = 'manager'
}
Next, we’ll create a function that loads the object with a hardcoded user name and its
role. In real-world apps, we’d make a request to some authorization server, supplying a
user ID to get the user’s role, but for our purposes, returning a hardcoded object will
suffice.
function loadUser<T>(): T {
return JSON.parse('{ "name": "john", "role": "admin" }');
}
The following listing shows a generic function that returns the User and then maps
the received user role to an action using the string enum.
interface User {
name: string;
role: UserRole;
}
enum UserRole {
Administrator = 'admin',
Manager = 'manager'
}
function loadUser<T>(): T {
return JSON.parse('{ "name": "john", "role": "admin" }');
}
const user = loadUser<User>();
switch (user.role) {
case UserRole.Administrator: console.log('Show control panel'); break;
case UserRole.Manager: console.log('Hide control panel'); break;
}
The script in listing 4.18 uses the User type during the invocation of loadUser(), and
the generic type T declared as the return type of this function becomes the concrete
type User. Note that the hardcoded object returned by this function has the same
structure as the User interface.
Listing 4.18 Mapping string enums
Declares a custom User type
Declares a string enum
Declares a generic function
Invokes the generic function
with a concrete User type
Switches on the user role
with a string enum
Using generics 91
 Here, user.role will always be admin, which maps to the enum member UserRole
.Administrator, and the script will print “Show control panel.” You can see this script
in action on CodePen at http://mng.bz/5Aqa.
TIP In chapter 10 (listing 10.15), you’ll see the code of a MessageServer class
that uses a generic <T> type in several methods.
4.2.4 Enforcing the return type of higher-order functions
If a function can receive a function as an argument or return another function, we call
it a higher-order function. In this section we’ll show you an example that enforces the
return type of a higher-order function while allowing arguments of different types.
 Let’s say we need to write a higher-order function that returns a function with the
following signature:
(c: number) => number
This fat arrow function takes a number as an argument and returns a number. The
higher-order function (we’ll use fat arrow notation) might look like this:
(someValue: number) => (multiplier: number) => someValue * multiplier;
We didn’t write the return statement after the first fat arrow because in single-line fat
arrow functions, return is implicit. The following listing shows the use of such a function.
const outerFunc = (someValue: number) =>
(multiplier: number) => someValue * multiplier;
const innerFunc = outerFunc(10);
let result = innerFunc(5);
console.log(result);
Now let’s make our task a bit more complex. We want to allow our higher-order function to be called with arguments of different types, ensuring that it always returns a
function with the same signature:
(c: number) => number
Let’s start by declaring a generic function that can take a generic type T but returns
the function (c: number) ? number:
type numFunc<T> = (arg: T) => (c: number) => number;
Listing 4.19 Using a higher-order function
Declares the higherorder function
innerFunc is a closure that
knows that someValue = 10.
Invokes the returned function
This will print 50.
92 CHAPTER 4 Using enums and generics
Now we can declare variables of type numFunc, and TypeScript will ensure that these
variables are functions of type (c: number) ? number.
const noArgFunc: numFunc<void> = () =>
(c: number) => c + 5;
const numArgFunc: numFunc<number> = (someValue: number) =>
(multiplier: number) => someValue * multiplier;
const stringArgFunc: numFunc<string> = (someText: string) =>
(padding: number) => someText.length + padding;
const createSumString: numFunc<number> = () => (x: number) => 'Hello';
The last line won’t compile because the signature of the returned function is
(c: number) ? string, which can’t be assigned to the variable of type numFunc. You
can see this example in the Playground at http://mng.bz/6wqA.
Summary
 TypeScript has an enum keyword that can define a limited set of constants.
 TypeScript supports numeric and string enums.
 If you use the const keyword while declaring an enum, its values will be inlined
and no JavaScript will be generated.
 A generic is a piece of code that can handle values of multiple types that are
specified when the code is used.
 You can create your own generic classes, interfaces, and functions.
Listing 4.20 Using the generic numFunc<T> function
Invokes the function with no arguments Invokes the function with a
numeric argument
Invokes the
function with a
string argument
Compiler error: numFunc
expects another signature
93
Decorators
 and advanced types
In previous chapters, we covered the main types, which should be sufficient for
most of your coding activities. But TypeScript goes further and offers you additional derivative types that can be quite handy in certain scenarios.
 We used the word “advanced” in this chapter’s title for a couple of reasons. First,
you don’t have to know these types to be a productive member of your team. Second, their syntax may not be immediately obvious to a software developer who’s
familiar with other programming languages.
 The title of this book is TypeScript Quickly, and the content in the first four chapters deliver on the promise to introduce the main syntax constructs of the language
at a fast pace. But you may need to read this chapter more then once.
This chapter covers
 What the TypeScript decorators are for
 How to create a new type based on an existing
one using mapped types
 How conditional types work
 Combining mapped and conditional types
94 CHAPTER 5 Decorators and advanced types
 The good news is that learning the advanced types presented in this chapter is not
required to understand the rest of this book. You can skip them if you need to learn
the language quickly. You may want to study this chapter if:
 It’s time to prepare for your next technical interview, where rarely used knowledge is expected.
 You look at some specific code and have a gut feeling that there should be a
more elegant solution.
 You’re curious to learn what else is available, as if dealing with interfaces, generics, and enums is not enough.
NOTE In this chapter, we’ll use the syntax of generics a lot, and we assume that
you’ve read and understand the materials from section 4.2, related to the syntax of generics. That’s a must for understanding the mapped and conditional
types described in this chapter.
5.1 Decorators
TypeScript’s documentation defines a decorator as
a special kind of declaration that can be attached to a class declaration, method,
accessor, property, or parameter. Decorators use the form @expression, where
expression must evaluate to a function that will be called at runtime with
information about the decorated declaration.
—“Decorators” in the Typescript documentation
(www.typescriptlang.org/docs/handbook/decorators.html)
Say you have class A {…}, and there’s a magic decorator called @Injectable() that
knows how to instantiate classes and inject their instances into other objects. We could
decorate one of our classes like this:
@Injectable() class A {}
As you can guess, the @Injectable() decorator will somehow change the behavior of
class A. Alternatively, we could change the behavior of class A without modifying its
code by creating a subclass of class A and adding or overriding the behavior there.
But simply adding a decorator to the class definition looks more elegant.
 We could also say that a decorator adds metadata to a specific target, which is
class A in our example. In general, metadata is additional data about some other
data. Take an mp3 file—we can say that the song is the data. But the mp3 file may have
additional properties, like the name of the artist and album, an image, and so on—
this is the metadata of the mp3 file. Thus you can use decorators to annotate a TypeScript class with metadata describing additional features you’d like this class to have.
 Names of TypeScript decorators start with the @ sign, such as @Component. You can
write your own decorators, but it’s more likely that you’ll use the decorators available
in a library or framework.
Decorators 95
 Consider the following simple class:
class OrderComponent {
quantity: number;
}
Imagine that you want to turn this class into a UI component. Moreover, you want to
declare that the value for the quantity property will be provided by the parent component. If you were using the Angular framework with TypeScript, you could use the
built-in @Component() decorator for a class and @Input() for a property.
@Component({
selector: 'order-processor',
template: `Buying {{quantity}} items`
})
export class OrderComponent {
@Input() quantity: number;
}
In Angular, the @Component() decorator can be applied only to a class, and it supports
various properties, such as selector and template. The @Input() decorator can be
applied only to the properties of a class. We could also say that these two decorators provided metadata about the OrderComponent class and the quantity property respectively.
 To make decorators useful, there should be some code that knows how to parse
them and to do what these decorators prescribe. In the example from listing 5.1, the
Angular framework would parse these decorators and generate additional code to
turn the OrderComponent class into a renderable UI component.
NOTE In chapters 11 and 12, we’ll be working with the Angular framework,
and you’ll see many examples of using decorators there. Decorators are
heavily used by the server-side Nest.js framework (https://docs.nestjs.com/
custom-decorators), state management MobX library (https://mobx.js.org/
refguide/modifiers.html), and Stencil.js UI library (https://stenciljs.com/
docs/decorators).
TypeScript doesn’t come with any built-in decorators, but you can create your own or
use the ones provided by a framework or library of your choice.
 The decorators in listing 5.1 allowed you to specify additional behavior for the class
and its property in a concise and declarative manner. Of course, using decorators is not
the only way to add behavior to an object. For example, the creators of Angular could
have created an abstract UIComponent class with a particular constructor and forced
developers to extend it every time they wanted to turn a class into a UI component. But
using decorator components is a nicer, more readable and declarative solution.
Listing 5.1 An example of an Angular component
Applies the @Component decorator This component can be used in
HTML as <order-processor>.
The browser should render this text.
The value for this input property is
provided by the parent component.
96 CHAPTER 5 Decorators and advanced types
 Decorators (unlike inheritance) separate the concerns and facilitate easier code
maintenance, since the framework is free to interpret them as it wants. In contrast, if a
component was a subclass, it could override or just expect or rely on certain behavior
of the methods in the superclass.
NOTE There is a proposal to add decorators to JavaScript. The proposal is
currently a Stage 2 draft (https://tc39.github.io/proposal-decorators).
Although decorators were introduced back in 2015, they are still considered an experimental feature, and you have to compile your app with the --experimentalDecorators
tsc option. If you use tsconfig.json, add the following compiler option there:
"experimentalDecorators": true
A decorator can be used to observe or modify the definition of the target (a class,
method, and so on), and the signatures of these special functions (the decorators) differ depending on what the target is. In this chapter, we’ll show you how to create class
and method decorators.
5.1.1 Creating class decorators
A class decorator is applied to a class, and the decorator function is executed when
the constructor executes. A class decorator requires one parameter—a constructor
function for the class. In other words, a class decorator will receive the constructor
function of the decorated class.
 The following listing declares a class decorator that just logs information about the
class on the console.
function whoAmI (target: Function): void {
console.log(`You are: \n ${target}`)
}
NOTE If the return type of a class decorator is void, this function doesn’t
return any value (see section 2.1.1). Such a decorator won’t replace the class
declaration—it’ll just observe the class, as in listing 5.3. But a decorator can
modify a class declaration, in which case it would need to return the modified
version of the class (the constructor function), and we’ll show you how to do
that at the end of this section.
To use the whoAmI decorator, just prepend the class name with @whoAmI. The constructor function of the class will be automatically provided for the decorator. In our example, the constructor has two arguments: a string and number.
Listing 5.2 Declaring a custom whoAmI decorator
Declares the decorator, which takes a
constructor function as an argument
Logs the target class
information
Decorators 97
@whoAmI
class Friend {
constructor(private name: string, private age: number){}
}
When the TypeScript code is transpiled into JavaScript, tsc checks if any decorators
were applied, in which case it will generate additional JavaScript code that will be used
at runtime.
 You can run this code sample in the Playground at http://mng.bz/omop. You’ll
need to turn on the experimentalDecorators config option and click the Run button. It will run the JavaScript version of the code, producing the following output on
the browser’s console:
You are:
function Friend(name, age) {
this.name = name;
this.age = age;
}
TIP To see the effect of the @whoAmI decorator on the generated code in the
Playground, remove the decorator from the Friend class declaration and
note the difference in the JavaScript that’s generated.
You may think that the @whoAmI decorator is not overly useful, so let’s create another
one. Say you’re developing a UI framework and want to allow classes to be turned into
UI components in a declarative way. You want to create a function that takes an arbitrary argument, such as an HTML string for rendering.
function UIcomponent (html: string) {
console.log(`The decorator received ${html} \n`);
return function(target: Function) {
console.log(`Someone wants to create a UI component from \n ${target} `);
}
}
Here you see a decorator function inside another function. We can call the outer
function a decorator factory. It can take any arguments and apply some app logic to
decide which decorator to return. In this case, listing 5.4 has just one return statement, and this code always returns the same decorator function, but nothing stops
you from conditionally returning the decorator you need based on the parameters
provided to the factory function.
Listing 5.3 Applying the decorator whoAmI to a class
Listing 5.4 Declaring a custom UIcomponent decorator
This decorator factory has an argument.
Prints the string received
by the decorator
This is a decorator function.
98 CHAPTER 5 Decorators and advanced types
NOTE A bit later, in the sidebar titled “Formal declarations of decorator signatures,” you’ll see that the requirements for decorator signatures depend on
what they decorate. So how were we able to use an arbitrary argument in the
UIComponent() function? The reason is that UIComponent() is not a decorator, but a decorator factory that returns a decorator with the proper function
(target: Function) signature.
The next listing shows the Shopper class decorated as UIcomponent.
@UIcomponent('<h1>Hello Shopper!</h1>')
class Shopper {
constructor(private name: string) {}
}
Running this code will produce the following output:
The decorator received <h1>Hello Shopper!</h1>
Someone wants to create a UI component from
function Shopper(name) {
this.name = name;
}
You can see this code in action on CodePen at http://mng.bz/nv62.
 So far, all our decorator examples observed the classes—they didn’t modify class
declarations. In the next example, we’ll show you a decorator that does. But first, we’ll
show you a constructor mixin.
 In JavaScript, a mixin is a piece of code that implements a certain behavior. Mixins
are not meant to be used alone, but their behavior can be added to other classes.
Although JavaScript doesn’t support multiple inheritance, you can compose behaviors
from multiple classes using mixins.
 If a mixin has no constructor, mixing its code with other classes comes down to
copying its properties and methods to the target class. But if a mixin has its own constructor, it needs to be capable of taking any number of parameters of any type; otherwise it wouldn’t be “mixable” with arbitrary constructors of target classes.
 TypeScript supports a constructor mixin that has the following signature:
{ new(...args: any[]): {} }
It uses a single rest argument (three dots) of type any[] and can be mixed with other
classes that have constructors. Let’s declare a type alias for this mixin:
type constructorMixin = { new(...args: any[]): {} };
Listing 5.5 Applying a custom UIcomponent decorator
Passes HTML to the decorator
A class constructor that
takes a shopper’s name
Decorators 99
Accordingly, the following signature represents a generic type T that extends
constructorMixin; in TypeScript, this also means that type T is assignable to the
constructorMixin type:
<T extends constructorMixin>
You’ll use this signature to create a class decorator that modifies the original constructor of a class. The class decorator signature will look like this:
function <T extends constructorMixin> (target: T) {
// the decorator is implemented here
}
Now we’re prepared to write a decorator that modifies the declaration (and constructor) of a target class. Let’s say we have the following Greeter class.
class Greeter {
constructor(public name: string) { }
sayHello() { console.log(`Hello ${this.name} `) };
}
We can instantiate and use it like this:
const grt = new Greeter('John');
grt.sayHello(); // prints "Hello John"
We want to create a decorator that can accept a salutation parameter, and add to the
class a new message property, concatenating the given salutation and name. Also, we
want to change the code of the sayHello() method so that it prints the message.
 The following listing shows a higher-order function (a function that returns a function) that implements our decorator. We can also call it a factory function because it
constructs and returns a function.
function useSalutation(salutation: string) {
return function <T extends constructorMixin> (target: T) {
return class extends target {
name: string;
private message = 'Hello ' + salutation + this.name;
sayHello() { console.log(`${this.message}`); }
}
}
}
Listing 5.6 Undecorated Greeter class
Listing 5.7 Declaring the useSalutation decorator
A factory function that takes one parameter—salutation
The decorator’s
body
Redeclares the decorated class
Adds a private
property to the
new class
Redeclares the method
100 CHAPTER 5 Decorators and advanced types
Starting from the return class extends target line, we provide another declaration
of the decorated class. In particular, we’ve added a new message property to the original
class and replaced the body of the sayHello() method so that it uses the salutation provided in the decorator.
 In the next listing, we use the @useSalutation decorator with the Greeter class.
Invoking grt.sayHello() will print “Hello Mr. Smith.”
@useSalutation("Mr. ")
class Greeter {
constructor(public name: string) { }
sayHello() { console.log(`Hello ${this.name} `) }
}
const grt = new Greeter('Smith');
grt.sayHello();
You can see and run this code in the Playground at http://mng.bz/vlM4. Click the
Run button and open the browser console to see the output.
 It’s great that we have such a powerful mechanism for replacing a class declaration,
but use it with caution. Avoid changing the public API of a class, because the static type
analyzer won’t offer autocomplete for decorator-added public properties or methods.
 Say you modified the useSalutation() decorator so it would add a public sayGoodbye() method to the target class. After typing grt., as in listing 5.8, your IDE would still
only prompt you with the sayHello() method and the name property. It wouldn’t suggest sayGoodbye(), even though typing grt.sayGoodbye() will work just fine.
Listing 5.8 Using the decorated Greeter class
Applies the decorator with
an argument to the class
Formal declarations of decorator signatures
A decorator is a function, and its signature depends on the target. Signatures for
class and method decorators won’t be the same. After you install TypeScript, it will
include several files with type declarations. One of them is called lib.es5.d.ts, and it
includes type declarations for decorators for various targets:
declare type ClassDecorator =
<TFunction extends Function>(target: TFunction) =>
➥ TFunction | void;
declare type PropertyDecorator =
(target: Object, propertyKey: string | symbol) => void;
declare type MethodDecorator =
<T>(target: Object, propertyKey: string | symbol,
descriptor: TypedPropertyDescriptor<T>) =>
➥ TypedPropertyDescriptor<T> | void;
declare type ParameterDecorator =
(target: Object, propertyKey: string | symbol,
➥ parameterIndex: number) => void;
The signature for
a class decorator
The signature
for a property
decorator
The signature
for a method
decorator
The signature for a
parameter decorator
Decorators 101
5.1.2 Creating method decorators
Now let’s create a decorator that can be applied to a class’s method. For example, you
may want to create a @deprecated decorator to mark methods that will be removed
soon. As you can see in the sidebar titled “Formal declarations of decorator signatures,” the MethodDecorator function requires three parameters:
 target—An object that refers to the instantiated class that defines the method
 propertyKey—The name of the method being decorated
 descriptor—A descriptor of the method being decorated
The descriptor parameter contains an object describing the method that your code
is decorating. Specifically, TypedPropertyDescriptor has a value property that stores
the original code of the decorated method. By changing the value of this property
inside the method’s decorator, you can modify the original code of the decorated
method.
 Let’s consider a Trader class that has a placeOrder() method:
class Trade {
placeOrder(stockName: string, quantity: number, operation:
➥ string, traderID: number) {
In section 4.2.3, we explained generic functions and the syntax for named functions
as well as fat arrow functions. That section should help you in understanding the signatures shown in the preceding code. Consider the following line:
<T>(someParam: T) => T | void
That’s right—it declares that a fat arrow function can take a parameter of a generic
type T and return either the value of type T or void. Now let’s try to read the declaration of the ClassDecorator signature:
<TFunction extends Function>(target: TFunction) => TFunction | void
It declares that a fat arrow function can take a parameter of a generic type TFunction, which has an additional constraint: the concrete type must be a subtype of
Function. Any TypeScript class is a subtype of Function, which represents a constructor function. In other words, the target for this decorator must be a class, and
the decorator can either return a value of this class’s type or return no value.
Take another look at the @whoAmI class decorator shown in listing 5.2. We didn’t use
a fat arrow expression there, but that function had the following signature, which is
allowed for class decorators:
function whoAmI (target: Function): void
Because the signature of the whoAmI() function doesn’t return a value, we can say
that this decorator just observes the target. If you wanted to modify the original target
in the decorator, you’d need to return the modified class, but its type would be the
same as that originally provided in lieu of TFunction (a subtype of Function). 
102 CHAPTER 5 Decorators and advanced types
// the method implementation goes here
}
// other methods go here
}
Say there’s a trader with the ID 123, and she can place an order to buy 100 shares of
IBM as follows:
const trade = new Trade();
trade.placeOrder('IBM', 100, 'Buy', 123);
This code has worked fine for years, but a new regulation came in: “For audit purposes, all trades must be logged.” One way to do this is to go through all methods
related to buying or selling financial products, which may have different parameters,
and add code that logs their invocations. But creating a @logTrade method decorator
that works with any method and logs the parameters would be a more elegant solution. The following listing shows the code of the @logTrade method decorator.
function logTrade(target, key, descriptor) {
const originalCode = descriptor.value;
descriptor.value = function () {
console.log(`Invoked ${key} providing:`, arguments);
return originalCode.apply(this, arguments);
};
return descriptor;
}
We stored the original method’s code and then modified the received descriptor by
adding a console.log() statement. Then we used the JavaScript function apply() to
invoke the decorated method. Finally, we returned the modified method descriptor.
 In the following listing we applied the @logTrade decorator to the method
placeOrder(). No matter how placeOrder() is implemented, its decorated version
will start by printing this message: “Invoked placeOrder providing:”.
class Trade {
@logTrade
placeOrder(stockName: string, quantity: number,
Listing 5.9 The @logTrade method decorator
Listing 5.10 Using the @logTrade decorator
The method decorator must have three arguments. Stores the original
method’s code
Modifies the code of the
method being decorated
Invokes the target method
Returns the modified method
Decorates the
placeOrder() method
Mapped types 103
operation: string, tradedID: number) {
// the method implementation goes here
}
}
const trade = new Trade();
trade.placeOrder('IBM', 100, 'Buy', 123);
You can run a sample implementation of the preceding code in the TypeScript Playground at http://mng.bz/4e7j.
 After invoking the decorated placeOrder() method, the console output will look
similar to this:
Invoked placeOrder providing:
Arguments(4)
0: "IBM"
1: 100
2: "Buy"
3: 123
By creating a method decorator, we eliminated the need to update the code of potentially many trade-related methods that require auditing. Besides, the @logTrade decorator can work with methods that haven’t even been written yet.
 We’ve shown you how to write class and method decorators, which should give you
a good foundation for mastering property and parameter decorators on your own.
5.2 Mapped types
Mapped types allow you to create new types from existing ones. This is done by applying a transformation function to an existing type. Let’s see how they work.
5.2.1 The Readonly mapped type
Imagine that you need to pass objects of type Person (shown next) to the doStuff()
function for processing:
interface Person {
name: string;
age: number;
}
The Person type is used in multiple places, but you don’t want to allow the doStuff()
function to accidentally modify some of the Person’s properties, like age in the following listing.
Invokes placeOrder()
104 CHAPTER 5 Decorators and advanced types
const worker: Person = {name: "John", age: 22};
function doStuff(person: Person) {
person.age = 25;
}
No properties of the Person type were declared with the readonly modifier. Should
we declare another type just to be used with doStuff(), as follows?
interface ReadonlyPerson {
readonly name: string;
readonly age: number;
}
Do you need to declare (and maintain) a new type each time you need a read-only version of an existing one? There’s a better solution. We can use a built-in mapped type,
Readonly, to turn all the properties of a previously declared type to be readonly. We
just need to change the signature of the doStuff() function to take an argument of
type Readonly<Person> instead of Person.
const worker: Person = {name: "John", age: 22};
function doStuff(person: Readonly<Person>) {
person.age = 25;
}
To understand why an attempt to change the value of the age property generates a
compiler error, you need to see how the Readonly type is declared, which in turn
requires an understanding of the keyof lookup type.
KEYOF AND A LOOKUP TYPE
Reading the declarations of the built-in mapped types in the typescript/lib/lib.es5.d.ts
file can help you understand their inner workings, but it requires some familiarity
with TypeScript’s index type query keyof and a lookup type.
 You can find the following declaration of the Readonly mapping function in
lib.es5.d.ts:
type Readonly<T> = {
readonly [P in keyof T]: T[P];
};
Listing 5.11 The unlawful change of the age
Listing 5.12 Using the mapped type Readonly
Listing 5.13 The declaration of the Readonly mapped type
We don’t want to allow this.
Modifies the existing type with
the mapped type Readonly
This line generates a compiler error.
Mapped types 105
We assume that you’ve read about generics in chapter 4, and that you know what <T>
in angle brackets means. Usually the letter T in generics represents type: K for key, V for
value, P for property, and so on.
keyof is also called an index type query, and
it represents a union of allowed property
names (the keys) of the given type. If the type
Person is our T, then keyof T would represent
a union of name and age. Figure 5.1 shows a
screenshot taken while hovering the mouse
over the propNames custom type. As you can
see, the type of propName is a union of name
and age.
 In listing 5.13, the fragment [P in keyof T] means “give me the union of all the
properties of the given type T.” This fragment looks as if we’re accessing the elements
of some object, but actually, this is done to declare types. The keyof type query can be
used only in type declarations.
 We now know how to get access to the property names of a given type, but to create
a mapped type from an existing one, we also need to know the property types. In case
of the Person type we need to be able to find out programmatically that the property
types are string and number.
 This is what lookup types are for. The
piece T[P] (from listing 5.13) is a lookup
type, and it means “Give me the type of a
property P.” Figure 5.2 shows a screenshot
taken while hovering the mouse over the
propTypes type. The types of the properties are string and number.
 Now let’s read the code in listing 5.13 one more time. The declaration of the
Readonly<T> type means “Find the names and types of the properties of the provided
concrete type, and apply the readonly qualifier to each property.” In our example,
Readonly<Person> will create a mapped type that will look like the following.
interface Person {
readonly name: string;
readonly age: number;
}
Now you can see why an attempt to modify the person’s age results in the compiler
error “Cannot assign to age because it’s a read-only property.” Basically, we took an
existing type, Person, and mapped it to a similar type but with read-only properties.
You can see that this code won’t compile in the Playground at http://mng.bz/Q05v.
Listing 5.14 Applying the Readonly mapped type to the Person type
Figure 5.1 Applying keyof to the Person
type
Figure 5.2 Getting the types of Person’s
properties
106 CHAPTER 5 Decorators and advanced types
 You may say, “OK, I understand how to apply the mapped type Readonly, but
what’s the practical use of it?” Later, in listing 10.16, you’ll see two methods that use
the Readonly type with their message argument, something like this:
replyTo(client: WebSocket, message: Readonly<T>): void
This method can send messages to blockchain nodes over the WebSocket protocol.
The messaging server doesn’t know what types of messages will be sent, and the message type is generic. To prevent accidental modification of the message inside
replyTo(), we use the Readonly mapped type there.
 Let’s consider one more code sample that illustrates the benefits of using keyof
and T[P]. Imagine we need to write a function to filter a generic array of objects,
keeping only those that have a specified value in a specified property. In the first version, we won’t use type checking and will write the function as follows.
function filterBy<T>(
property: any,
value: any,
array: T[]) {
return array.filter(item => item[property] === value);
}
Calling this function with a non-existing property name or the wrong value type will
result in hard-to-find bugs.
 The following listing declares a Person type and function. The last two lines invoke the
function, providing either a non-existing lastName property or the wrong type for age.
interface Person {
name: string;
age: number;
}
const persons: Person[] = [
{ name: 'John', age: 32 },
{ name: 'Mary', age: 33 },
];
function filterBy<T>(
property: any,
value: any,
array: T[]) {
return array.filter(item => item[property] === value);
}
Listing 5.15 A poor version of filterBy()
Listing 5.16 A buggy version of filterBy()
Keeps only those objects that
have the provided value in the
specified property
This function doesn’t
do any type checking.
Filters data
based on the
property/value
Mapped types 107
console.log(filterBy('name', 'John', persons));
console.log(filterBy('lastName', 'John', persons));
console.log(filterBy('age', 'twenty', persons));
Both of the last two code lines will return zero objects without any complaints, even
though the Person type has no lastName property and the type of the age property is
not a string. In other words, the code in listing 5.16 is buggy.
 Let’s change the signature of the filterBy() function so it catches these bugs at
compile time. The new version of filterBy() is shown in the following listing.
function filterBy<T, P extends keyof T>(
property: P,
value: T[P],
array: T[]) {
return array.filter(item => item[property] === value);
}
First of all, the fragment <T, P extends keyof T> tells us that our function accepts
two generic values: T and P. We also added a restriction that P extends keyof T. In
other words, P must be one of the properties of the provided type T. If the concrete
type of T is Person, then P can be either name or age.
 The function signature in listing 5.17 has yet another restriction, value: T[P],
which means the provided value must be of the same type as that declared for P in
type T. That’s why the following lines will give compile errors.
filterBy('lastName', 'John', persons)
filterBy('age', 'twenty', persons)
As you see, introducing keyof and a lookup type in the function signature allows you
to catch possible errors at compile time. You can see this code sample in action in the
TypeScript Playground at http://mng.bz/XpXa.
5.2.2 Declaring your own mapped types
Listing 5.13 shows the transformation function for the built-in Readonly mapped type.
You can define your own transformation functions using similar syntax.
Listing 5.17 A better version of filterBy()
Listing 5.18 These lines won’t compile
A correct invocation
of the function
An incorrect invocation
of the function
An incorrect invocation of the function
Checks that the provided property,
P, belongs to the union [keyof T]
The property to filter by
A value for filtering must be of the
type of the provided property, P
Non-existing lastName property
The value of age
must be a number.
108 CHAPTER 5 Decorators and advanced types
 Let’s try to define a Modifiable type—the opposite of Readonly. In the previous
section, we took a Person type and made all of its properties read-only by applying the
Readonly mapped type: Readonly<Person>. Now suppose the properties of the Person type were originally declared with the readonly modifier as follows:
interface Person {
readonly name: string;
readonly age: number;
}
How could you remove the readonly qualifiers from the Person declaration if you
needed to? There’s no built-in mapped type for that, so let’s declare one.
type Modifiable<T> = {
-readonly[P in keyof T]: T[P];
};
The minus sign in front of the readonly qualifier removes it from all properties of the
given type. Now you can remove the readonly restriction from all properties by applying the Modifiable mapped type.
interface Person {
readonly name: string;
readonly age: number;
}
const worker1: Person = {name: "John", age: 25};
worker1.age = 27;
const worker2: Modifiable<Person> = {name: "John", age: 25};
worker2.age = 27;
You can see this code in the Playground at http://mng.bz/yzed.
5.2.3 Other built-in mapped types
You know that if a property name in a type declaration ends with the modifier ?, the
property is optional. Say we have the following declaration of the Person type:
interface Person {
name: string;
age: number;
}
Listing 5.19 Declaring a custom mapped type, Modifiable
Listing 5.20 Applying the Modifiable mapped type
Results in a compiler error
No errors here
Mapped types 109
Since none of the property names end with a question mark, providing values for name
and age is mandatory. What if you need a type that has the same properties as Person,
but all of its properties should be optional? This is what the mapped type Partial<T>
is for. Its mapping function is declared in lib.es5.d.ts as follows.
type Partial<T> = {
[P in keyof T]?: T[P];
};
Have you spotted the question mark there? Basically, we create a new type by appending the question mark to each property name of the given type. The mapped type
Partial makes all properties in the given type optional.
 Figure 5.3 shows a screenshot taken while hovering the mouse over the declaration
of the worker1 variable. It shows an error message because the worker1 variable has
the type Person, where each property is required, but the value for age was not provided. There are no errors in initializing worker2 with the same object because the
type of this variable is Partial<Person>, so all its properties are optional.
You can now make all properties of a type optional, but can you do the opposite? Can
you take a type that was declared with some optional properties, and make all of them
required? You bet! This can be done with the Required mapped type, which is
declared as follows:
type Required<T> = {
[P in keyof T]-?: T[P];
};
The -? means it’s removing the modifier ?.
 Figure 5.4 shows a screenshot taken while hovering the mouse over the declaration
of the worker2 variable. The properties age and name were optional in the Person
Listing 5.21 The declaration of the Partial mapped type
Figure 5.3 Applying
the Partial type
110 CHAPTER 5 Decorators and advanced types
base type but they’re required in the Required<Person> mapped type. Hence the
error about the missing age.
TIP The Required type was introduced in TypeScript 2.8. If your IDE doesn’t
recognize this type, make sure it uses the proper version of the TypeScript language service. In Visual Studio Code you can see the version in the bottom-right
corner. Click on it to change to a newer version if you have one installed.
You can apply more than one mapped type to a given type. In the following listing, we
apply Readonly and Partial to the Person type. The former will make each property
read-only, and the latter will make each property optional.
interface Person {
name: string;
age: number;
}
const worker1: Readonly<Partial<Person>>
= {name: "John"};
worker1.name = "Mary"; // compiler's error
TypeScript offers yet another useful mapped type called Pick. It allows you to declare
a new type by picking a subset of properties of the given type. Its transformation function looks like this:
type Pick<T, K extends keyof T> = {
[P in K]: T[P];
};
Listing 5.22 Applying more than one mapped type
Figure 5.4 Applying
the Required type
worker1 is still a Person, but
its properties are read-only
and optional. Initializes the property name
but not the optional age
name is read-only and can
be initialized only once.
Conditional types 111
The first argument expects an arbitrary type, T, and the second expects a subset, K, of
the properties of T. You can read it as “from T, pick a set of properties whose keys are
in the union K.” The next listing shows the Person type, which has three properties.
With the help of Pick, we declare a PersonNameAddress mapped type that has two
string properties: name and address.
interface Person {
name: string;
age: number;
address: string;
}
type PersonNameAddress<T, K> = Pick<Person, 'name' | 'address' >;
You may be thinking, “The discussion of built-in mapped types is good, and they do
seem to be useful, but do I need to know how to implement my own?” The answer is
yes, and you’ll see examples of using the Pick mapped type to define a custom
mapped type shortly in figure 5.5 and later on in chapter 10 in the sidebar titled
“Examples of conditional and mapped types.”
 Mapped types allow you to modify existing types, but TypeScript offers yet another
way of changing a type based on some condition. We’ll look at this next.
5.3 Conditional types
With mapped types the transformation function is always the same, but with conditional types the transformation depends on a specific condition.
 Many programming languages, including JavaScript and TypeScript, support conditional (ternary) expressions:
a < b ? doSomething() : doSomethingElse()
If the value of a is less than b, this code line invokes the function doSomething(), otherwise it invokes doSomethingElse(). This expression checks the values and conditionally executes different code. A conditional type would similarly use a conditional
expression, but it would check the expression’s type.
 A conditional type will always be declared in the following form:
T extends U ? X : Y
Here, extends U means “inherits from U” or “is U.” As with generics, these letters can
represent any types.
 In object-oriented programming, the declaration class Cat extends Animal
means that Cat is Animal and Cat has the same (or more) features as Animal. Another
Listing 5.23 Using the Pick mapped type
Declares the Person type
with three properties Declares the
PersonNameAddress
mapped type with
two properties
112 CHAPTER 5 Decorators and advanced types
way to put it is that a cat is a more specific version of an animal. This also means that
the Cat object can be assigned to a variable of type Animal.
 But can an Animal object be assigned to a variable of type Cat? No, it can’t. Every
cat is an animal, but not every animal is a cat.
 Similarly, the expression T extends U ? checks if T is assignable to U. If this is true,
we’ll use type X, and otherwise type Y. The expression T extends U means that the
value of type T can be assigned to a variable of type U.
TIP We already mentioned assignable types while discussing listings 2.20 and
3.6. Look back to those discussions if you need a refresher.
Let’s consider a function that can have different return types based on some condition. Specifically, we want to write a getProducts() function that should return the
type Product if a numeric product ID was provided as an argument. Otherwise, this
function has to return a Product[] array. Using conditional types, the signature of
this function could look like this:
function getProducts<T>(id?: T):
T extends number ? Product : Product[]
If the type of the argument is number, then the return type of this function is Product;
otherwise Product[].
 The following listing includes a sample implementation of such a function. If the
provided optional id is a number, we return one product; if not, we return an array of
two products.
class Product {
id: number;
}
const getProducts = function<T>(id?: T):
T extends number ? Product : Product[] {
if (typeof id === 'number') {
return { id: 123 } as any;
} else {
return [{ id: 123 }, {id: 567}] as any;
}
}
const result1 = getProducts(123);
const result2 = getProducts();
Listing 5.24 A function with a conditional return type
Declares a conditional
return type
Checks the type of the
provided argument
Invokes the function with
a numeric argument
Invokes the function
with no arguments
Conditional types 113
The type of the result1 variable is Product, and the type of result2 is Product[].
You can see it for yourself by hovering the mouse pointer over these variables in the
Playground at http://mng.bz/MOqB.
 In listing 5.24, we used the as type assertion, which tells TypeScript that it
shouldn’t be inferring the type because you know about this type better than TypeScript does. The as any means “TypeScript, don’t complain about this type.” The
problem is that the narrowed type of id is not picked up by the conditional type, so
the function can’t evaluate the condition and narrow the return type to Product.
 Section 3.1.6 had a similar example, where we implemented a getProducts()
function that could be invoked with or without an argument (see listing 3.9). There,
getProducts() was implemented using method overloading.
 Conditional types can be used in many different scenarios. Let’s discuss another
use case.
 TypeScript has a built-in conditional Exclude type, which allows you to discard the
specified types. Exclude is declared in the lib.es5.d.ts file as follows:
type Exclude<T, U> = T extends U ? never : T;
This type excludes types that are assignable to U. Note the use of the type never, which
means “this should never exist; filter it out.” If the type T is not assignable to U, then
keep it.
 Let’s say we have a Person class, and we use this class in multiple places in the app
for the popular TV show The Voice:
class Person {
id: number;
name: string;
age: number;
}
All the vocalists must go through blind auditions, where the judges can’t see them and
know nothing about them. For these auditions, we want to create another type, which
is the same as Person except it won’t have their names or ages. In other words, we
want to exclude the name and age properties from the Person type.
 From the previous section, you remember that the keyof lookup type can give you
a list of all properties from a type. Hence, the following type will contain all the properties of T except those that belong to the given type K:
type RemoveProps<T, K> = Exclude<keyof T, K>;
Let’s create a new type that will be like Person minus name and age:
type RemainingProps = RemoveProps<Person, 'name' | 'age'>;
114 CHAPTER 5 Decorators and advanced types
In this example, the type K is represented by the union 'name' | 'age', and the
RemainingProps type represents the union of the remaining properties, which is only
id in our example.
 Now we can construct a new type that will contain just the RemainingProperties with
the help of the Pick mapped type (which was illustrated previously in listing 5.24).
type RemainingProps = RemoveProps<Person, 'name' | 'age'>;
type PersonBlindAuditions = Pick<Person, RemainingProps>;
Figure 5.5 shows a screenshot taken while hovering the mouse pointer over the
PersonBlindAuditions type. You can also see it in the TypeScript Playground at
http://mng.bz/adGm.
You might say “Wouldn’t it be easier to create a separate PersonBlindAuditions type
that has just the id property?” This might be true in this simple case, where the Person type has just 3 properties. But a person could be described by 30 properties, and
we might want to use it as a base class and create more descriptive conditional types
based on it.
 Even with our 3-property class, using conditional types can be beneficial. What if
some time down the road a developer decides to replace the name property with firstName and lastName in the Person class? If you used the PersonBlindAuditions conditional type, its declaration would start giving you a compile error, and you’d fix it. But
if you didn’t declare PersonBlindAuditions as a conditional type, and simply created
an independent PersonBlindAuditionsIndie class, the developer who renamed the
properties in Person would need to remember to replicate the changes in the PersonBlindAuditionsIndie class.
The Person type,
minus two properties
Figure 5.5 Combining Pick and Exclude
Conditional types 115
 Besides, the RemoveProperties type is generic. You can use it to remove any properties from any types.
5.3.1 The infer keyword
Our next challenge is to try to find the return type of a function and replace it with
another one. Let’s say we have an interface that declares some properties and methods, and we need to wrap each of the methods into a Promise so they run asynchronously. For simplicity, we’ll consider a SyncService interface, which declares one
property, baseUrl, and one method, getA():
interface SyncService {
baseUrl: string;
getA(): string;
}
There’s nothing to do with the baseUrl property as it is, but we want to promisify the
getA() method. Here are the challenges:
 How can we differentiate properties from methods?
 How can we reach out to the original return type of the method before wrapping it into a Promise?
 How can we preserve the existing argument types of the methods?
Since we need to differentiate properties from methods, we’ll use conditional types;
mapped types will help us with modifying method signatures. Our goal is to create a
Promisify type and apply it to SyncService. This way the implementation of getA()
will have to return a Promise. We want to be able to write code as in the following listing.
class AsyncService
implements Promisify<SyncService> {
baseUrl: string;
getA(): Promise<string> {
return Promise.resolve('');
}
}
We want to declare a new Promisify mapped type that will loop through all properties of a given type T and convert the signatures of its methods so they become asynchronous. The conversion will be done by a conditional type, with the condition being
that the type U (the supertype of T) has to be a function that can take any number of
arguments of any types and may return any value:
T extends (...args: any[]) => any ?
Listing 5.25 Promisifying synchronous methods
Maps SyncService to Promisify
No need to modify properties
from SyncService
The original return type must
be wrapped in Promise.
116 CHAPTER 5 Decorators and advanced types
After the question mark, you provide the type to use if T is a function; you provide
another type after the colon if T isn’t a function (not shown).
 The type T must be assignable to a type that looks like a function signature. If the
provided type is a function, we want to wrap the return of this function into a Promise.
The problem is that if we use the type any, we’ll lose the type information for function
arguments as well as the return type.
 Let’s assume that a generic type R represents the return type of the function. Then
we can use the infer keyword with this variable R:
T extends (...args: any[]) => infer R ?
By writing infer R, we instruct TypeScript to check the provided concrete return type
(for example, string for the getA() method) and replace infer R with this concrete
type. Similarly, we can replace the any[] type in the function arguments with infer A:
T extends (...args: infer A) => infer R ?
Now we can declare our conditional type as follows:
type ReturnPromise<T> =
T extends (...args: infer A) => infer R ? (...args: A) => Promise<R> : T;
This instructs TypeScript: “If a concrete type for T is a function, wrap its return type:
Promise<R>. Otherwise, just preserve its type T.” The conditional ReturnPromise<T>
type can be applied to any type, and if we want to enumerate all the properties of a class,
interface, and so on, we can use the keyof lookup type to get ahold of all the properties.
 If you read section 5.2 on mapped types, the syntax of the next snippet should be
familiar to you:
type Promisify<T> = {
[P in keyof T]: ReturnPromise<T[P]>;
};
The Promisify<T> mapped type will iterate through the properties of T and apply to
them the conditional ReturnPromise type. In our example, we’ll do Promisify
<SyncService>, which won’t do anything to the baseUrl property, but will change
the return type of getA() to Promise<string>.
 Figure 5.6 shows the entire script, which you can also see in the Playground at
http://mng.bz/gVWv. 
Summary 117
Summary
 Using TypeScript decorators, you can add metadata to a class, function, property, or parameter.
 Decorators allow you to modify the type declaration or a behavior of a class,
method, property, or parameter. Even if you don’t write your own decorators,
you need to understand their use if one of the frameworks uses them.
 You can create a type based on another one.
 Mapped types allow you to create apps that have a limited number of basic types
and many derived types based on the basic ones.
 Conditional types allow you to postpone making a decision about what type to
use; the decision is made at runtime based on some condition.
 These language features are not simple to understand, but they show the power
of the language. When we discuss the code of the blockchain app in chapter 10,
you’ll see the practical use of mapped and conditional types.
Hover over result: it’s a
type of Promise<string>.
Figure 5.6 Combining conditional and mapped types
118
Tooling
TypeScript is one of the most loved languages. Yes, people love its syntax. But probably the main reason why it’s loved is the tooling. TypeScript developers appreciate
autocompletion, those squiggly lines indicating errors as you type, and the refactoring offered by IDEs.
 And the best part is that most of these features are implemented by the TypeScript team and not by IDE developers. You’ll see the same autocomplete and error
messages in the online TypeScript Playground, in Visual Studio Code, or in WebStorm. When you install TypeScript, its bin directory includes two files: tsc and
tsserver. The latter is the TypeScript Language Service that IDEs use to support
This chapter covers
 Debugging TypeScript code with the help of
source maps
 The role of linters
 Compiling and bundling TypeScript apps with
Webpack
 Compiling TypeScript apps with Babel
 How to compile TypeScript with Babel and bundle
it with Webpack
Source maps 119
these productivity features. When you type your TypeScript code, the IDEs communicate with tsserver, which compiles the code in memory.
 With the help of source map files, you can debug your TypeScript code right in the
browser. There are tools called linters; they allow you to enforce the coding styles in
your organization.
 Type declaration files (.d.ts files) allow tsserver to offer context-sensitive help showing the signatures of available functions or of object properties. Thousands of type
declaration files for popular JavaScript libraries are available publicly, and they allow
you to be more productive even with code that’s not written in TypeScript.
 All these conveniences add up and explain why people like TypeScript. But just
having a great compiler is not enough for real-world projects, which consist of a
diverse set of assets like JavaScript code, CSS, images, and so on. We’ll take a look at
some essential tools for modern web development: Webpack bundler and Babel. We’ll
also briefly review the emerging tools ncc and Deno.
6.1 Source maps
Code written in TypeScript gets compiled into the JavaScript, which is executed in a
browser or a standalone JavaScript engine. To debug a program, you need to provide
its source code to the debugger, but we have two versions of the source code: the executable code is in JavaScript, and the original is in TypeScript. We’d like to debug the
TypeScript code, and source map files allow us to do this.
 Source map files have .map extensions, and they contain JSON-formatted data that
maps the corresponding code in the generated JavaScript to the original language,
which in our case is TypeScript. If you decide to debug a running JavaScript program
that was written in TypeScript, just have the browser download the source map files
generated during compilation, and you’ll be able to place breakpoints in the TypeScript code even though the engine runs JavaScript.
 Let’s take a simple TypeScript program, shown in the following listing, and compile it with the --sourceMap generation option turned on. After that, we’ll peek inside
the generated source map.
class Greeter {
static sayHello(name: string) {
console.log (`Hello ${name}`);
}
}
Greeter.sayHello('John');
Let’s compile this file, generating the source map file:
tsc greeter.ts --sourceMap true
Listing 6.1 The greeter.ts file
Prints the name
on the console
Invokes the
sayHello() method
120 CHAPTER 6 Tooling
After the compilation is complete, you’ll see the files greeter.js and greeter.js.map.
The latter is a source map file, a fragment of which is shown in the following listing.
{"file":"greeter.js",
"sources":["greeter.ts"],
"mappings":"AAAA;IAAA;IAMA,CAAC;IAJU,gBAAQ,..."
}
This file is not expected to be read by humans, but you can see that it has a file property with the name of the generated JavaScript file and a sources property with the
name of the source TypeScript file. The mappings property contains mappings of code
fragments in the JavaScript and TypeScript files.
 How does the JavaScript engine guess that the name of the file that contains the
mapping is greeter.js.map? No guesses are needed. The Typescript compiler simply
adds the following line at the end of the generated greeter.js file:
//# sourceMappingURL=greeter.js.map
Now let’s run our little greeter app in the browser and see if we can debug its TypeScript code. First, let’s create an HTML file that loads greeter.js:
<!DOCTYPE html>
<html>
<body>
<script src="greeter.js"/>
</body>
</html>
Next we need a web server that will serve the preceding HTML document to the
browser. You can download and install a convenient live-server npm package as follows:
npm install -g live-server
Finally, start this server in the Terminal window from the directory where the greeter
files are located:
live-server
It’ll open the Chrome browser at localhost:8080 and will load the code from
index.html (listing 6.3). You’ll see a blank page. Open the Chrome Dev Tools in the
Sources tab and select the greeter.ts file. In the source code, click to the left of line 5
to place a breakpoint there. The Sources panel should look similar to the one in figure 6.1.
Listing 6.2 The generated source map file, greeter.js.map
Listing 6.3 An index.html file that loads greeter.js
Loads the JavaScript file
here, not the TypeScript
Source maps 121
Refresh the page, and the execution of this code will stop at the breakpoint, as shown
in figure 6.2. You’ll be able to use familiar debugger controls, like step forward, step
into, and so on.
NOTE Although each IDE comes with its own debugger, we prefer debugging
the source code in Chrome Dev Tools. You can even debug code that runs as
a standalone Node.js app in Chrome, and we’ll explain how to do that in the
sidebar “Debugging Node.js code in the browser” at the end of section 10.6.1.
(We cover debugging Node.js apps in chapter 10, where you can see a Node.js
server used in the blockchain app.)
Now we’ll show you the --inlineSources Typescript compiler option, which affects
the process of generating source maps. With this option, the .js.map file will also
Figure 6.1 Setting a breakpoint in the TypeScript code
Figure 6.2 The execution paused in the debugger
122 CHAPTER 6 Tooling
include the TypeScript source code of your app. Try compiling the greeter.ts file as
follows:
tsc greeter.ts --sourceMap true --inlineSources true
It still produces the greeter.js and greeter.js.map files, but the latter now also includes
the code from greeter.ts. This option eliminates the need to deploy separate .ts files
under your web server, but you can still debug the TypeScript code.
NOTE Deploying files with the .js.map extension in production servers
doesn’t increase the size of the code downloaded by the browser. The browser
downloads the source map files only if the user opens the browser’s Dev
Tools. The only reason not to deploy the source maps in a production server
is if you want to prevent users from reading the source code of your app.
6.2 The TSLint linter
Linters are tools that check and enforce coding style. For example, you may want to
ensure that all string values are specified in single quotes, or you may want to disallow
unnecessary parentheses. Such restrictions are rules that you can configure in text
files.
 JavaScript developers use several linters: JSLint, JSHint, and ESLint. TypeScript
developers use the TSLint linter, which is an open source project maintained by
Palantir (https://palantir.github.io/tslint). There is also a plan to merge TSLint and
ESLint to ensure a unified linting experience—you can read more about this effort on
the Palantir blog (“TSLint in 2019” at http://mng.bz/eD9V). Once this effort is complete, JavaScript and TypeScript developers will use ESLint (https://eslint.org).
Because many TypeScript teams continue to use TSLint, we’ll provide a basic introduction to this tool here.
 To get started using TSLint, you first need to install it in your project. Let’s start
from scratch. Create a new directory, open the Terminal window there, and initialize a
new npm project with the following command:
npm init -y
The -y option will silently accept all the default options while creating the package
.json file there.
 Then install TypeScript and ts-lint there as follows:
npm install typescript tslint
This will create a node_modules directory and install TypeScript and tslint in it. The
tslint executable will be located in the node_modules/.bin directory.
 Now create a tslint.json configuration file using the following command:
./node_modules/.bin/tslint --init
The TSLint linter 123
TIP Starting from version 5.2, npm comes with an npx command line that can
run executables from node_modules/.bin; for example, npx tslint --init.
You can read more about this useful command on the npm site at www.npmjs
.com/package/npx.
That command will create the tslint.json file with the following content.
{
"defaultSeverity": "error",
"extends": [
"tslint:recommended"
],
"jsRules": {},
"rules": {},
"rulesDirectory": []
}
This configuration file states that tslint should extend the preset recommended rules,
which you can find in the node_modules/tslint/lib/configs/recommended.js file.
Figure 6.3 shows a snapshot of the recommended.js file.
Listing 6.4 The generated tslint.json file
Optional Uses the recommended rules
custom
rules go
here An optional directory
with custom rules
Figure 6.3 A fragment from recommended.js
124 CHAPTER 6 Tooling
The rule in lines 126–128 looks like this:
quotemark: {
options: ["double", "avoid-escape"],
},
This enforces the use of double quotes around strings. The "avoid-escape" rule
allows you to use the “other” quote mark in cases where escaping would normally be
required. For double quotes, “other” would mean single quotes.
 Figure 6.4 shows a screenshot of the WebStorm IDE, showing a linting error on the
first line. Although using single quotes around strings is fine with the Typescript compiler, the quotemark rule states that in this project you should use single quotes.
TIP Hover over the TSLint error, and the IDE may offer you an auto-fix.
To avoid using escape characters for a string inside another string in figure 6.4, on
line 5 we surrounded “World” with double quotes, and the linter didn’t complain
because of the avoid-escape option.
TIP To enable TSLint in VS Code, install its TSLint extension. Click on the
Extension icon on the sidebar, and search for TSLint in the Marketplace.
Make sure that VS Code uses the current version of TypeScript (the version
number is shown at the bottom-right corner in the status bar). Changing the
TypeScript version is described in the VS Code documentation (http://
mng.bz/pynK).
In figure 6.4 you may notice a short squiggly line in the empty line 3. If you hover your
mouse over this squiggly line, you’ll see another linter error: “TSLint: Consecutive
blank lines are forbidden (no-consecutive-blank-lines).” The name of the rule is
shown in parentheses here, and you can find the following rule in the recommended.js file:
"no-consecutive-blank-lines": true,
TIP You can see a description of the TSLint core rules on the TSLint site at
https://palantir.github.io/tslint/rules/.
Let’s override the no-consecutive-blank-lines rule in the tslint.json file shown in
listing 6.4. We’ll add a rule there allowing consecutive blank lines.
Figure 6.4 TSLint reports errors
Bundling code with Webpack 125
{
"defaultSeverity": "error",
"extends": [
"tslint:recommended"
],
"jsRules": {},
"rules": {"no-consecutive-blank-lines": false},
"rulesDirectory": []
}
By assigning the value false to the no-consecutive-blank-lines rule, we’ve overridden its value from the recommended.js file. Now that little squiggly line in figure 6.4
will disappear.
 You can override the recommended rules or add new ones that comply with your
coding style or the style accepted by your project team.
6.3 Bundling code with Webpack
When a browser makes requests to a server, it gets HTML documents, which may
include additional files like CSS, images, videos, and so on. In part 2 of this book,
we’ll be working with a sample blockchain app that has multiple files. Often you’ll
also be using one of the popular JavaScript frameworks, which could add hundreds of
files to your app. If all these app files were deployed separately, the browser would
need to make hundreds of requests to load them. The size of your app could be several megabytes.
 Real-world applications consist of hundreds and even thousands of files, and we
want to minimize, optimize, and bundle them together during deployment. Figure 6.5
shows how various files are given to Webpack, which produces a smaller number of
files for deployment.
Listing 6.5 Overriding a rule in tslint.json
We added this line.
.js
.js .css
.jpg .png
.png Webpack
.ts .css .sass
.ts .png
Modules with
dependencies
Static assets
for deployment
.js
Figure 6.5 Bundling sources with Webpack
126 CHAPTER 6 Tooling
Bundling multiple files into one can offer better performance and faster downloads.
Technically, you can specify one output file in tsconfig.json, and tsc can place the generated code of multiple .ts file into a single output file, but this only works if the compiler’s module option is System or AMD; the output file generated by the bundler won’t
include third-party dependencies like JavaScript libraries.
 Also, you want to be able to configure development and production builds differently. For production, you’ll add optimization and minimization, whereas in development you’ll just be bundling files together.
 Several years ago, JavaScript developers were mostly using general task runners like
Grunt and Gulp for orchestrating the build process. Today they use such bundlers as
Webpack, Rollup, and Browserify. With the help of plugins, bundlers can also wear
the hats of compilers if need be.
 In this section, we’ll introduce the Webpack bundler (https://github.com/
webpack), which was created for web applications running in a browser. Webpack supports many typical tasks required to prepare web application builds, and it requires
minimal configuration.
 You can install Webpack either globally or locally (in the node_modules subdirectory
of your project). Webpack also has a command-line interface (CLI) tool, webpack-cli
(https://github.com/webpack/webpack-cli). Prior to Webpack 4, properly configuring a Webpack project wasn’t easy, but the webpack-cli scaffolding tool greatly
simplifies Webpack configuration.
 To install Webpack and its CLI globally on your computer, run the following command (-g is for global):
npm install webpack webpack-cli -g
NOTE Installing Webpack (or any other tool) globally allows you to use it with
multiple projects. This is great, but in your organization, the production
build might be done on a dedicated computer with restrictions as to what
software can be installed globally. That’s why we’ll use locally installed Webpack and Webpack CLI.
Webpack is probably the most popular bundler in the JavaScript ecosystem, and in the
next section we’ll bundle a simple JavaScript app.
6.3.1 Bundling JavaScript with Webpack
The goal of this section is to show you how to configure and run Webpack for a basic
JavaScript app, so you can see how this bundler works and what to expect as its output.
 The source code that comes with this chapter has several projects, and we’ll start
with the project called webpack-javascript. This is an npm project, and you need to
install its dependencies by running the following command:
npm install
Bundling code with Webpack 127
In this project we have a tiny JavaScript file that uses a third-party library called Chalk
(www.npmjs.com/package/chalk), which is listed in the dependencies section of
package.json for this project.
{
"name": "webpack-javascript",
"description": "Code sample for chapter 6",
"homepage": "https://www.manning.com/books/typescript-quickly",
"license": "MIT",
"scripts": {
"bundleup": "webpack-cli"
},
"dependencies": {
"chalk": "^2.4.1"
},
"devDependencies": {
"webpack": "^4.28.3",
"webpack-cli": "^3.1.2"
}
}
The Webpack packages are located in the devDependencies section because we need
them only on the developer’s computer. We’ll be running this app by entering the
command npm run bundleup, which will run the webpack-cli executable located in
the node_modules/.bin directory.
 The Chalk package paints the Terminal window’s text in different colors, but what
this package does is irrelevant for us here. Our goal is to bundle our JavaScript code
(index.js) with a library. The code of index.js is shown in the next listing.
const chalk = require('chalk');
const message = 'Bundled by the Webpack';
console.log(chalk.black.bgGreenBright(message));
Typically, developers use the webpack.config.js file for creating custom configurations,
even though it’s optional as of Webpack version 4. This is where you configure the
build for the project. The following listing shows the webpack.config.js file for this
project.
const { resolve } = require('path');
module.exports = {
entry: './src/index.js',
output: {
Listing 6.6 The package.json of the webpack-javascript project
Listing 6.7 index.js: the source code of the webpack-javascript app
Listing 6.8 webpack.config.js: Webpack’s config file
Defines a command
to run Webpack
The chalk library
The
Webpack
bundler The Webpack commandline interface
Loads the library
Uses the library
The source filename
to bundle
128 CHAPTER 6 Tooling
filename: 'index.bundle.js',
path: resolve(__dirname, 'dist')
},
target: 'node',
mode: 'production'
};
To create a bundle, Webpack needs to know the main module (the entry point) of
your application, which may have dependencies on other modules or third-party
libraries. Webpack loads the entry point module and builds a memory tree of all
dependent modules, if any.
 In this config file, we use Node’s path module, which can resolve the absolute path
of a file with the help of the __dirname environment variable. Webpack runs under
Node, and the __dirname variable will store the directory where the executable JavaScript module (webpack.config.js) is located. The fragment resolve(__dirname,
'dist') thus instructs Webpack to create a subdirectory named dist in the root of the
project, and the bundled app will be located in the dist directory.
TIP Storing the output files in a separate directory will allow you to configure
your version control system to exclude the generated files. If you use Git, just
add the dist directory to the .gitignore file.
We specified production as the value of the mode property so Webpack would minimize the size of the bundle.
 Starting with Webpack 4, configuring a project became much easier because it
introduced the default modes production and development. In production mode,
the size of the generated bundles is small, the code is optimized for runtime, and the
development-only code is removed from the sources. In development mode, the compilation is incremental and you can debug the code in the browser.
 The webpack-cli tool allows you to bundle your app files providing the entry, output, and other parameters right on the command line. It can also generate a configuration file for your project.
 Let’s run Webpack to see how it will bundle up our app. You’ll be running the local version of Webpack installed in the node_modules directory of your project. In listing 6.6, we
defined the npm command bundleup so we can run the locally installed webpack-cli:
npm run bundleup
The bundling will finish in several seconds, and your console may look similar to figure 6.6.
 From this output we can see that Webpack built the bundle named index.bundle
.js, and its size is about 22 KB. This is the main chunk (a.k.a. bundle). In our simple
example we have only one bundle, but larger apps are usually split into modules, and
Webpack can be configured to build multiple bundles.
The name of the
output bundle
The
location of
the output
bundle We’ll run this app under
Node.js; don’t inline builtOptimizes the file size in Node.js modules.
of the output bundle
Bundling code with Webpack 129
Note that the size of the original src/index.js file was only 125 bytes. The size of the
bundle is much larger because it includes not only the three lines of our index.js file,
but also the Chalk library and all its transitive dependencies. Webpack also adds its
own code to keep track of the bundle content.
 Change the value of the mode property in webpack.config.js to be development,
and rerun the bundling. Under the hood, Webpack will apply different predefined
configuration settings, and the size of the generated file will be more than 56 KB (as
opposed to 22 KB in production mode).
 Open the dist/index.bundle.js file in any plain text editor. In the production version, you’ll just see one very long optimized line, whereas the development bundle
will have readable content with comments.
 The generated index.bundle.js file is a regular JavaScript file, and you can use it in
an HTML <script> tag or any other place where JavaScript filenames are allowed.
This particular app is not intended to run in a browser—it’s for Node.js, and you can
run it with the following command:
node dist/index.bundle.js
Figure 6.7 shows the console output, where the
Chalk library displayed the message “Bundled by
the Webpack” on a bright green background.
 We ran this example using the Node.js runtime, but for web apps, Webpack offers a development server that can serve your web pages. It has to be installed separately:
npm install webpack-dev-server -D
Running this
executable
Generated
bundle
The source
code
Figure 6.6 The console output of npm run bundleup
Figure 6.7 Running your first bundle
130 CHAPTER 6 Tooling
After that, add the start npm command in the scripts section of package.json. It’ll
look like this:
"scripts": {
"bundleup": "webpack-cli --watch",
"start": "webpack-dev-server"
}
Now let’s create a super-simple index.js JavaScript file:
document.write('Hello World!');
We’ll ask Webpack to generate a bundle for this file and save it in dist/bundle.js. The
webpack.config.js file will be similar to the one shown in listing 6.8 with two changes:
we’ll add the devServer property and change the mode to development.
const { resolve } = require('path');
module.exports = {
entry: './src/index.js',
output: {
filename: 'index.bundle.js',
path: resolve(__dirname, 'dist')
},
target: 'node',
mode: 'development',
devServer: {
contentBase: '.'
}
};
In devServer you can configure any options that webpack-dev-server allows on the command line (see the Webpack documentation at https://webpack.js.org/configuration/
dev-server/). We use contentBase to specify that the files should be served from the
current directory.
 Accordingly, our HTML file, index.html, can refer to index.bundle.js.
<!DOCTYPE html>
<html>
<body>
<script src="dist/index.bundle.js"></script>
</body>
</html>
Listing 6.9 Adding the devServer property
Listing 6.10 The index.html file that loads the bundled JavaScript
Run webpack in watch mode so it
rebuilds the bundle on code changes.
Configuring Webpack
for dev mode
Adding a section for
webpack-dev-server
Bundling code with Webpack 131
We’re now ready to build the bundle and start the web server. Building the bundle
comes first:
npm run bundleup
To start the server using webpack-dev-server, just run the following command:
npm start
You can find this app in the webpack-devserver
directory. Open your browser to localhost:8080
and it will greet the world as seen in figure 6.8.
 When you serve your application with webpack-dev-server, it’ll run on the default port
8080. Because we started Webpack in watch
mode, it will recompile the bundle each time
you modify the code.
 Now that you’ve seen how to bundle a pure JavaScript project, let’s see how to use
Webpack with TypeScript code.
6.3.2 Bundling TypeScript with Webpack
The source code that comes with this chapter has several projects, and in this section
we’ll work with the one in the webpack-typescript directory. This project is almost the
same as the one in the previous section. It includes a three-line TypeScript index.ts file
(in the previous section it was index.js) that uses the same JavaScript Chalk library.
 Let’s highlight the differences, starting with the index.ts file.
import chalk from 'chalk';
const message: string = 'Bundled by the Webpack';
console.log(chalk.black.bgGreenBright(message));
The Chalk library explicitly exposes a default export. That’s why, instead of writing
import * as chalk from 'chalk', we wrote import chalk from chalk. We didn’t
have to explicitly declare the type of message, but we wanted to make it obvious that
this is TypeScript code.
 In this project, the package.json file has two additional lines in the devDependencies
section (compared to listing 6.6). We’ve added ts-loader and typescript.
"devDependencies": {
"ts-loader": "^5.3.2",
Listing 6.11 index.ts: the source code of the webpack-typescript app
Listing 6.12 The devDependencies section in package.json
Imports the default object to access this library The type of the
message variable is
declared explicitly.
Uses the library
Added the
TypeScript loader
Figure 6.8 The browser rendering the
index.html document
132 CHAPTER 6 Tooling
"typescript": "^3.2.2",
"webpack": "^4.28.3",
"webpack-cli": "^3.1.2"
}
Usually build-automation tools provide developers with a way to specify additional
tasks that need to be performed during the build process, and Webpack offers loaders
and plugins that allow you to customize builds. Webpack loaders preprocess one file at
a time, whereas plugins can operate on a group of files.
TIP You can find a list of loaders in the Webpack docs on GitHub at
http://mng.bz/U0Yv.
Webpack loaders are transformers that take a source file as input and produce
another file as output (in memory or on disk). For example, json-loader takes an
input file and parses it as JSON. For compiling TypeScript to JavaScript, ts-loader
uses the Typescript compiler, which explains why we added it to the devDependencies
section in package.json. The Typescript compiler uses tsconfig.json, which has the following content.
{
"compilerOptions": {
"target": "es2018",
"moduleResolution": "node"
}
}
The moduleResolution option tells tsc how to resolve modules if the code includes
import statements. If your app includes an import { a } from "moduleA" statement,
tsc needs to know where to look for moduleA.
 There are two strategies for module resolution: Classic and Node. In the Classic
strategy, tsc will look for the definition of moduleA in the moduleA.ts file and in the
moduleA.d.ts type definition file. In the Node strategy, module resolution will also try
to find the module in files located in the node_modules directory, which is exactly
what we need, because the third-party chalk library is installed in node_modules.
TIP To read more about module resolution, visit the TypeScript documentation at www.typescriptlang.org/docs/handbook/module-resolution.html.
The following listing shows how we added ts-loader in the rules section of webpack
.config.js.
Listing 6.13 tsconfig.json: the compiler’s config options
Added the Typescript
compiler
Specifies compiling into the
JavaScript syntax described
in the ECMAScript 2018 spec
Uses the Node.js
module resolution
Bundling code with Webpack 133
const { resolve } = require('path');
module.exports = {
entry: './src/index.ts',
output: {
filename: 'index.bundle.js',
path: resolve(__dirname, 'dist')
},
module: {
rules: [
{
test: /\.ts$/,
exclude: /node_modules/,
use: 'ts-loader'
}
]
},
resolve: {
extensions: [ '.ts', '.js' ]
},
target: 'node',
mode: 'production'
};
In short, we say to Webpack, “If you see a file with the name extension .ts, use ts-loader
to preprocess them. Ignore the .ts files located under the node_modules directory—
there’s no need to compile them.”
 In this simple example, the rules array has just one loader configured—ts-loader.
In a real-world project, it usually includes multiple loaders. For example, css-loader is
used for processing CSS files; file-loader resolves import/require() in a source file
into a URL and emits it into the output bundle (it’s used for handling images or other
files that have specified name extensions).
TIP There’s an alternative Webpack loader for TypeScript called awesometypescript-loader (see https://github.com/s-panferov/awesome-typescriptloader), which may show better performance on large projects.
Now you can build the index.bundle.js bundle just as we did in the previous section by
running the following command:
npm run bundleup
To make sure that the bundled code works, just run it:
node dist/index.bundle.js
Listing 6.14 webpack.config.js: the Webpack configuration file
Rules for modules (configure
loaders, parser options, etc.)
Applies to
files with
the .ts
extension
Rules for modules (configure
loaders, parser options, etc.)
Compiles TypeScript using options
from the existing tsconfig.json
Adds the .ts extension to the
resolve property so it can
import TypeScript files
134 CHAPTER 6 Tooling
The configuration file presented in listing 6.14 is rather small and simple; in realworld projects, the webpack.config.js file can be complex and include multiple loaders and plugins. We just used the TypeScript loader in our tiny app, but you’ll likely
also be using loaders for HTML, CSS, images, and others.
 Your project may have multiple entry files, and you might want to use special
plugins to create bundles in a special way. For example, if your app will be deployed as
10 bundles, Webpack can extract the common code (from the framework you use)
into a separate bundle so 9 others won’t duplicate it.
 As the complexity of the bundling process increases, the webpack.config.js JavaScript file grows too, and it becomes more difficult to write and maintain. Providing a
value of a wrong type can result in errors during the bundling process, and the error
description may not be easy to understand. The good news is that you can write the
Webpack configuration file in TypeScript (webpack.config.ts), getting help from
the static type analyzer as with any other TypeScript code. You can read about using
What Webpack plugins are for
Webpack loaders transform files one at a time, but plugins have access to multiple
files, and they can process them before or after the loaders kick in. You can find a
list of available Webpack plugins at https://webpack.js.org/plugins.
For example, the SplitChunksPlugin plugin allows you to break a bundle into separate
chunks. Say your app code is split into two modules, main and admin, and you want
to build two corresponding bundles. Each of these modules uses a framework, such
as Angular. If you just specify two entry points (main and admin), each bundle will
include the application code as well as its own copy of Angular’s code.
To prevent this from happening, you can process the code with the SplitChunkPlugin.
With this plugin, Webpack won’t include any of the Angular code in the main and admin
bundles; it will create a separate shareable bundle with only the Angular code. This
will lower the total size of your application, because it shares one copy of Angular
between two application modules. In this case, your HTML file should include the vendor bundle first (for example, the code of the Angular framework), followed by the application bundle.
The UglifyJSPlugin plugin performs code minification of all compiled files. It’s a wrapper for the popular UglifyJS minifier, which takes the JavaScript code and performs
various optimizations. For example, it compresses the code by joining consecutive
var statements, removes unused variables and unreachable code, and optimizes if
statements. Its mangler tool renames local variables to single letters.
The TerserWebpackPlugin also performs code minification using terser (a special
JavaScript parser), mangler, optimizer, and the beautifier toolkit for ES6.
Using the mode: "production" option in the webpack.config.js file, you can implicitly engage a number of Webpack plugins that will optimize and minimize your code
bundles. If you’re interested in which specific plugins are being used in production
mode, see the Webpack documentation at https://webpack.js.org/concepts/mode/
#mode-production.
Using the Babel compiler 135
TypeScript for configuring Webpack in the Webpack documentation at https://
webpack.js.org/configuration/configuration-languages.
 In this section, we presented a project where the TypeScript code uses the Chalk
JavaScript library. In chapter 7, we’ll provide a more detailed discussion of mixed
TypeScript-JavaScript projects. Meanwhile, let’s see how we can use TypeScript with
another popular tool called Babel.
6.4 Using the Babel compiler
Babel is a popular JavaScript compiler that offers a remedy for a well-known issue: not
every browser supports every language feature declared in ECMAScript. We’re not
even talking about the full implementation of a specific ECMAScript version. At any
given time, one browser may implement a specific subset of ECMAScript 2019, while
another still understands only ECMAScript 5. Visit the caniuse.com site and search for
“arrow functions.” You’ll see that Internet Explorer 11, Opera Mini, and some others
do not support them.
 If you’re developing a new web app, you’ll want to test it against all the browsers
your users may have. Babel allows you to write modern JavaScript and compile it down
to older syntaxes. Although tsc allows you to specify a particular ECMAScript spec as a
target for compiling (such as ES2019), Babel is more fine-grained. It allows you to
selectively pick language features that should be transformed to the JavaScript supported by older browsers.
 Figure 6.9 shows a fragment of a browser compatibility table (from http://
mng.bz/O9qw). At the top, you’ll see the names of the browsers and compilers. On the
left is a list of features. A browser, compiler, or server’s runtime may fully or partially
support some of the features, and Babel plugins allow you to specify that only certain
features should be transformed into the older code. The complete list of plugins is
available in the Babel documentation at https://babeljs.io/docs/en/plugins.
Checking the string trimming
feature implementation
The Edge 18 browser partially
implements string trimming.
Figure 6.9 A fragment of a browser compatibility table
136 CHAPTER 6 Tooling
For the sake of discussion, we picked the “string trimming” feature from the ES2019
spec (see the black arrow on the left of figure 6.9). Let’s say our app needs to work in
the Edge browser. Follow the vertical arrow, and you’ll see that Edge 18 only partially
(2/4) implements string trimming at this time.
 We can use the string trimming feature in our code, but we’ll need to ask Babel to
compile this feature into the older syntax. Sometime later, when Edge fully supports this
feature and no compiling is needed, Babel will be flexible enough to help you with this.
 Babel consists of many plugins, each compiling a particular feature of the language, but trying to find and map features to plugins would be a time-consuming task.
That’s why Babel plugins are combined into presets, which are lists of plugins that
you’ll want to apply for compiling. In particular, preset-env allows you to specify the
ECMAScript features and the browsers that your app should support.
 In section A.12 of the appendix, we include a screenshot from http://babeljs.io
illustrating Babel’s REPL tool. Take a look at Babel’s Try It Out menu, shown in figure
6.10, concentrating on the left navigation bar that allows you to configure presets.
 Each preset is just a group of plugins, and if you want to compile your code into
the ES2015 syntax, just select the es2015 check box. Instead of using ECMAScript spec
names, you can configure specific versions of the browsers or other runtimes using
the ENV PRESET option. The white arrow in figure 6.10 shows the editable box with
the suggested values for the ENV preset: >2%, ie 11, safari > 9. This means that you
Specify browsers
support here. Figure 6.10 Configuring the ENV preset
Using the Babel compiler 137
want Babel to compile the code so it’ll run in all browsers with a market share of 2%
or more, and also in Internet Explorer 11 and Safari 9.
 Neither IE 11 nor Safari 9 support arrow functions, and if you enter (a,b) ? a + b;,
Babel will transform it to JavaScript that these browsers understand, as shown in figure
6.11 on the right.
TIP If you see errors after entering the names of the browsers, uncheck the
Enabled check box after entering the browsers and versions. This seems like a
bug, but it may be fixed by the time you read this.
Now let’s change the preset to be “last 2 chrome versions,” as shown in figure 6.12.
Babel is smart enough to understand that the last two versions of Chrome support
arrow functions and there’s no need to do any transformation.
 The ENV preset comes with a list of browsers, and you need to use proper names
and phrases to specify the constraints (for example, last 2 major versions, Firefox
>= 20, or > 5% in US). These phrases are listed in the browserslist project available at
https://github.com/browserslist/browserslist.
NOTE We used the ENV preset in the Babel REPL to play with target environments, but these options can be configured and used in Babel from a command line. In listing 6.15, we’ll add @babel/preset-env to the .babelrc
configuration file. In listing 6.17, you’ll see the .browserslistrc file, where you
can configure specific browsers and versions as we did in the Babel REPL. You
can read more about preset-env in the Babel documentation at https://
babeljs.io/docs/en/next/babel-preset-env.html.
Figure 6.11 Applying the ie and safari presets
138 CHAPTER 6 Tooling
Babel can be used to compile such programming languages as JavaScript, TypeScript,
CoffeeScript, Flow, and more. For example, the React framework uses the JSX syntax,
which is not even standard JavaScript, and Babel understands it. In chapter 12, we’ll
use Babel with a React app.
 When Babel compiles TypeScript, it doesn’t perform type checking, unlike tsc.
Babel’s creators didn’t implement the full-featured Typescript compiler. Babel just
parses the TypeScript code and generates the corresponding JavaScript syntax.
 You may be thinking, “I’m pretty happy with the Typescript compiler. Why include
this section on a JavaScript-to-JavaScript compiler in the TypeScript book?” The reason is that you may join a project where some of the modules were written in JavaScript and some in TypeScript. In such projects, Babel may well already be part of the
development-deployment workflow. For example, Babel is popular among developers
who use the React framework, which only recently started supporting TypeScript.
 Like any npm package, you can install Babel either locally or globally (with the -g
option). Installing it locally within the project directory makes your project selfsufficient because, after running npm install, you can use Babel without expecting
that the computer has it installed elsewhere (someone may work with your project
using a different computer).
npm install @babel/core @babel/cli @babel/preset-env
Here, @babel/core is the Babel compiler, @babel/cli is the command-line interface,
and @babel/preset-env is the ENV preset discussed earlier.
We want to support the two
latest versions of Chrome.
Figure 6.12 Applying the Chrome preset
Using the Babel compiler 139
NOTE In the npmjs.org registry, JavaScript packages can be organized as organizations. For example, @babel is the organization for Babel-related packages.
@angular is the organization for packages that belong to the Angular framework. @types is the place for TypeScript type definition files for various popular JavaScript libraries.
In the following sections, we’ll introduce you to three small projects. The first uses
Babel with JavaScript, the second uses Babel with TypeScript, and the third uses Babel,
TypeScript, and Webpack.
6.4.1 Using Babel with JavaScript
In this section, we’ll review a simple project that uses Babel with JavaScript, located in
the babel-javascript directory. We’ll continue working with the three-line index.js
script introduced in listing 6.7 that uses the Chalk JavaScript library. The only change
in the following listing is that the message now reads “Compiled with Babel.”
const chalk = require('chalk');
const message = 'Compiled with Babel';
console.log(chalk.black.bgGreenBright(message));
The following listing shows the npm script that we’ll use to run Babel and the dependencies that should be installed on the developer’s machine.
"scripts": {
"babel": "babel src -d dist"
},
"dependencies": {
"chalk": "^2.4.1"
},
"devDependencies": {
"@babel/cli": "^7.2.3",
"@babel/core": "^7.2.2",
"@babel/preset-env": "^7.2.3"
}
Babel is configured in the .babelrc file, and our configuration file will be very simple.
We just want to use preset-env for compiling.
{
"presets": [
"@babel/preset-env"
]
}
Listing 6.15 index.js: the source code of the babel-javascript app
Listing 6.16 A fragment from babel-javascript/package.json
Listing 6.17 The file .babelrc
The npm script to compile
the code from src to dist
Locally installed
dev dependencies
140 CHAPTER 6 Tooling
We didn’t configure any specific browser versions here, and without any configuration
options @babel/preset-env behaves exactly the same as @babel/preset-es2015,
@babel/preset-es2016, and @babel/preset-es2017. In other words, all language features introduced in ECMAScript 2015, 2016, and 2017 will be compiled to ES5.
TIP We’ve configured Babel in the .babelrc file, which is fine for static configurations like ours. If your project needs to create Babel configurations programmatically, you’ll need to use the babel.config.js file (see the Babel
documentation for details: https://babeljs.io/docs/en/config-files#projectwide-configuration). If you’d like to see how Babel compiles our src/index.js
file, install the dependencies of this project by running npm install, and
then run the npm script from package.json: npm run babel.
The next listing shows the compiled version of index.js that’s created in the dist directory. It will have the following content (compare with listing 6.15).
"use strict";
var chalk = require('chalk');
var message = 'Compiled with Babel';
console.log(chalk.black.bgGreenBright(message));
NOTE The compiled file still invokes require('chalk') and this library is
located in a separate file. Keep in mind that Babel is not a bundler. We’ll use
Webpack with Babel in section 6.4.3.
You can run the compiled version as follows:
node dist/index.js
The console output will look similar to figure 6.13.
 If we wanted to ensure that Babel generates
code that works in specific browser versions,
we’d need to add an additional .browserslistrc
config file. For example, imagine that we wanted
our code to work only in the two latest versions
of Chrome and Firefox. We could create the following file in the root of our project.
last 2 chrome versions
last 2 firefox versions
Now running Babel won’t convert const to var as in listing 6.18 because both Firefox and
Chrome have supported the const keyword for awhile. Try it out and see for yourself.
Listing 6.18 dist/index.js: a compiled version of src/index.js
Listing 6.19 A sample .browserslistrc file
Babel added this line.
Babel replaced
const with var.
Figure 6.13 Running the program
compiled by Babel
Using the Babel compiler 141
6.4.2 Using Babel with TypeScript
In this section, we’ll review a simple project that uses Babel with TypeScript; it’s
located in the babel-typescript directory. We’ll continue working with the three-line
script introduced in listing 6.11 that uses the Chalk JavaScript library. The only
change is that the message now reads “Compiled with Babel.”
import chalk from 'chalk';
const message: string = 'Compiled with Babel';
console.log(chalk.black.bgGreenBright(message));
Compared to the package.json from the pure JavaScript project (see listing 6.16), our
TypeScript project adds the preset-typescript dev dependency that strips
TypeScript types from the code, so Babel can treat it as plain JavaScript. We’ll also add
an --extensions '.ts' option to the npm script that runs Babel as in listing 6.21.
Now Babel will read .ts files.
"scripts": {
"babel": "babel src -d dist --extensions '.ts'"
},
"dependencies": {
"chalk": "^2.4.1"
},
"devDependencies": {
"@babel/cli": "^7.2.3",
"@babel/core": "^7.2.2",
"@babel/preset-env": "^7.2.3",
"@babel/preset-typescript": "^7.1.0"
}
Typically presets include a number of plugins, but preset-typescript includes just
one, @babel/plugin-transform-typescript. That plugin internally uses @babel/
plugin-syntax-typescript to parse TypeScript and @babel/helper-plugin-utils
for general utilities for plugins.
 Although @babel/plugin-transform-typescript turns TypeScript code into
ES.Next syntax, it’s not a Typescript compiler. As strange as it sounds, Babel simply
erases TypeScript. For example, it’ll turn const x: number = 0 into const x = 0.
@babel/plugin-transform-typescript is a lot faster than the Typescript compiler
because it does not type-check the input files.
NOTE @babel/plugin-transform-typescript has several minor limitations
listed in the documentation at https://babeljs.io/docs/en/babel-plugintransform-typescript (for example, it doesn’t support const enum). For better
TypeScript support, consider using the @babel/plugin-proposal-classproperties and @babel/plugin-proposal-object-rest-spread plugins.
Listing 6.20 index.ts: the source code of the babel-typescript app
Listing 6.21 A fragment from package.json
Instructs Babel to process
files with .ts extensions
Adds the presettypescript dependency
142 CHAPTER 6 Tooling
You’ve read the first five chapters of this book, and started liking the type checking
and compile-time errors that the real Typescript compiler offers, and now are we
really suggesting you use Babel to erase the TypeScript-related syntax? Not really.
During development, you can continue using tsc (with tsconfig.json) and an IDE with
full TypeScript support. At the deployment stage, though, you may still introduce
Babel and its ENV preset. (You’ve already started to like the flexibility the ENV preset
offers in configuring target browsers, haven’t you?)
 In your build process, you can even add an npm script (in package.json) that runs
tsc:
"check_types": "tsc --noEmit src/index.ts"
Now you can sequentially run check_types and babel, assuming you have tsc installed
locally:
npm run check_types && npm run babel
The --noEmit option ensures that tsc won’t output any files (such as index.js) because
this will be done by the babel command that runs right after check_types. If there
are compile errors in index.ts, the build process will fail and the babel command
won’t even run.
TIP If you use && (double ampersand) between two npm scripts, they’ll run
sequentially. Use & (single ampersand) for parallel execution. See the sidebar
“Using ampersands in npm scripts on Windows” in chapter 10 for more details.
In this project, the .babelrc config file includes @babel/preset-typescript.
{
"presets": [
"@babel/preset-env",
"@babel/preset-typescript"
]
}
Compared to the babel-javascript project, we made the following TypeScript-related
changes:
 Added the --extensions '.ts' option to the command that runs Babel
 Added TypeScript-related dev dependencies to package.json
 Added @babel/preset-typescript to the .babelrc config file
To compile our simple index.ts script, run the following npm script from package
.json:
npm run babel
Listing 6.22 The file .babelrc
Using the Babel compiler 143
You’ll find the compiled version of index.js in the dist directory. You can run the compiled code the same way we did in the previous section:
node dist/index.js
Now let’s add Webpack to our workflow to bundle together the index.js script and the
Chalk JavaScript library.
6.4.3 Using Babel with TypeScript and Webpack
Babel is a compiler, but it’s not a bundler, which is required for any real-world app. There
are different bundlers to choose from (such as Webpack, Rollup, and Browserify), but
we’ll stick to Webpack. In this section, we’ll look at a simple project that uses Babel with
TypeScript and Webpack. It’s located in the webpack-babel-typescript directory.
 In section 6.3.2, we reviewed the TypeScript-Webpack setup, and we’ll continue
using our three-line source code from that project.
import chalk from 'chalk';
const message: string = 'Built with Babel bundled with Webpack';
console.log(chalk.black.bgGreenBright(message));
The devDependency section from package .json is shown in the following listing.
"devDependencies": {
"@babel/core": "^7.2.2",
"@babel/preset-env": "^7.2.3",
"@babel/preset-typescript": "^7.1.0",
"babel-loader": "^8.0.5",
"webpack": "^4.28.3",
"webpack-cli": "^3.1.2"
}
Compare the Babel dependencies in listings 6.24 and 6.21. There are three changes
in listing 6.24:
 We added babel-loader, which is a Webpack loader for Babel.
 We removed babel-cli because we won’t be running Babel from the command
line.
 Instead of babel-cli, Webpack will use babel-loader as a part of the bundling
process.
As you’ll remember from section 6.3, Webpack uses the webpack.config.js configuration file. While configuring TypeScript with Webpack, we used ts-loader (see listing
6.14). This time we want babel-loader to handle the files with the .ts extension. The
following listing shows the Babel-related section from webpack.config.js.
Listing 6.23 index.ts: the source code of the webpack-babel-typescript app
Listing 6.24 The devDependencies section in package.json
Adding the Webpack
Babel loader
144 CHAPTER 6 Tooling
module: {
rules: [
{
test: /\.ts$/,
exclude: /node_modules/,
use: 'babel-loader'
}
]
},
The .babelrc file will look exactly the same as in the previous section (see listing 6.20).
 After we’ve installed the dependencies with npm install, we’re ready to build the
bundle by running the bundleup command from package.json:
npm run bundleup
This command will build index.bundle.js in the dist directory. That file will contain
the compiled (by Babel) version of index.ts plus the code from the Chalk JavaScript
library. You can run this bundle as usual:
node dist/index.bundle.js
The output shown in figure 6.14 will
look familiar as well.
 As you can see, you don’t have to
select either Babel or tsc for generating
JavaScript. They can live happily together in the same project.
NOTE People who don’t like TypeScript often use this argument: “If I write in
plain JavaScript, I wouldn’t need to use a compiler. I can run my JavaScript
program as soon as it’s written.” This is plain wrong. Unless you’re ready to
ignore the newer JavaScript syntax introduced since 2015, you’ll need a process that can compile the code written in modern JavaScript to code that all
browsers can understand. Most likely, you’ll introduce a compiler in your
project anyway, whether it’s Babel, TypeScript, or something else.
6.5 Tools to watch
In this section, we’d like to mention a couple of tools that were not officially released
when we were writing, but that could become useful additions to the toolbox of a
TypeScript developer.
6.5.1 Introducing Deno
Every JavaScript developer knows about the Node.js runtime. We also use it in this
book for running apps outside browsers. Everybody likes Node.js . . . except for its
original creator, Ryan Dahl. In 2018 he delivered a presentation titled “10 Things I
Listing 6.25 A fragment from webpack-babel-typescript/webpack.config.js
Applies this rule for
files ending with .ts.
Processes .ts files
with babel-loader
Figure 6.14 Running the program compiled by Babel
Tools to watch 145
Regret About Node.js” (on YouTube at http://mng.bz/Yeyz) and he started working
on Deno, a secure runtime environment that’s built on top of the V8 engine (just like
Node) and that has a built-in Typescript compiler.
NOTE At the time of writing, Deno is still an experimental piece of software.
You can check its current status at https://deno.land.
Some of Ryan Dahl’s regrets were that Node apps need package.json, node_modules
.npm for module resolution, and a central repository for distributing packages. Deno
doesn’t need any of these. If your app needs a package, it should be able to get it
directly from the source code repository of that package. We’ll show you how this
works in a small project named deno, which comes with this chapter’s code.
 Deno can run both JavaScript and TypeScript code, and our project has just one
index.ts script as seen in the following listing.
import { bgGreen, black } from 'https://deno.land/std/colors/mod.ts';
const message: string = 'Ran with deno!';
console.log(black(bgGreen(message)));
Note that we’re importing a library named colors right from the source. There is no
package.json that would list this library as a dependency, and no npm install is
required to get the package from a central repository. It uses only ES6 modules, and
you just need to know its URL to import it into your app.
 You might ask, “Isn’t it dangerous using a direct link to a third-party library? What
if their code changes, breaking your app?” That’s not going to happen, because Deno
locally caches each third-party library when it’s loaded for the first time. Every subsequent run of your app will reuse the same version of each library unless you specify a
special --reload option.
NOTE We couldn’t use the Chalk library for this example, because it’s not
packaged for being consumed by Deno.
All you need for this script to run is the Deno executable, which can be downloaded
from https://github.com/denoland/deno/releases. Just pick the latest release and
get the zip file for the platform you use. For example, for macOS download and unzip
the deno_osx_x64.gz file.
 For simplicity, download the app in the deno directory. You can use the following
command to launch the app once you have Deno downloaded:
./deno_osx_x64 index.ts
Listing 6.26 index.ts: the source code of the app to run under Deno
Includes the colors library
Uses the TypeScript
type string
Uses the API from the colors library
146 CHAPTER 6 Tooling
TIP In macOS you may need to add a permission to execute this file: chmod
+x ./deno_osx_x64.
TIP If you run this on Windows, make sure you have at least version 6 of
PowerShell and Windows Management Framework. Otherwise you may see the
following error: “TS5009: Cannot find the common subdirectory path for the
input files.”
As you’ll see, Deno runs a TypeScript program out of the box, and it doesn’t require npm
or package.json. The first time you run this app, it’ll produce the following output:
Compiling file: ...chapter6/deno/index.ts
Downloading https://deno.land/std/colors/mod.ts...
Compiling https://deno.land/std/colors/mod.ts
Deno compiled index.ts and then downloaded and compiled the colors library. After that, it ran our app producing the output shown in figure 6.15.
 Deno cached the compiled colors library so it won’t
need to download and compile colors the next time you
run the app. As you can see, we didn’t need to configure project dependencies, and
there was nothing to install or configure prior to running the app.
 Deno doesn’t understand the format of npm packages, but if it gains traction,
maintainers of the popular JavaScript libraries will be packaging their products in a
format acceptable by Deno. Let’s keep an eye on this tool.
6.5.2 Introducing ncc
The second tool to watch is ncc (https://github.com/zeit/ncc). It’s a command-line
interface for compiling a Node.js module into a single file, together with all its dependencies. This tool can be used by TypeScript developers who write apps that run on
the server side.
 Just to give you some background on ncc, it’s a product of a company named Zeit,
which is a serverless cloud provider. You may have heard of their product Now
(https://zeit.co/now), which offers super-easy serverless deployment of web apps.
 Zeit also develops software that allows you to split any app into as many small
pieces as possible. For example, if you’re writing an app that uses the Express framework, they’ll want to represent each endpoint by a separate bundle that contains only
the code needed for the functionality of that endpoint.
 This allows them to avoid running live servers. If the client hits an endpoint, it
returns the serverless bundle in a miniature container, and the response time is only
100 milliseconds, which is pretty impressive. And ncc is the tool that can package a
server-side app into a small bundle.
Figure 6.15 Running an app
under Deno
Tools to watch 147
 ncc can take any JavaScript or TypeScript as an input and produce a bundle as output. It requires either minimal configuration or no configuration at all. The only
requirement is that your code should use ES6 modules or require().
 We’ll look at a small app, ncc-typescript, that has minimal configuration because we
use TypeScript. If we wrote this app in JavaScript, no configuration would be needed.
This app is located in the ncc-typescript directory, which contains the package.json file
in the following listing, tsconfig.json, and index.ts, which uses the Chalk library.
{
"name": "ncc-typescript",
"description": "A code sample for the TypeScript Quickly book",
"homepage": "https://www.manning.com/books/typescript-quickly",
"license": "MIT",
"scripts": {
"start": "ncc run src/index.ts",
"build": "ncc build src/index.ts -o dist -m"
},
"dependencies": {
"chalk": "^2.4.1"
},
"devDependencies": {
"@zeit/ncc": "^0.16.1"
}
}
You won’t see tsc as a dependency in this package.json file because the Typescript
compiler is an internal dependency of ncc. Still, you can list the compiler’s options in
tsconfig.json if needed. From the development perspective, ncc allows you to compile
and run your TypeScript code in one process.
 Note the scripts section, where we defined two commands: start and build.
This allows TypeScript developers to use two ncc modes:
 Run mode—ncc runs the TypeScript code without explicit compilation (it’ll
compile it internally).
 Build mode—The TypeScript is compiled into JavaScript.
The good part is that you don’t need to use a bundler like Webpack, because ncc will
build the bundle for you. Try it for yourself by doing npm install and running the
sample app located in index.ts:
npm run start
As specified in the package.json shown in listing 6.27, the start command will run
ncc that to compile and run index.ts, and the console output is shown in figure 6.16.
 Our start command compiled index.ts with the generation of source maps (the
default). In run mode, the compiled file was not generated. If you run the build
Listing 6.27 A fragment of ncc/package.json
Uses ncc in run mode
Compiles TypeScript
with ncc (-m is for
prod optimization)
The ncc tool
148 CHAPTER 6 Tooling
command, ncc will generate the index.js bundle in the dist directory, but the app
won’t run:
npm run build
The console output of the build command will look similar to this:
ncc: Using typescript@3.2.2 (ncc built-in)
24kB dist/index.js
24kB [1313ms] - ncc 0.16.1
The size of the optimized bundle is 24 KB (ncc uses Webpack internally). The nccgenerated bundle contains the code we wrote as well as the code of the Chalk library,
and you can run it app as usual:
node dist/index.js
The output shown in figure 6.17 is as expected.
 To summarize, ncc has a few benefits:
 There’s zero configuration for building
and running apps.
 You can use either the run or build modes.
Run mode spares you from explicitly compiling the TypeScript code.
 It supports hybrid projects where some code is written in JavaScript and some in
TypeScript.
In this section, we mentioned two interesting tools, Deno and ncc, but the TypeScript
ecosystem is evolving fast, and you should be watching for new tools that will make you
more productive, your apps more responsive, and build and deployment processes
more straightforward.
NOTE We didn’t mention yet another useful package called ts-node, which
can run both tsc and Node.js runtime as a single process. We’ll use it in section 10.4.2 in chapter 10 while starting a server written in TypeScript.
Figure 6.16 Running
the app with ncc
Figure 6.17 Running the app with ncc
Summary 149
Summary
 Source maps allow you to debug your TypeScript code even though the browser
runs the JavaScript version.
 Linters are used to check and enforce coding styles. We introduced TSLint too,
but it’s going to be merged with ESLint soon.
 To deploy a web app, we usually bundle up the source files to decrease the number of files the browser needs to download. Webpack is one of the most popular
bundlers.
 JavaScript developers use Babel to compile code that uses a newer ECMAScript
syntax into code that’s supported by specific versions of web browsers. In some
cases it make sense to use both TypeScript and Babel compilers.
 Knowing the syntax of any programming language is important, but understanding the process of how your program could be turned into a working runnable app is equally important.
150
Using TypeScript
 and JavaScript
 in the same project
In this chapter, we’ll show how you can benefit from TypeScript features such as
getting compilation errors and autocomplete even while using third-party libraries
written in JavaScript. We’ll start by explaining the role of type definition files, and
then we’ll discuss a concrete use case where an app written in TypeScript uses a
JavaScript library. Finally, we’ll discuss the things you should consider before implementing a gradual upgrade of your app from JavaScript to TypeScript.
This chapter covers
 Enjoying TypeScript’s benefits when working
with a JavaScript library
 The role of type definition files
 Upgrading an existing JavaScript app to
TypeScript
Type definition files 151
7.1 Type definition files
The JavaScript language was created in 1995, and gazillions of lines of code have been
written in this language since then. Developers from around the globe have released
thousands of libraries written in JavaScript, and the chances are that your TypeScript
app could benefit from using one of these libraries.
 It would be naïve to expect creators of JavaScript libraries to invest time rewriting
their libraries or frameworks in TypeScript, but we still want to be able to use JavaScript’s heritage in our TypeScript apps. Moreover, we’re spoiled by TypeScript conveniences like the static type analyzer, autocomplete, and immediate reports on
compilation errors. Can we continue enjoying these features while working with the
APIs of JavaScript libraries? Yes, we can, with the help of type definition files.
NOTE In section 6.3.2, you already saw a project where the TypeScript code
used a JavaScript library called Chalk. The goal of that example was to show
how to bundle TypeScript and JavaScript together, so we didn’t discuss how
the code interacts and if the TypeScript code analyzer was able to help with
the proper use of the Chalk library. We didn’t use type definition files in
chapter 6, but we will in this one.
7.1.1 Getting familiar with type definition files
The purpose of type definition files is to let the Typescript compiler know which types
are expected by the APIs of specific JavaScript libraries or runtimes. Type definition
files just include the names of the variables (with types) and function signatures (with
types) used by a particular JavaScript library.
 In 2012, Boris Yankov created a Github repository for type definition files (see
https://github.com/DefinitelyTyped/DefinitelyTyped). Other people started contributing, and currently more than 10,000 contributors work with this project. Then the
DefinitelyTyped.org site was created, and when TypeScript 2.0 was released, the
@types organization was created at npmjs.org, which became another repository for
type definition files. All declaration files from DefinitelyTyped.org are published automatically to the @types organization.
 Definition filenames have a d.ts suffix, and you can find these files for more than
7,000 JavaScript libraries at www.npmjs.com/~types. Just go there and search for the
JavaScript library you’re interested in. For example, you can find information on
jQuery type definitions at www.npmjs.com/package/@types/jquery—figure 7.1 shows
a screenshot of this web page.
 At the top right of figure 7.1, you can see the command that installs the type definition files for jQuery, but we like adding the -D option so that npm adds @types/
jquery to the devDependencies section of the project’s package.json file:
npm install @types/jquery -D
NOTE The preceding command doesn’t install the jQuery library; it just
installs the type definitions for jQuery members.
152 CHAPTER 7 Using TypeScript and JavaScript in the same project
In general, you install type definitions from npmjs.org, specifying the @types organization name followed by the name of the package. Once you’ve installed @types/
jquery, you can find several files with d.ts extensions, such as jQuery.d.ts and jQueryStatic.d.ts, located in the node_modules/@types/jquery directory of your project.
The Typescript compiler (and static analyzer) will use them to help you with autocomplete and type errors.
 In the middle of figure 7.1, you’ll see the URL for the sources of the jQuery type
definitions, and at the bottom are the names of global values offered by jQuery. For
example, you can use $ for accessing the jQuery API when it’s installed.
 You can create a new directory and turn it into an npm project by running the
command npm init -y there. This command creates the package.json file, and then
you can install the type definition for jQuery:
npm install @types/jquery -D
Command
to install
SourcesGlobal
values
Figure 7.1 jQuery type definitions at npmjs.org
Type definition files 153
Let’s see if, after installing type definition files, the static type analyzer and your IDE
will start helping you with jQuery API.
7.1.2 Type definition files and IDEs
To see how IDEs use type definition files, open the npm project created in the previous section in your IDE, create and open main.ts, enter $., and press Ctrl-Space. If
you use the WebStorm IDE, you’ll see the available jQuery APIs, as seen in figure 7.2.
VS Code will show the available jQuery APIs as well.
At the top of figure 7.2, you can see JQuery’s ajax() method with a strongly typed
argument, just like in any TypeScript program. Keep in mind that we didn’t even
install jQuery (which is written in JavaScript anyway); we just have the type definitions.
 This is great, but let’s open the same project in VS Code. You may not see any autocomplete, as in figure 7.3. The reason is that the WebStorm IDE automatically shows
all the definitions it can find in the project, whereas VS Code prefers us to explicitly
configure which d.ts files to use.
Figure 7.2 WebStorm’s autocomplete for jQuery
Figure 7.3 VS Code can’t find type definitions for jQuery.
154 CHAPTER 7 Using TypeScript and JavaScript in the same project
Let’s play by the VS Code rules and create a tsconfig.json file with the compiler’s
types option. In this array, you can specify which type definitions to use for autocomplete (using the names of the directories under node_modules/@types). The following listing shows the tsconfig.json file that we added to the project.
{
"compilerOptions": {
"types" : ["jquery"]
}
}
Although types: [jquery] works for this example, if you had to add type definition
files for several JavaScript libraries, you’d need to list all of them in the types compiler option (for example, types: [jquery, lodash]). But adding the compiler’s
types option is not the only way to help the compiler find type definitions—we’ll
show you the reference directive in section 7.1.4.
NOTE Your project may need to refer to external modules like jquery, lodash,
or others. The process the compiler goes through to figure out what an
import refers to is called module resolution. For details on this process, refer to
the TypeScript documentation at www.typescriptlang.org/docs/handbook/
module-resolution.html.
Now enter $. and press Ctrl-Space. The autocomplete should start working properly.
Click on the ajax() function, and VS Code will show its program documentation, as
seen in figure 7.4. No matter what IDE you use, having a d.ts file for JavaScript code
gives you intelligent help from the Typescript compiler and static analyzer.
Listing 7.1 The TypeScript config file, tsconfig.json
VS Code shows program
documentation for the
ajax() method
Figure 7.4 VS Code shows autocomplete and documentation for JQuery’s ajax() method.
Type definition files 155
NOTE In WebStorm, to see program documentation for an item selected in
the autocomplete list, select it and press Ctrl-J.
Let’s peek inside a type definition file. The arrow in figure 7.4 points at the interface
name where TypeScript found the type definitions for the ajax() function. Coincidentally, it’s defined in the file called JQueryStatic.d.ts, located in the node_modules/
@types/jquery directory. Hover your mouse pointer over the name of the function,
ajax(), and Ctrl-click. Both VS Code and WebStorm will open the following fragment
of ajax() type definitions.
/**
* Perform an asynchronous HTTP (Ajax) request.
* @param url A string containing the URL to which the request is sent.
* @param settings A set of key/value pairs that configure the Ajax request.
➥ All settings are optional.
* A default can be set for any option with $.ajaxSetup(). See jQuery.ajax(
➥ settings ) below
* for a complete list of all settings.
* @see \`{@link https://api.jquery.com/jQuery.ajax/ }\`
* @since 1.5
*/
ajax(url: string, settings?: JQuery.AjaxSettings): JQuery.jqXHR;
Type definition files can contain only type declarations. In the case of jQuery, its type
declarations are wrapped in an interface with multiple properties and method declarations, as you can see for ajax() in the preceding listing.
 In some d.ts files, you’ll see the use of the word declare:
declare const Sizzle: SizzleStatic;
export declare function findNodes(node: ts.Node): ts.Node[];
We’re not declaring const Sizzle or the function findNodes() here; we’re just stating that we’re going to use a JavaScript library that contains the declarations of const
Sizzle and findNodes(). In other words, this line tries to calm down tsc: “Don’t
scream if you see Sizzle or findNodes() in my TypeScript code. At runtime my app
will include the JavaScript library that has these types.” Such declarations are known
as ambient declarations—this is how you say to the compiler that the variable in
question will exist at runtime. If you don’t have type definitions for jQuery, you can
simply write declare var $: any in your TypeScript code and use the variable $ to
access the jQuery API. Just don’t forget to load jQuery along with your app.
 As you can see, type definition files allow us to kill two birds with one stone: use the
existing JavaScript libraries and enjoy the benefits of a strongly typed language.
Listing 7.2 A fragment from JQueryStatic.d.ts
Description of the ajax() function
Description of the
ajax() parameters
Signature of
ajax() with types
156 CHAPTER 7 Using TypeScript and JavaScript in the same project
NOTE Some JavaScript libraries include d.ts files, and there’s no need to install
them separately. A good example is the moment.js library, which is used for
validating, manipulating, and formatting dates. Visit its repository at https://
github.com/moment/moment, and you’ll see the moment.d.ts file there.
7.1.3 Shims and type definitions
A shim is a library that intercepts API calls and transforms the code so the old environment (such as IE 11) can support a newer API (such as ES6). For example, ES6 introduced the find() method for arrays, which finds the first element that meets the
provided criteria. In the following listing, the value of index will be 4 because it’s the
first value that’s greater than 3.
Using JavaScript libraries without type definition files
Although type definition files are the preferable way of using JavaScript libraries in a
TypeScript app, you can use these libraries even without having type definition files.
If you know the global variable of the selected JavaScript framework (such as $ in
jQuery), you can use it as-is. Modern JavaScript libraries may use module systems,
and instead of offering a global variable, they may require that a particular module
member be imported in your code. Refer to the product documentation for the library
of your choice.
Let’s take jQueryUI, which is a set of UI widgets and themes built on top of jQuery.
Let’s assume that the type definition file for jQueryUI doesn’t exist (even though it does).
JQueryUI’s Getting Started guide (http://learn.jquery.com/jquery-ui/getting-started)
states that to use this library in a web page, you need to install it locally and add the
following code to the HTML document.
<link rel="stylesheet" href="jquery-ui.min.css">
<script src="external/jquery/jquery.js"></script>
<script src="jquery-ui.min.js"></script>
After this is done, you can add jQueryUI widgets to the TypeScript code.
To get access to jQueryUI, you’d still use $ (the global variable from jQuery). For example, if you have an HTML <select id="customers"> dropdown, you can turn it into
a jQueryUI selectMenu() dropdown like this:
$("#customers").selectMenu();
The preceding code will work, but without the type definition file you won’t get any
help from TypeScript, and your IDE will highlight jQueryUI API as erroneous.
Of course, you can “fix” all the tsc errors with the following ambient type declaration:
declare const $: any;
It’s always better to use the type definition file, if it’s available.
Adds CSS
Adds jQuery
Adds jQueryUI
Type definition files 157
const data = [1, 2, 3, 4, 5];
const index = data.find(item => item > 3); // index = 4
If your code has to run in IE 11, which
doesn’t support the ES6 API, you’d add the
"target": ES5 compiler option in tsconfig
.json. As a result, your IDE would underline
the find() method with a squiggly line as
an error, because ES5 didn’t support it.
The IDE won’t even offer the find()
method in the autocomplete list, as shown
in figure 7.5.
 Can you still use the newer API and see
it in the autocomplete list? You can, if you
install the es6-shim.d.ts type definition file
and add it to the types compiler option in
tsconfig.json:
npm install @types/es6-shim -D
Add this shim to your tsconfig.json file ("types" : ["jquery", "es6-shim"]), and
your IDE won’t complain and will display the find() method in the autocomplete list,
as seen in figure 7.6.
NOTE There is a newer shim called core-js (www.npmjs.com/package/corejs), which can be used not only for the ES6 syntax, but for newer versions of
the ECMAScript specs as well.
Listing 7.3 Using the find() method on an array
Figure 7.6 es6-shim
helps with the ES6 API.
Figure 7.5 No find() method in ES5 arrays
158 CHAPTER 7 Using TypeScript and JavaScript in the same project
7.1.4 Creating your own type definition files
Let’s say some time ago you created a greeting() JavaScript function, which is
located in the hello.js file, as follows.
function greeting(name) {
console.log("hello " + name);
}
You want to continue using this excellent function (with autocomplete and type
checking) in your TypeScript project. In the src directory, create a typings.d.ts file with
the following content.
declare function greeting(name: string): void;
Finally, you need to let TypeScript know where this type definition file is located.
Because this greeting() function is not overly useful for the JavaScript community,
it’s not published at npmjs.org, and no one has created a d.ts file in the @types
organization either. In this case, you can use a special TypeScript reference directive
(a triple-slash directive), which has to be placed at the top of the .ts file that uses
greeting(). Figure 7.7 shows a screenshot taken while we were typing greeti in the
main.ts file in VS Code.
As you see, autocomplete prompts us with the argument and return types of the
greeting() JavaScript function. Figure 7.7 shows a triple-slash directive that uses the
path to the type definition file, but if you have the type definition file for some library
installed with npm, you can use types instead of path:
/// <reference types="some-library" />
NOTE If you want to write a type definition file for a JavaScript library, read
the TypeScript documentation at http://mng.bz/E1qq. You can read more
about triple-slash directives in the documentation at http://mng.bz/NeqE.
Listing 7.4 The JavaScript hello.js file
Listing 7.5 The ./src/typings.d.ts file
Figure 7.7 Getting autocomplete in main.ts
A sample TypeScript app that uses JavaScript libraries 159
7.2 A sample TypeScript app that uses JavaScript libraries
In this section, we’ll look at an app that’s written in TypeScript and that uses the
jQuery UI JavaScript library. This simple app will display three shapes: a rectangle, a
circle, and a triangle, as shown in figure 7.8.
If you read the printed version of this book, you’ll need to know that the rectangle is
blue, and both the circle and triangle are green. The user can enter a valid CSS selector, and the input field will render a dropdown list with the shape names that have the
provided selector.
 Figure 7.9 shows a screenshot taken after the user entered .green in the input
field and selected the triangle in the dropdown; the triangle was surrounded with a
red border.
In this app, we use jQuery to find the HTML elements that have one of the specified
selectors; the rendering of shapes is done by jQuery UI. This sample project is located
in the chapter7/jquery-ui-example directory, and it includes four files: package.json,
tsconfig.json, index.ts, and index.html. The content of the package.json file is shown
in the following listing.
{
"name": "jquery-ui-example",
"description": "Code sample for the TypeScript Quickly book",
Listing 7.6 package.json includes type definition files for jQuery and jQuery UI
Figure 7.8 Three shapes
rendered by jQuery UI
Figure 7.9 Finding an element
that has the CSS class .green
160 CHAPTER 7 Using TypeScript and JavaScript in the same project
"homepage": "https://www.manning.com/books/typescript-quickly",
"license": "MIT",
"devDependencies": {
"@types/jquery": "^3.3.29",
"@types/jqueryui": "^1.12.7",
"typescript": "^3.4.1"
}
}
As you can see, we didn’t add the jQuery and jQuery UI libraries to package.json
because we added the three lines shown in the following listing to the <head> section
of index.html.
<link rel="stylesheet"
➥ href="//code.jquery.com/ui/1.12.1/themes/base/jquery-ui.css">
<script src="//code.jquery.com/jquery-3.3.1.min.js"></script>
<script src="//code.jquery.com/ui/1.12.1/jquery-ui.min.js"></script>
You may ask, why didn’t we add a dependencies section to package.json as we do with
all other npm packages? The locally installed jQuery UI didn’t include the bundled
version of this library, and we didn’t want to complicate this app by adding Webpack
or another bundler. So we decided to find the URL of the content delivery network
(CDN) for these libraries.
 The jQuery home page (jQuery.com) includes a Download button that takes you
to the Download page (http://jquery.com/download), which includes the required
URLs of the CDN. If you need to include a JavaScript library in your project, you’ll
need to go through a similar discovery process.
 The <head> section of our index.html file also includes the styles shown in listing
7.8. In our TypeScript code we’ll use jQuery to get references to the HTML elements
with the IDs #shapes, #error, and #info.
NOTE In this demo app, we use jQuery selectors to find elements on the
page, but those selectors are already supported by the standard document
.querySelector() and document.querySelectorAll() methods. We use
jQuery just for the sake of showing how TypeScript code can work with JavaScript libraries.
The user will be able to enter in the input field any valid CSS style within the DOM
element with an ID of #shapes. They’ll see the results in the autocomplete list, as
shown earlier in figure 7.9.
Listing 7.7 Adding jQuery and jQueryUI to index.html
Type definitions for jQuery
Type definitions for jQueryUI
The Typescript compiler
Adds the jQuery UI styles
Adds the jQuery library
Adds the jQuery UI library
A sample TypeScript app that uses JavaScript libraries 161
<style>
#shapes {
display: flex;
margin-bottom: 16px;
}
#shapes > *:not(:last-child) {
margin-right: 32px;
}
@media (max-width: 640px) {
#shapes {
flex-direction: column;
align-items: center;
}
#shapes > *:not(:last-child) {
margin-bottom: 16px;
margin-right: 0;
}
}
#rectangle {
background-color: blue;
height: 100px;
width: 150px;
}
#circle {
background-color: green;
border-radius: 50%;
height: 100px;
width: 100px;
}
#triangle {
color: green;
height: 100px;
width: 150px;
}
</style>
The @media (max-width: 640px) media
query instructs the browser to change the layout on small devices (less than 640 pixels in
width). The flex-direction: column style
will render the shapes vertically, and alignitems: center; will center the shapes on the
page, as shown in figure 7.10.
Listing 7.8 A fragment from the <style> tag in index.html
Changes the page layout
for devices narrower than
640 pixels
The rectangle is blue.
The circle and
triangle are green.
Figure 7.10 Finding the DOM element that
has the CSS class .green
162 CHAPTER 7 Using TypeScript and JavaScript in the same project
 The <body> section of index.html has two containers implemented as <div> tags.
The first one, <div id="shapes">, has child <div> tags that represent the shapes. The
last container includes the input field for entering search criteria and two areas for displaying an error or info message (for example, “Found 2 element(s)” as in figure 7.9).
<body>
<div id="shapes">
<div id="rectangle"
class="blue"
hasAngles>
</div>
<div id="circle"
class="green"></div>
<div id="triangle"
class="green"
hasAngles>
<svg viewBox="0 0 150 100">
<polygon points="75, 0, 150, 100, 0, 100" fill="currentColor"/>
</svg>
</div>
</div>
<div class="ui-widget">
<label for="selector">Selector:</label>
<input id="selector" placeholder="Enter a valid CSS selector">
<span id="error"></span>
<span id="info"></span>
</div>
<script src="dist/index.js"></script>
</body>
Any of the valid selectors, such as div, .green, hasAngles, can be entered in the input
field. The only reason we added the hasAngles attribute to the rectangle and triangle
was to allow searching for these shapes by entering the [hasAngles] selector in the
input field.
 The main goal of this app is to illustrate the use of jQuery UI’s Autocomplete widget from the TypeScript code. It enables users to quickly find and select from a prepopulated list of values as they type, leveraging searching and filtering as you saw in
figure 7.9. If the user enters .green, the app displays the DOM elements that have this
CSS selector and will add them to the source list of values in the Autocomplete widget.
 The Autocomplete widget is described in the jQuery UI documentation at
http://api.jqueryui.com/autocomplete, and it requires an options object with a mandatory source property that defines the data to use.
Listing 7.9 The body section of index.html
The container with shapes
Any of these CSS attributes can be
used for finding a shape with our UI.
An error
message
will be
rendered
here. An info message will
be rendered here.
This script is a compiled
version of index.ts.
A sample TypeScript app that uses JavaScript libraries 163
$('#selector')
.autocomplete({
source: (request,
response) => {...}});
The code in listing 7.10 has no types, but since we have the jQuery UI type definition
file installed, VS Code leads us through the autocomplete API as shown in figure 7.11.
Note the digit 9 with the up and down arrows. jQuery UI offers many different ways of
invoking autocomplete(), and by clicking on the arrows, you can pick the API you like.
The prompt in figure 7.11 indicates that the type of the option object is JQueryUI
.AutocompleteOptions. You can always press the Ctrl-Space keys, and the IDE will
continue helping you. According to the widget’s documentation, we need to provide
the source of the autocomplete values, and VS Code lists the source option among
others, as shown in figure 7.12.
Listing 7.10 Using the Autocomplete widget
Our <input> field has id=“#selector” Appends the jQuery UI
Autocomplete widget
A function that gets the
data and returns a callback
A callback to invoke when a
value from the list is selected
Figure 7.11 The first prompt by VS Code
Figure 7.12 VS Code continues prompting
164 CHAPTER 7 Using TypeScript and JavaScript in the same project
Cmd-click on Mac (or Ctrl-click on Windows) on the autocomplete list, and it’ll open
the index.d.ts file with all the possible options. Cmd-click on JQueryUI.AutocompleteOptions and you’ll see its type definitions, as shown in figure 7.13.
 The IDE’s typeahead help may not be perfect; it’s only as good as the provided
type definition file. Take another look at the source property in figure 7.13. It’s
declared as any, and the comment states that it could be an array, string, or function.
This declaration could be improved by declaring a union type that would allow only
these types:
type arrayOrFunction = Array<any> | string | Function;
let source: arrayOrFunction = (request, response) => 123;
Introducing the arrayOrFunction type would eliminate the need for writing the // [],
string, or () comment. Of course, you’d need to replace the 123 with some code that
handles request and response.
NOTE As you can imagine, the library code and the API listed in its d.ts file
may go out of sync. We depend on the goodwill of the code maintainers to
keep the type definitions up to date.
Now let’s review the TypeScript code in our index.ts file, shown in listing 7.11. If you
were developing web apps 10 years ago, you’ll recognize the jQuery style of coding:
we start by getting references to the DOM elements on the browser’s page. For example, $('#shapes') means that we want to find a reference to the DOM element with
id="shapes".
This type declaration
can be improved.
Figure 7.13 The JQueryUI.AutocompleteOptions type definitions
A sample TypeScript app that uses JavaScript libraries 165
const shapesElement = $('#shapes');
const errorElement = $('#error');
const infoElement = $('#info');
$('#selector')
.autocomplete({
source: (request: { term: string },
response: ([]) => void) => {
try {
const elements = $(request.term, shapesElement);
const ids = elements.map((_index, dom) => ({ label: $(dom).attr('id'),
value: request.term })).toArray();
response(ids);
infoElement.text(`Found ${elements.length} element(s)`);
errorElement.text('');
} catch (e) {
response([]);
infoElement.text('');
errorElement.text('Invalid selector');
$('*', shapesElement).css({ border: 'none' });
}
},
focus: (_event, ui) => {
$('*', shapesElement).css({ border: 'none' });
$(`#${ui.item.label}`, shapesElement).css({ border: '5px solid red' });
}
});
$('#selector').on('input', (event: JQuery.TriggeredEvent<HTMLInputElement>)
➥ => {
if (!event.target.value) {
$('*', shapesElement).css({ border: 'none' });
errorElement.text('');
infoElement.text('');
}
});
We pass the Autocomplete widget an object with two properties: source and focus.
Our source property is a function that takes two parameters:
 request—An object with the search criterion, such as {term: '.green'}
 response—A callback that finds the IDs of the DOM elements that meet the
search criterion. In our case, we pass a callback function that contains one
try/catch block.
The focus property is also a function. It’s an event handler that will be invoked when
you move the mouse over one of the items in the rendered list. There we clear previously bordered shapes and add a border to the currently selected one.
Listing 7.11 index.ts: the source code of our app
Uses jQuery to find
references to DOM elements
The first parameter of the
function is the search criterion.
The second
parameter is
a callback to
modify the
DOM.
Finds the elements with
the shapes that meet the
search criterion
Finds the IDs of shapes
Invokes the callback, passing that meet the criterion
the autocomplete values
Handles the
event fired when
the focus moves
to one of the IDs
Removes the borders
from the shapes, if any
Adds a red border to the DOM
element with the selected ID
Resets all previous
selections and messages
166 CHAPTER 7 Using TypeScript and JavaScript in the same project
 Before running this app, we need to compile our TypeScript to JavaScript. The
Typescript compiler will use the following compiler options.
{
"compilerOptions": {
"outDir": "dist",
"target": "es2018"
}
}
In previous chapters, we created an npm script command in package.json to run the
locally installed version of the executable we wanted to run. For example, adding the
command "tsc": "tsc" to the scripts section in package.json would allow us to run
the locally installed compiler as follows:
npm run tsc
This time we were lazy and didn’t configure this command. Actually, we wanted to
illustrate the npx tool (which comes with npm). It runs the locally installed program if
it exists or installs the requested program temporarily and runs it. Here’s how you can
compile the index.ts file using the locally installed tsc with the npx tool:
npx tsc
After running this command, you’ll see the index.js file in the dist directory. This file
is used in index.html as follows:
<script src="dist/index.js"></script>
We’re almost there. The only missing player is a web server that can serve our app to
the browser. One of the simplest web servers you can install is live-server (www.npmjs
.com/package/live-server). Let’s install it:
npm i live-server -g
NOTE Instead of installing live-server manually, you can run it as npx liveserver. If live-server isn’t found in the project’s node_modules, npx will
download it from npmjs.com, cache it globally on your computer, and run
the live-server binary.
To run the server, enter the following command in the Terminal window in the root
directory of your project:
live-server
Listing 7.12 tsconfig.json: the compiler configuration
Where to place the
compiled JavaScript
Compiles into JavaScript
compatible with the ES2018 spec
Introducing TypeScript in your JavaScript project 167
Point your browser to localhost:8080, and you’ll see the app running. The best way to
understand how the code works is by running it through a debugger; figure 7.14 shows
the app running in Chrome. It paused in the running script, dist/index.js, at the breakpoint placed on line 22, which is invoked when the focus event is fired. In the Watch
panel on the right, we’ve added ui.item.label, and it has the value circle, matching
the selection in the UI.
NOTE In section 6.1, we explained that having source map files allows you to
debug the TypeScript code. Just add the line "sourceMap": true to the
tsconfig.json file, and you’ll be able to debug index.ts while running index.js.
Now that we’ve discussed using third-party JavaScript libraries with your TypeScript
code, let’s consider another scenario: you already have an app written in JavaScript,
and you’re considering switching to TypeScript.
7.3 Introducing TypeScript in your JavaScript project
In an ideal world, you always work with the latest languages and technologies, but in
reality, you generally have a mix of old and new. Suppose you’re an enterprise developer, and your team has been working on a JavaScript app for the last several years,
Debugger paused
the script at line 22.
Changing selection
results in focus change
Watching the value
of ui.item.label
Figure 7.14 Hitting a breakpoint in dist/index.js
168 CHAPTER 7 Using TypeScript and JavaScript in the same project
but after reading this book you have a strong desire to write code in TypeScript. You
can always come up with some pet project and develop it in TypeScript after hours,
but is it possible to bring TypeScript to your main JavaScript project at work?
 TypeScript supports optional typing, which means you don’t have to modify your
JavaScript code to declare types for each variable or function parameter, so why not
use the Typescript compiler on your JavaScript app? Well, the code base of your JavaScript app may have tens of thousands of lines of code, and using tsc to compile all of
them may reveal hidden bugs and slow down the deployment process. That may not
be the best way to start.
 Instead, select a part of your app that implements some isolated functionality (such
as a module for adding a new customer or for shipping) and run it through tsc as-is.
Most likely, your app already has a build process that uses tools such as Grunt, Gulp,
Babel, Webpack, and so on. Find the right place, and incorporate tsc into this process.
 You don’t even need to rename JavaScript files to give them .ts extensions. Just use
the "allowJs": true Typescript compiler option, which tells tsc, “Please compile not
only the .ts files but the .js files as well, and don’t perform type checking—just compile
it according to the setting in the compiler’s target option.”
NOTE If you don’t change the file extensions from .js to .ts, your IDE will still
highlight the types in the JavaScript files as erroneous, but tsc will compile the
files if you use the "allowJs": true option.
You may wonder, “Why ask tsc to skip type checking if types are optional anyway?” One
of the reasons is that tsc might not be able to fully infer all the type info from your
JavaScript code, and it may report errors. Another reason is that your existing code
can be buggy (even if the bugs aren’t showstoppers), and allowing tsc to work at its
fullest may reveal lots of compile errors that you don’t have time or resources to fix. If
it ain’t broke, don’t fix it, right?
 Of course, you may have a different approach: “If it ain’t broke, improve it.” If you’re
confident that your JavaScript code is written well, opt in to type checking by adding the
"checkJs": true tsc compiler option to tsconfig.json. Some of your JavaScript files may
still generate errors, and you can skip checking them by adding a //@ts-nocheck
comment to these files. Conversely, you can choose to check only a few .js files by adding
a //@ts-check comment to them without setting "checkJs": true. You can even
turn off type checking for a specific line of code by adding //@ts-ignore on the preceding line.
 To illustrate the effect of type-checking existing JavaScript code, we randomly
selected the OpenAjax.js file from the GitHub repository of the Dojo framework
(https://github.com/dojo/dojo/blob/master/OpenAjax.js). Let’s pretend we want
to start turning this code into TypeScript, and so we added the //@ts-check comment
to the top of this file. You’ll see some of the lines with squigglies in VS Code, as shown
in figure 7.15.
Introducing TypeScript in your JavaScript project 169
Let’s ignore the errors on the import statements at the top; we wouldn’t see them if all
these files were present. It looks like the error in line 8 is not an error either. It seems
that the OpenAjax object will be present at runtime. Adding //@ts-ignore above line
8 would remove that squiggly.
 But to fix the errors in lines 19–23 we’d need to declare a type or an interface
with all these properties. (Note that if you were changing this code to TypeScript, you
might want to rename the variable h with a more meaningful name.)
 Let’s consider another piece of JavaScript code, shown in figure 7.16. We want to
get the price of a product, and if it’s less than $20, we’ll buy it. The IDE doesn’t complain and the code seems legitimate.
Type check errors
Figure 7.15 Adding the //@ts-check comment to the top of a JavaScript file
Figure 7.16 Buggy JavaScript code
170 CHAPTER 7 Using TypeScript and JavaScript in the same project
Let’s add the //@ts-check comment at the top so the static type analyzer can check
this code for validity, as shown in figure 7.17.
 Oops! Our JavaScript code had a bug—we forgot to add parentheses after getPrice in the if statement (so we never invoked this function). If you were wondering
why this code never gave you the OK to buy the product, now you know the reason:
the expression getPrice < 20 was never evaluated to true! Simply adding the //@tscheck at the top helped us find a runtime bug in a JavaScript program.
 There’s another tsc option, noImplicitAny, that could help you with JavaScript-toTypeScript migration. If you’re not planning to specify the types of function parameters
and return types, tsc may have a hard time inferring the right types, and you may temporarily have to keep the compiler’s "noImplicitAny": false option (the default). In
this mode, if tsc cannot infer the variable type based on how it’s used, the compiler
silently defaults the type to any. That’s what is meant by implicit any. But ideally,
noImplicitAny should be set to true, so don’t forget to turn it back on when the migration to TypeScript is complete.
NOTE After turning on noImplicitAny, add another option, strictNullChecks, to catch all possible cases that may contain null or undefined where
other values are expected.
The Typescript compiler will go easy on your .js files. It’ll allow properties to be added
to a class or a function after their declaration. The same applies to object literals in .js
files: you can add properties to the object literal even if they were not defined originally.
TypeScript supports the CommonJS module format and will recognize the require()
function calls as module imports. All function parameters are optional by default, and
calls with fewer arguments than the declared number of parameters are allowed.
 You can also help tsc with type inference by adding the JSDoc annotations (such as
@param and @return) to your JavaScript code. Read more on the subject in the document “JSDoc support in JavaScript” on GitHub at http://mng.bz/DNqy.
NOTE The type-coverage CLI tool (www.npmjs.com/package/type-coverage)
allows you to count all the identifiers in your app that are declared with an
explicit type (except any) and report the type-coverage percentage.
Figure 7.17 @ts-check found a bug.
Summary 171
The process of upgrading your JavaScript project to TypeScript is not overly complicated, and we’ve given you a high-level overview of one approach to doing it gradually.
For more details, read the document “Migrating from JavaScript” in the TypeScript
documentation at http://mng.bz/lolj. There you can find specifics on integrating
with Gulp and Webpack, converting a React.js app to TypeScript, and more.
This concludes part 1 of this book, where we introduced the TypeScript language. We
didn’t cover each and every feature of the language, and we didn’t plan to. The book’s
title is TypeScript Quickly, isn’t it?
 If you understand all the material from part 1, you’ll easily be able to pass a TypeScript technical interview. But to become a really productive TypeScript developer, we
encourage you to study and run all the sample apps described in part 2.
Summary
 You can use thousands of existing JavaScript libraries in your TypeScript project.
 Type definition files allow you to enjoy the type-checking and autocomplete features in libraries that were written in JavaScript. These files make you more productive in writing code.
 You can create type definition files for any proprietary JavaScript code.
 Even if a JavaScript library doesn’t have a type definition file, you can still use it
in your TypeScript project.
 There are well-defined steps that allow you to gradually upgrade your existing
JavaScript code to TypeScript.
Once again: Why TypeScript?
TypeScript isn’t the first attempt at creating an alternative to JavaScript that can run
either in a browser or in a standalone JavaScript engine. TypeScript is only seven
years old, but it’s already in the top-10 programming languages in various ratings.
Why don’t top-10 language lists include older languages like CoffeeScript or Dart that
were supposed to become alternative ways of writing JavaScript?
In our opinion, there are three major forces that make TypeScript stand out:
 TypeScript strictly follows the ECMAScript standards. If a proposed feature
made it to the stage 3 of the TS39 process, it’ll be included in TypeScript today.
 TypeScript IDEs work with the same static type analyzer, offering you consistent help as you’re writing code.
 TypeScript easily interoperates with JavaScript code, which means that you
can use thousands of existing JavaScript libraries in your TypeScript apps
(and you learned how to do that in this chapter).

```

Part 2: Applying TypeScript in a blockchain app

In a blockchain, transactions are combined into blocks, which then are validated and linked into a chain.

If a record of new transactions has to be
added to a blockchain, an app creates a new block,
which is given to the blockchain nodes (computers) for validation using one of the algorithms (for
example, calculating a special hash code) used in blockchain systems. If the block is
valid, it’s added to the blockchain; otherwise it’s rejected.
 Where is the data about transactions stored, and what’s the meaning of the word
decentralized in this context? A typical blockchain is decentralized because no single
person or company controls or owns the data. Being decentralized also means that
there is no single point of failure.
 Imagine a server that has information about the available seats on some airlines.
Multiple travel agencies connect to the same server to browse and book the air tickets.
Block 2 Block 3Block 1
Figure 8.1 A blockchain of three
blocks
Blockchain 101 177
Some agencies (nodes) are small and have only one computer connected to the server. Some have two, three, or even
more computers in the same node, but they all depend on
the data from that single server. This is centralized data processing, as illustrated in figure 8.2. If the server is down, no one
can book air tickets.
 In the case of decentralized data processing, as in most
blockchains, there is no central data server. Full copies of the
blockchain are stored on the nodes of a peer-to-peer network,
which could include your computers if you decide to join a
blockchain. A single node doesn’t mean one computer—you
may be the owner of a computer cluster that represents one
node. Figure 8.3 illustrates a decentralized network. If any of
the nodes are down, the system remains operational as long as
at least one node is running.
NOTE An ability to pass the digital currency from peer to
peer without a central authority became crucial to the popularity of blockchain technology. Now blockchains are
being used in other types of applications, like logistics, realestate transactions, ticket reservation, voting, and so on.
Is it secure to store copies of transactions on multiple computers that belong to other people or organizations? What if one
of these computer owners (a bad guy) modifies my transaction, changing the paid amount to 0? The good news is that
this isn’t possible. After a block is added to the chain, its data
can’t be changed—the data in a blockchain is immutable.
 Think of a blockchain as storage where the data is “written in stone.” Once a piece
of data is added to the store, you can neither remove nor update it. A new block can
only be inserted after some node in the network solves a math problem. This may well
sound like magic—the best way to understand how a blockchain works is to build one,
and we’ll start doing so in next section.
 Basically, a blockchain is a decentralized immutable ledger represented by a collection of blocks. Each block can store any type of data, such as information about financial transactions, voting results, medical records, and so on. Each block is linked to the
previous one by storing the hash value of the previous block in the chain.
 In the next section, we’ll give you a mini primer on hashing.
8.1.1 Cryptographic hash functions
As defined on Wikipedia, a hash function “is a mathematical algorithm that maps data
of arbitrary size (often called the ‘message’) to a bit string of a fixed size (the ‘hash
value,’ ‘hash,’ or ‘message digest’) and is a one-way function, that is, a function which
Node 1
Node 2
Node 3
Server
Figure 8.2 Centralized
data processing
Node 1
Node 2
Node 3
Figure 8.3 Decentralized
data processing
178 CHAPTER 8 Developing your own blockchain app
is practically infeasible to invert” (https://en.wikipedia.org/wiki/Cryptographic_hash
_function). It also states that hash functions are suitable for use in cryptography.
 A cryptographic hash function allows one to easily verify that some input data
maps to a given hash value, but if the input data is unknown, it is deliberately difficult
to reconstruct it (or any equivalent alternatives) by knowing the stored hash value.
Encryption involves a bidirectional function that takes a value and applies a secret
key to return an encrypted value. Using the same key, the encrypted value can be
decrypted.
 In contrast, a hashing utility uses a unidirectional function. The process can’t be
reversed to reveal the original value. A hash function always produces the same hash
value if provided the same input value, and complex hashing algorithms are used to
minimize the probability that more than one input will provide the same hash value.
 Let’s consider a very basic and not overly secure hash function to understand the
unidirectional nature of hashes. Suppose an app has numbers-only passwords, and we
don’t want to store them in clear in the database. We want to write a hash function
that applies the modulo operator to the provided password and then adds 10 to it.
function hashMe(password: number): number{
const hash = 10 + (password % 2);
console.log(`Original password: ${password}, hashed value: ${hash}`);
return hash;
}
For any even number, the expression input % 2 will produce 0. Now let’s invoke the
hashMe() function several times, providing different even numbers as input parameters:
hashMe (2);
hashMe (4);
hashMe (6);
hashMe (800);
Each of these invocations will produce a hash value of 10, and the output will look like
this:
Original password: 2, hashed value: 10
Original password: 4, hashed value: 10
Original password: 6, hashed value: 10
Original password: 800, hashed value: 10
You can see this function in action on CodePen at http://mng.bz/BYqv.
 When a hashing function generates the same output for more than one input, it’s
called a collision. In cryptography, various Secure Hash Algorithms (SHA) offer more
Listing 8.1 A very simple hash function
Creates a modulo-based hash
Blockchain 101 179
or less secure ways for creating hashes, and they are created to be collision resistant,
making it extremely hard to find two inputs that will produce the same hash value.
 A block in a blockchain is represented by a hash value, and it’s very important that
cyber criminals can’t replace one block with another by preparing fraudulent content
that produces the same hash as the legitimate block. Our simple hash function in listing 8.1 has no resistance to collision attacks. Blockchains, in contrast, use collisionresistant hashing algorithms, such as SHA-256, which takes a string of any length and
produces a hash value of 256 bits or 64 hexadecimal characters. Even if you decided to
generate a SHA-256 hash for the entire text of this book, its length would be 64 hexadecimal numbers. There are 2256 possible combinations of bits in SHA-256 hashes,
which is more than the number of grains of sand in the world.
TIP To learn more about the SHA-256 algorithm, see the Wikipedia article at
https://en.wikipedia.org/wiki/SHA-2.
To calculate a SHA-256 hash on a Unix-based OS, you can use the shasum utility. On
Windows, you can use the program certUtil. There are multiple online SHA-256 generators as well. For creating hashes programmatically, you can use the crypto module
in Node.js apps or the crypto object in modern browsers.
 Here’s how you can calculate the SHA-256 hash for the text “hello world” on
macOS:
echo -n 'hello world' | shasum -a 256
This command produces the following hash:
b94d27b9934d3e08a52e52d7da7dabfac484efe37a5380ee9088f7ace2efcde9
No matter how many times you repeat the preceding command for the string “hello
world” you’ll always get the same hash value, but changing any character in the input
string will produce a completely different SHA-256 hash. Applying functionalprogramming terminology, we can say that a hash function is a pure function because it
always returns the same value for a given input.
TIP If you’re interested in hashing methodologies and algorithms, see Arash
Partow’s discussion of hashing algorithms (www.partow.net/programming/
hashfunctions) or read the Wikipedia article at https://en.wikipedia.org/
wiki/Hash_function.
We’ve stated already that blocks in a blockchain are linked using hashes, and in the
next section we’ll get familiar with the block internals.
8.1.2 What a block is made of
You can think of a block as a record in a ledger. Each block in a blockchain contains
app-specific data, and it also has a timestamp, its own hash value, and the hash value
180 CHAPTER 8 Developing your own blockchain app
of the previous block. In a very simple (and easy to hack) blockchain, an app could
perform the following actions to add a new block to the chain:
1 Find the hash of the most recently inserted block, and store it as a reference to
the previous block.
2 Generate a hash value for the newly created block.
3 Submit the new block to the blockchain for validation.
Let’s discuss these steps. At some point the blockchain was created, and the very first
block was inserted into this chain. The very first block in a chain is called a genesis
block. Obviously there are no blocks before this first one, so the previous block’s hash
doesn’t exist.
 Figure 8.4 shows a sample blockchain, where each block has a unique index, a
timestamp, its data, and two hashes—its own and the previous block’s. Note that the
genesis block has an empty string instead of the previous block’s hash.
In figure 8.4, we used empty quotes in the genesis block to show the lack of the previous block’s hash. Our data is represented by the text describing a transaction, such as
“Joe paid Mary $20.” In a real-world blockchain, Joe and Mary would be represented
by long and encrypted account numbers.
TIP You can think of a blockchain as a special type of a linked list, where
each node has a reference only to the previous one. It’s not a classical singlylinked list, where each node has a reference to the next one.
Now let’s imagine that there is a bad guy named Rampage, and he found out that the
hash of some block in our blockchain is “e68d27a…” Can he modify its data, stating
that he paid Mary $1,000, and then regenerate the other blocks so the hash values play
well? To prevent this from happening, a blockchain requires algorithms to be solved,
taking time and resources. That’s why blockchain members are required to spend time
and resources to mine a block, rather than quickly generating a hash value.
8.1.3 What’s block mining
Everyone understands what gold mining is—you perform some work to get gold,
which can then be exchanged for real money or goods. The more people who mine
gold, the more gold that exists in the world. In the past, the gold and other precious
commodities were the basis for the value of paper money.
Figure 8.4 A sample blockchain
Blockchain 101 181
 In the USA, the money supply is managed by the Federal Reserve, which consists of
a number of commercial banks and has a board of directors. The Federal Reserve has
the authority to manipulate paper currency, coins, funds in checking and savings
accounts, and other legally accepted forms of exchange.
 The money in cryptocurrencies (such as Bitcoin) is “produced” as the incentive for
people (miners) to solve the math problems required by a particular blockchain.
Since every new block has to be approved by other miners, more miners means a
more secure blockchain.
 In our distributed blockchain, we want to make sure that only the blocks that have
certain hashes can be added. For example, our blockchain may require each hash to
start with 0000. The hash is calculated based on the content of the block, and it won’t
start with four zeros unless someone finds an additional value to add to the block’s
content to produce such a hash. Finding such a value is called block mining.
 Before a new block is added to the blockchain, it is given to all nodes on the network for processing, and these nodes will start calculating the special value that produces a valid hash. The first one to find this value wins. Wins what? A blockchain may
offers rewards—in the Bitcoin blockchain, a successful data miner may earn Bitcoins.
 Let’s assume that our blockchain requires the hash of each block to start with 0000;
otherwise the block will be rejected. Suppose an app wants to add a new block (number 4) to our blockchain, and it has the following data:
4
June 4 15:30?
Simon refunded Al $200
Prior to adding any block, its hash must be calculated, so we’ll concatenate the preceding values into one string and generate the SHA-256 hash by running the following
command:
echo -n '4June 4 15:30?Simon refunded Al $200' | shasum -a 256
The generated hash will look like this:
d6f9255c5fc579594bef56403778d475ab441abbd56bff788d597ae1e8d4ad22
This hash doesn’t start with 0000, so it will be rejected by our blockchain. We need to
do some data mining to find a value that, if added to our block, will result in the generation of a hash that starts with 0000. Brute force to the rescue! We can write a program that will be adding sequential numbers (1, 2, 3, etc.) to the end of our input
string until the generated hash starts with 0000.
 After trying for a while, we found that secret value for our block—it’s 236499. Let’s
append this value to our input string and recalculate the hash:
echo -n '4June 4 15:30?Simon refunded Al $200236499' | shasum -a 256
182 CHAPTER 8 Developing your own blockchain app
Now the generated hash starts with four zeros:
0000696c2bde5add287a7b6ccf9a7e57c9d69dad8a6a93922b0451a5150e6696
Perfect! We know the number to include in the block’s content, so the generated hash
will conform to the blockchain requirements. This number can be used just once with
this particular input string. Changing any character in the input string will generate a
hash that won’t start with 0000.
 In cryptography, a number that can be used just once is called a nonce, and the
value 236499 is such a nonce for our specific block. How about this: we’ll add a property called nonce to the block object, and we’ll let the data miners calculate its value
for each new block.
 A miner has to spend some computational resources to calculate the nonce, which
is used as a proof of work, which is a must-have for any block to be considered for adding
to a blockchain. We’ll write a program to calculate nonces while developing our own
little blockchain in the next section. Meanwhile, let’s agree on the following structure
of a block.
interface Block {
index: number;
timestamp: number;
data: string;
nonce: number;
hash: string;
previousBlockHash: string;
}
Please note that we used the TypeScript interface to declare the Block custom type.
In the next section, we’ll decide if it should remain an interface or become a class.
Listing 8.2 A sample block type
A sequential block number The first parameter of the
function is the search criterion.
Data about one or more
app-specific transactions
A number to be figured
out by miners
This block’s hash The hash value of the previous
block in the blockchain
Bitcoin mining
Now that you’ve seen how the new block can be verified and added to a blockchain,
you can understand how Bitcoin data mining works. Say there’s a new transaction in
Bitcoins between Joe and Mary, and this transaction needs to be added to the Bitcoin
blockchain (the ledger). This transaction has to be placed into a block, and the block
has to be verified first.
Anyone who participates in the Bitcoin blockchain can become a data miner—a person (or a firm) who wants to use their hardware to be the first to solve the computationally difficult puzzle. This puzzle will require a lot more computational resources
than our puzzle with four zeros.
Blockchain 101 183
8.1.4 A mini project with hash and nonce
Modern browsers come with the crypto object to support cryptography. In particular,
you can use the crypto.subtle.digest() API to generate hashes (see the Mozilla
documentation for the method at http://mng.bz/dxKD).
 We’d like to give you a little assignment to work on. After the assignment, we’ll provide you with a solution, but without much explanation. Try to understand the code
on your own:
Over time the number of miners grows, the computational resources increase, and
the Bitcoin blockchain may increase the difficulty of the puzzle used for block mining.
This is done so that the time required to mine a block remains the same, perhaps
ten minutes, which may require finding hashes with 15–20 leading zeros.
The first person (say, the Bitcoin miner Peter) who solves the puzzle for a specific
transaction (such as “Joe pays Mary five Bitcoins”) will earn a newly released Bitcoin.
Peter may also earn money from the transaction fees associated with adding transactions to the Bitcoin blockchain. But what if Peter likes Mary, and he decides to do
her a “favor” by committing fraud, increasing the transaction amount from five to
fifty? That’s not possible, because Joe’s transaction will be digitally signed using public-private key cryptography.
Earlier in this section, we concatenated the block number, the time, and a single
transaction’s text to calculate a hash. Bitcoin’s blockchain stores blocks with multiple transactions (about 2,500) per block. There is no particular significance to the
size of the block or number of transactions. Increasing the number of leading zeros
required in a hash code makes it harder to solve the puzzle.
Bitcoins can be used to pay for services and can be bought and sold using conventional money or other crypto currencies. But Bitcoin mining is the only process that
results in releasing new Bitcoins into circulation.
Ledgers, and cooking the books
Every business has to keep track of its transactions. In the past, these transactions
would be manually recorded and categorized in a book: sales, purchases, and so on.
These days, such records are stored in files, but the concept of a ledger remains
unchanged. For our purposes, you can think of a blockchain as a representation of a
ledger.
Cooking the books is a phrase that refers to falsifying financial statements. But if a
ledger is implemented as a blockchain, cooking the books becomes nearly impossible. (More than 50% of the blockchain nodes would need to conspire to approve the
illegal modification of the block, which is more of a theoretical possibility than a practical one.) 
184 CHAPTER 8 Developing your own blockchain app
1 Write a generateHash(input: string) function that takes a string input and
finds its SHA-256 hash using the browser’s crypto API.
2 Write a calculateHashWithNonce(nonce: number) function that will concatenate the provided nonce with an input string and will invoke generateHash().
3 Write a mine() function that will invoke calculateHashWithNonce() in a loop
until the generated hash starts with 0000.
The mine() function should print the generated hash and the calculated nonce like this:
“Hash: 0000bfe6af4232f78b0c8eba37a6ba6c17b9b8671473b0b82305880be077edd9,
➥ nonce: 107105”
The following listing shows our solution to this assignment. We used the JavaScript
keywords async and await, which are explained in the appendix. Read the code—you
should be able to understand how it works.
import * as crypto from 'crypto';
let nonce = 0;
async function generateHash(input: string): Promise<string> {
const msgBuffer = new TextEncoder().encode(input);
const hashBuffer = await crypto.subtle.digest('SHA-256', msgBuffer);
const hashArray = Array.from(new Uint8Array(hashBuffer));
const hashHex = hashArray.map(b =>
➥ ('00' + b.toString(16)).slice(-2)).join('');
return hashHex;
}
async function calculateHashWithNonce(nonce: number): Promise<string> {
const data = 'Hello World' + nonce;
return generateHash(data);
}
async function mine(): Promise<void> {
let hash: string;
do {
hash = await this.calculateHashWithNonce(++nonce);
} while (hash.startsWith('0000') === false);
console.log(`Hash: ${hash}, nonce: ${nonce}`);
}
mine();
Listing 8.3 The solution to the hash and nonce project
Generates the SHA-256 hash from the provided input
Encodes as UTF-8
Hashes the
message Converts
ArrayBuffer
to Array
Converts bytes to a hex string
Adds the nonce to the string
and then calculates the hash
Comes up with the nonce that will result
in a hash that starts with four zeros
Uses await because
this function is
asynchronous
Developing your first blockchain 185
You can see this solution in action in CodePen at http://mng.bz/rP4g. Initially the console panel will be empty, but after several seconds of work it will print the following:
Hash: 0000bfe6af4232f78b0c8eba37a6ba6c17b9b8671473b0b82305880be077edd9,
➥ nonce: 107105
If you change the code of the mine() method, replacing four zeros with five, the calculation may take minutes. Try it with ten zeros, and the calculation may take hours.
 Calculating the nonce is time-consuming, but verification is fast. To check if the
program in listing 8.3 calculated the nonce (107105) correctly, we used the macOS
utility shasum as follows:
echo -n 'Hello World107105' | shasum -a 256
This utility printed the same hash as our program:
0000bfe6af4232f78b0c8eba37a6ba6c17b9b8671473b0b82305880be077edd9
In the mine() method in listing 8.3, we hardcoded the required number of zeros as
0000. To make this method more useful, we could add an argument to it:
mine(difficulty: number): Promise<void>
The value of difficulty could be used to represent the number of zeros at the beginning of the hash value. Increasing the difficulty would substantially increase the
time required to find the nonce.
 In the next section, we’ll start applying our TypeScript skills and build a simple
blockchain app.
8.2 Developing your first blockchain
Reading and understanding the previous section is a prerequisite for understanding
the content of this section, where we’ll create two blockchain apps: one without a
proof of work, and one with.
 The first app (without the proof of work) will create a blockchain and provide an
API for adding blocks to it. Prior to adding a block to the chain, we’ll calculate the
SHA-256 hash for the new block (no algorithm solving), and we’ll store a reference to
the hash of the previous block. We’ll also add an index, timestamp, and some data to
the block.
 The second app (with the proof of work) won’t accept blocks with arbitrary hashes,
but it will require mining to calculate nonces that produce hashes starting with 0000.
 Both programs will run from the command line using the node.js runtime, and
they’ll use the crypto module (https://nodejs.org/api/crypto.html) to generate SHA256 hashes.
186 CHAPTER 8 Developing your own blockchain app
8.2.1 The project’s structure
Each chapter in part 2 of this book is a separate project
with its own package.json file(s) (dependencies) and
tsconfig.json file(s) (Typescript compiler options).
You can find the source code for this chapter’s project
at https://github.com/yfain/getts.
 Figure 8.5 shows a screenshot of the VS Code
IDE after we opened the project for chapter 8, ran
npm install, and compiled the code with the tsc
compiler. This project has two apps, and their source
code is located in the src/bc101.ts and src/bc101
_proof_of_work.ts files.
 Running the tsc compiler will create the dist
directory and the JavaScript code. Running the npm
install command from the project’s root directory installs all the project dependencies in the node_modules directory. This project’s dependencies are listed in the following package.json file.
{
"name": "chapter8_blockchain",
"version": "1.0.0",
"license": "MIT",
"scripts": {
"tsc": "tsc"
},
"devDependencies": {
"@types/node": "^10.5.1",
"typescript": "~3.0.0"
}
}
The tsc compiler is one of the dependencies of this project, and we defined a custom
npm command to run it in the scripts section. npm scripts allow you to redefine
some npm commands or to define your own. You can define a command of any name
and ask npm to run it by entering npm run command-name.
 As per the content of the scripts section of our project, you can run the tsc compiler as follows:
npm run tsc
You may ask, why not just run the tsc command from the command line? You can do
that if the tsc compiler is installed globally on the computer where you run it, and this
may be the case if you run this command on your computer, where you have full control over the globally installed tools.
Listing 8.4 The project’s package.json file
The npm script command that runs
the locally installed tsc compiler
The type definition
file for Node.js
The Typescript compiler
Figure 8.5 The blockchain project
structure
Developing your first blockchain 187
 This may not be the case if your firm has a dedicated team responsible for building
and deploying projects on their computers. They may require you to provide the
application code as well as the build utilities in one package. When you run any program using the npm run command, npm will look for the specified program in the
node_modules/bin directory.
 In our project, after running npm install, the tsc compiler will be installed locally
in node_modules/bin, so our package includes the tooling required to build our app.
Our project also includes the following tsconfig.json configuration file with the TypeScript compiler options.
{
"compilerOptions": {
"module": "commonjs",
Listing 8.5 tsconfig.json: the configuration for the tsc compiler
A type definition file in action
We introduced type definition files in chapter 6. These files include type declarations
for public JavaScript APIs, and in this project we use the file describing the Node.js
API. Type definition files have *.d.ts extensions.
Type definition files allow the TypeScript type checker to warn you if you’re trying to
use the API incorrectly, such as if a function expects a numeric argument and you’re
trying to invoke it with a string. This type checking wouldn’t be possible if you were
just using the JavaScript library, which has no type annotations. If type definition files
for a JavaScript library or module are present, IDEs can offer context-sensitive help.
The following screenshot shows the VS Code editor after we entered the word
crypto., and the IDE offered context-sensitive help for the crypto module’s API,
which comes with Node.js.
If we hadn’t installed the @types/node type definition file, we wouldn’t get this help.
The npmjs.org repository has a special @type section (or “organization”) that stores
thousands of type definition files for popular JavaScript libraries.
Autocomplete for the crypto module
How to generate code
for JavaScript modules
188 CHAPTER 8 Developing your own blockchain app
"outDir": "./dist",
"target": "es2017",
"lib": [
"es2017"
]
}
}
While the outDir and target compiler options are self-explanatory, module and lib
require additional explanation.
 Prior to ES6, JavaScript developers were using different syntax for splitting code
into modules. For example, the AMD format was popular for browser-based apps, and
CommonJS was used by Node.js developers. ES6 introduced the import and export
keywords, so a script from one module could import whatever was exported from
another.
 In TypeScript, we always use ES6 modules, and if a script needs to load some code
from a module, we use the import keyword. For example, to use code from the
Node.js crypto module, we could add the following line to our script:
import * as crypto from 'crypto';
But the Node.js runtime implements the CommonJS spec for modules, which requires
you to write the JavaScript as follows:
const crypto = require("crypto");
By specifying the "module": "commonjs" compiler option in listing 8.5, we instruct tsc
to turn the import statement into require(), and all module members with the
export qualifier will be added to the module.exports={…} construct as prescribed by
the CommonJS spec.
 As for lib, TypeScript comes with a set of libraries that describe APIs provided by
browsers and JavaScript specs of different versions. You can selectively make these
libraries available to your program via the lib compiler option. For example, if you
want to use a Promise (introduced in ES2015) in your program, and run it in the target browsers that support promises, you can use the following compiler option:
{
"compilerOptions": {
"lib": [ "es2015" ]
}
}
The lib option includes only type definitions; it doesn’t provide the actual implementation of the API. Basically, you tell the compiler, “Don’t worry when you see Promise
in this code—the runtime JavaScript engine natively implements this API.” But you
The directory where compiled
JavaScript is stored
Compiles into the ES2017 syntax
This project will use the API
described in the library es2017.
Developing your first blockchain 189
have to either run your code in an environment that natively supports Promise or
include a polyfill library that provides a Promise implementation for older browsers.
TIP You can find a list of available libraries at https://github.com/Microsoft/
TypeScript/tree/master/lib.
Now that we’ve gone through the configuration files for our project, let’s review the
code in the two TypeScript files in the src directory, as seen earlier in figure 8.5.
8.2.2 Creating a primitive blockchain
The chapter8/src/bc101.ts script is the first version of our blockchain. It contains the
classes Block and Blockchain as well as a short script that uses the API of Blockchain
to create a blockchain of three blocks. In this chapter, we won’t use web browsers—
our scripts will run under the Node.js runtime. Let’s look at the code of bc101.ts, starting with the Block class.
 The Block class declares the properties required for each block (such as the index,
and hash values of the current and previous blocks) as well as the method to calculate
its hash using the Node.js crytpo module. During the instantiation of the Block
object, we calculate its hash based on the concatenated values of all of its properties.
import * as crypto from 'crypto';
class Block {
readonly hash: string;
constructor (
readonly index: number,
readonly previousHash: string,
readonly timestamp: number,
readonly data: string
) {
this.hash = this.calculateHash();
}
private calculateHash(): string {
const data = this.index + this.previousHash + this.timestamp + this.data;
return crypto
.createHash('sha256')
.update(data)
.digest('hex');
}
};
The constructor of the Block class invokes the calculateHash() method, which starts
by concatenating the values of the block’s properties: index, previousHash, timestamp,
Listing 8.6 The Block class from bc101.ts
The hash of this block
The sequential number
of this block
The hash of the
previous block
The time
of the
block’s
creation
The app-specific data
Calculates the hash of this
block on its creation
Creates an instance of the Hash object
for generating SHA-256 hashes
Computes and
updates the
hash value
inside the
Hash object Converts the hash value
into a hexadecimal string
190 CHAPTER 8 Developing your own blockchain app
and data. This concatenated string is given to the crypto module, which calculates its
hash in the form of a string of hexadecimal characters. This hash is assigned to the hash
property of the newly created Block object, which will be given to the Blockchain object
to be added to the chain.
 The block’s transaction data is stored in the data property, which in our Block
class has the string type. In a real-world application, the data property would have a
custom type that describes the structure of the data, but in our primitive blockchain,
using the string type is fine.
 Now let’s create a Blockchain class that uses an array to store blocks and has an
addBlock() method that does three things:
1 Creates an instance of the Block object
2 Gets the hash value of the most recently added block and stores it in the new
block’s previousHash property
3 Adds the new block to the array
When the Blockchain object is instantiated, its constructor will create the genesis
block, which won’t have a reference to a previous block.
class Blockchain {
private readonly chain: Block[] = [];
private get latestBlock(): Block {
return this.chain[this.chain.length - 1];
}
constructor() {
this.chain.push(
new Block(0, '0', Date.now(),
'Genesis block'));
}
addBlock(data: string): void {
const block = new Block(
this.latestBlock.index + 1,
this.latestBlock.hash,
Date.now(),
data
);
this.chain.push(block);
}
}
Now we can invoke the Blockchain.addBlock() method to mine blocks. The following code creates an instance of Blockchain and invokes addBlock() twice, adding two
blocks with the data “First block” and “Second block” respectively. The genesis block is
created in the constructor of Blockchain.
Listing 8.7 The Blockchain class
Our blockchain is stored here.
The getter to get a reference to
the most recently added block
Creates the genesis block
and adds it to the chain
Creates a new instance of Block
and populates its properties
Adds the block to the array
Developing your first blockchain 191
console.log('Creating the blockchain with the genesis block...');
const blockchain = new Blockchain();
console.log('Mining block #1...');
blockchain.addBlock('First block');
console.log('Mining block #2...');
blockchain.addBlock('Second block');
console.log(JSON.stringify(blockchain, null, 2));
The bc101.ts file includes the scripts shown in listings 8.6–8.8. To run this script, compile it and run its bc101.js JavaScript version under the Node.js runtime:
npm run tsc
node dist/bc101.js
The bc101.js script will print the content of our blockchain on the console, as shown
in the following listing. The chain array stores three blocks of our primitive blockchain.
Creating the blockchain with the genesis block...
Mining block #1...
Mining block #2...
{
"chain": [
{
"index": 0,
"previousHash": "0",
"timestamp": 1532207287077,
"data": "Genesis block",
"hash": "cc521dd5bbf1786977b14d16ce5d7f8da0e9f3353b3ebe0762ad9258c8ab1a
04"
},
{
"index": 1,
"previousHash": "cc521dd5bbf1786977b14d16ce5d7f8da0e9f3353b3ebe0762ad92
58c8ab1a04",
"timestamp": 1532207287077,
"data": "First block",
"hash": "52d40c33a8993632d51754c952fdb90d61b2c8bf13739433624bbf6b04933e
52"
},
{
"index": 2,
"previousHash": "52d40c33a8993632d51754c952fdb90d61b2c8bf13739433624bbf
6b04933e52",
Listing 8.8 Creating a 3-block blockchain
Listing 8.9 The console output produced by bc101.js
Creates a new blockchain
Adds the first block
Adds the second block
Prints the content
of the blockchain
192 CHAPTER 8 Developing your own blockchain app
"timestamp": 1532207287077,
"data": "Second block",
"hash": "0d6d43368772e2bee5da8a1cc92c0c7f28a098bfef3880b3cc8caa5f40c597
76"
}
]
}
NOTE When you run the bc101.js script, you won’t see the same hash values
as in listing 8.9 because we use the timestamp for hash generation. It will be
different for each reader.
Notice that the previousHash of each block (except the genesis block) has the same
value as the hash property of the previous block in the chain. This program runs very
quickly, but a real-world blockchain would require data miners to spend some CPU
cycles and solve an algorithm so that the generated hashes conform to certain requirements, as described in section 8.1.3.
 Let’s make the block-mining process a bit more realistic by introducing problem
solving.
8.2.3 Creating a blockchain with proof of work
The next version of our blockchain is located in the bc101_proof_of_work.ts file. This
script has a lot of similarities to bc101.ts, but it has some extra code to force data miners to provide proof of work, so that their blocks can be considered for addition to the
blockchain.
 The bc101_proof_of_work.ts script also has the Block and Blockchain classes, but
although the latter is exactly the same as listing 8.7, the Block class has additional
code.
 In particular, the Block class has a nonce property that is calculated in the new
mine() method. The nonce will be concatenated to other properties of the block to
produce a hash that starts with five zeros.
 The process of calculating a nonce that meets our requirements will take some time.
This time, we want hashes that start with five zeros. The mine() method will invoke
calculateHash() multiple times with different nonce values until the generated hash
starts with 00000. The new version of the Block class is shown in the following listing.
class Block {
readonly nonce: number;
readonly hash: string;
constructor (
readonly index: number,
readonly previousHash: string,
readonly timestamp: number,
readonly data: string
Listing 8.10 The Block class from bc101_proof_of_work.ts
The new nonce
property
Developing your first blockchain 193
) {
const { nonce, hash } = this.mine();
this.nonce = nonce;
this.hash = hash;
}
private calculateHash(nonce: number): string {
const data = this.index + this.previousHash + this.timestamp +
➥ this.data + nonce;
return crypto.createHash('sha256').update(data).digest('hex');
}
private mine(): { nonce: number, hash: string } {
let hash: string;
let nonce = 0;
do {
hash = this.calculateHash(++nonce);
} while (hash.startsWith('00000') === false);
return { nonce, hash };
}
};
Note that the calculateHash() method is almost identical to the method from listing
8.6. The only difference is that we append the value of the nonce to the input string
used for calculating the hash. The mine() method keeps calling calculateHash() in
a loop, providing the sequential numbers 0, 1, 2, … as the nonce argument. Sooner or
later, the calculated hash will start with 00000, and the mine() method will return the
hash as well as the calculated nonce.
 We’d like to bring your attention to the line that invokes the mine() method:
const { nonce, hash } = this.mine();
The curly braces on the left of the equal sign represent JavaScript destructuring (see
the appendix). The mine() method returns the object with two properties, and we
extract their values into two variables: nonce and hash.
 We’ve reviewed only the Block class from the bc101_proof_of_work.ts script
because the rest of the script is the same as in bc101.ts. You can run this program as
follows:
node dist/bc101_proof_of_work.js
This script won’t end as quickly as bc101.ts. It may take several seconds to complete, as
it spends time doing block mining. The output of this script is shown in the following
listing.
Calculates nonce
and hash
Nonce is part of the input
for calculating the hash
Uses brute force
for data mining
Runs this loop until the
hash starts with 00000
194 CHAPTER 8 Developing your own blockchain app
Creating the blockchain with the genesis block...
Mining block #1...
Mining block #2...
{
"chain": [
{
"index": 0,
"previousHash": "0",
"timestamp": 1532454493124,
"data": "Genesis block",
"nonce": 2832,
"hash": "000005921a5611d92cdc81f89d554743d7e33af2b35b4cb1a0a52cd4664445
ca"
},
{
"index": 1,
"previousHash": "000005921a5611d92cdc81f89d554743d7e33af2b35b4cb1a0a52c
d4664445ca",
"timestamp": 1532454493140,
"data": "First block",
"nonce": 462881,
"hash": "000009da95386579eee5e944b15eab2539bc4ac223398ccef8d40ed83502d4
31"
},
{
"index": 2,
"previousHash": "000009da95386579eee5e944b15eab2539bc4ac223398ccef8d40e
d83502d431",
"timestamp": 1532454494233,
"data": "Second block",
"nonce": 669687,
"hash": "0000017332a9321b546154f255c8295e4e805417e50b78609ff59a10bf9c23
7c"
}
]
}
Once again, the chain array stores the blockchain of three blocks, but this time each
block has a different value in its nonce property, and the hash value of each block
starts with 00000, which serves as proof of work: we did the block mining and solved
the algorithm for the block!
 Take another look at the code in listing 8.10 and identify the familiar TypeScript
syntax elements. Each of the six class properties has a type and is marked as readonly.
The properties nonce and hash were explicitly declared on this class, and four more
properties were created by the Typescript compiler because we used the readonly
qualifier with each argument of the constructor.
 Both class methods explicitly declare the types of their arguments and return values. Both methods were declared with the private access level, which means that they
can be invoked only within the class.
Listing 8.11 The console output produced by bc101_proof_of_work.ts
Summary 195
 The return type of the mine() method is declared as { nonce: number, hash: string }.
Because this type is used only once, we didn’t create a custom data type for it.
Summary
 The big idea of a blockchain is that it offers decentralized processing of transactions without relying on a single authority.
 In a blockchain, each block is identified by a hash value and is linked to the previous block by storing the hash value of the previous block.
 Before inserting a new block into the blockchain, a mathematical problem is
offered to each node of the blockchain interested in calculating an acceptable
hash value for a reward. When we use the term “node,” we mean a computer,
network, or farm that represents one member of a blockchain.
 In cryptography, a number that can be used just once is called a “nonce,” and a
miner has to spend some computational resources to calculate the nonce—this
is how they will get proof of work, which is a must-have for any block to be considered for addition to the blockchain.
 In our sample blockchain app, the acceptable hash value had to start with five
zeros, and we calculated the nonce to ensure that the hash of the block indeed
started with five zeros. Calculating such a nonce takes time, which delays the
insertion of the new block into the blockchain, but it can be used as the proof
of work needed to award the blockchain node that did it faster than others.
196
Developing
 a browser-based
 blockchain node
In chapter 8, we developed an app that would create a blockchain, and we provided
a script for adding blocks to it. We launched that app from the command line, and
it ran under the Node.js runtime.
 In this chapter, we’ll modify the blockchain app so that it runs in the browser.
We won’t use a web framework here, so it will have a pretty basic UI. We’ll use standard browser API methods, like document.getElementById() and addEventListener().
This chapter covers
 Creating a web client for a blockchain
 Creating a small library for hash generation
 Running the blockchain web app and debugging
TypeScript in the browser
Running the blockchain web app 197
 In this app, each block will store data about several transactions. They won’t be
simple strings as in chapter 8 but will be implemented as TypeScript custom types.
We’ll accumulate several transactions and then create the block to be inserted in the
blockchain. Also, we created a small library that contains the code for mining blocks
and generating the proper hash; this library can be used in a browser as well as in the
Node.js environment.
 This chapter’s app includes practical examples of the following TypeScript (and
JavaScript) syntax elements covered in part 1 of this book:
 Using private and readonly keywords
 Using TypeScript interfaces and classes for declaring custom types
 Cloning objects using the JavaScript spread operator
 Using the enum keyword
 Using async, await, and a Promise
We’ll start by looking at the project structure and running our blockchain web app.
After that, we’ll review the code in detail.
9.1 Running the blockchain web app
In this section, we’ll start by showing you how this blockchain project will be configured. Then you’ll see the commands to compile and deploy it, and finally you’ll see
how the user can work with this app in the browser.
9.1.1 The project structure
You can find the source code for this project at https://github.com/yfain/getts. Figure 9.1 shows how this project is structured.
Compiled code is here.
Project dependencies are installed here.
Sources of the
web client
Sources of reusable
hashing library
The source code of
a standalone client
The child Typescript compiler’s config file
The main Typescript compiler’s config file
Figure 9.1 Project structure
for the blockchain app
198 CHAPTER 9 Developing a browser-based blockchain node
The sources (TypeScript, HTML, and CSS) are located in the subdirectories called
browser, lib, and node. The main machinery for creating the blockchain is implemented in the lib directory, but we provided two demo apps—one for the web browser
and another for the Node.js runtime. Here’s what the subdirectories of src contain:
 lib—The lib directory implements blockchain creation and block mining. It
also has a universal function for generating hashes for both the browser and
Node.js environments.
 browser—The browser directory contains code that implements the UI of the
blockchain web app. This code uses the code in the lib directory.
 node—The node directory contains a small app that you can run independently;
it also uses the code from the lib directory.
This project will have some dependencies you haven’t seen yet in the package.json file
(listing 9.1). Since this is a web app, we’ll need a web server, and in this chapter, we’ll
use the package called “serve,” available on npmjs.org. From a deployment perspective, this app will have not only JavaScript files but also HTML and CSS files copied
into the dist deployment directory. The copyfiles package will do this job.
 Finally, to avoid manually invoking the copyfiles script, we’ll add a couple of commands to the npm scripts section of package.json. We started using npm scripts in section 8.2.1, but the scripts section in this chapter’s package.json file will have more
commands.
{
"name": "TypeScript_Quickly_chapter9",
"version": "1.0.0",
"license": "MIT",
"scripts": {
"start": "serve",
"compileDeploy": "tsc && npm run deploy",
"deploy": "copyfiles -f src/browser/*.html src/browser/*.css dist"
},
"devDependencies": {
"@types/node": "^10.5.1",
"serve": "^10.0.1",
"copyfiles": "^2.1.0",
"typescript": "~3.0.0"
}
}
After you run the npm install command, all the project dependencies will be installed
in the node_modules directory, and the serve and copyfiles executables will be installed
in node_modules/.bin. The sources of this project are located in the src directory, and
the deployed code will be saved in dist.
Listing 9.1 The package.json file
The npm start command
will start the web server.
Combines two commands:
tsc and deploy
The deploy command will
copy HTML and CSS files.
The serve
package is a new
dev dependency.
The copyfiles package is
a new dev dependency.
Running the blockchain web app 199
 Note that this project has two tsconfig.json files that are used by the Typescript
compiler. The base tsconfig.json is located in the project’s root directory, and it
defines compiler options for the whole project, such as the JavaScript target and
which libraries to use.
{
"compilerOptions": {
"sourceMap": true,
"outDir": "./dist",
"target": "es5",
"module": "es6",
"lib": [
"dom",
"es2018"
]
}
}
For the code that runs in the browser, we want the Typescript compiler to generate
JavaScript that uses modules (as explained in section A.11 of the appendix).
 Also, we want to generate source map files that map the lines in the TypeScript
code to the corresponding lines in the generated JavaScript. With source maps, you
can debug your TypeScript code while you run the web app in the browser, even
though the browser executes JavaScript. We’ll show you how to do this in section 9.5.
If the browser’s Developer Tools panel is open, it will load the source map file along
with the JavaScript file, and you can debug your TypeScript code there.
 The other tsconfig.json is in the src/node directory. This file inherits all the properties from the tsconfig.json file in the root of the project, as specified in the extends
option. tsc will load the base tsconfig.json file first, and then the inherited ones, overriding or adding properties.
{
"extends": "../../tsconfig.json",
"compilerOptions": {
"module": "commonjs"
}
}
In the base tsconfig.json file, the module property has a value of es6, which is fine for
generating JavaScript that runs in the browser. The child configuration file in the
node directory overrides the module property with the value commonjs, so the TypeScript compiler will generate module-related code as per the CommonJS rules.
Listing 9.2 The base tsconfig.json file
Listing 9.3 The src/node/tsconfig.json child config file
Generates source maps files
Compiled JavaScript files
go to the dist directory.
Uses the ES6 modules syntax
Uses type definitions for
the browser’s DOM API
Uses type definitions
supported by ES2018
Inherits properties from this file
200 CHAPTER 9 Developing a browser-based blockchain node
TIP You can find a description of all the options allowed in tsconfig.json in
the TypeScript documentation at www.typescriptlang.org/docs/handbook/
tsconfig-json.html.
9.1.2 Deploying the app using npm scripts
To deploy our web app, we need to compile the TypeScript files into the dist directory,
and copy index.html and styles.css there as well.
 The scripts section of package.json (see listing 9.1) has three commands: start,
compileDeploy, and deploy.
"scripts": {
"start": "serve",
"compileDeploy": "tsc && npm run deploy",
"deploy": "copyfiles -f src/browser/*.html src/browser/*.css dist"
},
The deploy command just copies the HTML and CSS files from the src/browser
directory to dist.
 The compileDeploy command runs two commands: tsc and deploy. In npm
scripts, the double ampersand (&&) is used to specify command sequences, so to compile the TypeScript files and copy index.html and styles.css to the dist directory, we
need to run the following command:
npm run compileDeploy
After we run this command, the dist directory will
contain the files shown in figure 9.2.
NOTE The source code of a real-world app contains hundreds of files, and prior to deploying
them in a web server, you’d use tools to optimize
and bundle the code as a smaller number of files.
One of the most popular bundlers is Webpack,
which we introduced in section 6.3.
Now you can start the web server by running the following command:
npm start
This command will start the web server on localhost
on port 5000. The console will show the output you
can see in figure 9.3.
Starts the web server
Runs the commands tsc and deploy
Copies the HTML and CSS files from
the src/browser directory to dist
Figure 9.2 The files for our web
app deployment
Running the blockchain web app 201
TIP npm supports a limited number of scripts, and start is one of them (see
the npm documentation at https://docs.npmjs.com/misc/scripts). With
these scripts, you don’t have to use the run option, and that’s why we didn’t
use the run command, as in npm run start. However, custom scripts like
compileDeploy require the run command.
NOTE In chapter 10, we’ll create an app that will have separate code for the
client and server; we’ll be starting the server using an npm package called
nodemon.
9.1.3 Working with the blockchain web app
Open your browser to http://localhost:5000/dist, and the web server will send
dist/index.html to the browser. The index.html file will load the compiled JavaScript
files, and after a couple of seconds spent on genesis block mining, the UI of the blockchain web app will look like figure 9.4.
Figure 9.3 Running the web server
1. Enter the transaction
 here.
2. Add the transaction
 to the pending ones.
3. Pending transactions
 are shown here.
4. The disabled Confirm
 Transactions button
5. The genesis block
Figure 9.4 Running the web server
202 CHAPTER 9 Developing a browser-based blockchain node
The landing page of this app shows the initial state of our blockchain with a single
genesis block. The user can add transactions and click on the Transfer Money button
after each transaction, which will add the transaction to the Pending Transactions
field. The Confirm Transactions button is enabled, and the browser’s window will look
like figure 9.5.
As you can see, we’ve added two pending transactions, and we’ve neither created nor
submitted a new block to our blockchain yet. This is what the Confirm Transactions
button is for. Figure 9.6 shows the browser window after we clicked this button and the
app spent some time performing data mining.
NOTE For simplicity, we’ve assumed that if John pays Mary a certain amount
of money, he has this amount. In real-world apps, the account balance is
checked first, and only afterwards is the pending transaction created.
As in chapter 8, the new block, #1, was created with a hash value that starts with four
zeros. But now the block includes two transactions: one between John and Mary, and
the other between Alex and Bill. The block has also been added to the blockchain. As
you see, no pending transactions are left.
Figure 9.5 Creating pending transactions
The web client 203
You may be wondering why seemingly unrelated transactions are placed in the same
block. That’s because adding a block to the blockchain is a slow operation, and creating a new block for each transaction would slow down the process even more.
 To add some context to our blockchain, let’s imagine that this blockchain has been
created for a large real estate agency. John is buying an apartment from Mary and
needs to provide proof that he paid for it. Similarly, Alex is paying Bill for a house,
and this is another transaction. While these transactions are in a pending state, they
are not considered proof of payment. But once they have been added to the block
and the block is added to the blockchain, it’s considered a done deal.
 Now that you’ve seen the app running, let’s review the code, starting with UI.
9.2 The web client
The browser directory has three files: index.html, main.ts, and styles.css. We’ll review
the code of the first two files, starting with index.html.
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
Listing 9.4 browser/index.html: the script that loads the web app
No pending transactions
are left.
The new block is
added to the chain.
Figure 9.6 Adding a new block to the blockchain
204 CHAPTER 9 Developing a browser-based blockchain node
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Blockchain</title>
<link rel="stylesheet" href="dist/styles.css">
<script type="module" src="dist/browser/main.js"></script>
</head>
<body>
<main>
<h1>Blockchain</h1>
<aside>
<p id="status">⏳ Initializing the blockchain, creating the genesis
➥ block...</p>
</aside>
<section>
<h2>Add transaction</h2>
<form>
<input id="sender" type="text" autocomplete="off" disabled
➥ placeholder="Sender">
<span>•</span>
<input id="recipient" type="text" autocomplete="off" disabled
➥ placeholder="Recipient">
<input id="amount" type="number" autocomplete="off" disabled
➥ placeholder="Amount">
<button id="transfer" type="button" class="ripple" disabled>
TRANSFER MONEY</button>
</form>
</section>
<section>
<h2>Pending transactions</h2>
<pre id="pendingtransactions">No pending transactions at the moment.</pre>
<button id="confirm" type="button" class="ripple" disabled>
CONFIRM TRANSACTIONS</button>
<div class="clear"></div>
</section>
<section>
<h2>Blockchain state</h2>
<div class="wrapper">
<div id="blocks"></div>
<div id="overlay"></div>
</div>
</section>
</main>
</body>
</html>
The <head> section of index.html includes the tags to load styles.css and main.js. The
latter is a compiled version of main.ts, which is not the only TypeScript file in our app,
but since we modularized our app, the main.ts script starts by importing members of
the lib/bc_transactions.ts JavaScript module, as shown in the following listing.
Includes CSS
Includes the
main module
of our app
This section is for
adding transactions.
Adds transaction to the
list of pending ones
Pending
transactions
are displayed
in this section.
Starts mining a new block
with pending transactions
The content
of the
blockchain
is rendered
here.
The web client 205
NOTE Note that we use the type="module" attribute in the <script> tag that
loads main.ts. You can learn more about the module type in section A.11 of
the appendix.
import { Blockchain, Block } from '../lib/bc_transactions.js';
enum Status {
Initialization = '⏳ Initializing the blockchain, creating the genesis
➥ block...',
AddTransaction = '✉ Add one or more transactions.',
ReadyToMine = '✅ Ready to mine a new block.',
MineInProgress = '⏳ Mining a new block...'
}
// Get HTML elements
const amountEl = document.getElementById('amount')
➥ as HTMLInputElement;
const blocksEl = document.getElementById('blocks') as
➥ HTMLDivElement;
const confirmBtn = document.getElementById('confirm')
➥ as HTMLButtonElement;
const pendingTransactionsEl =
➥ document.getElementById('pending-transactions') as HTMLPreElement;
const recipientEl = document.getElementById('recipient')
➥ as HTMLInputElement;
const senderEl = document.getElementById('sender')
➥ as HTMLInputElement;
const statusEl = document.getElementById('status')
➥ as HTMLParagraphElement;
const transferBtn = document.getElementById('transfer')
➥ as HTMLButtonElement;
In chapter 4, we explained enums, which are named constants. In listing 9.5, we use
enums to declare a finite number of statuses for our app: Initialization, AddTransaction, ReadyToMine, and MineInProgress. The statusEl constant represents the
HTML element where we want to display the current status of the app.
NOTE The little icons that you see in the enum strings are emoji symbols. You
can insert them into strings on macOS by pressing Cmd-Ctrl-Space and on Windows 10 by pressing Win-. (Win and a period) or Win-; (Win and a semicolon).
The rest of listing 9.5 shows code that gets references to various HTML elements that
either store the values entered by the user or display the blocks of our blockchain.
Each of these HTML elements has a unique id (see listing 9.4), and we use the
browser API’s getElementById() method to get a hold of these DOM objects.
Listing 9.5 The first part of browser/main.ts
Imports the Block and Blockchain classes
Declares possible statuses of the app
Gets references to all the important HTML elements
206 CHAPTER 9 Developing a browser-based blockchain node
 Listing 9.6 shows the immediately invoked function expression (IIFE) main(), and
we use the async/await keywords here (see section A.10.4 in the appendix). This
function creates a new blockchain with an initial genesis block, and it assigns event listeners to the buttons that add pending transactions and initiate new block mining.
(async function main(): Promise<void> {
transferBtn.addEventListener('click', addTransaction);
confirmBtn.addEventListener('click', mineBlock);
statusEl.textContent = Status.Initialization;
const blockchain = new Blockchain();
await blockchain.createGenesisBlock();
blocksEl.innerHTML = blockchain.chain.map((b, i) =>
➥ generateBlockHtml(b, i)).join('');
statusEl.textContent = Status.AddTransaction;
toggleState(true, false);
function addTransaction() {
blockchain.createTransaction({
sender: senderEl.value,
recipient: recipientEl.value,
amount: parseInt(amountEl.value),
});
toggleState(false, false);
pendingTransactionsEl.textContent =
➥ blockchain.pendingTransactions.map(t =>
`${t.sender} • ${t.recipient}: $${t.amount}`).join('\n');
statusEl.textContent = Status.ReadyToMine;
senderEl.value = '';
recipientEl.value = '';
amountEl.value = '0';
}
async function mineBlock() {
statusEl.textContent = Status.MineInProgress;
toggleState(true, true);
await blockchain.minePendingTransactions();
pendingTransactionsEl.textContent = 'No pending transactions at
➥ the moment.';
statusEl.textContent = Status.AddTransaction;
blocksEl.innerHTML = blockchain.chain.map((b, i) =>
➥ generateBlockHtml(b, i)).join('');
toggleState(true, false);
}
})();
Listing 9.6 The second part of browser/main.ts
Adds event listeners to buttons
Shows the initial
status using enum
Creates an instance
of Blockchain
Creates the genesis block
Generates HTML for
rendering block(s)
Adds a new pending
transaction
Renders
pending
transactions
as strings
Resets the form’s values
Mines the block and renders
it on the web page
Creates a new block,
calculates the hash,
and adds it to the
blockchain
Renders the newly inserted
block on the web page
The web client 207
In listing 9.6, we use the Blockchain class (discussed in section 9.3), which has an
array called chain that stores the content of our blockchain. The Blockchain class has
a minePendingTransactions() method that adds transactions to the new block.
 This mining process starts when the user clicks on the Confirm Transactions button. In a couple of places, we invoke blockchain.chain.map() to convert the blocks
into text or HTML elements for rendering on the web page. This workflow is shown in
figure 9.7.
Whenever we invoke an asynchronous function, we change the content of the HTML
element statusEl to keep the user informed about the current status of the web app.
 Listing 9.7 shows the remaining part of main.ts, which contains two functions:
toggleState() and generateBlockHtml().
function toggleState(confirmation: boolean, transferForm: boolean): void {
transferBtn.disabled = amountEl.disabled = senderEl.disabled =
➥ recipientEl.disabled = transferForm;
confirmBtn.disabled = confirmation;
}
function generateBlockHtml(block: Block, index: number) {
return `
<div class="block">
<span class="block__index">#${index}</span>
<span class="block__timestamp">${new
➥ Date(block.timestamp).toLocaleTimeString()}</span>
<div class="prev-hash">
<div class="hash-title">• PREV HASH</div>
<div class="hash-value">${block.previousHash}</div>
</div>
<div class="this-hash">
<div class="hash-title">THIS HASH</div>
<div class="hash-value">${block.hash}</div>
</div>
<div class="block__transactions">
Listing 9.7 The third part of browser/main.ts
Figure 9.7 The userblockchain workflow
Disables/enables the form
and the confirm button
Generates the
block’s HTML
208 CHAPTER 9 Developing a browser-based blockchain node
<div class="hash-title">TRANSACTIONS</div>
<pre class="transactionsvalue">${block.transactions.map(t => `${t.sender} •
➥ ${t.recipient} - $${t.amount}`)}</pre>
</div>
</div>
`;
}
The toggleState() function has two boolean parameters. Depending on the value of
the first parameter, we either enable or disable the form, which consists of the Sender,
Recipient, and Amount input fields and the Transfer Money button. The second
parameter enables or disables the Confirm Transactions button.
 The generateBlockHtml() function returns the <div> container with information
on each block, as seen at the bottom of figure 9.6. We use several CSS class selectors,
which are defined in the browser/styles.css file (not shown here).
 To summarize, the code in the browser/main.ts script is responsible for the user
interaction, and it performs the following:
1 Gets access to all HTML elements
2 Sets listeners for the buttons, which add pending transactions and create a new
block
3 Creates the instance of the blockchain with the genesis block
4 Defines the method that creates pending transactions
5 Defines the method that mines the new block with pending transactions
6 Defines a method that generates HTML for rendering the blocks of the blockchain
Now that you’ve seen how the browser’s UI is implemented, let’s review the code that
creates the blockchain, adds the blocks, and handles hashes.
9.3 Mining blocks
Our project contains the lib directory, which is a small library that supports creating
blocks with multiple transactions. This library also checks if our blockchain app runs
in the browser or in the standalone JavaScript engine, so the appropriate API for generating SHA-256 hashes is used.
 This library consists of two files:
 bc_transactions.ts—Implements the creation of blocks with transactions
 universal_sha256.ts—Checks the environment and exports the proper
sha256() function, which will use either the browser’s or Node’s API for generating SHA-256 hashes.
In chapter 8 (listing 8.6), we used Node’s crypto API to synchronously invoke three
functions:
crypto.createHash('sha256').update(data).digest('hex');
Mining blocks 209
Now we’d like to generate hashes in Node.js as well as in browsers. But the browser’s
crypto API is async, unlike the package we use with Node.js, so the hash-generation
code needs to be wrapped into asynchronous functions. For example, the mine()
function that invokes the crypto API returns a Promise and is marked as async:
async mine(): Promise<void> {...}
Basically, instead of simply returning the value from a function, we’ll wrap it in a
Promise, which will make the mine() function asynchronous.
 We’ll review the code of bc_transactions.ts in two parts. Listing 9.8 shows the code
that imports the SHA-256 generating function and declares the Transaction interface and the Block class. A block may contain more than one transaction, and the
Transaction interface declares the structure of one transaction.
import {sha256} from './universal_sha256.js';
export interface Transaction {
readonly sender: string;
readonly recipient: string;
readonly amount: number;
}
export class Block {
nonce: number = 0;
hash: string;
constructor (
readonly previousHash: string,
readonly timestamp: number,
readonly transactions: Transaction[]
) {}
async mine(): Promise<void> {
do {
this.hash = await this.calculateHash(++this.nonce);
} while (this.hash.startsWith('0000') === false);
}
private async calculateHash(nonce: number): Promise<string> {
const data = this.previousHash + this.timestamp +
➥ JSON.stringify(this.transactions) + nonce;
return sha256(data);
}
}
The UI of our blockchain allows the user to create several transactions using the
Transfer Money button (see figure 9.5), and only clicking the Confirm Transactions
button creates the Block instance, passing the Transaction array to its constructor.
Listing 9.8 The first part of lib/bc_transactions.ts
Imports the function
for hash generation
A custom type representing
a single transaction
A custom type representing
a single block
Passes an array of transactions
to the newly-created block
The asynchronous function
to mine the block
Uses brute force to find
the proper nonce
The asynchronous wrapper
function for hash generation
Invokes the function that uses the
crypto API and generates the hash
210 CHAPTER 9 Developing a browser-based blockchain node
 In chapter 2, we showed you how to declare TypeScript custom types using classes
and interfaces. In listing 9.8, you can see both: the Transaction type is declared as an
interface but the Block type as a class. We couldn’t make Block an interface, because
we wanted it to have a constructor so we could use the new operator, as you’ll see in
listing 9.9. The Transaction type is a TypeScript interface, which will prevent us from
making type errors during development, but the lines that declare Transaction won’t
make it into the compiled JavaScript.
 Our Transaction type is rather simple; in a real-world blockchain we could introduce more properties in the Transaction interface. For example, buying real estate is
a multistep process that requires several payments over time (an initial deposit, a payment for the title search, a payment for property insurance, and so on). In a realworld blockchain, we could introduce a propertyID to identify the property (house,
land, or apartment) and a type to identify the transaction type.
 Every transaction will include properties that describe the business domain of a
particular blockchain—you’ll always have a transaction type. A block will also have
properties required by the blockchain implementation, such as creation date, hash
value, and previous block’s hash value. Also, a block type may include some utility
methods, such as mine() and calculateHash() in listing 9.8.
NOTE We already had a Block class in chapter 8 (see listing 8.6), which
stored its data as a string in the data property. This time the data is stored in
a more structured way in a property of type Transaction[]. You may have
also noticed that three of the properties of the Block class were declared
implicitly via the constructor’s arguments.
Our Block class has two methods: mine() and calculateHash(). The mine() method
keeps increasing the nonce and invoking calculateHash() until it returns a hash
value that starts with four zeros.
 As specified in its signature, the mine() function returns a Promise, but where’s the
return statement of this function? We don’t really want this function to return
anything—the loop should simply end when the proper hash is generated. But any
function marked with the async keyword must return a Promise, which is a generic type
(explained in chapter 4) and must be used with a type parameter. By using
Promise<void>, we specify that this function returns a Promise with an empty value, so
a return statement is not required.
 Since the method calculateHash() shouldn’t be used by scripts external to Block,
we declared it as private. This function invokes sha256(), which takes a string and generates a hash. Note that we use JSON.stringify() to turn an array of type Transaction
into a string. The sha256() function is implemented in the universal_sha256.ts script,
and we’ll discuss it in section 9.4.
NOTE For simplicity, our calculateHash() function concatenates multiple
transactions into a string and then calculates the hash. In real-world blockchains, a more efficient algorithm called Merkle Tree (https://en.wikipedia
Mining blocks 211
.org/wiki/Merkle_tree) is used for calculating the hashes of multiple transactions. Using this algorithm, a program can build a tree of hashes (one per two
transactions), and if someone tries to tamper with one transaction, there’s no
need to traverse all the transactions to recalculate and verify the final hash.
The first line in listing 9.8 imports from the ./universal_sha256.js JavaScript file, even
though the lib directory only has the TypeScript version of this file. TypeScript doesn’t
allow us to use .ts extensions to reference imported filenames. This ensures that references to external scripts won’t change after compilation, where all files have .js extensions. This import statement looks like we’re importing a single function, sha256(),
but under the hood it’ll import different functions that use different crypto APIs,
depending on the environment the app runs in. We’ll show you how it’s done in section 9.4 where we’ll review the code of universal_sha256.ts.
 In the second part of the bc_transactions.ts file, we declare the Blockchain class.
export class Blockchain {
private readonly _chain: Block[] = [];
private _pendingTransactions: Transaction[] = [];
private get latestBlock(): Block {
return this._chain[this._chain.length - 1];
}
get chain(): Block[] {
return [ ...this._chain ];
}
get pendingTransactions(): Transaction[] {
return [ ...this._pendingTransactions ];
}
async createGenesisBlock(): Promise<void> {
const genesisBlock = new Block('0', Date.now(), []);
await genesisBlock.mine();
this._chain.push(genesisBlock);
}
createTransaction(transaction: Transaction): void {
this._pendingTransactions.push(transaction);
}
async minePendingTransactions(): Promise<void> {
const block = new Block(this.latestBlock.hash,
Date.now(), this._pendingTransactions);
await block.mine();
this._chain.push(block);
this._pendingTransactions = [];
}
}
Listing 9.9 The second part of lib/bc_transactions.ts
The getter for the latest
block in the blockchain
The getter for all blocks
in the blockchain
The getter for all
pending transactions
Creates the genesis block
Creates the hash for the genesis block
Adds the genesis block to the chain
Adds a pending transaction
Creates a block with
pending transactions and
adds it to the blockchain
Creates the hash for the new block
Adds the new block to the blockchain
212 CHAPTER 9 Developing a browser-based blockchain node
Adding a new block to the blockchain intentionally takes time to prevent double-spending
attacks (see the “Double-spending attacks” sidebar). For example, Bitcoin keeps this
time at around 10 minutes by controlling the complexity of the algorithm that needs to
be solved by the blockchain nodes. Creating a new block for each transaction would
make the blockchain extremely slow, which is why a block can contain multiple
transactions. We accumulate pending transactions in the pendingTransactions
property and then create a new block that stores all of them. For example, one Bitcoin
block contains about 2,500 transactions.
When a user adds transactions using the UI shown in figure 9.5, we invoke the createTransaction() method for each transaction, and it adds one transaction to the pendingTransactions array (see listing 9.9). At the end of the minePendingTransactions()
method, when the new block is added to the blockchain, we remove all pending transactions from this array.
 Note that we declared the _pendingTransactions class variable as private, so it
can only be modified via the createTransaction() method. We also provided a public getter that returns an array of pending transactions. It looks like this:
get pendingTransactions(): Transaction[] {
return [ ...this._pendingTransactions ];
}
This method has just one line that creates a clone of the _pendingTransactions array
(using the JavaScript spread operator explained in section A.7 of the appendix). By
creating a clone, we make a copy of the transactions’ data. Also, each property of the
Transaction interface is readonly, so any attempt to modify the data in this array will
result in a TypeScript error. This getter is used in the browser/main.ts file, which
Double-spending attacks
Let’s say you have only two $1 bills in your pocket, and you want to buy a cup of coffee that cost $2. You hand two $1 bills to the barista, he gives you the coffee, and
you have no money in your pocket, which means that you can’t buy anything else,
unless you steal or counterfeit more. Counterfeiting money takes time and can’t be
done on the spot in the coffee shop.
The digital currency could be subject to counterfeiting. For example, a dishonest person might try to pay a given amount of money to multiple recipients. Suppose Joe has
only one Bitcoin and he pays it to Mary (creating a block with the transaction “Joe
Mary 1”), and then he immediately pays one Bitcoin to Alex (creating another block
with the transaction “Joe Alex 1”). This is an example of a double-spending attack.
Bitcoin and other blockchains implement mechanisms for preventing such an attack,
and they have a consensus process for validating each block and resolving conflicts.
In section 10.1, we’ll explain the longest chain rule, which can be used to prevent
the insertion of invalid blocks.
Using crypto APIs for hash generation 213
displays pending transactions on the UI as seen in listing 9.6. The same cloning
technique is used with the chain() getter, which returns a clone of the blockchain.
 The genesis block is created by invoking the createGenesisBlock() method (in
listing 9.9), and the browser/main.ts script does this (see listing 9.6). The genesis
block has an empty transactions array, and invoking mine() on this block calculates its
hash. Mining the block may take some time, and we don’t want the UI to freeze, so
this method is asynchronous. Also, we added await to the invocation of the mine()
method to ensure that the block is added to the blockchain only after the mining
is complete.
 Our blockchain app was created for educational purposes, so every time the user
launches our app, the blockchain will contain only the genesis block. The user starts
creating pending transactions, and at some point they’ll click on the Confirm Transactions button. This will invoke the minePendingTransactions() method, which creates
a new Block instance, calculates its hash, adds the block to the blockchain, and resets
the _pendingTransactions array.
TIP After mining a new block, the miner should be rewarded. We’ll take care
of this in the blockchain app in chapter 10.
You may want to take another look at the code of the mineBlock() method in listing
9.6 to get a better understanding of how the results of block mining are rendered on
the UI.
 Whether you’re mining the genesis block or a block with transactions, the process
invokes the mine() method in the Block class (see listing 9.8). The mine() method
runs a loop invoking calculateHash(), which in turn invokes sha256(), discussed
next.
9.4 Using crypto APIs for hash generation
Because we wanted to create a blockchain that could be used by both web and standalone apps, we needed to use two different crypto APIs to generate SHA-256 hashes:
 For web apps, we can invoke the API of the crypto object supported by all
browsers.
 Standalone apps will run under the Node.js runtime, which comes with the
crypto module (https://nodejs.org/api/crypto.html). We’ll use it for generating hashes just like we did in listing 8.6.
We want our little library to make the decision about which API to use at runtime, so
the client apps will use one function without knowing which specific crypto API will be
used. The lib/universal_sha256.ts file in the following listing declares three functions:
sha256_node(), sha256_browser(), and sha256(). Note that only the last one is
exported.
214 CHAPTER 9 Developing a browser-based blockchain node
function sha256_node(data: string): Promise<string> {
const crypto = require('crypto');
return Promise.resolve(crypto.createHash('sha256').update(data)
➥ .digest('hex'));
}
async function sha256_browser(data: string): Promise<string> {
const msgUint8Array = new TextEncoder().encode(data);
const hashByteArray = await crypto.subtle.digest('SHA-256',
msgUint8Array);
const hashArray = Array.from(new Uint8Array(hashByteArray));
const hashHex = hashArray.map(b => ('00' +
➥ b.toString(16)).slice(-2)).join('');
return hashHex;
}
export const sha256 = typeof window === "undefined" ?
sha256_node :
sha256_browser;
When a JavaScript file contains import or export statements, it becomes an ES6 module (see the appendix for a discussion of this). The universal_sha256.ts module
declares the functions sha256_node() and sha256_browser(), but it doesn’t export
them. These functions become private and can be used only inside the module.
 The sha256() function is the only one that is exported and can be imported by
other scripts. This function has a very simple mission—to find out if the module is
running in the browser or not. Depending on the result, we want to export either
sha256_browser() or sha256_node(), but under the name sha256().
 We came up with a simple solution: if the runtime environment has a global window
variable, we assume that this code is running in the browser, and we export sha256
_browser() under the name sha256(). Otherwise we export sha256_node() under the
same name of sha256(). In other words, this is an example of a dynamic export.
 We already used the Node.js crypto API in chapter 8, but here we’ve wrapped this
code in a Promise:
Promise.resolve(crypto.createHash('sha256').update(data).digest('hex'));
We did this to align the signatures of the functions sha256_node() and sha256
_browser().
Listing 9.10 lib/universal_sha256.ts: a wrapper for crypto APIs
The function to be used in the Node.js runtime
Generates a
SHA-256 hash
The function to be used in browsers
Encodes the
provided string
as UTF-8
Hashes the data
Converts the
ArrayBuffer to Array
Converts bytes to a
hexadecimal string
Checks if the runtime
has a global variable
window Exports the hashing
function for Node Exports the hashing
function for browsers
Using crypto APIs for hash generation 215
 The sha256_browser() function runs in the browser and uses the asynchronous
crypto API, which makes this function asynchronous as well. As per the browser’s crypto
API requirements, we start by using the Web Encoding API’s TextEncoder.encode()
method (see the Mozilla documentation at http://mng.bz/1wd1), which encodes the
string as UTF-8 and returns the result in a special JavaScript typed array of unsigned
8-bit integers (see http://mng.bz/POqY). Then the browser’s crypto API generates a
hash in the form of an array-like object (an object with a length property and indexed
elements). After that, we use the Array.from() method to create a real array.
 Figure 9.8 shows a screenshot taken in Chrome’s debugger. It shows fragments of
data in the hashByteArray and hashArray variables. The breakpoint was placed right
before calculating the value of the hashHex variable. (We’ll explain how to debug your
TypeScript code in the browser in section 9.6.)
 Finally, we want to turn the calculated hash into a string of hexadecimal values,
and this is done in the following statement:
const hashHex = hashArray.map(
b => ('00' + b.toString(16)).slice(-2))
.join('');
We convert each element of the hashArray into a hexadecimal value with the
Array.map() method. Some hexadecimal values are represented by one character,
while others need two. To ensure that one-character values are prepended with a zero,
we concatenate 00 and the hexadecimal value, and then use slice(-2) to take just
the two characters at the right. For example, the hexadecimal value a becomes 00a
and then 0a.
A special JavaScript typed array
provides access to binary raw data.
The ArrayBuffer is converted into Array.
Figure 9.8 The hashByteArray and hashArray variables in the debugger
216 CHAPTER 9 Developing a browser-based blockchain node
The join('') method concatenates all the converted elements of hashArray into a
hashHex string, specifying an empty string as a separator (such as no separator). Figure 9.9 shows a fragment of the result of applying map(), along with the final hash in
the hashHex variable.
You may ask why the signature of the sha256_browser() function declares the return
type Promise, but it actually returns a string. We declared this function as async, so it
automatically wraps its returned value into a Promise.
 By now, you should understand the code in the lib directory and how the web client uses it. The last piece to review is the standalone client that can be used instead of
the web client.
9.5 The standalone blockchain client
The directory node of this project has a little script that can create a blockchain without the web UI, but we wanted to show you that the code in the lib directory is reusable, and that if the app runs under Node.js, the proper crypto API will be engaged.
The following listing shows a script that doesn’t need a browser but runs under the
Node runtime. The first line imports the Blockchain class, which hides the details of
the specific crypto API.
import { Blockchain } from '../lib/bc_transactions';
(async function main(): Promise<void> {
console.log('? Initializing the blockchain, creating the genesis
➥ block...');
const bc = new Blockchain();
Listing 9.11 node/main.ts: a standalone script for block mining
The elements of the hashArray
are turned into a string hashHex.
Figure 9.9 Applying map() and join()
Creates a new blockchain
The standalone blockchain client 217
await bc.createGenesisBlock();
bc.createTransaction({ sender: 'John', recipient: 'Kate', amount: 50 });
bc.createTransaction({ sender: 'Kate', recipient: 'Mike', amount: 10 });
await bc.minePendingTransactions(); 4((CO12-5))
bc.createTransaction({ sender: 'Alex', recipient: 'Rosa', amount: 15 });
bc.createTransaction({ sender: 'Gina', recipient: 'Rick', amount: 60 });
await bc.minePendingTransactions();
console.log(JSON.stringify(bc, null, 2));
})();
This program starts by printing a message stating that the new blockchain is being created. The process of creating the genesis block can take some time, and the first await
waits for it to complete.
 What would happen if we didn’t use await in the line that invokes bc.createGenesisBlock()? The code would proceed with mining blocks before the genesis
block was created, and the script would fail with runtime errors.
 After the genesis block is created, the script proceeds with creating two pending
transactions and mining a new block. Again, await will wait for this process to complete, and then we create two more transactions and mine another block. Finally, the
program prints the content of the blockchain.
TIP Node.js supports async and await starting with version 8.
Remember that the directory node has its own tsconfig.json file, as was shown in figure 9.1. Its content was shown in listing 9.3.
 To launch this program, make sure that the code is compiled by running tsc. It’s
important that you run tsc from the src/node directory, to ensure that the compiler
picks up the options from the whole hierarchy of tsconfig.json files. As specified in the
base tsconfig.json file, the compiled code will be saved in the dist directory.
TIP The compiler’s -p option allows you to specify the path to a valid JSON
configuration file. For example, you could compile the TypeScript code by
running the following command: tsc -p src/node/tsconfig.json.
Now you can ask Node.js to launch the JavaScript version of the code shown in listing
9.11. If you’re still in the src/node directory, you can run the app as follows:
node ../../dist/node/main.js
Listing 9.12 shows the console output of this command.
Creates the genesis block
Creates a pending
transaction Creates a
new block
and adds
 it to the
blockchain
Prints the content
of the blockchain
218 CHAPTER 9 Developing a browser-based blockchain node
? Initializing the blockchain, creating the genesis block...
{
"_chain": [
{
"previousHash": "0",
"timestamp": 1540391674580,
"transactions": [],
"nonce": 239428,
"hash": "0000d1452c893a79347810d1c567e767ea55e52a8a5ffc9743303f780b6c30
➥ 8f"
},
{
"previousHash": "0000d1452c893a79347810d1c567e767ea55e52a8a5ffc9743303f
➥ 780b6c308f",
"timestamp": 1540391675729,
"transactions": [
{
"sender": "John",
"recipient": "Kate",
"amount": 50
},
{
"sender": "Kate",
"recipient": "Mike",
"amount": 10
}
],
"nonce": 69189,
"hash": "00006f79662bde59ff46cd57cff928977c465d931b2ba2d11e05868afcfee8
➥ 36"
},
{
"previousHash": "00006f79662bde59ff46cd57cff928977c465d931b2ba2d11e0586
➥ 8afcfee836",
"timestamp": 1540391676138,
"transactions": [
{
"sender": "Alex",
"recipient": "Rosa",
"amount": 15
},
{
"sender": "Gina",
"recipient": "Rick",
"amount": 60
}
],
"nonce": 33462,
"hash": "0000483b745526f48afde33435c21517dd72ea0a25407bc35be3f921029a32
➥ 09"
}
],
"_pendingTransactions": []
}
Listing 9.12 Creating the blockchain in a standalone app
The genesis block
The second block
The third block
The array of pending
transactions is empty
Debugging TypeScript in the browser 219
Whereas the web version of this app is more interactive, the standalone version runs
the entire process in batch mode. Still, the main focus of this chapter was developing
a web app. Now we’ll show you how to debug the TypeScript code of a web app in the
browser.
9.6 Debugging TypeScript in the browser
Writing code in TypeScript is fun, but web browsers don’t understand this language.
They load and run only the JavaScript version of the app. Moreover, the executable
JavaScript may be optimized and compressed, which makes it unreadable. But there is
a way to load the original TypeScript code into the browser as well.
 For that, you need to generate source map files, which map the executable lines of
code back to the corresponding source code, which is TypeScript in our case. If the
browser loads the source map files, you can debug the original sources! In the tsconfig.json file shown in listing 9.2, we asked the compiler to generate source maps,
which are files with the extension .js.map, as shown in figure 9.2.
 When a browser loads the JavaScript code of a web app, it loads only the .js files,
even if the deployed app includes the .js.map files. But if you open the browser’s dev
tools, the browser will then load the source maps as well.
TIP If your browser doesn’t load the source map files for your app, check the
settings of the dev tools. Ensure that the JavaScript source maps option is
enabled.
Now let’s load our web client (as explained in section 9.1) in the Chrome browser and
open dev tools from the Sources tab. This will split the screen into three parts, as
shown in figure 9.10.
On the left you can browse and select a source file from your project. We’ve selected
universal_sha256.ts, and its TypeScript code is shown in the middle. On the right, you
see the debugger panel.
 Let’s set a breakpoint at line 12 by clicking to the left of the line number, and then
refresh the browser window. The app will stop at the breakpoint, and the browser window will look like the one in figure 9.11.
Figure 9.10 The Sources panel of Chrome’s dev tools
220 CHAPTER 9 Developing a browser-based blockchain node
You can see the values of the variables by hovering your mouse over a variable name.
Figure 9.12 shows the values of the msgUint8Array variable as we hovered our mouse
over its name in line 8.
You can also view the values of any variable or expression by adding it to the Watch
area in the debugger panel on the right. Figure 9.13 shows the program paused at line
15. We’ve added a couple of variable names and one expression by clicking on the
plus sign in the Watch area on the right.
Figure 9.11 The program paused at the breakpoint
Figure 9.12 Viewing a variable’s values by hovering the mouse on its name
Summary 221
If you want to remove a watched variable, click on the minus sign to the right of the
variable or expression you want to remove. In figure 9.13, you can see this minus sign
to the right of the hashHex variable in the Watch area.
 The Chrome browser comes with a full-featured debugger. To learn more about it,
watch the video “Debugging JavaScript - Chrome DevTools 101” on YouTube at
http://mng.bz/JzqK. With source maps, you’ll be able to debug your TypeScript even
if the JavaScript was optimized, minimized, or uglified.
NOTE IDEs also come with debuggers, and in chapter 10, we’ll use VS Code’s
debugger to debug a standalone TypeScript server. But when it comes to web
apps, we prefer debugging right in the browser because we may need to know
more about the execution context (network requests, application storage, session storage, and so on) to figure out a problem.
Summary
 Typically, a TypeScript app is a project that consists of multiple files. Some contain configuration options for the Typescript compiler and the bundler, and
others contain the source code.
 You can organize the source code of a project by splitting the code into several
directories. In our app, the lib directory was reused by two different client
apps—a web app (in the browser directory) and the standalone app (in the
node directory).
 You can use npm scripts to create custom commands for app deployment, starting the web server, and so on.
Figure 9.13 Watching variables in the Watch area
222
Client-server
 communications using
 Node.js, TypeScript,
 and WebSockets
In the previous chapter, you learned that each block miner can take a number of
pending transactions, create a valid block that includes the proof of work, and add
the new block to the blockchain. This workflow is easy to follow when there is only
one miner creating the proof of work. Realistically, there could be thousands of
This chapter covers
 Why a blockchain may need a server
 The longest chain rule
 How to create a Node.js WebSocket server in
TypeScript
 Practical use of TypeScript interfaces, abstract
classes, access qualifiers, enums, and generics
Resolving conflicts using the longest chain rule 223
miners around the world trying to find the valid hash for a block with the same transactions, which may cause conflicts.
 In this chapter, we’ll use TypeScript to create a server that uses the WebSocket protocol to broadcast messages to the blockchain’s nodes. The web clients can also make
requests of this server.
 While writing code in TypeScript remains our main activity, we’ll use several JavaScript packages for the first time in this book:
 ws—A Node.js library that supports the WebSocket protocol
 express—A small Node.js framework offering HTTP support
 nodemon—A tool that restarts Node.js-based apps when script file changes are
detected
 lit-html—HTML templates in JavaScript for rendering to the browser’s DOM
These packages are included in the package.json file as dependencies (see section 10.4.2).
 Before discussing the TypeScript code of this chapter’s blockchain app, we need to
cover the following subjects:
 The blockchain concept known as the longest chain rule
 How to build and run a blockchain app emulating more than one block miner
We’ll also need to go over these infrastructure-related subjects:
 The project structure, its configuration files, and npm scripts.
 The WebSocket protocol—what it’s about and why it’s better than HTTP for
implementing a notification server. As an illustration, we’ll create a simple WebSocket server that can push messages to a web client.
Let’s start with the longest chain rule.
10.1 Resolving conflicts using the longest chain rule
In chapter 9, we had a simplified way to start block mining—the user would click the
Confirm Transaction button to create a new block. In this chapter, let’s consider a
more realistic example of a blockchain that has already 100 blocks and a pool of pending transactions. Multiple miners could grab pending transactions (such as 10 transactions each) and start mining blocks.
NOTE While reading about block mining in this chapter, keep in mind that
we’re talking about a decentralized network. The blockchain nodes work in
parallel, which may result in conflict situations if more than one node claims
to have mined the next block. That’s why a consensus mechanism is required
to resolve conflicts.
Let’s pick three arbitrary miners, M1, M2, and M3, and assume they’ve found the
proper hash (the proof of work) and broadcast their versions of the new block, number 101, as a candidate for adding to the blockchain. Each of their candidate blocks
may contain different transactions, but each wants to become block number 101.
224 CHAPTER 10 Client-server communications using Node.js, TypeScript, and WebSockets
The miners are located on different continents, and forks are created in these miners’
nodes as shown in figure 10.1. Temporarily, three forks exist—they’re identical to the
first 100 blocks, but block 101 is different in each fork. At this point, each of these
three blocks is validated (they have the right hashes), but they’re not confirmed yet.
 How can we decide which of these three should be added to the blockchain? We’ll
use the longest chain rule. A multinode chain may have multiple blocks that are candidates for being added to the network, but the blockchain is like a living, growing
organism that keeps adding nodes all the time. By the time all of the miners have finished mining, one of their chains will have been used by some miner to add more
blocks, and that will have become the longest chain.
 For simplicity, we’ll consider just our three miners, though there could be hundreds of thousands of them at the same time. Our miners are mining blocks 102, 103,
and so on, based on one of the versions of block 101. Suppose some other miner, M4,
already requested the longest chain and picked block 101 from M2’s fork, and M4
already calculated the hash for the next candidate block, 102. Let’s assume that M2
had a more powerful CPU than M1 and M3, so the M2’s fork was the longest (it had
the extra block 101) at some point. That’s why miner M4 linked it to block 101, as
shown in figure 10.2.
 Now, even though we have three forks that include block 101, the longest chain is
the one that ends with block 102, and it will be adopted by all other nodes. The forks
created by miners M1 and M3 will be discarded, and their transactions will be placed
back into the pool of pending transactions so other miners can pick them up while
creating other blocks.
Block 99 Block 100 Block 101
(M2)
Block 101
(M3)
Block 101
(M1)
Figure 10.1 A forked blockchain
The node with
the longest chain
Block 99 Block 100 Block 101
(M2)
Block 101
(M3)
Block 101
(M1) X
X
Block 102
(M4)
Figure 10.2 M2 has the longest chain.
Resolving conflicts using the longest chain rule 225
M2 will get the reward for mining block 101; miners M1 and M3 just wasted electricity
calculating the hash for their versions of block 101. M4 was similarly risking useless
work when it selected M2’s block with an unconfirmed, but valid, hash.
NOTE Blockchains implement a mechanism to ensure that if a transaction is
included in a valid block by any node, it isn’t placed back in the pool of pending transactions when blocks are discarded.
We’ve used a small number of blocks in this example for simplicity, but public blockchains may have many thousands of nodes. In our scenario, there was a moment when
the fork of miner M2 had a longest chain that was just one block longer than the
other forks. In real-world blockchains, there could be multiple forks of different
lengths. Since the blockchain is a distributed network, blocks are being added on multiple nodes, and each node may have a chain of a different length. The longest chain
is considered to be the correct one.
TIP Later in this chapter, we’ll go over the process of requesting the longest
chain and getting responses. Figures 10.5–10.8 show the communication
between two nodes while requesting the longest chain and announcing newly
mined blocks.
Now let’s see how the longest chain rule helps in preventing the double-spending problem, and other fraud. Let’s say one miner has a friend, John, who has $1,000 in his
account. One of the transactions in block 99 is: “John paid Mary $1,000.” What if the
miner decides to commit fraud by forking the chain and adding another transaction,
“John paid Alex $1,000,” in block 100? Technically, this criminal miner is trying to
cheat the blockchain by making it appear that John spent the same $1,000 twice—
once in a transaction to Mary, and again in a transaction to Alex. Figure 10.3 shows an
attempt by the criminal miner to convince others that the correct view of the blockchain is the fork that contains the John-to-Alex transaction.
 Remember, the block’s hash value is easy to check, but it’s time-consuming to calculate, so our criminal miner has to calculate the hashes for blocks 100, 101, and 102.
Meanwhile, other miners continue mining new blocks (103, 104, 105, and so on) adding them to the chain shown at the top of figure 10.3. There’s no way that the criminal
miner can recalculate all the hashes and create a longer chain faster than all the other
nodes. The chain with the most work done (the longest chain) wins. In other words,
the chances of modifying the content of the existing block(s) is close to zero, which
makes a blockchain immutable.
Double-spending
Block 100
Block 101
Block 99
John -> Mary ...
Block 102
Block 101 Block 102
Block 100
John -> Alex Figure 10.3
An attempt to
double-spend
226 CHAPTER 10 Client-server communications using Node.js, TypeScript, and WebSockets
So far we haven’t discussed how blockchain nodes communicate with each other.
We’ll do that in the next section.
10.2 Adding a server to the blockchain
As you’ll recall, we’ve praised blockchain technology for being completely decentralized without a server. The server we add here won’t be a central authority for creating,
validating, and storing blocks. The blockchain can remain decentralized and still use
a server for utility services, such as caching hashes, broadcasting new transactions,
requesting the longest chain, or announcing newly created blocks. Later in this chapter (section 10.6.1), we’ll review the code of such a server. In this section we’ll discuss
the process of communication between clients via this server.
TIP Anton Moiseev has implemented our blockchain app using a peer-topeer technology called WebRTC (see https://github.com/antonmoiseev/
blockchain-p2p), and you can experiment with it on your own.
Suppose the M1 node has mined a block and requested the longest chain from the
server, which broadcasts this request to all other nodes on the blockchain. Each of the
nodes responds with their chains (just the block headers), and the server forwards
these responses to M1.
 The M1 node receives the longest chain and validates it by checking the hashes of
each block. Then M1 adds its newly mined block to this longest chain, and this chain
is saved locally. For some time, M1’s node will enjoy the status of “the source of truth,”
because it will have the longest chain until someone else mines one or more new
blocks.
 Let’s say M1 wants to create a new transaction: “Joe sent Mary $1,000.” Creating a new
block for each transaction would be too slow (lots of hashes would need to be calculated) and expensive (the electricity cost). Typically, one block includes multiple transactions, and M1 can simply broadcast its new transaction to the rest of the blockchain
members. Miners M2 and M3 and any others will do the same with their transactions.
What’s a consensus?
Any decentralized system needs to have rules allowing all the nodes to agree that a
valid event happened. What if two nodes calculated the hash of a new block at the
same time? Which block would be added to the chain, and which node would be
rewarded? All nodes of the blockchain have to come to a general agreement—a
consensus—about who the winner is.
Consensus is required from all members because there is no system administrator
who could modify or delete a block. Blockchains use different rules (consensus protocols), and in our app we’ll use proof of work combined with the longest chain to
reach consensus on the true state of the blockchain. The aim of the consensus protocol is to guarantee that a single chain is used.
The project structure 227
All broadcast transactions go into the pool of pending transactions, and any node can
pick a group of transactions (say, 10 of them) from there and start mining.
TIP For our version of blockchain, we’ll use Node.js to create a server that
implements broadcasting via a Websocket connection. As an alternative, you
could go completely serverless by implementing broadcasting using some peerto-peer technology like WebRTC (https://en.wikipedia.org/wiki/WebRTC).
10.3 The project structure
The blockchain app that comes with this chapter consists of two parts, the server and
the client, both of which are implemented using TypeScript. We’ll show you the project structure and explain how to run this app first. Then we’ll discuss selected parts of
the code that illustrate the practical use of particular TypeScript syntax constructs.
 In your IDE, open the project in the chapter10 directory, and run npm install in
the terminal window. The structure of this project is shown in figure 10.4.
 The public directory is created during the build process, and the public/
index.html file loads the compiled version of the client/main.ts file, along with all its
imported scripts. This is the web client that we’ll use to illustrate the blockchain node.
 The server/main.ts file contains the code that imports additional scripts and starts
the WebSocket and the blockchain notification servers.
HTML and CSS
for the browser
The client’s
implementation
The server’s
implementation
Shared data
types definitions
tsc config
files
nodemon utility’s
config file
Figure 10.4 The project structure
228 CHAPTER 10 Client-server communications using Node.js, TypeScript, and WebSockets
NOTE In section 10.4.4, we’ll explain how to build and run the client and the
server.
Have you noticed that there are three config files for the Typescript compiler? The
tsconfig.json file in the root directory contains the compiler options that are common
to both the client and server. The tsconfig.client.json and tsconfig.server.json files
extend tsconfig.json and add compiler options specific to the client and server respectively. We’ll review their content in the next section.
 Our server runs under the Node.js runtime, and its compiled code will be stored in
the build/server directory. You can start the server with the following command:
node build/server/main.js
However, the node executable doesn’t support live reload (it won’t restart if you change
and recompile the TypeScript code of the server). That’s why we use the nodemon
utility (https://nodemon.io), which monitors the Node.js app’s codebase and restarts
the Node.js server on file changes. If you have the nodemon utility installed, you can
use it instead of node. For example, this is how you can start the Node.js runtime and
run the code using the globally installed nodemon:
nodemon build/server/main.js
If you don’t have nodemon, run it as follows:
npx nodemon build/server/main.js
In our project, we’ll be starting the server with nodemon, which will be configured in
the nodemon.json file discussed in the next section. The package.json file includes
the npm scripts section, and we’ll review it in the next section as well.
10.4 The project’s configuration files
The project that comes with this chapter includes several JSON configuration files
that we’ll review in this section.
10.4.1 Configuring the TypeScript compilation
A tsconfig.json file can inherit configurations from another file using the extends
config property. In our project, we have three config files:
 tsconfig.json contains the common tsc compiler options for the entire project.
 tsconfig.client.json contains the options for compiling the client portion of the
project.
 tsconfig.server.json includes the options for compiling the server portion of the
project.
The project’s configuration files 229
The following listing shows the content of the base tsconfig.json file.
{
"compileOnSave": false,
"compilerOptions": {
"target": "es2017",
"sourceMap": true,
"plugins": [
{
"name": "typescript-lit-html-plugin".
}
]
}
}
We specified es2017 as the compilation target because we’re sure that the users of this
app will use modern browsers that support all the features in the ECMAScript 2017
specification.
 The next listing shows the tsconfig.client.json file, which contains the tsc options
we want to use for compiling the client’s portion of the code. This file is located in the
src/client directory.
{
"extends": "../../tsconfig.json",
"compilerOptions": {
"module": "es2015",
"outDir": "../../public",
"inlineSources": true
}
}
Source maps allow you to debug TypeScript while the browser executes JavaScript.
They work by instructing the browser’s dev tools which lines of the compiled code
(JavaScript) correspond to which lines of the source (TypeScript). However, the
source code needs to be available to the browser, and you can either deploy TypeScript along with JavaScript to the web server, or use the inlineSources option as we
did here, which embeds the original TypeScript right inside the source maps files.
TIP If you don’t want to reveal the source of your app to users, don’t deploy
source maps in production.
Listing 10.1 tsconfig.json—common tsc options
Listing 10.2 tsconfig.client.json—tsc options for the client
Doesn’t autocompile on each
modification of the TypeScript files
Compiles into JavaScript using the
syntax supported by ECMAScript 2017
Generates source map files
This plugin enables HTML
autocompletion for template
strings tagged with html.
Inherits the config
options from this file
Uses import/export statements
in the generated JavaScript
Places the compiled code
in the public directory
Emits the original TypeScript code
and source maps within a single file
230 CHAPTER 10 Client-server communications using Node.js, TypeScript, and WebSockets
The following listing shows the tsconfig.server.json file, which contains tsc options for
compiling the server’s code. This file is located in the src/server directory.
{
"extends": "../../tsconfig.json",
"compilerOptions": {
"module": "commonjs",
"outDir": "../../build"
},
"include": [
"**/*.ts"
]
}
TIP In a perfect world, you would never see the same compiler option in the
base and inherited config files. But sometimes IDEs don’t handle tsc configuration inheritance properly, and you may need to repeat an option in more
than one file.
Now that we have more than one tsc config file, how do we let the Typescript compiler
know which one to use? We use the -p option. The following command compiles the
web client code using the options from tsconfig.client.json:
tsc -p src/client/tsconfig.client.json
The next command compiles the server code using tsconfig.server.json:
tsc -p src/server/tsconfig.server.json
NOTE If you introduce tsc configuration files named other than tsconfig.json,
you need to use the -p option and specify the path to the file you want to use.
For example, if you ran the tsc command in the src/server directory, it
wouldn’t use the options from the tsconfig.server.json file, and you might
experience unexpected compilation results.
Now let’s take a look at the dependencies and the npm scripts for this project.
10.4.2 What’s in package.json
Listing 10.4 shows the content of package.json. The scripts section contains custom
npm commands, and the dependencies section lists only three packages that are
required to run our app (both the client and server):
 ws—A Node.js library that supports WebSocket protocol
 express—A small Node.js framework offering HTTP support
 lit-html—HTML templates in JavaScript for rendering to the browser’s DOM
Listing 10.3 tsconfig.server.json—tsc options for the server
Inherits config options
from this file
Converts import/export statements
to commonjs-compliant code
Places compiled code
in the build directory
Compiles all .ts files
from all subdirectories
The project’s configuration files 231
The web part of this app uses lit-html (https://github.com/Polymer/lit-html). It’s a
templating library for JavaScript, and typescript-lit-html-plugin will enable autocomplete (IntelliSense) in your IDE.
 The devDependencies section includes packages that are needed only during
development.
{
"name": "blockchain",
"version": "1.0.0",
"description": "Chapter 10 sample app",
"license": "MIT",
"scripts": {
"build:client": "tsc -p src/client/tsconfig.client.json",
"build:server": "tsc -p src/server/tsconfig.server.json",
"build": "concurrently npm:build:*",
"start:client": "tsc -p src/client/tsconfig.client.json --watch",
"start:server": "nodemon --inspect src/server/main.ts",
"start": "concurrently npm:start:*",
"now-start": "NODE_ENV=production node build/server/main.js"
},
"dependencies": {
"express": "^4.16.3",
"lit-html": "^0.12.0",
"ws": "^6.0.0"
},
"devDependencies": {
"@types/express": "^4.16.0",
"@types/ws": "^6.0.1",
"concurrently": "^4.0.1",
"nodemon": "^1.18.4",
"ts-node": "^7.0.1",
"typescript": "^3.1.1",
"typescript-lit-html-plugin": "^0.6.0"
}
}
ts-node launches a single Node process. After Node is launched, it registers a custom
extension/loader pair using the Node’s require.extensions mechanism. When
Node’s require() call resolves to a file with the extension .ts, Node invokes a custom
loader, which compiles TypeScript into JavaScript on the fly using tsc’s programmatic
API, without launching a separate tsc process.
 Note that the start:client command runs tsc in watch mode (using the --watch
option). This ensures that as soon as you modify and save any TypeScript code on the
client, it’ll get recompiled. But what about recompiling the server’s code?
Listing 10.4 package.json: dependencies of our web app
Our custom npm
script commands
Runs tsc for
the client in
a watch
mode
The web framework for Node.js
The templating library for the client
The package to support Type WebSocket in Node.js apps definition
files The package to run multiple commands concurrently
The utility for the live reload
of the Node.js runtime
Runs both tsc and node
as a single process The plugin to enable
IntelliSense for the lit-html tags
232 CHAPTER 10 Client-server communications using Node.js, TypeScript, and WebSockets
10.4.3 Configuring nodemon
We could start the tsc compiler in watch mode on the server as well, and the JavaScript
would be regenerated when the TypeScript code is modified. But having fresh JavaScript code is not enough on the server—we’ll need to restart the Node.js runtime on
each code change. That’s why we’ve installed the nodemon utility, which will start the
Node.js process and monitor the JavaScript files in the specified directory.
 The package.json file (listing 10.4) includes the following command:
"start:server": "nodemon --inspect src/server/main.ts"
TIP The --inspect option allows you to debug the code that runs in Node.js
in Chrome’s dev tools (see Node’s “Debugging Guide” for details: http://
mng.bz/wlX2). Nodemon just passes the --inspect option to Node.js, so it
will be started in debug mode.
It might seem that the start:server command requests nodemon to start the main.ts
TypeScript file, but since our project has the nodemon.json file, nodemon will use the
options from there. Our nodemon.json file contains the following configuration
options for nodemon.
{
"exec": "node -r ts-node/register/transpile-only",
"watch": [ "src/server/**/*.ts" ]
}
The exec command allows us to specify the options for starting Node.js. In particular,
the -r option is a shortcut for --require module, which is used for preloading modules on startup. In our case, it asks the ts-node package to preload TypeScript’s faster
transpile-only module, which simply converts the code from TypeScript to JavaScript
without performing type-checking. This module will automatically run the Typescript
compiler for each file with the extension .ts loaded by Node.js.
 By preloading the transpile-only module, we eliminate the need to start a separate tsc process for the server. Any TypeScript file will be loaded and autocompiled as
part of the single Node.js process.
NOTE You can use the ts-node package in different ways. For example, you can
use it to start Node.js with TypeScript compilation: ts-node myScript.ts. See
the ts-node page on npm for more details: www.npmjs.com/package/ts-node.
You’ve seen a high-level overview of the blockchain app’s configuration. The next step
is to see it in action.
Listing 10.5 nodemon.json: the config file for the nodemon utility
How to start the node
Watches all .ts files located
in all server’s subdirectories
The project’s configuration files 233
10.4.4 Running the blockchain app
In this section, we’ll show you how to run both the server and two clients emulating
blockchain nodes. To start the processes we’ll be using npm scripts, so let’s take a
closer look at the scripts section of the package.json file.
"scripts": {
"build:client": "tsc -p src/client/tsconfig.client.json",
"build:server": "tsc -p src/server/tsconfig.server.json",
"build": "concurrently npm:build:*",
"start:tsc:client": "tsc -p src/client/tsconfig.client.json --watch",
"start:server": "nodemon --inspect src/server/main.ts",
"start": "concurrently npm:start:*",
}
The first two build commands start the tsc compiler for the client and server respectively. These processes compile TypeScript to JavaScript. The compilation of the client
and server code can be done in parallel, so the third command uses the npm package
called “concurrently” (www.npmjs.com/package/concurrently), which allows you to
run multiple commands concurrently.
 The start:tsc:client command compiles the client’s code in watch mode, and
start:server starts the server using nodemon, as described in the previous section. The
third start command runs both start:tsc:client and start:server concurrently.
 In general, you can start more than one npm command by simply adding an
ampersand between commands. For example, you could define two custom commands, first and second, and then run them concurrently with npm start. In npm
scripts, the ampersand means “run these commands concurrently.”
"scripts": {
"first": "sleep 2; echo First",
"second": "sleep 1; echo Second",
"start": "npm run first & npm run second"
},
If you run npm start, it’ll print “Second” and then “First,” which proves that the commands ran concurrently. Replacing & with && will print “First” and then “Second,” indicating sequential execution.
 Using the concurrently package rather than an ampersand gives you a clean separation of the messages printed by each concurrent process.
Listing 10.6 The scripts section of package.json
Listing 10.7 Concurrent execution with the ampersand
Compiles the client’s code Compiles the
server’s code
Concurrently
runs all
commands
that start with
npm:build
Starts tsc in
watch mode for
Starts the server with nodemon the client’s code
Concurrently runs all commands
that start with npm:start
Sleeps for 2 seconds and prints “First” Sleeps for 1 second
and prints “Second”
Runs the first and second
commands concurrently
234 CHAPTER 10 Client-server communications using Node.js, TypeScript, and WebSockets
To start the blockchain app, run the npm start command in the Terminal window.
The following listing shows the terminal output. We’re running two commands concurrently: start:tsc:client and start:server. Each line in the terminal output
starts with the name of the process (in square brackets) that produced this message.
> blockchain@1.0.0 start /Users/yfain11/Documents/get_ts/code/getts/chapter10
10:47:18 PM - Starting compilation in watch mode...
[start:tsc:client]
[start:server] [nodemon] 1.18.9
[start:server] [nodemon] to restart at any time, enter `rs`
[start:server] [nodemon] watching: src/server/**/*.ts
[start:server] [nodemon] starting `node -r ts-node/register/transpile-only
➥ --inspect src/server/main.ts`
[start:server] Debugger listening on
➥ ws://127.0.0.1:9229/2254fc00-3640-4390-8302-1e17285d0d23
[start:server] For help, see: https://nodejs.org/en/docs/inspector
[start:server] Listening on http://localhost:3000
[start:tsc:client] 10:47:21 PM - Found 0 errors. Watching for file changes.
Listing 10.8 Starting the blockchain app
Using ampersands in npm scripts on Windows
A single ampersand (&) doesn’t run npm commands in parallel on Windows. To start
running them at the same time, you can use the npm-run-all package (www.npmjs
.com/package/npm-run-all).
Here is what the code from listing 10.7 would look like on Windows:
"scripts": {
"first": "timeout /T 2 > nul && echo First",
"second": "timeout /T 1 > nul && echo Second",
"start": "run-p first second"
}
Instead of sleep we use timeout and specify how long the process should be inactive in seconds, using the /T parameter. While running, the timeout command prints
to the console how many seconds are still left to wait. To avoid these messages interfering with the “First” and “Second” output, we redirect timeout’s output to nul,
which throws messages away.
run-p is a command that comes with the npm-run-all package. It runs npm scripts
with the specified names in parallel.
timeout is a Windows
alternative for the sleep
command on Unix systems.
run-p is a command installed along
with the npm-run-all package.
The output of the start:tsc:client process
The output of the start:tsc:client process
The output of the
start:server process
The Node.js debugger runs locally
on port 9229.
The server is up and
running on port 3000.
The project’s configuration files 235
NOTE The URL of the Node.js debugger is ws://127.0.0.1. It starts with “ws,”
indicating that the dev tools connect to the debugger using the WebSocket
protocol, which we’ll introduce in the next section. When the Node.js debugger is running, you’ll see a green hexagon on the Chrome dev tools toolbar.
Read more about the Node.js debugger in Paul Irish’s article “Debugging
Node.js with Chrome Dev Tools” on Medium, at http://mng.bz/qX6J.
The server is up and running, and entering localhost:3000 will start the first client
of our blockchain. After a couple of seconds, the genesis block will be generated, and
you’ll see a web page as in figure 10.5.
 We’ll show you what’s happening under the hood in section 10.6, after we explain
how the clients communicate with each other via the WebSocket server. At this point,
suffice it to say that before creating any blocks, the client makes a request to the server
to find the longest chain.
The first client
The genesis block created
by the first client
Figure 10.5 The
view of the very first
blockchain client
236 CHAPTER 10 Client-server communications using Node.js, TypeScript, and WebSockets
Start the second client by opening a separate browser window at localhost:3000, as
shown in figure 10.6.
 Now let’s discuss the use case when only the first client adds pending transactions,
starts block mining, and invites other nodes to do the mining too. By this time, our
blockchain has a genesis block, and the server broadcast it to all connected clients (to
the client on the right in figure 10.6). Note that both clients see the same block that
was mined by the first client. Then the first client enters two transactions, as shown in
figure 10.7. No requests for the new block’s generation have been made yet.
NOTE While a client is adding pending transactions, there are no communications with other clients, and the messaging server is not being used.
Now the first client starts mining by clicking on the GENERATE BLOCK button.
Under the hood, the first client sends a message with the block’s content to the server,
announcing that the first client started mining this block. The server broadcast this
message to all connected clients, and they start mining the same block as well.
The first client The second client
The genesis block created
by the first client
Figure 10.6 The view of the first two blockchain clients
The project’s configuration files 237
One of the clients will be faster, and its new block is added to the blockchain and is
broadcast to other connected clients so they can add this block to their versions of the
blockchain. After the blocks are accepted in the blockchain, all clients will contain the
same blocks, as shown in figure 10.8.
Figure 10.7 One client created pending transactions.
Figure 10.8 Each client adds the same block.
The first client The second client
Newly mined block
238 CHAPTER 10 Client-server communications using Node.js, TypeScript, and WebSockets
In section 10.6, we’ll discuss the messages that go over the sockets while the blocks are
being mined, so the process of mining in a multinode network won’t look like magic.
For now, figure 10.9 shows a sequence diagram that should help you follow the messaging exchange, assuming that there are just two nodes: M1 and M2.
 In this example, the user works with the M2 node. When pending transactions are
created, no messages are being sent to other nodes. The messaging starts when the
user initiates the generate-block operation. The M2 node sends a “request new block”
message and starts mining at the M2 node. The server broadcasts this message to
other nodes (M1 in this case), which also starts mining, competing with M2.
 In this diagram, the M2 node is faster and is the first to announce the new block,
and the server broadcasts this message across the blockchain. The M1 node added
M2’s block to its local blockchain. A bit later, M1 finished mining too, but the addition
of M1’s block failed because it had already accepted and added M2’s block, and the
existing chain is longer. In other words, M2’s block was approved as the winning block
by consensus between M1 and M2. Finally, the new block is rendered on the user’s UI.
Node M2
Add transaction
Display pending
transaction
User
Generate block
Notification
server
Mine block
Add M2’s block
Add M2’s block
Request new block
Announce new block
Broadcast new
block request
Broadcast new
block announcement
Node M1
Mine block
Failed
adding M1’s
block
X
Display new block
Figure 10.9 The mining process in a two-node blockchain
A brief introduction to WebSockets 239
 The code that corresponds to the failed attempt of adding the new node will be
shown later in this chapter (in the sidebar titled, “When the new block is rejected”).
NOTE In our simplified blockchain, the UI and the block-creation logic are
implemented in the same web app. A real-world app that uses blockchain
technology would have separate apps for adding transactions and mining
blocks.
Now that you’ve seen how the app works, let’s get familiar with the client-server communications over WebSockets. If you’re familiar with WebSockets, you can skip ahead
to section 10.6.
10.5 A brief introduction to WebSockets
The blockchain app in this chapter will be pushing notifications using the WebSocket
protocol (https://en.wikipedia.org/wiki/WebSocket), so we’d like to give you a brief
overview of this low-overhead binary protocol supported by all modern web browsers
as well as all web servers written in Node.js, .Net, Java, Python, and so on.
 The WebSocket protocol allows bidirectional message-oriented streaming of text
and binary data between browsers and web servers. In contrast to HTTP, WebSocket is
not a request-response based protocol, and both the server and client apps can initiate
the data push to the other party as soon as the data becomes available, in real time.
This makes the WebSocket protocol a good fit for various apps:
 Live trading, auctions, and sports notifications
 Controlling medical equipment over the web
 Chat applications
 Multiplayer online games
 Real-time updates in social streams
 Blockchain
All of these apps have one thing in common: there’s a server (or a device) that may
need to send an immediate notification to the user because some important event
happened elsewhere. This is different from the use case when the user decides to send
a request for fresh data to the server.
 For example, you could use WebSockets to send a notification immediately to all
users when a stock trade happens on the stock exchange. Or a server could broadcast
notifications from one blockchain node to others. It’s important to understand that
the WebSocket server can push a notification to a client without needing to receive a
request from the client for data.
 In our blockchain example, miner M1 may start or finish mining a block, and all
other nodes must know about it right away. M1 sends the message to the WebSocket
server announcing the new block, and the server can push this message to all other
nodes immediately.
240 CHAPTER 10 Client-server communications using Node.js, TypeScript, and WebSockets
10.5.1 Comparing HTTP and WebSocket protocols
With the request-based HTTP protocol, a client sends a request over a connection and
waits for a response to come back. Both requests and responses use the same browserserver connection. First the request goes out, and then the response comes back via
the same “wire.” Think of a narrow bridge over a river, where cars from both sides
have to take turns crossing the bridge. Cars on the server side can only go over the
bridge after a car from the client side passes. In the web realm, this type of communication is called half-duplex.
 In contrast, the WebSocket protocol allows data to travel in both directions simultaneously (full-duplex) over the same connection, and any party can initiate the data
exchange. It’s like a two-lane road. Another analogy is a phone conversation, where
two callers can speak and be heard at the same time. The WebSocket connection is
kept alive continuously, which has an additional benefit: low latency in the interaction
between the server and the client.
 A typical HTTP request/response adds several hundred bytes (HTTP headers) to
the application data. Say you want to write a web app that reports the latest stock
prices every second. With HTTP, such an app would need to send an HTTP request
(about 300 bytes) and receive a stock price that would arrive with an additional 300
bytes of an HTTP response object.
 With WebSockets, the overhead is as low as a couple of bytes. Besides, there’s no
need to keep sending requests for a new price quote every second. A particular stock
may not be traded for a while. Only when the stock price changes will the server push
the new value to the client.
 Every browser supports a WebSocket object for creating and managing a socket
connection to the server (see Mozilla’s WebSocket documentation: http://mng.bz/
1j4g). Initially the browser establishes a regular HTTP connection with the server, but
then your app requests a connection upgrade, specifying the server’s URL that supports the WebSocket connection. After that, the communication goes on without the
need of HTTP. The URLs of WebSocket endpoints start with “ws” instead of “http”—
for instance, ws://localhost:8085. Similarly, for secure communications, you’d use
“wss” instead of “https.”
 The WebSocket protocol is based on events and callbacks. For example, when your
browser app establishes a connection with the server, it receives a connection event,
and your app invokes a callback to handle this event. To handle the data that the
server may send over this connection, the client’s code expects a message event providing the corresponding callback. If the connection is closed, the close event is dispatched, so your app can react accordingly. In case of an error, the WebSocket object
gets an error event.
 On the server side, you’ll have to process similar events. Their names may be different, depending on the WebSocket software you use on the server. In the blockchain
app that comes with this chapter, we use the Node.js runtime to implement notifications using a WebSocket server. 
A brief introduction to WebSockets 241
10.5.2 Pushing data from a Node server to a plain client
To get you familiar with WebSockets, let’s consider a simple use case: the server pushes
data to a tiny browser client as soon as the client connects to the socket. Our client won’t
need to send a request for data—the server will initiate the communications.
 To enable WebSocket support, we’ll use the npm package called “ws” (www.npmjs
.com/package/ws) as you saw in package.json in listing 10.4. The @types/ws type definitions are needed, so that the Typescript compiler won’t complain when we use the
API from the ws package.
 This section shows a pretty simple WebSocket server: it will push the message “This
message was pushed by the WebSocket server” to a plain HTML/JavaScript client as
soon as the client connects to the socket. We purposely don’t want the client to send
any requests to the server so you can see that the server can push data without any
request ceremony.
 This example app creates two servers. The HTTP server (implemented with the
Express framework) runs on port 8000 and is responsible for sending the initial
HTML page to the browser. When this page is loaded, it immediately connects to the
WebSocket server that runs on port 8085. This server will push the message with the
greeting as soon as the connection is established.
 The code of this app is located in the server/simple-websocket-server.ts file, and
it’s shown in the following listing.
import * as express from "express";
import * as path from "path";
import { Server } from "ws";
const app = express();
// HTTP Server
app.get('/', (req, res) => res.sendFile(
path.join(__dirname, '../../public/simple-websocket-client.html')));
const httpServer = app.listen(8000, 'localhost', () => {
console.log('HTTP server is listening on localhost:8000');
});
// WebSocket Server
const wsServer = new Server({port: 8085});
console.log('WebSocket server is listening on localhost:8085');
wsServer.on('connection',
wsClient => {
wsClient.send('This message was pushed by the WebSocket server');
wsClient.onerror = (error) =>
console.log(`The server received: ${error['code']}`);
}
);
Listing 10.9 simple-websocket-server.ts: a simple WebSocket server
We’ll use Server from the ws module
to instantiate a WebSocket server.
Instantiates the
Express framework
When the HTTP client connects
with the root path, the HTTP
server sends back this HTML file.
Starts the HTTP
server on port
8000 Starts the WebSocket
server on port 8085
Listens to the
connection
event from
clients
Pushes the message to the
newly connected client
Handles connection errors
242 CHAPTER 10 Client-server communications using Node.js, TypeScript, and WebSockets
NOTE We could start two server instances on the same port, and we’ll do that
later, in listing 10.12. For now, to simplify the explanations, we’ll keep the
HTTP and WebSocket servers on different ports.
As soon as any client connects to our WebSocket server via port 8085, the connection
event is dispatched on the server, and the server will also receive a reference to the
object that represents this particular client. Using the send() method, the server
sends the greeting to this client. If another client connects to the same socket on port
8085, it’ll also receive the same greeting.
NOTE As soon as a new client connects to the server, a reference to this
connection is added to the wsServer.clients array, so you can broadcast
messages to all connected clients if needed: wsServer.clients.forEach
(client ? client.send('…'));.
Resolving paths in Node.js
The following code line starts with app.get(), and it maps the URL of an HTTP
request to a specific endpoint in the code or a file on disk (in this case it’s GET, but
it could be POST or another request). Let’s consider this code fragment:
app.get('/',
(req, res) => res.sendFile(
path.join(__dirname, '../../public/simple-websocket-
➥ client.html')));
The path.join() method uses the Node.js _dirname environment variable as a
starting point and then builds the full absolute path. _dirname represents the directory name of the main module.
Suppose we start the server with the following command:
node build/server/simple-websocket-server.js
In this case, the value of __dirname will be the path to the build/server directory.
Accordingly, the following code will go two levels up and one level down into public
from the build/server directory, to where the simple-websocket-client.html file is
located.
path.join(__dirname,'../../public/simple-websocket-client.html')
To make this line 100% cross-platform, it’s safer to write it without using the forward
slash as a separator:
path.join(__dirname, '..', '..', 'public', 'simple-websocket-
➥ client.html')
Server receives an HTTP
GET with the base URL
Sends the file back to
the client via the HTTP
response object
Builds an absolute
path to the HTML file
A brief introduction to WebSockets 243
The content of the public/simple-websocket-client.html file is shown in the following
listing. This is a plain HTML/JavaScript client that uses the browser’s WebSocket
object.
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
</head>
<body>
<span id="messageGoesHere"></span>
<script type="text/javascript">
const ws = new WebSocket("ws://localhost:8085");
const mySpan = document.getElementById("messageGoesHere");
ws.onmessage = function(event){
mySpan.textContent = event.data;
};
ws.onerror = function(event) {
console.log(`Error ${event}`);
}
</script>
</body>
</html>
When the browser loads simple-websocket-client.html, its script connects to your WebSocket server at ws://localhost:8085. At this point, the server upgrades the protocol
from HTTP to WebSocket. Note that the protocol is ws and not http.
 To see this sample in action, run npm install and compile the code by running
the custom command defined in package.json:
npm run build:server
The compiled version of all TypeScript files from the server directory will be stored in
the build/server directory. Run this simple WebSocket server as follows:
node build/server/simple-websocket-server.js
You’ll see the following messages on the console:
WebSocket server is listening on localhost:8085
HTTP server is listening on localhost:8000
Open the Chrome browser and its dev tools to http://localhost:8000. You’ll see the
message, as shown in figure 10.10 at the top left. Under the Network tab on the right,
Listing 10.10 simple-websocket-client.html: a simple WebSocket client
Establishes the
socket connection Gets a reference to
the DOM element
for showing
messages
The
callback for
handling
messages Displays the message in
the <span> element
In case of an error, the
browser logs the error
message on the console.
244 CHAPTER 10 Client-server communications using Node.js, TypeScript, and WebSockets
you’ll see two requests made to the servers running on localhost. The first one loads
the simple-websocket-client.html file via HTTP, and the second request goes to the
socket opened on port 8085 on our server.
 In this example, the HTTP protocol is used only to initially load the HTML file.
Then the client requests the protocol upgrade to WebSocket (status code 101), and
from then on this web page won’t use HTTP.
 Click on the Frames tab, and you’ll see the content of the message that arrived
over the socket connection from the server: “This message was pushed by the WebSocket server” (see figure 10.11).
Note the arrow pointing down beside the message in the Frames tab. It denotes
incoming socket messages. Up arrows mark messages sent by the client.
 To send a message from the client to the server, invoke the send() method on the
browser’s WebSocket object:
ws.send("Hello from the client");
Figure 10.10 Getting the message from the socket
Figure 10.11 Monitoring
the frame content
Reviewing notification workflows 245
Actually, before sending messages, you should always check the status of the socket
connection to ensure that it’s still active. The WebSocket object has a readyState
property, which can have one of the values shown in table 10.1.
You’ll see the readyState property used later, in the code of the message server in listing 10.17.
TIP If you keep looking at this table with its limited set of constants, sooner
or later TypeScript enums will come to mind, right?
In the next section, we’ll go over the process of block mining, and you’ll see how the
WebSocket server is used there.
10.6 Reviewing notification workflows
In this section, we’ll review only the parts of the code that are crucial for understanding how the server communicates with the blockchain clients. We’ll start with the scenario of two clients communicating that we looked at earlier in this chapter, but this
time we’ll keep the Chrome dev tools panel open, so we can monitor the messages
going over the WebSocket connections between the clients and server.
 Once again, we’ll launch the server on port 3000 using the npm start command.
Open the first client in the browser and connect to localhost:3000, having the tabs
Network > WS opened in the Chrome dev panel, as shown in figure 10.12. Click on
the name “localhost” at the bottom left, and you’ll see the messages sent over the
socket. The client connects to the server and makes a request to find the longest chain
by sending a message of type GET_LONGEST_CHAIN_REQUEST to the server.
TIP In the Frames panel, an arrow on the left pointing up means the message
went up to the server. An arrow pointing down means that the message
arrived from the server.
This client happened to be the very first one on this blockchain, and it received the
message GET_LONGEST_CHAIN_RESPONSE from the server with an empty payload,
because the blockchain doesn’t exist, and there are no other nodes just yet. If there
were other nodes, the server would have broadcast the request to other nodes, collected their responses, and sent them to the original node-requestor—the client that
requested the longest chain.
Table 10.1 Possible values of WebSocket.readyState
Value State Description
0 CONNECTING Socket has been created. The connection is not yet open.
1 OPEN The connection is open and ready to communicate.
2 CLOSING The connection is in the process of closing.
3 CLOSED The connection is closed or couldn’t be opened.
246 CHAPTER 10 Client-server communications using Node.js, TypeScript, and WebSockets
The message format is defined in the shared/messages.ts file shown in the following
listing. Note that we define custom types using the type, interface, and enum TypeScript keywords.
export type UUID = string;
export interface Message {
correlationId: UUID;
type: string;
payload?: any;
}
export enum MessageTypes {
Listing 10.11 shared/messages.ts: defining the message types
The first client
Msg from
client to
server
Msg from
server to
client
UUID Empty payload
Figure 10.12 The very first client connected
Declares an alias type for UUID
Declares a custom Message type
The payload is optional
Declares an enum with a set of constants
Reviewing notification workflows 247
GetLongestChainRequest = 'GET_LONGEST_CHAIN_REQUEST',
GetLongestChainResponse = 'GET_LONGEST_CHAIN_RESPONSE',
NewBlockRequest = 'NEW_BLOCK_REQUEST',
NewBlockAnnouncement = 'NEW_BLOCK_ANNOUNCEMENT'
}
The messages are being sent asynchronously, and we added a correlationId property
so we can match the outgoing and arriving messages. If the client sends the message
GET_LONGEST_CHAIN_REQUEST to the server, it’ll contain a unique correlation ID. Sometime later, the server sends a GET_LONGEST_CHAIN_RESPONSE message, which will also
contain the correlation ID. By comparing the correlation IDs of outgoing and incoming messages, we can find matching requests and responses. We use universally unique
identifiers (UUIDs) as the values for correlationId.
Because a UUID is a string of characters, we could declare the Message.correlationId property to be of type string. Instead, we created a UUID type alias using the
type keyword (see listing 10.11), and declared correlationId to be of type UUID,
which increased the code’s readability.
 When a client (a blockchain node) starts mining, it sends the message
NEW_BLOCK_REQUEST, inviting other nodes to do the same. A node announces that the
mining is complete by sending the message NEW_BLOCK_ANNOUNCEMENT to other nodes,
and its block becomes a candidate for adding to the blockchain.
10.6.1 Reviewing the server’s code
We start the server by loading the server/main.ts script (listing 10.12) in the Node.js
runtime. In this script, we import the Express framework for configuring the HTTP
endpoints and the directory where the web client’s code is deployed. We also import
the http object from Node.js, and the ws package for WebSocket support. The
server/main.ts script starts two server instances on the same port: httpServer, which
supports HTTP, and wsServer, which supports the WebSocket protocol.
Generating UUIDs
As per specification RFC 4122 (www.ietf.org/rfc/rfc4122.txt), “A UUID is an identifier
that is unique across both space and time, with respect to the space of all UUIDs.
Since a UUID is a fixed size and contains a time field, it is possible for values to rollover (around A.D. 3400, depending on the specific algorithm used).”
A UUID is a fixed-length string of ASCII characters in the following format: "xxxxxxxxxxxx-xxxx-xxxx-xxxxxxxxxxxx", and you can see an example in the messages in
figure 10.12. We borrowed the code for generating UUIDs from StackOverflow
(http://mng.bz/7z6e).
In our app, all requests are initiated by the client, so UUIDs are generated in the client’s code. You can find the uuid() function in the client/lib/cryptography.ts script.
248 CHAPTER 10 Client-server communications using Node.js, TypeScript, and WebSockets
import * as express from 'express';
import * as http from 'http';
import * as path from 'path';
import * as WebSocket from 'ws';
import { BlockchainServer } from './blockchain-server';
const PORT = 3000;
const app = express();
app.use('/', express.static(path.join(__dirname, '..', '..', 'public')));
app.use('/node_modules', express.static(path.join(__dirname, '..', '..',
➥ 'node_modules')));
const httpServer: http.Server = app.listen(PORT, () => {
if (process.env.NODE_ENV !== 'production') {
console.log(`Listening on http://localhost:${PORT}`);
}
});
const wsServer = new WebSocket.Server({ server: httpServer });
new BlockchainServer(wsServer);
The lines that start with app.use() map the URLs that come from the client to the
specific resources on the server. This was discussed in the “Resolving paths in Node.js”
sidebar in the previous section.
TIP The fragment '..', '..', 'public' will resolve to '../../public' in
Unix-based systems and to '..\..\public' on Windows.
While instantiating WebSocket.Server, we pass the instance of the existing HTTP
server to it. This allows us to run both HTTP and WebSocket servers on the same port.
Finally, we instantiate the BlockchainServer TypeScript class, which uses WebSockets.
Its code is located in the blockchain-server.ts script. The BlockchainServer class is a
subclass of MessageServer, which encapsulates all the work related to WebSocket
communications. We’ll review its code later in this section.
 The first part of the blockchain-server.ts script is shown in the following listing.
import * as WebSocket from 'ws';
import { Message, MessageTypes, UUID } from '../shared/messages';
import { MessageServer } from './message-server';
type Replies = Map<WebSocket, Message>;
export class BlockchainServer extends MessageServer<Message> {
private readonly receivedMessagesAwaitingResponse = new Map<UUID,
➥ WebSocket>();
Listing 10.12 server/main.ts: the script to start HTTP and WebSocket servers
Listing 10.13 The first part of server/blockchain-server.ts
Enables our script with
WebSocket support Imports the
BlockchainServer class
Instantiates Express Specifies the location of
the client’s code
Specifies the location
of node_modules used
by the client
Starts the HTTP server
Starts the
WebServer
Starts the blockchain notification server
Replies from the blockchain nodes
This class extends
MessageServer.
A collection
of clients’
messages
waiting for
responses
Reviewing notification workflows 249
private readonly sentMessagesAwaitingReply = new Map<UUID, Replies>();
➥ // Used as accumulator for replies from clients.
protected handleMessage(sender: WebSocket, message: Message): void {
switch (message.type) {
case MessageTypes.GetLongestChainRequest :
return this.handleGetLongestChainRequest(sender, message);
case MessageTypes.GetLongestChainResponse :
return this.handleGetLongestChainResponse(sender, message);
case MessageTypes.NewBlockRequest :
return this.handleAddTransactionsRequest(sender, message);
case MessageTypes.NewBlockAnnouncement :
return this.handleNewBlockAnnouncement(sender, message);
default : {
console.log(`Received message of unknown type:
➥ "${message.type}"`);
}
}
}
private handleGetLongestChainRequest(requestor: WebSocket, message:
➥ Message): void {
if (this.clientIsNotAlone) {
this.receivedMessagesAwaitingResponse.set(message.correlationId,
➥ requestor);
this.sentMessagesAwaitingReply.set(message.correlationId,
➥ new Map());
this.broadcastExcept(requestor, message);
} else {
this.replyTo(requestor, {
type: MessageTypes.GetLongestChainResponse,
correlationId: message.correlationId,
payload: []
});
}
}
The handleMessage() method serves as a dispatcher for messages received from the
clients. It has a switch statement to invoke the appropriate handler based on the
received message. For example, if one of the clients sends a GetLongestChainRequest
message, the handleGetLongestChainRequest() method is invoked. First it stores the
request (the reference to the open WebSocket object) in a map using the correlation
ID as a key. Then it broadcasts the message to other nodes, requesting their longest
chains. The handleGetLongestChainRequest() method can return an object with an
empty payload only if the blockchain has just one node. The handleMessage()
method was declared as abstract in the MessageServer superclass; we’ll review it
later in this section.
A handler for all message types
Invokes the
appropriate
handler based on
the message type
Stores the client’s request using
the correlation ID as a key
This map
accumulates
replies from
clients.
Broadcasts the message
to other nodes
There are no longest chains
in a single-node blockchain.
250 CHAPTER 10 Client-server communications using Node.js, TypeScript, and WebSockets
NOTE The method signature of handleMessage() includes the void keyword,
which means that it doesn’t return a value, so why does its body have a number of return statements? Typically, every case clause in a JavaScript switch
statement has to end with break, so the code doesn’t “fall through” and execute the code in the next case clause. Using return statements in every case
clause allows us to avoid these break statements and still guarantee that the
code doesn’t fall through.
Listing 10.14 shows the second part of blockchain-server.ts. It has the code that handles responses from other nodes sending their longest chains.
private handleGetLongestChainResponse(sender: WebSocket, message: Message):
➥ void {
if (this.receivedMessagesAwaitingResponse.has(message.correlationId)) {
const requestor =
➥ this.receivedMessagesAwaitingResponse.get(message.correlationId);
if (this.everyoneReplied(sender, message)) {
const allReplies =
➥ this.sentMessagesAwaitingReply.get(message.correlationId).values();
const longestChain =
➥ Array.from(allReplies).reduce(this.selectTheLongestChain);
this.replyTo(requestor, longestChain);
}
}
}
private handleAddTransactionsRequest(requestor: WebSocket, message:
➥ Message): void {
this.broadcastExcept(requestor, message);
}
private handleNewBlockAnnouncement(requestor: WebSocket, message:
➥ Message): void {
this.broadcastExcept(requestor, message);
}
private everyoneReplied(sender: WebSocket, message: Message): boolean {
const repliedClients = this.sentMessagesAwaitingReply
.get(message.correlationId)
.set(sender, message);
const awaitingForClients =
➥ Array.from(this.clients).filter(c => !repliedClients.has(c));
return awaitingForClients.length === 1;
}
Listing 10.14 The second part of server/blockchain-server.ts
Finds the client that
requested the longest chain
Gets the reference to the
client’s socket object
Finds the longest chain
Relays the longest
chain to the client
that requested it
Checks if every node
replied to the request
Have all nodes except the
original requestor replied?
Reviewing notification workflows 251
private selectTheLongestChain(currentlyLongest: Message,
current: Message, index: number) {
return index > 0 && current.payload.length >
➥ currentlyLongest.payload.length ?
current : currentlyLongest;
}
private get clientIsNotAlone(): boolean {
return this.clients.size > 1;
}
}
When nodes send their GetLongestChainResponse messages, the server uses the correlation ID to find the client who requested the longest chain. When all nodes have
replied, the handleGetLongestChainResponse() method turns the allReplies set
into an array and uses the reduce() method to find the longest chain. Then it sends
the response back to the requesting client using the replyTo() method.
 This is all good, but where’s the code that supports the WebSocket protocol and
defines such methods as replyTo() and broadcastExcept()? All this machinery is
located in the abstract MessageServer superclass, shown in listings 10.15 and 10.16.
import * as WebSocket from 'ws';
export abstract class MessageServer<T> {
constructor(private readonly wsServer: WebSocket.Server) {
this.wsServer.on('connection', this.subscribeToMessages);
this.wsServer.on('error', this.cleanupDeadClients);
}
protected abstract handleMessage(sender: WebSocket, message: T): void;
protected readonly subscribeToMessages = (ws: WebSocket): void => {
ws.on('message', (data: WebSocket.Data) => {
if (typeof data === 'string') {
this.handleMessage(ws, JSON.parse(data));
} else {
console.log('Received data of unsupported type.');
}
});
};
private readonly cleanupDeadClients = (): void => {
this.wsServer.clients.forEach(client => {
if (this.isDead(client)) {
this.wsServer.clients.delete(client);
}
});
};
Listing 10.15 The first part of server/message-server.ts
This method is used
while reducing the
array of longest
chains.
Checks if there is more than
one node in the blockchain
Subscribes to
messages from a newly
connected client
Cleans up
references to
disconnected
clients
This method is implemented in the BlockchainServer class.
The message from the
client has arrived. Passes
the
message
to the
handler
Purges the
disconnected clients
252 CHAPTER 10 Client-server communications using Node.js, TypeScript, and WebSockets
While reading the code of MessageServer, you’ll recognize many of the TypeScript
syntax elements covered in part 1 of this book. First of all, this class is declared as
abstract (see section 3.1.5):
export abstract class MessageServer<T>
You can’t instantiate an abstract class; you have to declare a subclass that will provide a
concrete implementation of all abstract members. In our case, the BlockchainServer
subclass implements the only abstract member, handleMessage(). Also, the class declaration uses the generic type T (see section 4.2), which is also used as an argument
type in the handleMessage(), broadcastExcept(), and replyTo() methods. In our
app, the concrete type Message (see listing 10.11) replaces the generic <T>, but in
other apps it could be a different type.
 By declaring the handleMessage() method as abstract, we state that any subclass
of MessageServer is free to implement this method any way it likes, as long as the signature of the method looks like this:
protected abstract handleMessage(sender: WebSocket, message: T): void;
Because we enforced this method signature in the abstract class, we know how the
handleMessage() method should be invoked when implemented. We do that in
subscribeToMessages() as follows:
this.handleMessage(ws, JSON.parse(data));
Strictly speaking, we can’t invoke an abstract method, but at runtime the this keyword
will refer to the instance of the BlockchainServer concrete class where the handleMessage method won’t be abstract any longer.
 The following listing shows the second part of the MessageServer class. These
methods implement broadcasting and replying to clients.
protected broadcastExcept(currentClient: WebSocket, message: Readonly<T>):
➥ void {
this.wsServer.clients.forEach(client => {
if (this.isAlive(client) && client !== currentClient) {
client.send(JSON.stringify(message));
}
});
}
protected replyTo(client: WebSocket, message: Readonly<T>): void {
client.send(JSON.stringify(message));
}
Listing 10.16 The second part of server/message-server.ts
Broadcasts to all other nodes
Sends a message
to a single node
Reviewing notification workflows 253
protected get clients(): Set<WebSocket> {
return this.wsServer.clients;
}
private isAlive(client: WebSocket): boolean {
return !this.isDead(client);
}
private isDead(client: WebSocket): boolean {
return (
client.readyState === WebSocket.CLOSING ||
client.readyState === WebSocket.CLOSED
);
}
}
In the last line of the server/main.ts script (see listing 10.12), we passed the instance
of the WebSocket server to the constructor of the BlockchainServer. This object has a
clients property, which is a collection of all active WebSocket clients. Whenever we
need to broadcast a message to all clients, we iterate through this collection as in the
broadcastExcept() method. If we need to remove a reference to a disconnected client, we also use the clients property in the cleanupDeadClients() method.
 The signatures of the broadcastExcept() and replyTo methods have an argument
of a Readonly<T> mapped type, which we covered in section 5.2. It takes a T type and
marks all of its properties as readonly. We use the Readonly to avoid accidental modifications being made to the value within the method. You can see more examples in the
sidebar titled “Examples of conditional and mapped types,” later in this chapter.
 Now let’s continue discussing the workflow started in figure 10.12 by the first client. The second client joins the blockchain and sends a message to the WebSocket
server requesting the longest chain, which is just the genesis block at the moment. Figure 10.13 shows the messages transferred over the WebSocket connections. We numbered the messages so it’s easier to understand their sequence:
1 The second client (on the right in the figure) connects to the messaging server,
requesting the longest chain. The server broadcasts this request to other clients.
2 The first client (on the left) receives this request. This client has a single genesis
block, which is its longest chain.
3 The first client (on the left) sends a message back with its longest chain in the
message payload.
4 The second client (on the right) receives the longest chain in the message
payload.
In this example, we have only two clients, but the WebSocket server broadcasts the
messages to all connected clients, so all of them would respond.
 Next, the first client (on the left in figure 10.14) creates two pending transactions,
as shown in figure 10.14. This is a local event and no messages are being sent to the
WebSocket server.
Checks if a particular
client is disconnected
254 CHAPTER 10 Client-server communications using Node.js, TypeScript, and WebSockets
Figure 10.13 The second client connected
2. Received longest
 chain request
3. Responded to
 longest chain
 request
1. Requesting Payload
 longest chain
4. Longest chain
 received
The very first client The second client
Figure 10.14 Adding transactions doesn’t send messages.
Reviewing notification workflows 255
Now the first client clicks on the GENERATE BLOCK button, starting the block mining and requesting other nodes to do the same. The NEW_BLOCK_REQUEST message is
this invitation to mining. Some time later, the mining is complete (in our case the first
miner finished first), and the first client announces the new candidate block by sending a NEW_BLOCK_ANNOUNCEMENT message. Figure 10.15 shows the messages related to
block mining, and the content of the NEW_BLOCK_ANNOUNCEMENT message.
Both clients now show the same two blocks: #0 and #1. Note that the hash value of
block #0 and the previousHash value of block #1 are the same.
 The following listing shows the content of the NEW_BLOCK_ANNOUNCEMENT message.
We shortened the hash values to improve readability.
correlationId: "2773c28b-aa55-e2ba-a6ec-75bb6b980d89"
payload: {previousHash: "00004441e92af1",…}
hash: "00001befe1b1e4df392e601..."
nonce: 51803
previousHash: "00004441e92a..."
timestamp: 1549207329217
transactions: [{sender: "Joe", recipient: "Mary", amount: 100},
{sender: "Alex", recipient: "Pete", amount: 300}]
type: "NEW_BLOCK_ANNOUNCEMENT"
Listing 10.17 A message containing the new block
An invite
to mine
Announcing a
candidate block a payload
A message withNode.js
debugger
Figure 10.15 New block messaging
The correlation ID (UUID)
The hash value of block #1
The calculated nonce (the proof of work)
The hash value of block #0
The block’s transactions
The message type
256 CHAPTER 10 Client-server communications using Node.js, TypeScript, and WebSockets
Revisit listing 10.11 to find the definitions of custom data types used in this message.
The next question is: Who sent the message with the payload containing the newly
mined block? The web client did, and in the next section we’ll review the relevant
code from the web client.
Debugging Node.js code in the browser
We started Node.js with the --inspect option, so when you open Chrome’s dev tools
you’ll see a green octagon to the left of the Elements tab (see figure 10.15). That
octagon represents the Node.js debugger—click it to open a detached window of the
Node.js dev tools.
Node.js connects with dev tools via port 9229, as you saw in listing 10.8 when we
started the server. By default, the Node.js window shows you content from the Connection tab. Switch to the Sources tab, as shown in the following figure, find the TypeScript code that you want to debug, and add a breakpoint. The following image shows
a breakpoint on line 9 in the blockchain-server.ts script.
Opening Chrome’s dev tools for Node.js
Reviewing notification workflows 257
We want to intercept the moment when the client sends a message to the server.
The following screenshot was taken after the client created an “Alice -> Julie -> $200”
transaction and clicked the GENERATE BLOCK button.
Selecting the Sources tab
The line with a breakpoint The Sources tab
The line with a breakpoint Watching the message object
Debugging Node.js in the Chrome browser
258 CHAPTER 10 Client-server communications using Node.js, TypeScript, and WebSockets
10.6.2 Reviewing the client’s code
We’ve been using the word “client” because in this app we’re discussing communications between a client web app and a server. But “node” would be the right word here.
Our node’s code is implemented as a web app, but in a real blockchain the nodes that
mine blocks and the UI where users can add transactions would be separate apps.
NOTE The main subject of this chapter is introducing the messaging server.
We’re not going to review each and every line of the code, but we will show
you the parts of the code that are crucial for understanding the client’s
implementation.
The code that runs in the web browser is located in the src/client directory. For HTML
rendering we use a small library called lit-html (www.npmjs.com/package/lit-html),
which allows you to write HTML templates with JavaScript template literals. This library
uses tagged templates, which are plain JavaScript functions combined with HTML (as
explained in section A.3.1 of the appendix). In short, this library can take a string with
HTML markup, create a browser DOM node, and re-render it when its data changes.
 The lit-html library efficiently renders the content from string templates to the
DOM by updating only those nodes that need to display updated values. Listing 10.18
is from the lit-html documentation (http://mng.bz/m4D4); it defines a tagged template with the ${name} expression, and then it passes “Steve” as a name to render: <div>
Hello Steve</div>. Then, when the value of the name variable changes to “Kevin,” it
updates only the name in that <div>.
import {html, render} from 'lit-html';
const helloTemplate = (name) => html`<div>Hello ${name}!</div>`;
render(helloTemplate('Steve'), document.body);
render(helloTemplate('Kevin'), document.body);
NOTE The main idea of lit-html is that it parses HTML and builds the DOM
node once, and after that it only renders new values for the embedded variables when they change. It doesn’t create a virtual DOM like React.js, and it
doesn’t use change detectors like Angular. It just updates the variable values.
Listing 10.18 How lit-html renders HTML
(continued)
The execution obediently stopped at line 9. We added the message variable to the
Watch panel at the top right, and we can debug or monitor this variable. All the features of Chrome’s dev tools’ debugger are available for the Node.js code.
Imports the functions html and render Declares the
tagged template
Renders the <div>
greeting Steve
Replaces the name Steve
with Kevin in the <div>
Reviewing notification workflows 259
When you’re reading the scripts in the client/ui directory, you won’t see HTML files
there, but rather invocations of the render() function similar to the preceding example. If you’re familiar with the Angular or React libraries, think of a class that has the
render() function as a UI component. The main API of lit-html is html, and you’ll see
that every render() function uses it.
 Let’s consider the small PendingTransactionsPanel class that renders the panel
with pending transactions. Listing 10.19 shows the ui/pending-transactions-panel.ts
file that implements the render() function.
import { html, TemplateResult }
➥ from '../../../node_modules/lit-html/lit-html.js';
import { BlockchainNode } from '../lib/blockchain-node.js';
import { Callback, formatTransactions, Renderable, UI } from './common.js';
export class PendingTransactionsPanel implements
➥ Renderable<Readonly<BlockchainNode>> {
constructor(readonly requestRendering: Callback) {}
render(node: Readonly<BlockchainNode>): TemplateResult {
const shouldDisableGenerate = node.noPendingTransactions || node.isMining;
const formattedTransactions = node.hasPendingTransactions
? formatTransactions(node.pendingTransactions)
: 'No pending transactions yet.';
return html`
<h2>Pending transactions</h2>
<pre class="pending-transactions__list">${formattedTransactions}</pre>
<div class="pending-transactions__form">${UI.button('GENERATE BLOCK',
➥ shouldDisableGenerate)}</form>
<div class="clear"></div>
`;
}
}
The PendingTransactionsPanel class implements the Renderable interface, which
requires it to have the requestRendering property and the render() method, both
defined in the common.ts file as follows.
// other type definition go here
export type Callback = () => void;
export interface Renderable<T> {
requestRendering: Callback;
render(data: T): TemplateResult;
}
Listing 10.19 pending-transaction-panel.ts: the Pending Transactions panel
Listing 10.20 A fragment from ui/common.ts
Provides the
callback function
to the constructor
The argument type of the
render() function is defined
in blockchain-node.ts.
lit-html
needs the
html
tagged
template.
Defines the custom type
of a callback function
Defines a generic interface
A callback function
The function to render
HTML using lit-html
260 CHAPTER 10 Client-server communications using Node.js, TypeScript, and WebSockets
You’ll find multiple render() functions in the client’s code, but all of them work in a
similar way: they insert the values of JavaScript variables into HTML templates and
refresh the corresponding portion of the UI when the values in these variables
change.
 The client’s top-level class is called Application, and it also implements the
Renderable interface, so lit-html knows how to render it. The client/main.ts file
creates an instance of the Application class and passes a callback to it that invokes its
render() function.
import { render } from '../../node_modules/lit-html/lit-html.js';
import { Application } from './ui/application.js';
let renderingIsInProgress = false;
let application = new Application(async () => {
if (!renderingIsInProgress) {
renderingIsInProgress = true;
await 0;
renderingIsInProgress = false;
render(application.render(), document.body);
}
});
TIP Even though the render() function takes document.body as an argument, the lit-html library doesn’t re-render the entire page—only the values
in the templates’ placeholders that have changed.
When the Application class is instantiated, it receives the callback function (the constructor’s argument is called requestRendering), which invokes the render() function. We’ll show you two fragments of the Application class, and where you see
this.requestRendering(), it invokes this callback. The first fragment of the application.ts script follows.
export class Application implements Renderable<void> {
private readonly node: BlockchainNode;
private readonly server: WebsocketController;
private readonly transactionForm = new
➥ TransactionForm(this.requestRendering);
private readonly pendingTransactionsPanel =
➥ new PendingTransactionsPanel(this.requestRendering);
private readonly blocksPanel = new BlocksPanel(this.requestRendering);
constructor(readonly requestRendering: Callback) {
Listing 10.21 ui/main.ts: the script that creates the top-level component
Listing 10.22 The first fragment of ui/application.ts
A flag to prevent
double rendering
Passes a callback to the
Application instance
Invokes the render() function
This object is responsible
for the WebSocket
communications.
Passes the top-level callback
to each UI component
The callback reference
will be stored in the
requestRendering property.
Reviewing notification workflows 261
this.server = new WebsocketController(this.handleServerMessages);
this.node = new BlockchainNode();
this.requestRendering();
this.initializeBlockchain();
}
private async initializeBlockchain() {
const blocks = await this.server.requestLongestChain();
if (blocks.length > 0) {
this.node.initializeWith(blocks);
} else {
await this.node.initializeWithGenesisBlock();
}
this.requestRendering();
}
render(): TemplateResult {
return html`
<main>
<h1>Blockchain node</h1>
<aside>${this.statusLine}</aside>
<section>${this.transactionForm.render(this.node)}</section>
<section>
<form @submit="${this.generateBlock}">
${this.pendingTransactionsPanel.render(this.node)}
</form>
</section>
<section>${this.blocksPanel.render(this.node.chain)}</section>
</main>
`;
}
The initial rendering is initiated from the constructor by invoking this
.requestRendering(). After several seconds, the second rendering is done from the
initializeBlockchain() method. Child UI components get the reference to the
requestRendering callback, and they can decide when to refresh the UI.
 The initializeBlockchain() method is invoked after the initial rendering of the
UI, and this method requests the longest chain from the WebSocket server. If this
node is not the first and only one, the longest chain is returned and is rendered in the
block panel at the bottom of the screen. Otherwise the genesis block is generated and
rendered. Both requestLongestChain() and initializeWithGenesisBlock() are
asynchronous operations, and the code waits for their completion by using the await
JavaScript keyword.
 Listing 10.23 shows two methods from application.ts. The handleServerMessages() method is invoked when the WebSocket server sends a message to the client. Using the switch statement, it invokes the appropriate handler based on the
message type. The handleGetLongestChainRequest() method is invoked when the
client receives a request to send its longest chain.
Connects
to the
WebSocket
server
All blockchain and node
creation logic is here.
Initializes the blockchain
Requests the
longest chain
from all nodes
Renders the UI components
Re-renders the child component
262 CHAPTER 10 Client-server communications using Node.js, TypeScript, and WebSockets
private readonly handleServerMessages = (message: Message) => {
switch (message.type) {
case MessageTypes.GetLongestChainRequest: return
this.handleGetLongestChainRequest(message);
case MessageTypes.NewBlockRequest :
➥ return this.handleNewBlockRequest(message);
case MessageTypes.NewBlockAnnouncement : return
➥ this.handleNewBlockAnnouncement(message);
default: {
console.log(`Received message of unknown type: "${message.type}"`);
}
}
}
private handleGetLongestChainRequest(message: Message): void {
this.server.send({
type: MessageTypes.GetLongestChainResponse,
correlationId: message.correlationId,
payload: this.node.chain
});
Listing 10.23 The second fragment of ui/application.ts
Handles messages from the WebSocket server Dispatches the
message to the
right handler
The node sends its own
chain to the server.
Examples of conditional and mapped types
In chapter 5, we introduced conditional and mapped types. Here, you’ll see how we
use them in the blockchain app. We’re not going to review the entire client/lib/blockchain-node.ts script; you can read up on generics and the mapped type Pick in the
TypeScript documentation at http://mng.bz/5AJa.
Prior to version 3.5, TypeScript didn’t include the Omit type, and we had to declare
our own custom Omit type. Recently, Omit became a built-in utility type, but we
decided to keep this sidebar’s code sample as-is for illustration purposes. Let’s discuss the Omit, WithoutHash, and NotMinedBlock custom types presented in the
following code.
export interface Block {
readonly hash: string;
readonly nonce: number;
readonly previousHash: string;
readonly timestamp: number;
readonly transactions: Transaction[];
}
export type Omit<T, K> = Pick<T, Exclude<keyof T, K>>;
export type WithoutHash<T> = Omit<T, 'hash'>;
export type NotMinedBlock = Omit<Block, 'hash' | 'nonce'>;
Declares the Block type
A helper type
that uses Pick
Declares a
type similar
to Block but
without a
hash
Declares a type similar to Block
but without a hash and nonce
Reviewing notification workflows 263
The Omit type in the following line allows us to declare a type that will have all the
properties of type T except hash:
type WithoutHash<T> = Omit<T, 'hash'>;
The process of generating the block’s hash value takes time, and until we have it, we
can use the WithoutHash<Block> type, which won’t have the hash property.
You can specify more than one property to be excluded:
type NotMinedBlock = Omit<Block, 'hash' | 'nonce'>;
You could use this type as follows:
let myBlock: NotMinedBlock;
myBlock = {
previousHash: '123',
transactions: ["Mary paid Pete $100"]
};
Our NotMinedBlock type will always be Block minus hash and nonce. If at some
point, someone adds another required readonly property to the Block interface, the
assignment to the variable myBlock won’t compile, complaining that the newly added
property must be initialized. In JavaScript, we’d get a runtime error in the same
scenario.
The interface defines a custom Block type with five properties, and all of them are
required and readonly, meaning they can be initialized only during the instantiation
of the block. But creating a block takes some time, and not all the values for these
properties are available during instantiation. We’d like to benefit from strong typing,
but we’d also like to have the freedom to initialize some properties after the Block
instance is created.
TypeScript comes with a Pick mapped type and a conditional Exclude type, and we
can use them to define a new type that excludes some of the properties of existing
ones. Exclude enumerates the remaining properties. The Pick type allows you to
create a type from the provided list of properties.
The following line means that we want to declare a generic type, Omit, that can take
a type T and a key K, and exclude from T the properties that match K:
type Omit<T, K> = Pick<T, Exclude<keyof T, K>>;
The generic Omit type can be used with any type T, and keyof T returns a list of
properties of the concrete type. For example, if we provide Block as type T, the
keyof T construct will represent the list of properties defined in the Block interface.
Using Exclude<keyof T, K> allows us to remove some of the properties from the
list, and Pick will create a new type from that new list of properties.
264 CHAPTER 10 Client-server communications using Node.js, TypeScript, and WebSockets
Listings 10.22 and 10.23 show most of the application.ts file, where the client communicates with the WebSocket server via the WebsocketController class, which gets a
callback to handle messages via its constructor. Let’s get familiar with the code of the
WebsocketController class by reviewing the workflow when the user clicks on the
GENERATE BLOCK button. That should result in the following actions:
1 Broadcast the GET_LONGEST_CHAIN_REQUEST message to other nodes via the
WebSocket server.
2 Broadcast the NEW_BLOCK_REQUEST message to other nodes via the WebSocket
server.
3 Mine the block.
4 Process all GET_LONGEST_CHAIN_RESPONSE messages received from other nodes.
5 Broadcast the NEW_BLOCK_ANNOUNCEMENT message to other nodes, and save the
candidate block locally.
In listing 10.22, the render() method contains a form that looks like this:
<form @submit="${this.generateBlock}">
${this.pendingTransactionsPanel.render(this.node)}
</form>
The UI element with pending transactions and the GENERATE BLOCK button are implemented in the PendingTransactionsPanel class. We use the @submit directive from
lit-html, and when the user clicks on the GENERATE BLOCK button, the generateBlock() async method is invoked.
private readonly generateBlock = async (event: Event): Promise<void> => {
event.preventDefault();
this.server.requestNewBlock(this.node.pendingTransactions);
const miningProcessIsDone =
➥ this.node.mineBlockWith(this.node.pendingTransactions);
this.requestRendering();
const newBlock = await miningProcessIsDone;
this.addBlock(newBlock);
};
When we announce that we’re starting block mining, we provide a list of pending
transactions, this.node.pendingTransactions, so other nodes can compete and try
to mine the block for the same transactions faster. Then this node starts mining as
well.
Listing 10.24 The generateBlock() method of the Application class
Prevents a page refresh Lets all other nodes know that this
one has started mining
Starts the
block mining Refreshes the status on the UI
Waits for the mining
to complete
Adds the block to the local blockchain
Reviewing notification workflows 265
Now let’s review the code related to WebSocket communications. If you read the code
of the addBlock() method, you’ll see the following line there:
this.server.announceNewBlock(block);
This is how the node asks the WebSocket server to announce the fresh-from-the-oven
block candidate. The Application class also has a handleServerMessages() method,
which handles messages arriving from the server. It’s implemented in the client/
lib/websocket-controller.ts file, which declares the PromiseExecutor interface and
WebsocketController class.
 The Application class creates an instance of WebsocketController, which is our
only contact for all communications with the server. When the client needs to send a
message to the server, it’ll use such methods as send() or requestLongestChain(),
but sometimes the server will be sending messages to the clients. That’s why we pass a
callback method to the constructor of WebsocketController.
When the new block is rejected
The Application.addBlock() method invokes the addBlock() method in the
BlockchainNode class. We already discussed the process of block mining, in chapters 8 and 9, but we’d like to highlight the code that rejects an attempt to add a new
block:
const previousBlockIndex = this._chain.findIndex(b => b.hash ===
➥ newBlock.previousHash);
if (previousBlockIndex < 0) {
throw new Error(`${errorMessagePrefix} - there is no block in the
➥ chain with the specified previous hash "${newBlock.previousHash
➥ .substr(0, 8)}".`);
}
const tail = this._chain.slice(previousBlockIndex + 1);
if (tail.length >= 1) {
throw new Error(`${errorMessagePrefix} - the longer tail of the
➥ current node takes precedence over the new block.`);
}
This code is related to the failed attempt of adding a block, shown at the bottom-right
corner of figure 10.10. First we check whether the new block’s value of previousHash even exists in the blockchain. It may exist, but not in the last block.
The second if statement checks if there is at least one block after the one that contains this previousHash. This would mean that at least one new block has already
been added to the chain (received from other nodes). In this case, the longest chain
takes precedence and the newly generated block is rejected.
Does the new block contain
an existing previousHash?
Does the chain already have
at least one extra block?
266 CHAPTER 10 Client-server communications using Node.js, TypeScript, and WebSockets
export class WebsocketController {
private websocket: Promise<WebSocket>;
private readonly messagesAwaitingReply = new Map<UUID,
➥ PromiseExecutor<Message>>();
constructor(private readonly messagesCallback: (messages: Message) =>
➥ void) {
this.websocket = this.connect();
}
private connect(): Promise<WebSocket> {
return new Promise((resolve, reject) => {
const ws = new WebSocket(this.url);
ws.addEventListener('open', () => resolve(ws));
ws.addEventListener('error', err => reject(err));
ws.addEventListener('message', this.onMessageReceived);
});
}
private readonly onMessageReceived = (event: MessageEvent) => {
const message = JSON.parse(event.data) as Message;
if (this.messagesAwaitingReply.has(message.correlationId)) {
this.messagesAwaitingReply.get(message.correlationId).resolve(message);
this.messagesAwaitingReply.delete(message.correlationId);
} else {
this.messagesCallback(message);
}
}
async send(message: Partial<Message>, awaitForReply: boolean = false):
➥ Promise<Message> {
return new Promise<Message>(async (resolve, reject) => {
if (awaitForReply) {
this.messagesAwaitingReply.set(message.correlationId, { resolve,
➥ reject });
}
this.websocket.then(
ws => ws.send(JSON.stringify(message)),
() => this.messagesAwaitingReply.delete(message.correlationId)
);
});
}
TIP The Partial type is covered in chapter 5.
The connect() method connects to the server and subscribes to the standard WebSocket messages. These actions are wrapped in a Promise, so if this method returns,
we can be sure that the WebSocket connection is established and all handlers are
Listing 10.25 websocket-controller.ts: the WebsocketController class
A map of WebSocket clients
waiting for responses
Passes the callback
to the constructor
Connects to the WebServer
Assigns
callbacks to
WebSocket
messages
Handles
incoming
messages
Stores messages that
need a response
Reviewing notification workflows 267
assigned. An additional benefit is that now we can use the async and await keywords
with these asynchronous functions.
 The onMessageReceived() method handles messages coming from the server—
it’s essentially the message router. In that method, we deserialize the message and
check its correlation ID. If the incoming message is a response to another message,
the following code will return true:
messagesAwaitingReply.has(message.correlationId)
Every time the client sends a message out, we store its correlation ID mapped to the
PromiseExecutor in messagesAwaitingReply. The PromiseExecutor knows which client waits for the response.
interface PromiseExecutor<T> {
resolve: (value?: T | PromiseLike<T>) => void;
reject: (reason?: any) => void;
}
To construct a Promise, the PromiseConstructor interface (see the declaration in
lib.es2015.promise.d.ts) uses the PromiseLike type. TypeScript has a number of types
that end with “Like” (such as ArrayLike), and they define subtypes that have fewer
properties than the original type. Promises may have various constructor signatures; for
example, all of them have then(), but they may or may not have catch(). PromiseLike
tells tsc, “I don’t know what the implementation of this thing is, but at least it’s thenable.”
 When the client sends a message that requires a response, it uses the send()
method shown in listing 10.25, which stores a reference to the client’s messages using
the correlation ID and an object of type PromiseExecutor:
this.messagesAwaitingReply.set(message.correlationId, { resolve, reject });
The response is asynchronous, and we don’t know when it will arrive. In such scenarios,
a JavaScript caller can create a Promise, providing the resolve and reject callbacks as
explained in section A.10.2 of the appendix. That’s why the send() method wraps the
code sending a message in a Promise, and we store the references to the object containing
resolve and reject along with the correlation ID in messagesAwaitingReply.
 For messages expecting replies, we need a way to push the reply when it arrives. We
could use a Promise, which could be resolved (or rejected) using the callbacks that we
pass to the constructor as Promise(resolve, reject). To preserve the references to this
pair of resolve/reject functions until the moment we get a reply, we create a PromiseExecutor object that has exactly these two properties, resolve and reject, and puts
them aside. In other words, PromiseExecutor is just a container for two callbacks.
Listing 10.26 The PromiseExecutor interface from websocket-controller.ts
This type is used by tsc internally
to initialize the promise.
Enforces the signature
of the resolve() method
Enforces the signature
of the reject() method
268 CHAPTER 10 Client-server communications using Node.js, TypeScript, and WebSockets
 The PromiseExecutor interface just describes the type of object that we’re storing
in the messagesAwaitingReply map. When the response arrives, the onMessageReceived() method finds the PromiseExecutor object by the correlation ID, it invokes
resolve(), and it deletes this message from the map:
this.messagesAwaitingReply.get(message.correlationId).resolve(message);
this.messagesAwaitingReply.delete(message.correlationId);
Now that you understand how the send() method works, you should be able to understand the code that invokes it. The following listing shows how the client can request
the longest chain.
async requestLongestChain(): Promise<Block[]> {
const reply = await this.send(
{
type: MessageTypes.GetLongestChainRequest,
correlationId: uuid()
}, true);
return reply.payload;
}
The WebsocketController class has several other methods that deal with other types of
messages. They are implemented much like requestLongestChain(). Look at the client/
lib/websocket-controller.ts file to see the complete code of WebsocketController.
 To see this app in action, run npm install and then npm start. Open a couple of
browser windows and try to mine some blocks.
Summary
 In a blockchain, multiple nodes may be mining a block containing the same
transaction. The longest chain rule can help to find the winning node and
reach a consensus amongst nodes.
 If you want to develop an app where the client and the server portions are written in TypeScript, create two separate node-based projects. Each project will
have its own package.json file and all the required configuration scripts.
 If you need to arrange client-server communication where each party can initiate the data exchange, consider using the WebSocket protocol. Whereas HTTP
is a request-based protocol, WebSocket is not, which makes it a good choice
when a server needs to push data to a client.
 Node.js and most other technologies used in the backend support the WebSocket protocol, and you can implement a Node.js server in TypeScript.
Listing 10.27 Requesting the longest chain in websocket-controller.ts
Invokes the send() method and
waits for the response
The first argument
is a Message object.
true means
“waiting for
a reply.” Returns the payload
from the response
269

Developing React.js
 apps with TypeScript
The React.js library (a.k.a. React) was created by a Facebook engineer, Jordan
Walke, in 2013, and today it has 1,300 contributors and 140,000 stars on GitHub!
According to the Stack Overflow Developer Survey of 2019, it’s the second-most
popular JavaScript library (jQuery remains the most broadly used library). React is
not a framework but a library responsible for rendering views in the browser (think
of the letter V in the MVC design pattern). In this chapter, we’ll show you how to
start developing web apps in React using TypeScript.
 The main players in React are components, and the UI of a web app consists of
components having parent-child relations. But where Angular takes control of the
entire root element of the web page, React allows you to control a smaller page element (such as a <div>) even if the rest of the page is implemented with another
framework or in pure JavaScript.
This chapter covers
 A quick intro to the React.js library
 How React components use props and state
 How React components communicate with
each other
316 CHAPTER 13 Developing React.js apps with TypeScript
 You can develop React apps either in JavaScript or in TypeScript and deploy them
using tools like Babel and Webpack (described in chapter 6). Without further ado,
let’s start by writing the simplest version of the Hello World app using React and JavaScript; we’ll switch to TypeScript in section 13.2.
13.1 Developing the simplest web page with React
In this section, we’ll show you two versions of a simple web page written with React
and JavaScript. Each of these pages renders “Hello World,” but the first version will
use React with no additional tooling; the second version will engage Babel.
 In real-world apps, a React app is a project with configured dependencies, tools,
and a build process, but to keep things simple, our first web page will just have a single
HTML file that loads the React library from CDN. This version of the Hello World
page is located in the hello-world-simplest/index.html file, and its content is shown in
the following listing.
<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8">
<script
➥ crossorigin src="https://unpkg.com/react@16/umd/react.development.js">
</script>
<script
➥ crossorigin src="https://unpkg.com/react-dom@16/umd/
➥ react-dom.development.js">
</script>
</head>
<body>
<div id="root"></div>
<script >
const element = React.createElement('h1',
null,
'Hello World');
ReactDOM.render(element,
document.getElementById('root'));
</script>
</body>
</html>
The processes of declaring the page content (React.createElement()) and rendering it to the browser’s DOM (ReactDOM.render()) are decoupled. The former is supported by the React object’s API, whereas the latter is done by ReactDOM. That’s why
we loaded those two packages in the <head> section of the page.
Listing 13.1 hello-world-simplest/index.html: the Hello World app
Loads the React package from CDN
Loads the ReactDOM
package from CDN
Adds the <div>
with the id “root”
Creates the <h1> element
using the createElement
function
We don’t
 pass any data
(a props object)
to the <h1>
element. The text of
the <h1>
element
Renders the <h1>
inside the <div>
Developing the simplest web page with React 317
 In React, UI elements are represented as a tree of components that always has a
single root element. This web page has a <div> with the ID root that serves as such
an element for the content rendered by React. In the script in listing 13.1, we
prepare the element to be rendered using React.createElement(), and then we
invoke ReactDOM.render(), which finds the element with the root ID and renders it
in this element.
TIP In Chrome, right-click on the Hello World web page and select the
Inspect menu option. It will open the dev tools showing the <div> with the
<h1> element inside.
The createElement() method has three arguments: the name of the HTML element,
its props (immutable data to be passed to the element), and content. In this case, we
didn’t need to provide any props (think attributes) and used null here; we’ll explain
what props are for in section 13.4.3. The content of h1 is “Hello World,” but it can
contain child elements (such as a ul with nested li elements), which could be created
with the nested createElement() calls.
 Open the index.html file in your browser,
and it will render the text “Hello World” as
shown in figure 13.1.
 Invoking createElement() on a page that
has only one element is fine, but for a page that
has dozens of elements, this would be tedious
and annoying. React allows you to embed the
UI markup into JavaScript code, which looks like HTML but is JSX, as we’ll discuss in
the “JSX and TSX” sidebar a little later in this chapter.
 Let’s see what our Hello World page could look like if we used JSX. Note the line
const myElement = <h1>Hello World</h1> in the following listing—we use it instead
of invoking createElement().
<!DOCTYPE html>
<head>
<meta charset="utf-8">
<script
src="https://unpkg.com/react@16/umd/react.development.js"></script>
<script
src="https://unpkg.com/react-dom@16/umd/
➥ react-dom.development.js"></script>
<script src="https://unpkg.com/babel-standalone/
➥ babel.min.js"></script>
</head>
<body>
<div id="root"></div>
<script type="text/babel">
Listing 13.2 index_jsx.html: a JSX version of Hello World
Adds Babel from CDN
The type of the
script is text/babel.
Figure 13.1 Rendering the hello-worldsimplest/index.html file
318 CHAPTER 13 Developing React.js apps with TypeScript
const myElement = <h1>Hello World</h1>;
ReactDOM.render(
myElement,
document.getElementById('root')
);
console.log(myElement);
</script>
</body>
</html>
This app renders the same page you saw in figure 13.1, but it’s written differently. The
JavaScript code has an embedded <h1>Hello World!</h1> string that looks like
HTML, but it’s actually JSX. Browsers can’t parse this, so we need a tool to turn JSX
into a valid JavaScript. Babel to the rescue!
 The <head> section in listing 13.2 has an additional <script> tag that loads Babel from
a CDN. Also, we changed the type of the script to text/babel, which makes browsers
ignore it but tells Babel to transform the content of this <script> tag into JavaScript.
NOTE In a real-world project we wouldn’t use CDN to add Babel to a Nodebased project (as we did in listing 13.2), but it suffices for demo purposes. In
Node-based apps, Babel would be installed locally in the project, and it would
be a part of the build process.
Figure 13.2 shows a screenshot with the browser’s console open. Babel converted the
JSX value to a JavaScript object that was rendered inside the <div>, and we printed
this object in the console.
Assigns a JSX value
to a variable
Initiates the
rendering of
myElement
 to the <div>
Monitors the JavaScript
object that was rendered
The object
type is h1
The props
object
Figure 13.2 The rendered JavaScript object
Generating and running a new app with Create React App 319
Now that you have a fair understanding of how very basic pages use React, we’ll switch
to Node-based projects and component-based apps. Let’s see some tooling that React
developers use in the real world.
13.2 Generating and running a new app with Create React App
If you want to create a React app that includes a transpiler and a bundler, you’ll need to
add configuration files to your app. This process is automated by the command-line interface (CLI) called Create React App (see www.npmjs.com/package/create-react-app).
This tool generates all the required configuration files for Babel and Webpack, so you can
concentrate on writing your app instead of wasting time configuring the tooling. To
install the create-react-app package globally on your computer, run the following command in the terminal window:
npm install create-react-app -g
Now you can generate either a JavaScript or a TypeScript version of the app. To generate the TypeScript app, run the command create-react-app followed by the app
name and the --typescript option:
create-react-app hello-world --typescript
In a minute or so, all required files will be generated in the hello-world directory, and
the project dependencies will be installed. In particular, it installs the following React
packages:
 react—A JavaScript library for creating user interfaces
 react-dom—The React package for working with the DOM
 react-scripts—Scripts and configurations used by Create React App; for TypeScript support, you need react-scripts version 2.1 or higher
Besides the preceding packages, the CLI installs Webpack, Babel, TypeScript, their type
definition files, and other dependencies.
 To launch the generated web app, switch
to the hello-world directory and run npm
start, which in turn runs react-scripts
start. Webpack will bundle the app, and
webpack-dev-server will serve the app on
localhost:3000, as shown in figure 13.3. This
functionality is provided by the Webpack
DevServer.
TIP For bundling, Webpack uses the
config options from the webpack.config
.js file located in the node_modules/
react-scripts/config directory. Figure 13.3 Running the hello-world app
320 CHAPTER 13 Developing React.js apps with TypeScript
The UI of the generated app tells us to
edit the src/App.tsx file, which is the
main TypeScript file of the generated
app. Open the directory in VS Code,
and you’ll see the project files as shown
in figure 13.4.
 The source code of your app is
located in the src directory, and the public directory is for assets of your app that
shouldn’t be included in the app bundles. For example, your app has thousands of images and needs to
dynamically reference their paths—they
go in the public directory, along with
other files that don’t require any processing before deployment.
 The index.html file contains a <div
id="root"></div> element, which serves
as a container for the generated React
app. You won’t find any <script> tags for
loading the React library code there;
they’ll be added during the build process
when the app’s bundles are ready.
TIP Run the app and open the Chrome dev tools under the Elements tab to
see the runtime content of index.html.
NOTE The serviceWorker.ts file is generated just in case you want to develop
a progressive web app (PWA) that can be started offline using cached assets.
We won’t use it in our sample apps.
As you can see, some of the files have an unusual extension: .tsx. If we were writing the
code in JavaScript, the CLI would generate the app file with the extension .jsx (not
.tsx). JSX and TSX are explained in the “JSX and TSX” sidebar.
JSX and TSX
The draft of the JSX specification (https://facebook.github.io/jsx) offers the following
definition: “JSX is an XML-like syntax extension to ECMAScript without any defined
semantics. It’s NOT intended to be implemented by engines or browsers.”
JSX stands for JavaScript XML. It defines a set of XML tags that can be embedded
inside JavaScript code. These tags can be parsed and turned into regular HTML tags
for rendering by the browser, and React includes such a parser. In chapter 6, we
demonstrated Babel’s REPL (https://babeljs.io/repl), and the following figure shows
a screenshot of this REPL with some sample JSX.
The landing page
The unit
test code
The App
component
The app’s
entry point
Figure 13.4 The generated files and directories
Generating and running a new app with Create React App 321
A React component can be declared either as a function or a class. A functional
(function-based) component is implemented as a function and is structured as follows
(types are omitted).
On the left, we selected the React preset and pasted some sample code from the
JSX spec. This preset specifies that we want to turn each JSX tag into a React
.createElement() invocation. The sample code should render a dropdown menu
containing three items. On the right, you can see how the JSX was parsed into
JavaScript.
Every React app has at least one component, the root component, and our generated
app has only the root component App. The file with the App function code has the
extension .tsx, which tells the Typescript compiler that it contains JSX. But just having the .tsx extension is not enough for tsc to handle it: you need to enable JSX by
adding the jsx compiler option. Open the tsconfig.json file, and you’ll find the following line:
"jsx": "preserve"
The jsx option only affects the emit stage—type checking is unaffected. The preserve value tells tsc to copy the JSX portion into the output file, changing its extension to .jsx, because there will be another process (such as Babel) parsing it. If that
value was react, tsc would turn the JSX tags to React.createElement() invocations, as seen in the previous image on the right.
Parsing JSX in Babel
Turning each JSX tag
into createElement()
Select the
react preset
322 CHAPTER 13 Developing React.js apps with TypeScript
const MyComponent = (props) => {
return (
<div>...</div>
)
// other functions may go here
}
export default MyComponent;
Developers who prefer working with classes can create class-based components, which
are implemented as subclasses of React.Component. They are structured like the following example.
class MyComponent extends React.Component {
render() {
return (
<div>...</div>
);
}
// other methods may go here
}
export default MyComponent;
A functional component simply returns JSX, but a class-based component has to
include the render() method, which returns JSX. We prefer using functional components, which have several benefits over class-based ones:
 A function requires less code to write, and there is no need to inherit the component’s code from any class.
 A functional component generates less code during Babel compiling, and code
minifiers better infer unused code and can shorten variables names more
aggressively since all of them are local to the function, unlike class members,
which are considered a public API and cannot be renamed.
 Functional components have no need for the this reference.
 Functions are easier to test than classes; assertions simply map props to the
returned JSX.
NOTE You should use class-based components only if you have to use a React
version older than 16.8. In those older versions, only class-based components
will support state and life cycle methods.
Listing 13.3 A functional component
Listing 13.4 A class-based component
props is used to pass
data to the components.
Returns the
component’s JSX
The class must inherit
from React.Component.
The render() method
is invoked by React.
Returns JSX for rendering
Managing a component’s state 323
If you use the current version of Create React App with the --typescript option, the
generated App.tsx file file will already include the boilerplate code for a functional
component (a function of type React.FC), as shown in the following listing.
import React from 'react';
import logo from './logo.svg';
import './App.css';
const App: React.FC = () => {
return (
<div className="App">
<header className="App-header">
<img src={logo} className="App-logo" alt="logo" />
<p>
Edit <code>src/App.tsx</code> and save to reload.
</p>
<a
className="App-link"
href="https://reactjs.org"
target="_blank"
rel="noopener noreferrer"
>
Learn React
</a>
</header>
</div>
);
}
export default App;
NOTE We used version 3.0 of Create React App. Older versions of this tool
would generate a class-based App component.
The generated App function returns the markup (or template) that React uses to render this component’s UI, shown earlier in figure 13.3. During the build process, Babel
will convert the markup into a pure JavaScript object, JSX.element, with a <div> container that will update the Virtual DOM and the browser’s DOM (we’ll discuss Virtual
DOM in section 13.5). This App component didn’t have a separate place for storing its
data (its state), so we’ll add that in the next section.
13.3 Managing a component’s state
A component’s state is a datastore that contains data that should be rendered by the
component. The data in the component’s state is preserved even if React re-renders
the component. If you have a Search component, its state could store the last search
criteria and the last search result. Whenever the code updates the component’s state,
Listing 13.5 The App.tsx file
Imports the React library
Specifies a functional
component
Returns the
component’s
template as a
JSX expression
(it’s not a string)
In JSX, use className instead of the
“class” CSS selector to avoid conflicts
with the JavaScript “class” keyword.
Exports the App component
declaration so it can be used
in other modules
324 CHAPTER 13 Developing React.js apps with TypeScript
React updates the component’s UI to reflect changes caused by the user’s actions
(such as button clicks or typing in input fields) or other events.
NOTE Do not confuse an individual component’s state with the application’s
state. The application state stores data that may come from multiple components, functions, or classes.
How can you define and update the component’s state? This depends on how the
component was created in the first place. We’re going to move back to class-based
components for a minute so that you can understand the difference in dealing with
state in class-based and functional components. Then we’ll return to functional components, which we recommend using.
13.3.1 Adding state to a class-based component
If you have to work with a class-based component, you can define a type representing
the state, create and initialize an object of this type, and then update it as needed by
invoking this.setState(…).
 Let’s consider a simple class-based component that has a state object with two
properties: the user name and the image to be displayed. To serve images, we’ll use
the Lorem Picsum website, which returns random images of a specified size. For
example, if you enter the URL https://picsum.photos/600/150, the browser will
show a random image with a width of 600 px and a height of 150 px. The following
listing shows such a class-based component with a two-property state object.
interface State {
userName: string;
imageUrl: string;
}
+
export default class App extends Component {
state: State = { userName: 'John',
imageUrl: 'https://picsum.photos/600/150' };
render() {
return (
<div>
<h1>{this.state.userName}</h1>
<img src={this.state.imageUrl} alt=""/>
</div
);
}
}
Based on the code of the render() method, you can probably guess that this component will render “John” and an image. Note that we embedded the values of the state
properties into JSX by placing them inside curly braces: {this.state.userName}.
Listing 13.6 A class-based component with state
Defines the type for the
component’s state
Initializes the State object
Renders the userName here
Renders the imageUrl here
Managing a component’s state 325
 Any class-based component is inherited from the Component class, which has a
state property and the setState() method. If you need to change the value of any
state property, you must do it using this method:
this.setState({userName: "Mary"});
By invoking setState(), you let React know that a UI update may be required. If you
update the state directly (such as with this.state.userName='Mary'), React won’t
call the render() method to update the UI. As you might have guessed, the state
property is declared on the base class Component.
 In section 13.2, we listed the benefits of functional components over class-based
ones, and we won’t use class-based components any longer. In functional components,
we manage state by using hooks, which were introduced in React 16.8.
13.3.2 Using hooks to manage state in functional components
In general, hooks allow you to “attach” behavior to a functional component without
the need to write classes, create wrappers, or use inheritance. It’s as if you said to a
functional component, “I want you to have additional functionality while remaining a
plain old function.”
 The names of hooks must start with the word “use”—this is how Babel detects them
and distinguishes them from regular functions. For example, useState() is the hook
for managing the component’s state, and useEffect() is used to add a side-effect
behavior (such as fetching data from a server). In this section, we’ll focus on the
useState() hook, using the same example as in the previous section: a component
whose state is represented by a user name and an image URL, but this time it’ll be a
functional component.
 The useState() hook can create a primitive value or a complex object and preserve it between the functional component invocations. The following line shows how
you can define a state for the user name.
const [userName, setUserName] = useState('John');
The useState() function returns a pair: the current state value and a function that lets
you update it. Do you remember the syntax for array destructuring introduced in
ECMAScript 6? (If not, take a look at section A.8.2 in the appendix.) The preceding
line means that the useState() hook takes the string 'John' as an initial value and
returns an array, and we use destructuring to get the two elements of this array into two
variables: userName and setUserName. The syntax of array destructuring allows you to
give any names to these variables. If you need to update the value of userName from
“John” to “Mary” and make React update the UI (if needed), you can do it as follows:
setUserName('Mary');
326 CHAPTER 13 Developing React.js apps with TypeScript
TIP In your IDE, Cmd-Click or Ctrl-Click on useState(), and it will open the
type definition of this function, which will declare that the function returns a
stateful value and a function to update it. The useState() function is not a
pure function because it stores the component’s state somewhere inside
React. It’s a function with side effects.
The following listing shows a functional component that stores its state in two primitives, userName and imageUrl, and displays their values using JSX.
import React, {useState} from 'react';
const App: React.FC = () => {
const [userName, setUserName] = useState('John');
const [imageUrl, setImageUrl] = useState('https://picsum.photos/600/150');
return (
<div>
<h1>{userName}</h1>
<img src={imageUrl} alt=""/>
</div>
);
}
export default App;
Now let’s rewrite the preceding component so that instead of two primitives, it
declares its state as an object with two properties: userName and imageUrl. The following listing declares a State interface and uses the useState() hook to work with the
object of type State.
import React, {useState} from 'react';
interface State {
userName: string;
imageUrl: string;
}
const App: React.FC = () => {
const [state, setState] = useState<State>({
userName: 'John',
imageUrl: 'https://picsum.photos/600/150'
});
return (
<div>
Listing 13.7 A functional component using primitives to store state
Listing 13.8 Using an object to store state
Imports the useState hook
Defines the userName state
Defines the imageUrl state
Renders the value of the
userName state variable
Renders the value of the
imageUrl state variable
Defines the type for
the component state
Defines and initializes
the state object
Managing a component’s state 327
<h1>{state.userName}</h1>
<img src={state.imageUrl} alt=""/>
</div>
);
}
export default App;
Note that useState() is a generic function, and during its invocation we provided the
concrete type State.
 The source code of this sample app is located in the hello-world directory. Run the
npm start command, and the browser will render a window that looks similar to figure 13.5 (the image may be different, though).
The user name and image are too close to the left border of the window, but that’s
easy to fix with CSS. The generated app shown in listing 13.5 had a separate app.css
file with CSS selectors applied in the component using the className attribute (you
can’t use class because it would conflict with the JavaScript’s reserved class keyword). This time, we’ll add a margin by declaring a JavaScript object with styles and
using it in JSX. In the following listing, we’ve added the myStyles variable and used it
in the component’s JSX.
const App: React.FC = () => {
const [state, setState] = useState<State>({
userName: 'John',
imageUrl: 'https://picsum.photos/600/150'
});
const myStyles = {margin: 40};
return (
Listing 13.9 Adding styles to the component
Renders the value of the
userName state property
Renders the value of the
imageUrl state property
Figure 13.5 Rendering a user name and an image
Declares the styles
328 CHAPTER 13 Developing React.js apps with TypeScript
<div style ={myStyles}>
<h1>{state.userName}</h1>
<img src={state.imageUrl} alt=""/>
</div>
);
}
Note that the style property is strongly typed, which helps in validating CSS properties. This is one of JSX’s advantages over plain HTML—JSX will be turned into JavaScript, and TypeScript will add strong typing to HTML and CSS elements via type
definition files.
 With this margin, the browser will render the <div> with an additional 40 px of
space around it, as shown in figure 13.6.
Our first React app works and looks good! It has one functional component that
stores hardcoded data in the state object and renders that data using JSX. It’s a good
start, and in the next section, we’ll start writing a new app with more functionality.
13.4 Developing a weather app
In this section, we’ll develop an app that will let the user enter the name of a city and
get the current weather there. We’ll develop this app gradually:
1 We’ll add an HTML form to the App component where the user can enter the
name of the city.
2 We’ll add code to fetch the real weather data from the weather server, and the
App component will display the weather.
3 We’ll create another component, WeatherInfo, which will be a child of the App
component. The App component will retrieve the weather data and will pass it
to WeatherInfo, which will display the weather.
Applies the styles
Figure 13.6 Adding a margin
Developing a weather app 329
We’ll be getting real weather data from the weather service at http://openweathermap
.org, which provides an API for making weather requests for many cities around the
world. This service returns the weather information as a JSON-formatted string. For
example, to get the current temperature in London in Fahrenheit (units=imperial),
the URL would look like this: http://api.openweathermap.org/data/2.5/find?q=
London&units=imperial&appid=12345. (The creators of this service require you to
apply for an application ID, which is a simple process. If you want to run our weather app,
apply for one and replace 12345 in the preceding URL with the APPID you receive.)
 The sample code for this chapter includes the weather app, located in the weather
directory, which was initially generated with the following command:
create-react-app weather --typescript
Then we replaced the JSX code in the app.tsx file with a simple HTML form where
the user could enter the name of the city and press the Get Weather button. Also, the
entered city represents the state of this component, and the App component will
update its state as the user enters the city name.
13.4.1 Adding a state hook to the App component
The first version of our App component defines its state with the useState() hook as
follows:
const [city, setCity] = useState('');
The value in the city variable has to be updated with the setCity() function. Our
useState() hook initializes the city variable with an empty string, so TypeScript will
infer the type of city as string. Listing 13.10 shows the App component with the
declared state, and the form defined in the JSX section. This code also has an event
handler, handleChange(), which is invoked each time the user enters or updates any
character in the input field.
import React, { useState, ChangeEvent } from 'react';
const App: React.FC = () => {
const [city, setCity] = useState('');
const handleChange = (event: ChangeEvent<HTMLInputElement>) => {
setCity(event.target.value);
}
return (
<div>
<form>
Listing 13.10 The App.tsx file in the weather app
Declares the city state
Declares the function to
Updates the state by handle input field events invoking setCity()
330 CHAPTER 13 Developing React.js apps with TypeScript
<input type="text" placeholder="Enter city"
onChange = {handleChange} />
<button type="submit">Get weather</button>
<h2>City: {city}</h2>
</form>
</div>
);
}
export default App;
The input field defines the event handler: onChange = {handleChange}. Note that we
didn’t invoke handleClick() here; we just provided the name of this function. React’s
onChange behaves as onInput and is fired as soon as the content of the input field
changes. As soon as the user enters (or changes) a character in the input field,
the handleChange() function is invoked, which updates the state and thus causes the
UI update.
TIP There is no documentation about which types of React events you can
use with specific JSX elements. To avoid using event: any as the argument in
event handler functions, open the index.d.ts file in the node_modules/
@types/react directory and search for “Event Handler Types.” This should
help you figure out that the proper type for the onChange event is a generic
ChangeEvent<T> that takes the type of a specific element as a parameter, such
as ChangeEvent<HTMLInputElement>.
To illustrate the state updates, we’ve added an <h2> element that displays the current
value of the state: <h2>Entered city: {city}</h2>. Note that for re-rendering the
current value of city, we didn’t need to write any jQuery-like code to find the reference
to this <h2> element and change its value directly. The invocation of setCity(event
.target.value) forces React to update the corresponding node in the DOM.
 In general, if you need to update a functional component’s state, do it only using the
appropriate setXXX() function that’s returned by
the useState() hook. By invoking setXXX(), you
let React know that the UI update may be required.
If you update the state directly (such as with
city="London"), React won’t update the UI. React
may batch the UI updates before reconciling the
Virtual DOM with the browser’s DOM. Figure 13.7
shows a screenshot taken after the user entered
Londo in the input field.
TIP To see that React updates only the <h2> node in the DOM, run this app
(using npm start) with the Chrome dev tools open to the Elements tab.
Expand the DOM tree so the content of the <h2> element is visible, and start
typing in the input field. You’ll see that the browser changes only the content
of the <h2> element, while all other elements remain unchanged.
Assigns the handler to
the onChange attribute
Displays the current
state value
Figure 13.7 After the user entered
Londo
Developing a weather app 331
Redux and app state management
The useState() hook in functional components (or the setState() method in classbased components) is used to store the component’s internal data and synchronize
that data with the UI, but the entire app may also need to store and maintain data used
by multiple components or about the current state of UI (for example, the user selected
product X in component Y). In React apps, the most popular state-management Javascript library is Redux (you could also choose to use MobX, another popular library).
Redux is based on the following three principles:
 Single source of truth—There is a single data store, which contains the state
of your app.
 State is read-only—When an action is emitted, the reducer function clones
the current state and updates the cloned object based on the action.
 State changes are made with pure functions—Reducer functions take an
action and the current state object, and they return a new state.
In Redux, the data flow is unidirectional:
1 The app component dispatches the action on the store.
2 The reducer (a pure function) takes the current state object and then clones,
updates, and returns it.
3 The app component subscribes to the store, receives the new state object,
and updates the UI accordingly.
The following image shows the unidirectional Redux data flow.
The user clicks on the button to buy 100 shares of IBM stock. The click handler function invokes the dispatch() method, emitting an action, which is a JavaScript object
that has a type property describing what happened in your app (for example, the user
wants to buy the IBM stock). Besides the type property, an action object can optionally have another property with a payload of data to be stored in the app state:
{
type: 'BUY_STOCK',
stock: {symbol: 'IBM', quantity: 100}
}
The type of action
The action’s payload
View Component Store
Buy 100 Action Store.dispatch() Reducer
Buying 100 shares Store.subscribe()
IBM Current state
New state
The Redux data flow
332 CHAPTER 13 Developing React.js apps with TypeScript
Working with the component’s state is an internal function of the component. But at
some point, the component may need to start working with external data, and this is
where the useEffect() hook comes in.
13.4.2 Fetching data with the useEffect hook in the App component
You learned how to store the city name in the state of the App component, but our ultimate goal is finding the weather in the given city by fetching data from an external
server. Using terminology from functional programming, we need to write a function
with side effects. Unlike pure functions, functions with side effects use external data, and
every invocation may produce different results even if the function arguments remain
the same.
 In React’s functional components, we’ll be using the useEffect() hook to implement functionality with side effects. By default, React automatically invokes the callback function passed to useEffect() after every DOM rendering. Let’s add the
following function to the App component from listing 13.10:
useEffect(() => console.log("useEffect() was invoked"));
If you run the app with the browser console open, you’ll see the message “useEffect()
was invoked” each time you enter a character in the input field and the UI is
refreshed. Every React component goes through a set of life cycle events, and if you
need your code to be executed after the component is added to the DOM, or each
time it is re-rendered, the useEffect() hook is the right place for such code. But if
you want the code in useEffect() to be executed only once, after the initial rendering, specify an empty array as the second argument:
useEffect(() => console.log("useEffect() was invoked"), []);
(continued)
This object only describes the action and provides the payload, but it doesn’t know
how the state should be changed. Who does? The reducer does—it’s a pure function
that specifies how the app state should be changed. The reducer never changes the
current state, but creates a new version and returns a new reference to it. As you can
see in the preceding image, the component subscribes to the state changes and
updates the UI accordingly.
The reducer function doesn’t implement any app logic that requires working with
external services (such as placing an order). Reducers just update and return the app
state based on the action and its payload, if any. Implementing the app logic would
require interacting with the environment outside the reducer, causing side effects,
and pure functions can’t have side effects. For more details, read the Redux documentation on GitHub at http://mng.bz/Q0lv.
Developing a weather app 333
The code in the preceding hook will be executed only once, right after the component has been rendered, which makes it a good place to perform the initial data fetch.
 Let’s assume you live in London and would like to see the weather in London as
soon as this app is launched. Start by initializing the city state with “London:”
const [city, setCity] = useState('London');
Now you need to write a function that will fetch the data for the specified city. The
URL will include the following static parts (replace 12345 with your APPID).
const baseUrl = 'http://api.openweathermap.org/data/2.5/weather?q=';
const suffix = "&units=imperial&appid=12345";
In between, you need to place the name of the city, so the complete URL might look
like this:
baseUrl + 'London' + suffix
For making Ajax requests, we’ll use the browser’s Fetch API (see the Mozilla documentation at http://mng.bz/Xp4a). The fetch() function returns a Promise, and
we’ll use the async and await keywords (see section A.10.4 in the appendix) in our
getWeather() method.
const getWeather = async (city: string) => {
const response = await fetch(baseUrl + city + suffix);
const jsonWeather = await response.json();
console.log(jsonWeather);
}
NOTE We prefer using the async and await keywords for asynchronous code,
but using promises with chained .then() invocations would also work here.
When you use a standard browser’s fetch() method, getting the data is a two-step process: you get the response first, and then you call the json() function on the response
object to get to the actual data.
TIP JavaScript developers often use third-party libraries for handling HTTP
requests. One of the most popular ones is a promise-based library called Axios
(www.npmjs.com/package/axios).
Now you can use this function for the initial data fetch in useEffect():
useEffect( () => getWeather(city), []);
Listing 13.11 Fetching the weather data
Makes an async call to
the weather server
Converts the response
to JSON format
Prints the weather
JSON on the console
334 CHAPTER 13 Developing React.js apps with TypeScript
If you want the code in useEffect() to be executed only if a specific state variable
changes, you can attach the hook to that state variable. For example, you could specify
that useEffect() should run only if the city is updated:
useEffect(() => console.log("useEffect() was invoked"),
['city']);
The current version of the App component is shown in the following listing.
import React, { useState, useEffect, ChangeEvent } from 'react';
const baseUrl = 'http://api.openweathermap.org/data/2.5/weather?q=';
const suffix = "&units=imperial&appid=12345";
const App: React.FC = () => {
const [city, setCity] = useState('London');
const getWeather = async (city: string) => {
const response = await fetch(baseUrl + city + suffix);
const jsonWeather = await response.json();
console.log(jsonWeather);
}
useEffect( { () => getWeather(city) }, []);
const handleChange = (event: ChangeEvent<HTMLInputElement>) => {
setCity( event.target.value );
}
return (
<div>
<form>
<input type="text" placeholder="Enter city"
onInput = {handleChange} />
<button type="submit">Get Weather</button>
<h2>City: {city}</h2>
</form>
</div>
);
}
export default App;
The second argument of useEffect() is an empty array, so getWeather() will be
invoked only once, when the App component is initially rendered.
Listing 13.12 Fetching the London weather in useEffect()
Asynchronously fetches weather
data for the specified city
An empty array means
run this hook once.
Updates the state
Developing a weather app 335
NOTE If you run this app, the browser’s console will show the following warning: “React Hook useEffect has a missing dependency: ‘city’. Either include it
or remove the dependency array react-hooks/exhaustive-deps.” That’s because
inside this hook we use the state variable city, which is a dependency and
should be listed in the array. This is not an error, and for simplicity, we’ll keep
this code as-is, but you should keep this in mind while designing your hooks.
TIP For in-depth coverage of the useEffect() hook, read “A complete guide
to useEffect,” by Dan Abramov, available on his Overreacted blog at https://
overreacted.io/a-complete-guide-to-useeffect.
Run this app with the browser console open, and it’ll print the retrieved JSON with
the London weather, as shown in figure 13.8.
The initial data fetch for the default city is complete, and it looks like a good idea to store
the retrieved weather data in the component’s state. Let’s define a new Weather type for
storing the content of the name and main properties, both marked in figure 13.8.
export interface Weather {
city: string;
humidity: number;
pressure: number;
Listing 13.13 The weather.ts file
Weather
data
City You’ll add
these to state.
Figure 13.8 The London weather in the console
This property corresponds to the
name property in figure 13.8.
These values come from the
main property in figure 13.8.
336 CHAPTER 13 Developing React.js apps with TypeScript
temp: number;
temp_max: number;
temp_min: number;
}
In the App component, we’ll add a new weather state variable and a function to
update it, as follows:
const [weather, setWeather] = useState<Weather | null>(null);
Note that the useState() hook allows you to use a generic parameter for better type
safety.
 Now we need to update the getWeather() function so it saves the retrieved
weather and city name in the component’s state.
async function getWeather(location: string) {
const response = await fetch(baseUrl + location + suffix);
if (response.status === 200){
const jsonWeather = await response.json();
const cityTemp: Weather = jsonWeather.main;
cityTemp.city=jsonWeather.name;
setWeather(cityTemp);
} else {
setWeather(null);
}
}
This code takes the jsonWeather.main object and the city name from jsonWeather
.name, and saves them in the weather state variable.
 So far, our getWeather() function has been invoked by the useEffect() hook for
the initial retrieval of the London weather. The next step is to add code to invoke
getWeather() when the user enters any other city and clicks on the Get Weather button. As you saw in listing 13.12, this button is part of the form (its type is submit), so
we’ll add an event handler to the <form> tag. The handleSubmit() function and the
first version of the JSX are shown in the following listing.
const handleSubmit = (event: FormEvent) => {
event.preventDefault();
getWeather(city);
}
return (
Listing 13.14 Saving state in getWeather
Listing 13.15 Handling the button click
These values come from the
main property in figure 13.8.
Stores the content of
the main property
Stores the city name
Saves the weather in
the component’s state The weather retrieval failed.
When the Submit button is clicked,
the FormEvent is dispatched. Prevents the default
behavior of the form’s
submit button
Invokes getWeather()
for the entered city
Developing a weather app 337
<div>
<form onSubmit = {handleSubmit}>
<input type="text" placeholder="Enter city"
onInput = {handleChange} />
<button type="submit">Get Weather</button>
<h2>City: {city}</h2>
{weather && <h2>Temperature: {weather.temp}F</h2>}
</form>
</div>
);
In React, event handlers get instances of SyntheticEvent, which is an enhanced version of the browser’s native events (see https://reactjs.org/docs/events.html for
details). SyntheticEvent has the same interface as the browser’s native events (such
as preventDefault()), but the events work identically across all browsers (unlike
native browser events).
 To provide the parameter’s value to getWeather
(city), we don’t have to find a reference to the
<input> field on the UI. The component’s city state
was updated when the user typed the name of the city,
so the city variable already has its value displayed in
the <input> field. Figure 13.9 shows a screenshot of
the page after a user entered Miami and clicked the
Get Weather button.
NOTE In our book Angular Development with TypeScript, second edition (Manning, 2018), we also used this weather service. You can find the Angular version of this app on GitHub at http://mng.bz/yzMd.
What if the user enters a city that doesn’t exist or that’s not supported by openweathermap.org? The server returns 404, and we should add the appropriate error handling.
So far we have the following line to prevent displaying the temperature if the weather
state is falsy:
{ weather && <h2>Temperature: {weather.temp}F</h2> }
In the next version of this app, we’ll create a type guard to check if the weather for the
provided city was received or not.
 For now, let’s take a breather and recap what we’ve done so far, while developing
the weather app:
1 Applied for the APPID at openweathermap.org
2 Generated a new app and replaced the JSX with a simple <form>.
3 Declared the city state using the useState() hook.
4 Added the handleChange() function that updates city on each change in the
input field.
5 Added the useEffect() hook, which will be invoked only once on app startup.
Attaches the event
handler to the form
Displays
the retrieved
temperature
Figure 13.9 It’s hot in Miami.
338 CHAPTER 13 Developing React.js apps with TypeScript
6 Ensured that useEffect() invokes the getWeather() function, which uses the
fetch() API to retrieve the weather in London.
7 Declared the weather state to store the retrieved temperature and humidity,
which are some of the properties of the retrieved weather object.
8 Added the handleSubmit() event handler to invoke getWeather() after the
user enters the city name and clicks the Get Weather button.
9 Modified the getWeather() function to save the retrieved weather in the
weather state.
10 Displayed the retrieved temperature on the web page under the form.
This is all good, but we shouldn’t put all the app logic in one App component. In the
next section, we’ll create a separate component that will be responsible for displaying
the weather data.
13.4.3 Using props
A React app is a tree of components, and you need to decide which are going to be
the container components, and which the presentation ones. A container (a.k.a. smart)
component contains application logic, communicates with external data providers,
and passes data to its child components. Typically, container components are stateful
and have little or no markup.
 A presentation (a.k.a. dumb) component just receives data from its parent and displays it. A typical presentation component is stateless and has lots of markup. A presentation component gets the data to be displayed via its props JavaScript object.
TIP In section 14.4, we’ll look at the UI components of the React version of
the blockchain app. There you’ll see one container component and three
presentation components.
TIP If you use a library for managing the state of an entire app (such as
Redux), only the container components will communicate with such a library.
In our weather app, App is a container component that knows how to receive weather
data from the external server. So far our App component also displays the received
temperature, as shown in figure 13.9, but we should delegate the weather rendering
functionality to a separate presentation component, such as WeatherInfo. Any app
consists of multiple components, and having a separate WeatherInfo component will
allow us to illustrate how a parent component can send data to its child. Besides, having a separate WeatherInfo component that just knows how to display the data passed
via props makes it reusable.
 The App component (the parent) will contain the WeatherInfo component (the
child), and the parent will need to pass the received weather data to the child. Passing
data to a React component works similarly to passing data to HTML elements.
Developing a weather app 339
 We’ll start getting familiar with the role of props by using JSX elements as an example. Any JSX element can be rendered differently depending on the data it gets. For
example, the JSX of a red disabled button can look like this:
<button className="red" disabled />
This code instructs React to create a button element and pass it certain values via the
className and disabled attributes. React will start by transforming the preceding JSX
into an invocation of createElement():
React.createElement("button", {
className: "red",
disabled: true
});
Then, the preceding code will produce a JavaScript object that looks like this:
{
type: 'button',
props: { className: "red", disabled: true }
}
As you can see, props contains the data being passed to the React element. React uses
props for data exchange between parent and child components (the previous button
was a part of a parent element too).
 Suppose you created a custom Order component and added it to the parent’s JSX.
You could pass the data to it via props as well. For example, an Order component may
need to receive the values of such props as operation, product, and price:
<Order operation="buy" product="Bicycle" price={187.50} />
Similarly, we’ll add the WeatherInfo component to the App component’s JSX, passing
the received weather data. Also, we promised to add a user-defined type guard to
ensure that the WeatherInfo component won’t render anything if the weather for the
city is not available. The following listing shows the code from the App component
that defines and uses the type guard called has.
const has = (value: any): value is boolean => !!value;
...
return (
<>
<form onSubmit = {handleSubmit}>
<input type="text" placeholder="Enter city"
onInput = {handleChange} />
Listing 13.16 Adding the has type guard
Declares the
has type guard
An empty JSX tag can
be used as a container.
340 CHAPTER 13 Developing React.js apps with TypeScript
<button type="submit">Get Weather</button>
</form>
{has(weather) ? (
<WeatherInfo weather={weather} />
):(
<h2>No weather available</h2>
)}
</>
);
As you can see, the App component hosts the form and the WeatherInfo component,
which should render the weather data. All JSX tags have to be wrapped into a single
container tag. Earlier, we used <div> as a parent tag. In listing 13.16, we use an empty
tag instead, which is a shortcut for a special tag container, <React.Fragment>, that
doesn’t add an extra node to the DOM.
 In section 2.3, we introduced TypeScript’s user-defined type guards. In listing 13.16,
we declared the has type guard as a function whose return type is a type predicate:
const has = (value: any): value is boolean => !!value;
It takes a value of any type and applies to it the JavaScript double bang operator to
check if the provided value is truthy. Now the expression has(weather) will check if
the weather is received (as you can see in the JSX in listing 13.17). The received
weather is given to the WeatherInfo component via its weather props:
<WeatherInfo weather={weather} />
Now let’s discuss how we created the WeatherInfo component that receives and renders
the weather data. In VS Code, we created a new weather-info.tsx file and declared the
WeatherInfo component there. As in the App functional component, we used the arrow
function notation for WeatherInfo, but this time our component accepts an explicit
props. Hover the mouse over FC, and you’ll see its declaration, as shown in figure 13.10.
Applies
the type
guard has
Passes the weather to
WeatherInfo and renders it
Renders the text message
instead of WeatherInfo
Closes the empty JSX tag
Generic type with
default parameter
Figure 13.10 A generic type with default parameter
Developing a weather app 341
React.FC is a generic type that takes P (for props) as a type parameter. So why didn’t
the Typescript compiler complain when we declared the App component without
using the generic notation and a concrete type? The P = {} part did the trick. This is
how you can declare a generic type with a default value (see the sidebar titled “Default
values of generic types” in section 4.2.2). Our App component didn’t use props, and
by default, React assumed that props was an empty object.
 Each component has a property called props, which can be an arbitrary JavaScript
object with properties specific to your app. In JavaScript you can’t specify the type of
the props content, but TypeScript generics allow you to let the component know that
it’s going to get the props containing the Weather, as shown in the following listing.
import * as React from 'react';
import {Weather} from './weather';
const WeatherInfo: React.FC<{weather: Weather} >=
({ weather }) => {
const {city, humidity, pressure, temp, temp_max, temp_min} = weather;
return (
<div>
<h2>City: {city}</h2>
<h2>Temperature: {temp}</h2>
<h2>Max temperature: {temp_max}</h2>
<h2>Min temperature: {temp_min}</h2>
<h2>Humidity: {humidity}</h2>
<h2>Pressure: {pressure}</h2>
</div>
);
}
export default WeatherInfo;
The WeatherInfo component is a generic function with one parameter, <P>, as shown
in figure 13.10, and we used the {weather: Weather} type as its argument. To render
the data, we could access each property of the weather object by using dot notation
(such as weather.city), but the fastest way to extract the values of these properties
into local variables is destructuring (discussed in section A.8 of the appendix) as
shown in the following line:
const {city, humidity, pressure, temp, temp_max, temp_min} = weather;
Now all these variables can be used in the JSX returned by this component; for example, <h2>City: {city}</h2>.
Listing 13.17 WeatherInfo.tsx: the WeatherInfo functional component
Our component is a generic function
with an argument of type Weather.
The fat arrow function has
one argument—the weather
object.
Destructures the
weather object Renders the city
Renders the
weather data
342 CHAPTER 13 Developing React.js apps with TypeScript
Passing a markup to the child component
props can be used not only for passing data to a child component, but also for passing JSX fragments. If you place a JSX fragment between the opening and closing tags
of the component, as shown in the following snippet, this content will be stored in
the props.children property, and you can render it as needed.
<WeatherInfo weather = {weather} >
<strong>Hello from the parent!</strong>
</WeatherInfo>
Here, the App component passes the HTML element <strong>Hello from the
parent!</strong> to the WeatherInfo child component, and it could have passed
any other React component the same way.
The WeatherInfo component also has to declare its interest in receiving not only the
Weather object (shown earlier in listing 13.13), but also the content of props.children from React.FC as follows:
const WeatherInfo: React.FC<{weather: Weather} >=
➥ ({ weather, children }) =>...
This line tells React, “We’ll give your React.FC component the object with the property
weather, but we’d like to use the children property as well.” Now the local children variable contains the markup provided by the parent, which can be rendered
along with the weather data:
return (
<div>
{children}
<h2>City: {city}</h2>
<h2>Temperature: {temp}</h2>
{ /* The rest of the JSX is omitted */ }
</div>
);
After embedding the {children} expression, the WeatherInfo component will be
rendered, as follows.
Passes this markup
to WeatherInfo
Renders the markup
received from the parent
Displaying the content
received via props.children
This came from the parent.
Developing a weather app 343
To visualize the props and state of each component, install the Chrome extension
called React Developer Tools and run a React app with the Chrome dev tools open.
You’ll see an extra React tab that shows rendered elements on the left and the props
and state (if any) of every component on the right (figure 13.11). Our WeatherInfo
component is stateless; otherwise you’d see the content of the state as well.
It’s great that a parent component passes data to its child via props, but what about
sending data in the opposite direction?
13.4.4 How a child component can pass data to its parent
There are scenarios when a child component needs to send data to its parent, and this
is also done via props. Imagine a child component that’s connected to a stock
exchange, and it receives the latest stock prices every second. If its parent is responsible for handling that data, the child needs to be able to pass the data to its parent.
 For simplicity, we’ll just illustrate how the WeatherInfo component could send
some text to its parent. This will require some coding on both the parent’s and the
child’s ends. Let’s start with the parent.
React Developer Tools
The value of
props.children
Figure 13.11 React Developer Tools
344 CHAPTER 13 Developing React.js apps with TypeScript
 If a parent expects to get some data from the child, it has to declare a function that
knows how to handle this data. Then, in the parent’s JSX where it embeds the child
component, we add an attribute to the child’s tag containing a reference to this function. In the weather app, the App component is the parent, and we want it to be able
to receive text messages from its child, WeatherInfo. The following listing shows what
we need to add to the code of the App component developed in the previous section.
const [msgFromChild, setMsgFromChild] = useState('');
const getMsgFromChild = (msg: string) => setMsgFromChild(msg);
return (
<>
/* The rest of the JSX is omitted */
{msgFromChild}
{has(weather) ? (
<WeatherInfo weather = {weather} parentChannel = {getMsgFromChild}>
</>
Here we added the msgFromChild state variable to store the message received from
the child. The getMsgFromChild() function gets the message and updates the state
using the setMsgFromChild() function, which results in re-rendering {msgFromChild} in the UI of the App component.
 Finally, we need to give the child a reference for invoking the message handler. We
decided to call this reference parentChannel, and we passed it to WeatherInfo as follows:
parentChannel = {getMsgFromChild}
parentChannel is an arbitrary name that the child will use to invoke the getMsgFromChild() message handler.
 The modifications to the parent’s code are complete, so let’s take care of the
WeatherInfo child component. The following listing shows the additions to the
child’s code.
const WeatherInfo: React.FC<{weather: Weather, parentChannel:
➥ (msg: string) => void}> =
({weather, children, parentChannel}) => {
Listing 13.18 Adding code to the App component for receiving data
Listing 13.19 Adding code to WeatherInfo for sending data to parent
Declares the msgFromChild state
variable to store the child’s message
Declares the function to
store the child’s message
in the msgFromChild state
Displays the child’s message
Adds the parentChannel
property to the child
Adds parentChannel to the
type of the generic function Adds
parentChannel
to the function
argument
Developing a weather app 345
/* The rest of the WebInfo code is omitted */
return (
<div>
<button
onClick ={() => parentChannel ("Hello from child!")}>
Say hello to parent
</button>
/* The rest of the JSX code is omitted */
</div>
);
Since the parent gives the child the parentChannel reference to invoke the messagehandler function, we need to modify the type parameter of the component to include
this reference:
<{weather: Weather, parentChannel: (msg: string) => void}>
The type of parentChannel is a function that takes a string parameter and returns no
value. Our WeatherInfo component will process props, which is now an object with
three properties: weather, children, and parentChannel.
 The onClick button handler returns a function that will invoke parentChannel(),
passing the message text to it:
() => parentChannel ("Hello from child!")
Run this version of the weather app, and if you click on the button at the bottom of
the WeatherInfo component, the parent will receive the message and render it as
shown in figure 13.12.
Invokes the
parent’s function
on the button
click, using
parentChannel
as a reference
Parent renders the msg from child.
Child renders the msg from parent.
Click on child’s button to send msg to parent. Figure 13.12
Message from child
346 CHAPTER 13 Developing React.js apps with TypeScript
NOTE The code in listing 13.19 can be optimized, and this is your homework.
This code works, but it recreates the function () ? parentChannel("Hello
from child!") on each re-rendering of the UI. To keep this from happening,
read about useCallback() in the React documentation at http://mng.bz/
MO8B, and wrap the parentChannel() function in this hook.
Now that we have introduced both state and props, we’d like to stress that they serve
different purposes:
 state stores a private component’s data; props is used for passing data to child
components or back to the parent.
 A component may use the received props to initialize its state, if any.
 Direct modifications of state properties’ values is prohibited; you must use the
function returned by the useState() hook in functional components, or the
setState() method in class-based components, to ensure that the changes are
reflected on the UI.
 props are immutable, and a component can’t modify the original data received
via props.
NOTE State and props serve different purposes, but the UI in React is a function of both state and props.
In our brief overview of the React framework, we discussed components, state, and
props, but even the shortest overview of React must include one more topic: Virtual
DOM.
13.5 What’s Virtual DOM?
A unique feature of React is Virtual DOM, a
layer between the component and the
browser’s DOM. Each component consists of
UI elements, and Virtual DOM optimizes the
process of rendering these elements to the
browser’s DOM, as illustrated in figure 13.13.
 When you start the app, React creates a
tree of UI components in its own Virtual
DOM, and it renders this tree to the browser’s
DOM. As the user works with the React app,
the browser’s DOM triggers events to the app.
If the event is handled by JavaScript, and if the
handler code updates the component’s state,
then React re-creates the Virtual DOM, diffs its new version with the previous one, and
syncs the differences with the browser’s DOM. Applying this diffing algorithm is called
reconciliation (see more in React’s documentation at https://reactjs.org/docs/
reconciliation.html).
React app Reconciliation
Virtual
DOM
Browser’s
DOM
Figure 13.13 React’s Virtual DOM
Summary 347
NOTE The term “Virtual DOM” is a bit of a misnomer, because the React
Native library uses the same principles for rendering the iOS and Android
UIs, which don’t have DOMs.
In general, the rendering of the UI from the browser’s DOM is a slow operation, and
to make it faster, React doesn’t re-render all the elements of the browser’s DOM every
time an element changes. You may not see much of a difference in the speed of rendering a web page that has a handful of HTML elements, but in a page that has thousands of elements, the difference in rendering performed by React versus regular
JavaScript will be noticeable.
 If you can’t imagine a web page with thousands of HTML elements, think of a
financial portal showing the latest trading activity in tabular form. If such a table contains 300 rows and 40 columns, it has 12,000 cells, and each cell consists of several
HTML elements. A portal may need to render several such tables.
 The Virtual DOM spares developers from working with the browser DOM API like
jQuery does—just update the component’s state, and React will update the corresponding DOM elements in the most efficient manner. This is basically all the React
library does, which means you’ll need other libraries for implementing such functionality as making HTTP requests, routing, or working with forms.
 This concludes our brief introduction to developing web apps using the React.js
library and TypeScript. In chapter 14, we’ll review the code of a new version of the
blockchain client written in React.js.
Summary
 React.js is a great library for rendering UI components. You can use React.js in
any part of an existing app that uses other frameworks or just JavaScript. In
other words, you can develop a non-single-page-app with React if need be.
 You can generate a TypeScript-React app with the Create React App commandline tool in about a minute. This app will be fully configured and runnable.
 A React component is typically implemented either as a class or as a function.
 The UI portion of a component is typically declared using the JSX syntax. React
components written in TypeScript are stored in files with the .tsx extension,
which tell the tsc compiler that it contains JSX.
 A React component usually has state, which can be represented as one or more
properties of the component. Whenever a state property is modified, React rerenders the component.
 Parent and child components can exchange data using props.
 React uses in-memory Virtual DOM, which is a layer between the component
and the browser’s DOM. Each component consists of UI elements, and Virtual
DOM optimizes the process of rendering these elements to the browser’s DOM.
348
Developing a blockchain
 client in React.js
In the previous chapter, you learned the basics of React, and now we’ll review a new
version of the blockchain app with the client portion written in React. The source
code of the web client is located in the blockchain/client directory, and the messaging server is located in the blockchain/server directory.
 The code of the server remains the same as in chapters 10 and 12, and the functionality of this version of the blockchain app is the same as well, but the UI portion
of the app was completely rewritten in React.
This chapter covers
 Reviewing a blockchain web client written with
React.js
 How the React.js web client communicates with
the WebSocket server
 Running a React app that works with two servers
in dev mode
 Splitting the UI of a blockchain client into
components, and arranging their communications
Starting the client and the messaging server 349
 In this chapter, we won’t be reviewing the blockchain functionality, because it was
covered in the previous chapters, but we will review the code that’s specific to the
React.js library. You may want to look back at chapter 10 to refresh your memory
about the functionality of the blockchain client and messaging server.
 First, we’ll start the blockchain messaging server and the React client. Then, we’ll
introduce the code of the UI components, highlighting the difference between smart
and presentation components. You’ll also see multiple code samples that demonstrate
intercomponent communications via props.
14.1 Starting the client and the messaging server
To start the server, open the terminal window in the blockchain/server directory, run npm
install to install the server’s dependencies, and then run the npm start command.
You’ll see the message “Listening on http://localhost:3000;” keep the server running.
 To start the React client, open another terminal window in the blockchain/client
directory, run npm install to install React and its dependencies, and then run the
npm start command.
 The blockchain client will start on port 3001, and after a short delay, the genesis
block will be generated. Figure 14.1 shows a screenshot of the React client, with labels
pointing to the UI files of the app, with the App.tsx file containing the code of the
root component called App.
 The start command is defined in package.json as an alias to the react-scripts
start command, and it engages Webpack to build the bundles and use webpackdev-server to launch the app. We went through a similar workflow when starting the
Angular blockchain client in chapter 12.
 However, apps generated by the Create React App CLI are preconfigured to start
the webpack-dev-server on the port 3000, so we had a ports conflict, because port 3000
was already taken by our messaging server. We had to find a way to configure a different port for the client, and we’ll explain how we did it shortly.
 Projects that are generated by the Create React App tool are also preconfigured to
read custom environment variables from the .env.development file (shown in the following listing) in the dev mode, or from .env.production in prod.
TIP To prepare an optimized production build, run the npm run build command and you’ll find index.html and the app bundles in the directory build.
This command uses the environment variables from the .env.production file.
PORT=3001
REACT_APP_WS_PROXY_HOSTNAME=localhost:3000
In the preceding listing, we first declared port 3001 to be used when webpack-dev-server
starts the app, and this will resolve the conflict with the server, which runs on port 3000.
Then we declared a custom REACT_APP_WS_PROXY_HOSTNAME environment variable that
Listing 14.1 env.development: environment variables are defined here
350 CHAPTER 14 Developing a blockchain client in React.js
can be used to proxy the client’s requests to the server running on localhost:3000. This
variable will be used in the websocket-controller.ts script. You can read more about adding custom environment variables in Create React App projects in the documentation,
at http://mng.bz/adOm.
 Figure 14.2 shows how our blockchain client runs in dev mode. The CLI dev server
(currently, it’s webpack-dev-server) serves the React app on port 3001, which then
connects to another server that runs on port 3000.
NOTE The Create React App CLI allows you to add the proxy property to
package.json and specify a URL to be used if the dev server doesn’t find the
requested resource (for example, "proxy": "http://localhost:3000"). But
this property doesn’t work with the WebSocket protocol, so we had to use a
custom environment variable to specify the URL of our messaging server.
App.tsx
BlocksPanel.tsx
TransactionForm.tsx
PendingTransactionsPanel.tsx
Figure 14.1 The blockchain client is launched.
Starting the client and the messaging server 351
Both Angular and React projects have similar file structures and launching procedures. But because our React app uses Babel in its workflow, the configuration of the
supported browsers is different than for tsc. Listing 14.2 shows that we list specific
browser versions that our app should support.
 In section 6.4, we showed you how Babel can use presets to specify which versions
of specific browsers the app has to support. The Create React App tool adds a default
browserslist section to package.json. Open this file and you’ll find the following
default browser configuration.
"browserslist": {
"production": [
">0.2%",
"not dead",
"not op_mini all"
],
"development": [
"last 1 chrome version",
"last 1 firefox version",
"last 1 safari version"
]
}
TIP Even though we don’t use tsc to compile the code and emit JavaScript,
we still specified the option "target": "es5" in tsconfig.json to ensure that
TypeScript doesn’t complain about the TypeScript syntax from our blockchain app. Remove the target option or change its value to es3, and the get
url() getter from listing 14.5 will be underlined by a red squiggly line with
the error message, “Accessors are only available when targeting ECMAScript 5
and higher.”
Listing 14.2 A fragment from package.json
CLI dev server
React app
WebSocket server
Data
3001
3000
React app
in Web browser
Figure 14.2 One app, two servers
Browsers that must be
supported in prod builds
Browsers that must be
supported in dev builds
352 CHAPTER 14 Developing a blockchain client in React.js
In section 6.4, we explained the use of browserslist (see https://browserl.ist), which
allows you to configure tsc to emit JavaScript that runs in specific browsers. The Create React App supports it as well, and the settings from package.json are used when
you create a production build by running the npm build script or when you create
dev bundles by running the start script. Copy and paste each entry from your app’s
browserslist to the https://browserl.ist web page, and it’ll show you which browsers
are covered by this entry.
 As in previous versions of the blockchain app, the code is divided into a UI part
and the supporting scripts that implement the blockchain’s algorithms. In the React
version, the UI components are located in the components directory, and the supporting scripts are in lib.
14.2 What changed in the lib directory
As a reminder, the lib directory contains the code that generates new blocks, requests
the longest chain, notifies other nodes about newly generated blocks, and invites
other members of the blockchain to start generating new blocks for the specified
transactions. These processes were described in sections 10.1 and 10.2.
 We slightly modified the websocket-controller.ts file, which contains the script for
communicating with the WebSocket server. In chapter 10, we didn’t use JavaScript
frameworks and simply instantiated the WebsocketController class using the new
operator. We passed messageCallback to the constructor to handle messages coming
from the server.
 In listing 12.2, we used Angular’s dependency injection, and this framework instantiated and injected the WebsocketService object into AppComponent. We could rely on
Angular to instantiate the service first, and only after create the App component.
TIP If you have an Angular background and are accustomed to creating singleton services that can be injected into components, learn about React’s
Context (https://reactjs.org/docs/context.html), which can be used as common storage for data that should be passed from one component to another.
In other words, props is not the only way to pass data between components.
In the React version of the blockchain app, we’ll manually instantiate WebsocketController. The App.tsx script starts as shown in the following listing.
const server = new WebsocketController();
const node = new BlockchainNode();
const App: React.FC = () => {
// The code of the App component is omitted
// We'll review it later in this chapter
}
Listing 14.3 Instantiating classes before the component
First, instantiates
WebsocketController
Second, instantiates BlockchainNode
Third, declares the root UI component
What changed in the lib directory 353
To ensure that WebsocketController and BlockchainNode are global objects, we start
the script by instantiating them. But WebsocketController needs a callback method
from the App component to handle server messages changing the component’s state.
The problem is that the App component is not instantiated yet, so we can’t provide
such a callback to the component’s constructor.
 That’s why we created the connect() method in WebsocketController. This
method takes the callback as its parameter. The complete code of the connect()
method is shown in the following listing.
connect(messagesCallback: (messages: Message) => void):
Promise<WebSocket> {
this.messagesCallback = messagesCallback;
return this.websocket = new Promise((resolve, reject) => {
const ws = new WebSocket(this.url);
ws.addEventListener('open', () => resolve(ws));
ws.addEventListener('error', err => reject(err));
ws.addEventListener('message', this.onMessageReceived);
});
}
The App component will invoke connect() passing the proper callback (as we’ll discuss in the next section, and you’ll see in listing 14.10). What’s the value of this.url,
which is supposed to point at the WebSocket server? In section 14.1, we stated that the
server’s domain name and port will be taken from the environment variables. The following listing shows the code of the url getter in WebsocketController.
private get url(): string {
const protocol = window.location.protocol === 'https:' ? 'wss' : 'ws';
const hostname = process.env.REACT_APP_WS_PROXY_HOSTNAME
|| window.location.host;
return `${protocol}://${hostname}`;
}
The REACT_APP_WS_PROXY_HOSTNAME variable was defined in the env.development file
shown in listing 14.1. The env property of the process Node.js global variable is where
your code can access all the available environment variables. You might object and say
that our app runs in the browser and not in the Node.js runtime! This is correct, but
during the bundling, Webpack reads the values of the variables available in process
.env and inlines them into the bundles of the web app.
Listing 14.4 The connect() method from WebsocketController
Listing 14.5 The url getter
Passes the callback to the controller
Wraps the
socket creation
in a Promise
Gets the host name and port
from the environment variable
If the host name is not found in
process.env, uses the current app host
354 CHAPTER 14 Developing a blockchain client in React.js
TIP The values of custom environment variables are inlined into the bundles. Run the command npm run build and open the main bundle from the
build/static directory. Then search for the value of one of the variables from
the .env.production file, such as localhost:3002.
After adding the connect() method, we decided to add the disconnect() method,
which will close the socket connection:
disconnect() {
this.websocket.then(ws => ws.close());
}
In our version of the blockchain client, the WebSocket connection is established by
the App component, so when the App component is destroyed, the entire app is
destroyed, including the socket connection. But this may not always be the case, so it’s
nice to have a separate method that will close the socket. This allows other components to establish and destroy the connection if need be.
14.3 The smart App component
The UI of this version of blockchain consists of five React components located in the
.tsx files in the components directory:
 App
 BlocksPanel
 BlockComponent
 PendingTransactionsPanel
 TransactionForm
The App.tsx file contains the root App component. The other .tsx files contain the
code of its children, TransactionForm, PendingTransactionsPanel, and BlocksPanel, which is also a parent of one or more BlockComponent instances as shown in
figure 14.3.
App
TransactionForm
PendingTransactionsPanel
BlocksPanel
BlockComponent BlockComponent BlockComponent
Figure 14.3 Parent
and child components
The smart App component 355
In section 13.4.3, we introduced the concept of container (smart) and presentation
(dumb) components. App is a smart component that contains a reference to the
blockchain node instance and all related algorithms. The App component also performs all communications with the messaging server.
 A typical presentation component either displays data, or, based on the user’s
actions, sends its data to other components. Presentation components don’t implement complex application logic. For example, if a PendingTransactionsPanel component needs to initiate the creation of a new block, it simply invokes the proper
callback on the App component, which will start the block generation process. In our
blockchain client, the presentation components are TransactionForm, PendingTransactionPanel, and BlocksPanel.
 The smart App component is implemented in the App.tsx file, which also creates
instances of BlockchainNode and WebsocketController. To give you a big picture
view of how the App component communicates with its children, the following listing
shows the JSX part of the App component.
const App: React.FC = () => {
// Other code is omitted for brevity
return (
<main>
<h1>Blockchain node</h1>
<aside><p>{status}</p></aside>
<section>
<TransactionForm
onAddTransaction={addTransaction}
disabled={node.isMining || node.chainIsEmpty}
/>
</section>
<section>
<PendingTransactionsPanel
formattedTransactions={formatTransactions(node.pendingTransactions)}
onGenerateBlock={generateBlock}
disabled={node.isMining || node.noPendingTransactions}
/>
</section>
<section>
<BlocksPanel blocks={node.chain} />
</section>
</main>
);
}
14.3.1 Adding a transaction
The JSX of the App component includes child components, which can invoke callback
methods on App. We discussed how a React child component can interact with its
Listing 14.6 The App component’s JSX
The first child component
This child can invoke
addTransaction() on
the App component.
The second child
This child
 can invoke
generateBlock()
on the App
component.
The third child
356 CHAPTER 14 Developing a blockchain client in React.js
parent in section 13.4.4. In listing 14.6, you saw that the App component passes the
onAddTransaction callback to TransactionForm via props.
 Accordingly, when the user clicks on the ADD TRANSACTION button on the
TransactionForm component, it’ll invoke its onAddTransaction() method, which will
result in the invocation of the addTransaction() method on the App component.
This is what the word “implicitly” means in figure 14.4, which shows a screenshot of VS
Code with the outline of the App component.
To complete our discussion of the add transaction workflow, let’s see what triggers the
UI change when a new transaction is added. In React, we change the state of the component to update the UI, and we do this by invoking the function returned by the
useState() hook. In the App component, the name of this function is setStatus().
const node = new BlockchainNode();
const App: React.FC = () => {
const [status, setStatus] = useState<string>('');
function addTransaction(transaction: Transaction): void {
Listing 14.7 The App code that adds a transaction
Is implicitly
invoked by
the child
Is implicitly
invoked by
the child
Figure 14.4 The App
component has methods that
will be invoked by its children.
The node instance
Declares the status state
of the App component
The smart App component 357
node.addTransaction(transaction);
setStatus(getStatus(node));
}
// the rest of the code is omitted for brevity
}
function getStatus(node: BlockchainNode): string {
return node.chainIsEmpty ? '⏳ Initializing the blockchain...' :
node.isMining ? '⏳ Mining a new block...' :
node.noPendingTransactions ? '✉ Add one or more transactions.' :
'✅ Ready to mine a new block.';
}
We reviewed the blockchain node implementation and identified operations that
change the internal state of the node. We added the helper properties chainIsEmpty,
isMining, and noPendingTransactions to probe the node’s internal state. After each
operation that may change the internal node’s state, we verify the UI state against the
node’s internal state, and React applies any required changes. If any of these values
change, we need to update the UI of the App component. But what can we use in the
App component to trigger the invocation of setStatus()?
 The getStatus() function returns the text describing the current blockchain
node’s status, and the App component will render the corresponding message (see listing 14.7). Initially the status value is “Initializing the blockchain…” If the user adds a
new transaction, getStatus() will return “Add one or more transactions,” and the
setStatus(getStatus(node)); line will change the component’s state, resulting in
the UI being re-rendered. Figure 14.5 shows the app’s status (or state): “Add one or
more transactions.”
Adds the transaction received
from the child component Updates
the state
This function is
implemented outside
the App component.
Status before
adding transaction
Figure 14.5 The user is ready to add a transaction.
358 CHAPTER 14 Developing a blockchain client in React.js
Figure 14.6 shows a screenshot taken after the user clicked the ADD TRANSACTION
button. The App component’s state is “Ready to mine a new block,” and the GENERATE BLOCK button is enabled.
 While testing this app, we noticed an issue: if the user adds more than one transaction, it will still show only the first one in the Pending Transactions field. But after
clicking the GENERATE BLOCK button, the new block will include all pending transactions. For some reason, React didn’t re-render the UI for any new transactions
except the first one.
 The problem was that after adding the first transaction, the state of the App component contained “Ready to mine a new block,” and this value didn’t change after adding other transactions. Accordingly, the setStatus(getStatus(node)); line didn’t
change the component’s state and React didn’t see any reason for UI re-rendering!
 This issue was easy to fix. We slightly modified the getStatus() function by adding
a transaction counter to the status line. Now instead of keeping the “Ready to mine
new block” status, the status includes a changing part:
`Ready to mine a new block (transactions:
➥ ${node.pendingTransactions.length}).`;
Now every invocation of addTransaction() will change the component’s state.
Status after
adding transaction
Figure 14.6 The user is ready to mine a new block.
The smart App component 359
TIP Making the node immutable would be a better solution to this issue. For
an immutable object, whenever its state changes, a new instance (and a reference) would be created, and the hack with the transaction counter wouldn’t
be necessary.
14.3.2 Generating a new block
Let’s take another look at listing 14.6. The App component passes the onGenerateBlock() callback to PendingTransactionsPanel using props. When the user clicks on
the GENERATE BLOCK button, the PendingTransactionsPanel component invokes
onGenerateBlock(), which in turn invokes the generateBlock() method on the App
component. This method is shown in the following listing.
async function generateBlock() {
server.requestNewBlock(node.pendingTransactions);
const miningProcessIsDone = node.mineBlockWith(node. pendingTransactions);
setStatus(getStatus(node));
const newBlock = await miningProcessIsDone;
addBlock(newBlock);
}
NOTE By providing the parent’s method references to child components, we
control what the child can access in the parent. In this app, neither the
TransactionForm nor PendingTransactionsPanel components have access
to the BlockchainNode and WebsocketController objects. These children
are strictly presentational components; they can display data or notify the parent about some events.
14.3.3 Explaining the useEffect() hooks
In the App component’s code, you can find two useEffect() hooks. As you’ll recall,
useEffect() hooks can be automatically invoked when a specified variable has
changed. The App component has two such hooks. The following listing shows the
first useEffect() hook, which works only once on app startup.
useEffect(() => {
setStatus(getStatus(node));
}, []);
Listing 14.8 The generateBlock() method
Listing 14.9 The first useEffect() hook
Invites other nodes to start generating a
new block for the pending transactions
Declares an expression
for block mining
Changes the component’s state
Starts the block mining
and waits for completion Adds the new block
to the blockchain
360 CHAPTER 14 Developing a blockchain client in React.js
The goal of this effect is to initialize the component’s state with the message “Initializing the blockchain…” If you commented out this hook, the app would still work, but
there wouldn’t be any status message when the app starts.
 Listing 14.10 shows the second useEffect() hook, which connects to the WebSocket server, passing it the handleServerMessages callback that handles messages
pushed by the server.
useEffect(() => {
async function initializeBlockchainNode() {
await server.connect(handleServerMessages);
const blocks = await server.requestLongestChain();
if (blocks.length > 0) {
node.initializeWith(blocks);
} else {
await node.initializeWithGenesisBlock();
}
setStatus(getStatus(node));
}
initializeBlockchainNode();
return () => server.disconnect();
}, [handleServerMessages]);
If a function is used only inside the effect, it’s recommended that you declare it inside
the effect. Because initializeBlockchainNode() is used only by the preceding
useEffect(), we declared it inside this hook.
 This useEffect() hook makes the initial connection to the server and initializes
the blockchain, so we want it to be invoked only once. To ensure this, we tried to use
an empty array for the second argument, as the documentation prescribes. An empty
array means that this effect doesn’t use any values that could participate in the React
data flow, so it’s safe to invoke only once.
 But React noticed that this hook uses the component-scoped handleServerMessages() function, which, being a closure, could potentially capture a component’s
state variable. This might become outdated on the next rendering, but our effect
would keep the reference to handleServerMessages(), which captured the old state.
Because of this, React forced us to replace the empty array with [handleServerMessages]. However, because we won’t be changing state inside this callback, this
useEffect() will be invoked only once.
Listing 14.10 The useEffect() hook attached to handleServerMessages
Declares the
initializeBlockchainNode() function Connects to the WebSocket
server providing the callback
Requests the
longest chain
The
blockchain
already has
some blocks.
No blocks exist yet;
creates the genesis block
Updates the App state
Invokes the
initializeBlockchainNode() function
Disconnects from WebSocket when
the App component is destroyed This hook is attached to
handleServerMessages.
The smart App component 361
 Note the return statement at the end of the useEffect() shown in listing 14.10.
Returning a function from useEffect() is optional, but if it’s there, React guarantees
that it will call that function when the component is about to be destroyed. If we
established the WebSocket connection in other components (not in the root component), it would be good practice to have a return statement in useEffect() to avoid
memory leaks.
 While we were wrapping the asynchronous function inside the effect in listing
14.10, we initially tried to just add the async keyword as follows:
useEffect(async () => { await ...})
But because any async function returns a Promise, TypeScript started complaining
that “Type ‘Promise<void>’ is not assignable to type ‘() ? void | undefined’.” The
useEffect() hook didn’t like a function that would return a Promise. That’s why we
changed the signature a little bit:
useEffect(() => {
async function initializeBlockchainNode() {...}
initializeBlockchainNode();
}
)
We declared the asynchronous function first, and then invoked it. This made TypeScript happy, and we were able to reuse the same code as in chapters 10 and 12 inside
the React’s useEffect().
14.3.4 Memoization with the useCallback() hook
Now let’s talk about yet another React hook, useCallback(), which returns a memoized
callback. Memoization is an optimization technique that stores the results of function
calls and returns the cached result when the same inputs occur again.
 Suppose you have a function, doSomething(a,b), that performs some long-running
calculations on the supplied arguments. Let’s say the calculations take 30 seconds, and
that this is a pure function that always returns the same result if the arguments are the
same. The following code snippet should run 90 seconds, right?
let result: number;
result = doSomething(2, 3); // 30 sec
result = doSomething(10, 15); // 30 sec
result = doSomething(2, 3); // 30 sec
But if we saved the results for each pair of arguments in a table, we wouldn’t need to
invoke doSomething(2, 3) for the second time, because we already have the result for
this pair. We’d just need to do a quick lookup in our table of results. This is an example where memoization could optimize the code so it would run for a little more than
60 seconds instead of 90 seconds.
362 CHAPTER 14 Developing a blockchain client in React.js
 In React components you don’t need to implement memoization for each function manually—you can use the provided useCallback() hook. Listing 14.11 shows a
useCallback() hook that returns a memoized version of the doSomething() function.
const memoizedCallback = useCallback(
() => {
doSomething(a, b);
},
[a, b],
);
If the doSomething() function is a part of the React component, memoization will
prevent this function from being unnecessarily re-created during each UI rendering
unless its dependencies, a or b, change.
 In our App component, we wrapped all the functions that handle messages from
the WebSocket server, such as handleServerMessages(), inside the useCallback()
hook. Figure 14.7 shows a screenshot of the App component’s code with the collapsed
bodies of the functions wrapped inside useCallback().
Listing 14.11 Wrapping a function in a useCallback() hook
The useCallback() hook
The memoized doSomething() function
Dependencies of doSomething()
Dependencies
Wraps the function
in useCallback()
Figure 14.7 Memoized functions in the App component
The smart App component 363
In figure 14.7, each of the variables declared in lines 15, 23, 33, and 38 is local to the
App component, so React assumes that their values (the function expressions) can
change. By wrapping the bodies of these functions in useCallback() hooks, we
instruct React to reuse the same function instance on each rendering, which makes it
more efficient.
 Look at the last line of listing 14.10—handleServerMessages is a dependency of
useEffect(). Technically, if instead of the function expression const handleNewBlockRequest = useCallback() we used function handleNewBlockRequest(), the
app would still work, but each function would be recreated on each rendering.
 In lines 21, 31, and 36 in figure 14.7, the array of dependencies is empty, which tells
us that these callbacks can’t have any stale values, and there’s no need for any dependencies. In lines 48–49, we listed the variables handleGetLongestChainRequest,
handleNewBlockAnnouncement, and handleNewBlockRequest as dependencies, which
are used inside the handleServerMessages() callback, as seen in the following listing.
Didn’t we state in the previous paragraph that these callbacks won’t create a stale state
situation? We did, but React can’t peek inside these callback to see this.
const handleServerMessages = useCallback((message: Message) => {
switch (message.type) {
case MessageTypes.GetLongestChainRequest:
return handleGetLongestChainRequest(message);
case MessageTypes.NewBlockRequest :
return handleNewBlockRequest(message);
case MessageTypes.NewBlockAnnouncement :
return handleNewBlockAnnouncement(message);
default: {
console.log(`Received message of unknown type: "${message.type}"`);
}
}
}, [
handleGetLongestChainRequest,
handleNewBlockAnnouncement,
handleNewBlockRequest
]);
Besides the functions that communicate with the WebSocket server, the App component
has three functions that communicate with the BlockchainNode instance. The next listing shows the addTransaction(), generateBlock(), and addBlock() functions. We
didn’t change the logic of these operations, but each of these functions now ends with
an invocation of the React-specific setState(), which requests re-rendering.
Listing 14.12 The handleServerMessages() callback
Uses a dependency
inside useCallback()
Declares a dependency
of useCallback()
364 CHAPTER 14 Developing a blockchain client in React.js
function addTransaction(transaction: Transaction): void {
node.addTransaction(transaction);
setStatus(getStatus(node));
}
async function generateBlock() {
server.requestNewBlock(node.pendingTransactions);
const miningProcessIsDone = node.mineBlockWith(node.pendingTransactions);
setStatus(getStatus(node));
const newBlock = await miningProcessIsDone;
addBlock(newBlock);
}
async function addBlock(block: Block, notifyOthers = true): Promise<void> {
try {
await node.addBlock(block);
if (notifyOthers) {
server.announceNewBlock(block);
}
} catch (error) {
console.log(error.message);
}
setStatus(getStatus(node));
}
NOTE The invocation of the addTransaction() and generateBlock() functions is driven by the TransactionForm and PendingTransactionsPanel child
components respectively. We’ll review the relevant code in the next section.
The addTransaction() function accumulates pending transactions, which are handled
by the generateBlock() function, and when one of the nodes completes the mining
first, the addBlock() function attempts to add it to the blockchain. If our node was the
first in mining, this function would add the new block and notify others; otherwise, the
new block will arrive from the server via the handleNewBlockAnnouncement() callback.
 The getStatus() function is located in the App.tsx file, but it’s implemented outside of the App component.
function getStatus(node: BlockchainNode): string {
return node.chainIsEmpty ? '⏳ Initializing the blockchain...' :
node.isMining ? '⏳ Mining a new block...' :
node.noPendingTransactions ? '✉ Add one or more transactions.' :
'✅ Ready to mine a new block.';
}
Listing 14.13 Three more functions from the App component
Listing 14.14 The getStatus() function from App.tsx
The invocation of
this function is
initiated by the
child component.
Updates the
component’s
status
The TransactionForm presentation component 365
When the App component invokes setStatus(getStatus(node));, there are two possible outcomes: either getStatus() will return the same status as before, or it will
return a new one. If the status didn’t change, calling setStatus() won’t result in rerendering the UI, and vice versa.
 We’ve now covered the React specifics for the smart App component. Let’s now get
familiar with the code of the presentation components.
14.4 The TransactionForm presentation component
Figure 14.8 shows the UI of the TransactionForm component, which allows the user
to enter the names of the sender and recipient, as well as the transaction amount.
When the user clicks the ADD TRANSACTION button, this information has to be sent
to the parent App component (the smart component), because it knows how to process this data.
The JSX of the App component that communicates with TransactionForm is shown in
the following listing.
<TransactionForm
onAddTransaction={addTransaction}
disabled={node.isMining || node.chainIsEmpty}
/>
From this JSX, we can guess that when the TransactionForm component invokes its
onAddTransaction() function, the App component will invoke its addTransaction()
(shown earlier in listing 14.13). We can also see that the child component has the
disabled props, driven by the status of the node variable, which holds the reference to
the BlockchainNode instance.
 The following listing shows the first half of the code in the TransactionForm.tsx file.
import React, { ChangeEvent, FormEvent, useState } from 'react';
import { Transaction } from '../lib/blockchain-node';
type TransactionFormProps = {
onAddTransaction: (transaction: Transaction) => void,
Listing 14.15 The App component’s JSX for rendering TransactionForm
Listing 14.16 The first part of TransactionForm.tsx
Figure 14.8 The UI of the TransactionForm component
Child’s onAddTransaction() results
in parent’s addTransaction()
Conditionally enables
or disables the child
Props for sending
data to the parent
366 CHAPTER 14 Developing a blockchain client in React.js
disabled: boolean
};
const defaultFormValue = {recipient: '', sender: '', amount: 0};
const TransactionForm: React.FC<TransactionFormProps> =
➥ ({onAddTransaction, disabled}) => {
const [formValue, setFormValue] = useState<Transaction>(defaultFormValue);
const isValid = formValue.sender && formValue.recipient &&
➥ formValue.amount > 0;
function handleInputChange({ target }: ChangeEvent<HTMLInputElement>) {
setFormValue({
...formValue,
[target.name]: target.value
});
}
function handleFormSubmit(event: FormEvent<HTMLFormElement>) {
event.preventDefault();
onAddTransaction(formValue);
setFormValue(defaultFormValue);
}
return (
// The JSX is shown in listing 14.17
);
}
When the user clicks the ADD TRANSACTION button, the TransactionForm
component has to invoke some function on the parent. Since React doesn’t want the
child to know the internals of the parent, the child just gets the onAddTransaction
props, but it has to know the correct signature of the parent’s function that corresponds
to onAdd-Transaction. The following line maps the name of the onAddTransaction
props to the signature of the function to be called on the parent:
onAddTransaction: (transaction: Transaction) => void,
In listing 14.13, you saw that the parent’s addTransaction() function has the signature (transaction: Transaction) => void. In listing 14.6, you can easily find the
line that maps the parent’s addTransaction to the child’s onAddTransaction.
 The TransactionForm component renders a simple form and defines only one
state variable, formValue, which is the object containing the current form’s values.
When the user types in the input fields, the handleInputChange() event handler is
invoked and saves the entered value in formValue. In listing 14.17, you’ll see that this
event handler is assigned to each input field of the form.
 In the handleInputChange() handler, we use destructuring to extract the target
object, which points at the input field that triggered this event. We get the name and
value of the DOM element dynamically from the target object. The target.name
Props for getting data from the parent
The object with the default
values for the form
This component
accepts two props.
The isValid flag specifies whether The component’s state the button can be enabled.
One event handler for all input fields
Passes the formValue
object to the parent
Resets the form
The TransactionForm presentation component 367
property will contain the name of the field, and target.value contains its value. In
Chrome dev tools, put a breakpoint in the handleInputChange() method to see how
this works. By invoking setFormValue(), we change the component’s state to reflect
the current values of the input fields.
TIP While invoking setState(), we use object cloning with the spread operator. This technique is described in section A.7 of the appendix.
The default values for the transaction form are stored in the defaultFormValue variable, and they are used for the initial form rendering as well as resetting the form
after the ADD TRANSACTION button is clicked. When the user clicks on this button,
the handleFormSubmit() function invokes onAddTransaction(), passing the formValue object to the parent (the App component).
 The following listing shows the JSX of the TransactionForm component. It’s a
form with three input fields and a submit button.
return (
<>
<h2>New transaction</h2>
<form className="add-transaction-form" onSubmit={handleFormSubmit}>
<input
type="text"
name="sender"
placeholder="Sender"
autoComplete="off"
disabled={disabled}
value={formValue.sender}
onChange={handleInputChange}
/>
<span className="hidden-xs">•</span>
<input
type="text"
name="recipient"
placeholder="Recipient"
autoComplete="off"
disabled={disabled}
value={formValue.recipient}
onChange={handleInputChange}
/>
<input
type="number"
name="amount"
placeholder="Amount"
disabled={disabled}
value={formValue.amount}
onChange={handleInputChange}
/>
<button type="submit"
Listing 14.17 The second part of TransactionForm.tsx
Enables
the button
only when
the form
is valid
Binds the value from the
corresponding state property
Invokes handleInputChange
on every state mutation
Binds
 the disable
attribute to
the disabled
props
Binds the value from the
corresponding state property
Invokes handleInputChange
on every state mutation
Binds
 the disable
attribute to
the disabled
props
Binds the value from the
corresponding state property
Invokes handleInputChange
on every state mutation
368 CHAPTER 14 Developing a blockchain client in React.js
disabled={!isValid || disabled}
className="ripple">ADD TRANSACTION</button>
</form>
</>
);
React handles HTML forms differently than other elements because a form has an
internal state—an object with all the form fields’ values. In React, you can turn a regular form field into a controlled component by binding the state object’s property (such as
formValue.sender) to its attribute value and adding an onChange event handler.
 Our form has three controlled components (input fields), and every state mutation will have an associated handler function. In the TransactionForm component,
handleInputChange() is such a handler function. As you can see in listing 14.16,
we’re just cloning the state object in handleInputChange(), but you can put any app
logic in such a handler.
 We’d like to stress again that TransactionForm is a presentation component that
only knows how to present its values and which function to invoke when the form is
submitted. It has no knowledge about its parent and doesn’t communicate with any
external services, which makes it 100% reusable.
14.5 The PendingTransactionsPanel presentation component
Each time the user clicks the ADD TRANSACTION button in the TransactionForm
component, the entered transaction should be passed to the PendingTransactionsPanel. Figure 14.9 shows this component rendered with two pending transactions.
These two components don’t know about each other, so the App component can play
the role of mediator in passing the data from one component to another.
Listing 14.18 shows a snippet of the App component’s JSX that renders the PendingTransactionsPanel component. The App component communicates with PendingTransactionsPanel much like it does with TransactionForm. This component gets
three props from App.
Enables the button only
when the form is valid
Figure 14.9 The UI of the PendingTransactionsPanel component
The PendingTransactionsPanel presentation component 369
<PendingTransactionsPanel
formattedTransactions={formatTransactions(node.pendingTransactions)}
onGenerateBlock={generateBlock}
disabled={node.isMining || node.noPendingTransactions}
/>
The first props is formattedTransactions, and the App component passes it in to
PendingTransactionsPanel for rendering. While reviewing the code from the
App.tsx file, we left out its formatTransactions() utility function, which simply creates a nicely formatted message about this transaction. The following listing shows the
code of the self-explanatory formatTransactions() function, located in the App.tsx
file outside of the App component. Figure 14.9 shows what the formatted transactions
look like.
function formatTransactions(transactions: Transaction[]): string {
return transactions.map(t =>`${t.sender} • ${t.recipient}: $${t.amount}`)
.join('\n');
}
The second props, onGeneratedBlock, is a reference to the function that should be
called on the parent of PendingTransactionsPanel when the user clicks the GENERATE BLOCK button.
 The following listing shows the code of the PendingTransactionsPanel component. It’s pretty straightforward because it doesn’t contain any forms and doesn’t need
to handle user input, except for clicking the GENERATE BLOCK button.
import React from 'react';
type PendingTransactionsPanelProps = {
formattedTransactions: string;
onGenerateBlock: () => void;
disabled: boolean;
}
const PendingTransactionsPanel: React.FC<PendingTransactionsPanelProps> =
({formattedTransactions, onGenerateBlock, disabled}) => {
return (
<>
<h2>Pending transactions</h2>
<pre className="pending-transactions__list">
Listing 14.18 The App component’s JSX for rendering PendingTransactionsPanel
Listing 14.19 The formatTransactions() function
Listing 14.20 The PendingTransactionsPanel.tsx file
Formats the transaction and
passes it to this component The child’s onGenerateBlock() results in
the parent’s generateBlock().
This child has to be
disabled initially.
The props for formatted
transactions
The onGenerateBlock props
must use this method signature.
Aligns everything (including
subsequent sibling elements)
to the right side of the parent
container
370 CHAPTER 14 Developing a blockchain client in React.js
{formattedTransactions || 'No pending transactions yet.'}
</pre>
<div className="pending-transactions__form">
<button disabled={disabled}
onClick={() => onGenerateBlock()}
className="ripple"
type="button">GENERATE BLOCK</button>
</div>
<div className="clear"></div>
</>
);
}
export default PendingTransactionsPanel;
When the user clicks the GENERATE BLOCK button, we invoke the onGenerateBlock()
props, which in turn invokes the generateBlock() function on the App component.
 In the .pending-transactions__form style selector (in index.css), we use float:
right, which forces everything to be aligned to the right side of the parent container,
including subsequent sibling elements. The clear style is defined as clear: both, and
it stops the right-alignment rule so we don’t break the following Current Blocks section.
 The last component we have to review is the one that shows the blockchain at the
bottom of the window.
14.6 The BlocksPanel and BlockComponent presentation components
When the user clicks the GENERATE BLOCK button in the PendingTransactionsPanel component, all active blocks in the blockchain start the mining process, and
after the consensus, a new block will be added to the blockchain and rendered in the
BlocksPanel component, which can parent one or more BlockComponent components. Figure 14.10 shows the rendering of BlocksPanel with a two-block blockchain.
During the process of block mining and getting the consensus, the instances of
BlockchainNode and WebsocketController are involved, but because it’s a presentation component, BlocksPanel doesn’t directly communicate with either of these
objects. That work is delegated to the smart App component. The BlockPanel component doesn’t send any data to its parent; its goal is to render the blockchain provided
via the block props:
<BlocksPanel blocks={node.chain} />
Displays either a provided
transaction or the default text
Invokes the
onGenerateBlock() props
Clears the right-alignment
Figure 14.10 The UI
of the BlocksPanel
component
The BlocksPanel and BlockComponent presentation components 371
The BlocksPanel.tsx file contains the code of two components: BlocksPanel and BlockComponent. The following listing shows the code of BlockComponent, which renders a
single block in the blockchain. Figure 14.10 showed two instances of BlockComponent.
const BlockComponent: React.FC<{ index: number, block: Block }> =
➥ ({ index, block }) => {
const formattedTransactions = formatTransactions(block.transactions);
const timestamp = new Date(block.timestamp).toLocaleTimeString();
return (
<div className="block">
<div className="block__header">
<span className="block__index">#{index}</span>
<span className="block__timestamp">{timestamp}</span>
</div>
<div className="block__hashes">
<div className="block__hash">
<div className="block__label">• PREV HASH</div>
<div className="block__hash-value">{block.previousHash}</div>
</div>
<div className="block__hash">
<div className="block__label">THIS HASH</div>
<div className="block__hash-value">{block.hash}</div>
</div>
</div>
<div>
<div className="block__label">TRANSACTIONS</div>
<pre className="block__transactions">{formattedTransactions
|| 'No transactions'}</pre>
</div>
</div>
);
}
TIP We use the symbols __ and -- in naming some styles, as recommended by
the Block Element Modifier (BEM) methodology described on the Get BEM
website, at http://getbem.com.
The following listing shows the BlocksPanel component, which serves as a container
for all BlockComponent components.
import React from 'react';
import { Block, Transaction } from '../lib/blockchain-node';
type BlocksPanelProps = {
Listing 14.21 The BlockComponent
Listing 14.22 The BlocksPanel component
The formattedTransaction() function
is the same as in the App component.
The block number
The previous block’s hash
This block’s
hash
The block’s
transactions
372 CHAPTER 14 Developing a blockchain client in React.js
blocks: Block[]
};
const BlocksPanel: React.FC<BlocksPanelProps> = ({blocks}) => {
return (
<>
<h2>Current blocks</h2>
<div className="blocks">
<div className="blocks__ribbon">
{blocks.map((b, i) =>
<BlockComponent key={b.hash} index={i} block={b}>
➥ </BlockComponent>)}
</div>
<div className="blocks__overlay"></div>
</div>
</>
);
}
The BlocksPanel gets an array of Block instances from the App component and
applies the Array.map() method to convert each Block object into a BlockComponent.
The map() method passes the key (the hash code), the unique index of the block, and
the Block object to each instance of BlockComponent.
 The props of BlockComponent are index and block. Note that we’re assigning the
block hash as a key props to each instance of the BlockComponent, even though the
key props was never mentioned in its code in listing 14.21. That’s because when you
have a collection of rendered objects (such as list items or multiple instances of the
same component), React needs a way to uniquely identify each component during its
reconciliation with the Virtual DOM, to keep track of the data associated with each
DOM element.
 If you don’t use a unique value for the key props on each BlockComponent, React
will print a warning in the browser console stating that “Each child in array or iterator
should have a unique key props.” In our app, this won’t mess up the data, because we
are only adding new blocks to the end of the array, but if the user could add or
remove arbitrary elements from a collection of UI components, not using a unique
key props could create a situation where a UI element and the underlying data
wouldn’t match.
 This concludes our code review of the React version of our blockchain app.
Summary
 In development, our React web app was deployed under a Webpack dev server,
but it was communicating with another (messaging) server as well. To accomplish this, we declared custom environment variables with the messaging
server’s URL. For a WebSocket server, this was enough, but if you used other
HTTP servers, you’d have to proxy the HTTP requests as described in the Create React App documentation at http://mng.bz/gV9v.
An array of Block instances
is the only props here.
Uses Array.map() to turn
the data into components
Passes key, index,
and block props to
BlockComponent
Summary 373
 Typically, the UI of a React app consists of smart and presentation components.
Don’t place the application logic in the presentation components, which are
meant for presenting the data received from other components. The presentation components can also implement the interaction with the user and send the
user’s input to other components.
 A child component should never call an API from its parent directly. Using
props, the parent component should give the child a name that’s mapped to a
function that should be called as the result of a child’s action. The child will
invoke the provided function reference without knowing the real name of the
parent’s function.
 To prevent unnecessarily re-creating function expressions located in React components, consider using memoization with the useCallback() hook.
374

