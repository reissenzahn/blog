


VARIABLES

```ts
// variables

// type annotations can be used to explicitly specify the type of a variable
let myName: string = "Alice";

// types can be automatically inferred from the context
let myName = "Alice";
```


FUNCTIONS

```ts
// functions

// type annotations can be provided for parameters and return types
function greet(name: string) {
  console.log("Hello, " + name.toUpperCase() + "!!");
}

function getFavoriteNumber(): number {
  return 26;
}
```

// No type annotations here, but TypeScript can spot the bug
const names = ["Alice", "Bob", "Eve"];
 
// Contextual typing for function
names.forEach(function (s) {
  console.log(s.toUppercase());
Property 'toUppercase' does not exist on type 'string'. Did you mean 'toUpperCase'?
});
 
// Contextual typing also applies to arrow functions
names.forEach((s) => {
  console.log(s.toUppercase());
Property 'toUppercase' does not exist on type 'string'. Did you mean 'toUpperCase'?
});



ARRAYS

```ts
// arrays

// the type of an array can be specified using the T[] syntax
let odds: number[] = [1, 3, 5];

// alternatively the Array<T> generic type can be used
let evens: Array<number> = [2, 4, 6];
```


ANY

```ts
// the any type disables any type checking (it is the default type if the compiler cannot infer a type)
let obj: any = { x: 0 };

obj.foo();
obj();
obj.bar = 100;
obj = "hello";
const n: number = obj;
```


OBJECTS

```ts
// an object type is defined by listing its properties and their types (separated by either , or ;)
function printCoord(pt: { x: number; y: number }) {
  console.log("The coordinate's x value is " + pt.x);
  console.log("The coordinate's y value is " + pt.y);
}

printCoord({ x: 3, y: 7 });

// a property can be made optional by adding a ? after the property name
function printName(obj: { first: string; last?: string }) {
  // ...
}

printName({ first: "Bob" });
printName({ first: "Alice", last: "Alisson" });

// you have to check for undefined when reading from an optional property
function printName(obj: { first: string; last?: string }) {
  if (obj.last !== undefined) {
    // OK
    console.log(obj.last.toUpperCase());
  }
 
  // A safe alternative using modern JavaScript syntax:
  console.log(obj.last?.toUpperCase());
}
```


UNIONS

```ts
// a union type if formed from two or more other types (called union members) and represents values that may be any one of those types
function printId(id: number | string) {
  console.log("Your ID is: " + id);
}

printId(101);
printId("202");


// you can only perform an operation on a union type if it is valid for every member of the union and so narrowing may be necessary
function getFirstThree(x: number[] | string) {
  return x.slice(0, 3);
}

// narrowing occurs when TypeScript can deduce a more specific type for a value based on the structure of the code
function printId(id: number | string) {
  if (typeof id === "string") {
    console.log(id.toUpperCase());
  } else {
    console.log(id);
  }
}

function welcomePeople(x: string[] | string) {
  if (Array.isArray(x)) {
    console.log("Hello, " + x.join(" and "));
  } else {
    console.log("Welcome lone traveler " + x);
  }
}
```


TYPE ALIASES

```ts
// a type alias is a name for a type
type Point = {
  x: number;
  y: number;
};
 
function printCoord(pt: Point) {
  console.log("The coordinate's x value is " + pt.x);
  console.log("The coordinate's y value is " + pt.y);
}
 
printCoord({ x: 100, y: 100 });

// a type alias can give a name to any type
type ID = number | string;


// using a type alias is exactly the same as if you had written the aliased type
type UserInputSanitizedString = string;
 
function sanitizeInput(str: string): UserInputSanitizedString {
  return sanitize(str);
}
 
let userInput = sanitizeInput(getInput());
 
userInput = "new input";
```


INTERFACES

```ts
// an interface declaration is another way to name an object type
interface Point {
  x: number;
  y: number;
}

function printCoord(pt: Point) {
  console.log("The coordinate's x value is " + pt.x);
  console.log("The coordinate's y value is " + pt.y);
}
 
// TypeScript is only concerned with the structure of the value we passed to printCoord (i.e. that it has the expected properties)
printCoord({ x: 100, y: 100 });


// interface are similar to type aliases except that a type cannot be re-opened to add new properties while an interface is extendable
interface Animal {
  name: string
}

interface Bear extends Animal {
  honey: boolean
}

const bear = getBear() 
bear.name
bear.honey


Extending a type via intersections

type Animal = {
  name: string
}

type Bear = Animal & { 
  honey: boolean 
}

const bear = getBear();
bear.name;
bear.honey;
        
Adding new fields to an existing interface

interface Window {
  title: string
}

interface Window {
  ts: TypeScriptAPI
}

const src = 'const a = "Hello World"';
window.ts.transpileModule(src, {});
        
A type cannot be changed after being created

type Window = {
  title: string
}

type Window = {
  ts: TypeScriptAPI
}

 // Error: Duplicate identifier 'Window'.

Type aliases may not participate in declaration merging, but interfaces can.
Interfaces may only be used to declare the shapes of objects, not rename primitives.
Interface names will always appear in their original form in error messages, but only when they are used by name.
For the most part, you can choose based on personal preference, and TypeScript will tell you if it needs something to be the other kind of declaration.

// in short: use interface until you need to use features from type
```


TYPE ASSERTIONS

```ts
// a type assertion is used to specify a more specific type
const myCanvas = document.getElementById("main_canvas") as HTMLCanvasElement;

You can also use the angle-bracket syntax (except if the code is in a .tsx file), which is equivalent:

const myCanvas = <HTMLCanvasElement>document.getElementById("main_canvas");

Because type assertions are removed at compile-time, there is no runtime checking associated with a type assertion. There won’t be an exception or null generated if the type assertion is wrong.

TypeScript only allows type assertions which convert to a more specific or less specific version of a type.

Sometimes this rule can be too conservative and will disallow more complex coercions that might be valid. If this happens, you can use two assertions, first to any (or unknown, which we’ll introduce later), then to the desired type:

const a = (expr as any) as T;




Literal Types

In addition to the general types string and number, we can refer to specific strings and numbers in type positions.

 
const constantString = "Hello World";
// Because `constantString` can only represent 1 possible string, it
// has a literal type representation
constantString;
      
const constantString: "Hello World"

Try
By themselves, literal types aren’t very valuable:

let x: "hello" = "hello";
// OK
x = "hello";
// ...
x = "howdy";

Type '"howdy"' is not assignable to type '"hello"'.
Try
It’s not much use to have a variable that can only have one value!

But by combining literals into unions, you can express a much more useful concept - for example, functions that only accept a certain set of known values:

function printText(s: string, alignment: "left" | "right" | "center") {
  // ...
}
printText("Hello, world", "left");
printText("G'day, mate", "centre");
Argument of type '"centre"' is not assignable to parameter of type '"left" | "right" | "center"'.
Try
Numeric literal types work the same way:

function compare(a: string, b: string): -1 | 0 | 1 {
  return a === b ? 0 : a > b ? 1 : -1;
}
Try
Of course, you can combine these with non-literal types:

interface Options {
  width: number;
}
function configure(x: Options | "auto") {
  // ...
}
configure({ width: 100 });
configure("auto");
configure("automatic");
Argument of type '"automatic"' is not assignable to parameter of type 'Options | "auto"'.
Try
There’s one more kind of literal type: boolean literals. There are only two boolean literal types, and as you might guess, they are the types true and false. The type boolean itself is actually just an alias for the union true | false.

Literal Inference
When you initialize a variable with an object, TypeScript assumes that the properties of that object might change values later. For example, if you wrote code like this:

const obj = { counter: 0 };
if (someCondition) {
  obj.counter = 1;
}
Try
TypeScript doesn’t assume the assignment of 1 to a field which previously had 0 is an error. Another way of saying this is that obj.counter must have the type number, not 0, because types are used to determine both reading and writing behavior.

The same applies to strings:

const req = { url: "https://example.com", method: "GET" };
handleRequest(req.url, req.method);
Argument of type 'string' is not assignable to parameter of type '"GET" | "POST"'.
Try
In the above example req.method is inferred to be string, not "GET". Because code can be evaluated between the creation of req and the call of handleRequest which could assign a new string like "GUESS" to req.method, TypeScript considers this code to have an error.

There are two ways to work around this.

You can change the inference by adding a type assertion in either location:

// Change 1:
const req = { url: "https://example.com", method: "GET" as "GET" };
// Change 2
handleRequest(req.url, req.method as "GET");
Try
Change 1 means “I intend for req.method to always have the literal type "GET"”, preventing the possible assignment of "GUESS" to that field after. Change 2 means “I know for other reasons that req.method has the value "GET"“.

You can use as const to convert the entire object to be type literals:

const req = { url: "https://example.com", method: "GET" } as const;
handleRequest(req.url, req.method);
Try
The as const suffix acts like const but for the type system, ensuring that all properties are assigned the literal type instead of a more general version like string or number.

null
and
undefined
JavaScript has two primitive values used to signal absent or uninitialized value: null and undefined.

TypeScript has two corresponding types by the same names. How these types behave depends on whether you have the strictNullChecks option on.

strictNullChecks
off
With strictNullChecks off, values that might be null or undefined can still be accessed normally, and the values null and undefined can be assigned to a property of any type. This is similar to how languages without null checks (e.g. C#, Java) behave. The lack of checking for these values tends to be a major source of bugs; we always recommend people turn strictNullChecks on if it’s practical to do so in their codebase.

strictNullChecks
on
With strictNullChecks on, when a value is null or undefined, you will need to test for those values before using methods or properties on that value. Just like checking for undefined before using an optional property, we can use narrowing to check for values that might be null:

function doSomething(x: string | null) {
  if (x === null) {
    // do nothing
  } else {
    console.log("Hello, " + x.toUpperCase());
  }
}
Try
Non-null Assertion Operator (Postfix
!
)
TypeScript also has a special syntax for removing null and undefined from a type without doing any explicit checking. Writing ! after any expression is effectively a type assertion that the value isn’t null or undefined:

function liveDangerously(x?: number | null) {
  // No error
  console.log(x!.toFixed());
}
Try
Just like other type assertions, this doesn’t change the runtime behavior of your code, so it’s important to only use ! when you know that the value can’t be null or undefined.


NARROWING

Narrowing
Imagine we have a function called padLeft.

function padLeft(padding: number | string, input: string): string {
  throw new Error("Not implemented yet!");
}
Try
If padding is a number, it will treat that as the number of spaces we want to prepend to input. If padding is a string, it should just prepend padding to input. Let’s try to implement the logic for when padLeft is passed a number for padding.

function padLeft(padding: number | string, input: string) {
  return " ".repeat(padding) + input;
Argument of type 'string | number' is not assignable to parameter of type 'number'.
  Type 'string' is not assignable to type 'number'.
}
Try
Uh-oh, we’re getting an error on padding. TypeScript is warning us that adding a number | string to a number might not give us what we want, and it’s right. In other words, we haven’t explicitly checked if padding is a number first, nor are we handling the case where it’s a string, so let’s do exactly that.

function padLeft(padding: number | string, input: string) {
  if (typeof padding === "number") {
    return " ".repeat(padding) + input;
  }
  return padding + input;
}
Try
If this mostly looks like uninteresting JavaScript code, that’s sort of the point. Apart from the annotations we put in place, this TypeScript code looks like JavaScript. The idea is that TypeScript’s type system aims to make it as easy as possible to write typical JavaScript code without bending over backwards to get type safety.

While it might not look like much, there’s actually a lot going under the covers here. Much like how TypeScript analyzes runtime values using static types, it overlays type analysis on JavaScript’s runtime control flow constructs like if/else, conditional ternaries, loops, truthiness checks, etc., which can all affect those types.

Within our if check, TypeScript sees typeof padding === "number" and understands that as a special form of code called a type guard. TypeScript follows possible paths of execution that our programs can take to analyze the most specific possible type of a value at a given position. It looks at these special checks (called type guards) and assignments, and the process of refining types to more specific types than declared is called narrowing. In many editors we can observe these types as they change, and we’ll even do so in our examples.

function padLeft(padding: number | string, input: string) {
  if (typeof padding === "number") {
    return " ".repeat(padding) + input;
                        
(parameter) padding: number
  }
  return padding + input;
           
(parameter) padding: string
}
Try
There are a couple of different constructs TypeScript understands for narrowing.

typeof
type guards
As we’ve seen, JavaScript supports a typeof operator which can give very basic information about the type of values we have at runtime. TypeScript expects this to return a certain set of strings:

"string"
"number"
"bigint"
"boolean"
"symbol"
"undefined"
"object"
"function"
Like we saw with padLeft, this operator comes up pretty often in a number of JavaScript libraries, and TypeScript can understand it to narrow types in different branches.

In TypeScript, checking against the value returned by typeof is a type guard. Because TypeScript encodes how typeof operates on different values, it knows about some of its quirks in JavaScript. For example, notice that in the list above, typeof doesn’t return the string null. Check out the following example:

function printAll(strs: string | string[] | null) {
  if (typeof strs === "object") {
    for (const s of strs) {
Object is possibly 'null'.
      console.log(s);
    }
  } else if (typeof strs === "string") {
    console.log(strs);
  } else {
    // do nothing
  }
}
Try
In the printAll function, we try to check if strs is an object to see if it’s an array type (now might be a good time to reinforce that arrays are object types in JavaScript). But it turns out that in JavaScript, typeof null is actually "object"! This is one of those unfortunate accidents of history.

Users with enough experience might not be surprised, but not everyone has run into this in JavaScript; luckily, TypeScript lets us know that strs was only narrowed down to string[] | null instead of just string[].

This might be a good segue into what we’ll call “truthiness” checking.

Truthiness narrowing
Truthiness might not be a word you’ll find in the dictionary, but it’s very much something you’ll hear about in JavaScript.

In JavaScript, we can use any expression in conditionals, &&s, ||s, if statements, Boolean negations (!), and more. As an example, if statements don’t expect their condition to always have the type boolean.

function getUsersOnlineMessage(numUsersOnline: number) {
  if (numUsersOnline) {
    return `There are ${numUsersOnline} online now!`;
  }
  return "Nobody's here. :(";
}
Try
In JavaScript, constructs like if first “coerce” their conditions to booleans to make sense of them, and then choose their branches depending on whether the result is true or false. Values like

0
NaN
"" (the empty string)
0n (the bigint version of zero)
null
undefined
all coerce to false, and other values get coerced true. You can always coerce values to booleans by running them through the Boolean function, or by using the shorter double-Boolean negation. (The latter has the advantage that TypeScript infers a narrow literal boolean type true, while inferring the first as type boolean.)

// both of these result in 'true'
Boolean("hello"); // type: boolean, value: true
!!"world"; // type: true,    value: true
Try
It’s fairly popular to leverage this behavior, especially for guarding against values like null or undefined. As an example, let’s try using it for our printAll function.

function printAll(strs: string | string[] | null) {
  if (strs && typeof strs === "object") {
    for (const s of strs) {
      console.log(s);
    }
  } else if (typeof strs === "string") {
    console.log(strs);
  }
}
Try
You’ll notice that we’ve gotten rid of the error above by checking if strs is truthy. This at least prevents us from dreaded errors when we run our code like:

TypeError: null is not iterable
Keep in mind though that truthiness checking on primitives can often be error prone. As an example, consider a different attempt at writing printAll

function printAll(strs: string | string[] | null) {
  // !!!!!!!!!!!!!!!!
  //  DON'T DO THIS!
  //   KEEP READING
  // !!!!!!!!!!!!!!!!
  if (strs) {
    if (typeof strs === "object") {
      for (const s of strs) {
        console.log(s);
      }
    } else if (typeof strs === "string") {
      console.log(strs);
    }
  }
}
Try
We wrapped the entire body of the function in a truthy check, but this has a subtle downside: we may no longer be handling the empty string case correctly.

TypeScript doesn’t hurt us here at all, but this is behavior worth noting if you’re less familiar with JavaScript. TypeScript can often help you catch bugs early on, but if you choose to do nothing with a value, there’s only so much that it can do without being overly prescriptive. If you want, you can make sure you handle situations like these with a linter.

One last word on narrowing by truthiness is that Boolean negations with ! filter out from negated branches.

function multiplyAll(
  values: number[] | undefined,
  factor: number
): number[] | undefined {
  if (!values) {
    return values;
  } else {
    return values.map((x) => x * factor);
  }
}
Try
Equality narrowing
TypeScript also uses switch statements and equality checks like ===, !==, ==, and != to narrow types. For example:

function example(x: string | number, y: string | boolean) {
  if (x === y) {
    // We can now call any 'string' method on 'x' or 'y'.
    x.toUpperCase();
          
(method) String.toUpperCase(): string
    y.toLowerCase();
          
(method) String.toLowerCase(): string
  } else {
    console.log(x);
               
(parameter) x: string | number
    console.log(y);
               
(parameter) y: string | boolean
  }
}
Try
When we checked that x and y are both equal in the above example, TypeScript knew their types also had to be equal. Since string is the only common type that both x and y could take on, TypeScript knows that x and y must be a string in the first branch.

Checking against specific literal values (as opposed to variables) works also. In our section about truthiness narrowing, we wrote a printAll function which was error-prone because it accidentally didn’t handle empty strings properly. Instead we could have done a specific check to block out nulls, and TypeScript still correctly removes null from the type of strs.

function printAll(strs: string | string[] | null) {
  if (strs !== null) {
    if (typeof strs === "object") {
      for (const s of strs) {
                       
(parameter) strs: string[]
        console.log(s);
      }
    } else if (typeof strs === "string") {
      console.log(strs);
                   
(parameter) strs: string
    }
  }
}
Try
JavaScript’s looser equality checks with == and != also get narrowed correctly. If you’re unfamiliar, checking whether something == null actually not only checks whether it is specifically the value null - it also checks whether it’s potentially undefined. The same applies to == undefined: it checks whether a value is either null or undefined.

interface Container {
  value: number | null | undefined;
}
 
function multiplyValue(container: Container, factor: number) {
  // Remove both 'null' and 'undefined' from the type.
  if (container.value != null) {
    console.log(container.value);
                           
(property) Container.value: number
 
    // Now we can safely multiply 'container.value'.
    container.value *= factor;
  }
}
Try
The
in
operator narrowing
JavaScript has an operator for determining if an object has a property with a name: the in operator. TypeScript takes this into account as a way to narrow down potential types.

For example, with the code: "value" in x. where "value" is a string literal and x is a union type. The “true” branch narrows x’s types which have either an optional or required property value, and the “false” branch narrows to types which have an optional or missing property value.

type Fish = { swim: () => void };
type Bird = { fly: () => void };
 
function move(animal: Fish | Bird) {
  if ("swim" in animal) {
    return animal.swim();
  }
 
  return animal.fly();
}
Try
To reiterate optional properties will exist in both sides for narrowing, for example a human could both swim and fly (with the right equipment) and thus should show up in both sides of the in check:

type Fish = { swim: () => void };
type Bird = { fly: () => void };
type Human = { swim?: () => void; fly?: () => void };
 
function move(animal: Fish | Bird | Human) {
  if ("swim" in animal) {
    animal;
      
(parameter) animal: Fish | Human
  } else {
    animal;
      
(parameter) animal: Bird | Human
  }
}
Try
instanceof
narrowing
JavaScript has an operator for checking whether or not a value is an “instance” of another value. More specifically, in JavaScript x instanceof Foo checks whether the prototype chain of x contains Foo.prototype. While we won’t dive deep here, and you’ll see more of this when we get into classes, they can still be useful for most values that can be constructed with new. As you might have guessed, instanceof is also a type guard, and TypeScript narrows in branches guarded by instanceofs.

function logValue(x: Date | string) {
  if (x instanceof Date) {
    console.log(x.toUTCString());
               
(parameter) x: Date
  } else {
    console.log(x.toUpperCase());
               
(parameter) x: string
  }
}
Try
Assignments
As we mentioned earlier, when we assign to any variable, TypeScript looks at the right side of the assignment and narrows the left side appropriately.

let x = Math.random() < 0.5 ? 10 : "hello world!";
   
let x: string | number
x = 1;
 
console.log(x);
           
let x: number
x = "goodbye!";
 
console.log(x);
           
let x: string
Try
Notice that each of these assignments is valid. Even though the observed type of x changed to number after our first assignment, we were still able to assign a string to x. This is because the declared type of x - the type that x started with - is string | number, and assignability is always checked against the declared type.

If we’d assigned a boolean to x, we’d have seen an error since that wasn’t part of the declared type.

let x = Math.random() < 0.5 ? 10 : "hello world!";
   
let x: string | number
x = 1;
 
console.log(x);
           
let x: number
x = true;
Type 'boolean' is not assignable to type 'string | number'.
 
console.log(x);
           
let x: string | number
Try
Control flow analysis
Up until this point, we’ve gone through some basic examples of how TypeScript narrows within specific branches. But there’s a bit more going on than just walking up from every variable and looking for type guards in ifs, whiles, conditionals, etc. For example

function padLeft(padding: number | string, input: string) {
  if (typeof padding === "number") {
    return " ".repeat(padding) + input;
  }
  return padding + input;
}
Try
padLeft returns from within its first if block. TypeScript was able to analyze this code and see that the rest of the body (return padding + input;) is unreachable in the case where padding is a number. As a result, it was able to remove number from the type of padding (narrowing from string | number to string) for the rest of the function.

This analysis of code based on reachability is called control flow analysis, and TypeScript uses this flow analysis to narrow types as it encounters type guards and assignments. When a variable is analyzed, control flow can split off and re-merge over and over again, and that variable can be observed to have a different type at each point.

function example() {
  let x: string | number | boolean;
 
  x = Math.random() < 0.5;
 
  console.log(x);
             
let x: boolean
 
  if (Math.random() < 0.5) {
    x = "hello";
    console.log(x);
               
let x: string
  } else {
    x = 100;
    console.log(x);
               
let x: number
  }
 
  return x;
        
let x: string | number
}
Try
Using type predicates
We’ve worked with existing JavaScript constructs to handle narrowing so far, however sometimes you want more direct control over how types change throughout your code.

To define a user-defined type guard, we simply need to define a function whose return type is a type predicate:

function isFish(pet: Fish | Bird): pet is Fish {
  return (pet as Fish).swim !== undefined;
}
Try
pet is Fish is our type predicate in this example. A predicate takes the form parameterName is Type, where parameterName must be the name of a parameter from the current function signature.

Any time isFish is called with some variable, TypeScript will narrow that variable to that specific type if the original type is compatible.

// Both calls to 'swim' and 'fly' are now okay.
let pet = getSmallPet();
 
if (isFish(pet)) {
  pet.swim();
} else {
  pet.fly();
}
Try
Notice that TypeScript not only knows that pet is a Fish in the if branch; it also knows that in the else branch, you don’t have a Fish, so you must have a Bird.

You may use the type guard isFish to filter an array of Fish | Bird and obtain an array of Fish:

const zoo: (Fish | Bird)[] = [getSmallPet(), getSmallPet(), getSmallPet()];
const underWater1: Fish[] = zoo.filter(isFish);
// or, equivalently
const underWater2: Fish[] = zoo.filter(isFish) as Fish[];
 
// The predicate may need repeating for more complex examples
const underWater3: Fish[] = zoo.filter((pet): pet is Fish => {
  if (pet.name === "sharkey") return false;
  return isFish(pet);
});
Try
In addition, classes can use this is Type to narrow their type.

Discriminated unions
Most of the examples we’ve looked at so far have focused around narrowing single variables with simple types like string, boolean, and number. While this is common, most of the time in JavaScript we’ll be dealing with slightly more complex structures.

For some motivation, let’s imagine we’re trying to encode shapes like circles and squares. Circles keep track of their radiuses and squares keep track of their side lengths. We’ll use a field called kind to tell which shape we’re dealing with. Here’s a first attempt at defining Shape.

interface Shape {
  kind: "circle" | "square";
  radius?: number;
  sideLength?: number;
}
Try
Notice we’re using a union of string literal types: "circle" and "square" to tell us whether we should treat the shape as a circle or square respectively. By using "circle" | "square" instead of string, we can avoid misspelling issues.

function handleShape(shape: Shape) {
  // oops!
  if (shape.kind === "rect") {
This condition will always return 'false' since the types '"circle" | "square"' and '"rect"' have no overlap.
    // ...
  }
}
Try
We can write a getArea function that applies the right logic based on if it’s dealing with a circle or square. We’ll first try dealing with circles.

function getArea(shape: Shape) {
  return Math.PI * shape.radius ** 2;
Object is possibly 'undefined'.
}
Try
Under strictNullChecks that gives us an error - which is appropriate since radius might not be defined. But what if we perform the appropriate checks on the kind property?

function getArea(shape: Shape) {
  if (shape.kind === "circle") {
    return Math.PI * shape.radius ** 2;
Object is possibly 'undefined'.
  }
}
Try
Hmm, TypeScript still doesn’t know what to do here. We’ve hit a point where we know more about our values than the type checker does. We could try to use a non-null assertion (a ! after shape.radius) to say that radius is definitely present.

function getArea(shape: Shape) {
  if (shape.kind === "circle") {
    return Math.PI * shape.radius! ** 2;
  }
}
Try
But this doesn’t feel ideal. We had to shout a bit at the type-checker with those non-null assertions (!) to convince it that shape.radius was defined, but those assertions are error-prone if we start to move code around. Additionally, outside of strictNullChecks we’re able to accidentally access any of those fields anyway (since optional properties are just assumed to always be present when reading them). We can definitely do better.

The problem with this encoding of Shape is that the type-checker doesn’t have any way to know whether or not radius or sideLength are present based on the kind property. We need to communicate what we know to the type checker. With that in mind, let’s take another swing at defining Shape.

interface Circle {
  kind: "circle";
  radius: number;
}
 
interface Square {
  kind: "square";
  sideLength: number;
}
 
type Shape = Circle | Square;
Try
Here, we’ve properly separated Shape out into two types with different values for the kind property, but radius and sideLength are declared as required properties in their respective types.

Let’s see what happens here when we try to access the radius of a Shape.

function getArea(shape: Shape) {
  return Math.PI * shape.radius ** 2;
Property 'radius' does not exist on type 'Shape'.
  Property 'radius' does not exist on type 'Square'.
}
Try
Like with our first definition of Shape, this is still an error. When radius was optional, we got an error (with strictNullChecks enabled) because TypeScript couldn’t tell whether the property was present. Now that Shape is a union, TypeScript is telling us that shape might be a Square, and Squares don’t have radius defined on them! Both interpretations are correct, but only the union encoding of Shape will cause an error regardless of how strictNullChecks is configured.

But what if we tried checking the kind property again?

function getArea(shape: Shape) {
  if (shape.kind === "circle") {
    return Math.PI * shape.radius ** 2;
                      
(parameter) shape: Circle
  }
}
Try
That got rid of the error! When every type in a union contains a common property with literal types, TypeScript considers that to be a discriminated union, and can narrow out the members of the union.

In this case, kind was that common property (which is what’s considered a discriminant property of Shape). Checking whether the kind property was "circle" got rid of every type in Shape that didn’t have a kind property with the type "circle". That narrowed shape down to the type Circle.

The same checking works with switch statements as well. Now we can try to write our complete getArea without any pesky ! non-null assertions.

function getArea(shape: Shape) {
  switch (shape.kind) {
    case "circle":
      return Math.PI * shape.radius ** 2;
                        
(parameter) shape: Circle
    case "square":
      return shape.sideLength ** 2;
              
(parameter) shape: Square
  }
}
Try
The important thing here was the encoding of Shape. Communicating the right information to TypeScript - that Circle and Square were really two separate types with specific kind fields - was crucial. Doing that let us write type-safe TypeScript code that looks no different than the JavaScript we would’ve written otherwise. From there, the type system was able to do the “right” thing and figure out the types in each branch of our switch statement.

As an aside, try playing around with the above example and remove some of the return keywords. You’ll see that type-checking can help avoid bugs when accidentally falling through different clauses in a switch statement.

Discriminated unions are useful for more than just talking about circles and squares. They’re good for representing any sort of messaging scheme in JavaScript, like when sending messages over the network (client/server communication), or encoding mutations in a state management framework.

The
never
type
When narrowing, you can reduce the options of a union to a point where you have removed all possibilities and have nothing left. In those cases, TypeScript will use a never type to represent a state which shouldn’t exist.

Exhaustiveness checking
The never type is assignable to every type; however, no type is assignable to never (except never itself). This means you can use narrowing and rely on never turning up to do exhaustive checking in a switch statement.

For example, adding a default to our getArea function which tries to assign the shape to never will raise when every possible case has not been handled.

type Shape = Circle | Square;
 
function getArea(shape: Shape) {
  switch (shape.kind) {
    case "circle":
      return Math.PI * shape.radius ** 2;
    case "square":
      return shape.sideLength ** 2;
    default:
      const _exhaustiveCheck: never = shape;
      return _exhaustiveCheck;
  }
}
Try
Adding a new member to the Shape union, will cause a TypeScript error:

interface Triangle {
  kind: "triangle";
  sideLength: number;
}
 
type Shape = Circle | Square | Triangle;
 
function getArea(shape: Shape) {
  switch (shape.kind) {
    case "circle":
      return Math.PI * shape.radius ** 2;
    case "square":
      return shape.sideLength ** 2;
    default:
      const _exhaustiveCheck: never = shape;
Type 'Triangle' is not assignable to type 'never'.
      return _exhaustiveCheck;
  }
}


CLASSES

A field declaration creates a public writeable property on a class:

class Point {
  x: number;
  y: number;
}
 
const pt = new Point();
pt.x = 0;
pt.y = 0;


As with other locations, the type annotation is optional, but will be an implicit any if not specified.

Fields can also have initializers; these will run automatically when the class is instantiated:

class Point {
  x = 0;
  y = 0;
}
 
const pt = new Point();
// Prints 0, 0
console.log(`${pt.x}, ${pt.y}`);
Try
Just like with const, let, and var, the initializer of a class property will be used to infer its type:

const pt = new Point();
pt.x = "0";
Type 'string' is not assignable to type 'number'.
Try
--strictPropertyInitialization
The strictPropertyInitialization setting controls whether class fields need to be initialized in the constructor.

class BadGreeter {
  name: string;
Property 'name' has no initializer and is not definitely assigned in the constructor.
}
Try
class GoodGreeter {
  name: string;
 
  constructor() {
    this.name = "hello";
  }
}
Try
Note that the field needs to be initialized in the constructor itself. TypeScript does not analyze methods you invoke from the constructor to detect initializations, because a derived class might override those methods and fail to initialize the members.

If you intend to definitely initialize a field through means other than the constructor (for example, maybe an external library is filling in part of your class for you), you can use the definite assignment assertion operator, !:

class OKGreeter {
  // Not initialized, but no error
  name!: string;
}
Try
readonly
Fields may be prefixed with the readonly modifier. This prevents assignments to the field outside of the constructor.

class Greeter {
  readonly name: string = "world";
 
  constructor(otherName?: string) {
    if (otherName !== undefined) {
      this.name = otherName;
    }
  }
 
  err() {
    this.name = "not ok";
Cannot assign to 'name' because it is a read-only property.
  }
}
const g = new Greeter();
g.name = "also not ok";
Cannot assign to 'name' because it is a read-only property.
Try
Constructors
Background Reading:
Constructor (MDN)
Class constructors are very similar to functions. You can add parameters with type annotations, default values, and overloads:

class Point {
  x: number;
  y: number;
 
  // Normal signature with defaults
  constructor(x = 0, y = 0) {
    this.x = x;
    this.y = y;
  }
}
Try
class Point {
  // Overloads
  constructor(x: number, y: string);
  constructor(s: string);
  constructor(xs: any, y?: any) {
    // TBD
  }
}
Try
There are just a few differences between class constructor signatures and function signatures:

Constructors can’t have type parameters - these belong on the outer class declaration, which we’ll learn about later
Constructors can’t have return type annotations - the class instance type is always what’s returned
Super Calls
Just as in JavaScript, if you have a base class, you’ll need to call super(); in your constructor body before using any this. members:

class Base {
  k = 4;
}
 
class Derived extends Base {
  constructor() {
    // Prints a wrong value in ES5; throws exception in ES6
    console.log(this.k);
'super' must be called before accessing 'this' in the constructor of a derived class.
    super();
  }
}
Try
Forgetting to call super is an easy mistake to make in JavaScript, but TypeScript will tell you when it’s necessary.

Methods
Background Reading:
Method definitions
A function property on a class is called a method. Methods can use all the same type annotations as functions and constructors:

class Point {
  x = 10;
  y = 10;
 
  scale(n: number): void {
    this.x *= n;
    this.y *= n;
  }
}
Try
Other than the standard type annotations, TypeScript doesn’t add anything else new to methods.

Note that inside a method body, it is still mandatory to access fields and other methods via this.. An unqualified name in a method body will always refer to something in the enclosing scope:

let x: number = 0;
 
class C {
  x: string = "hello";
 
  m() {
    // This is trying to modify 'x' from line 1, not the class property
    x = "world";
Type 'string' is not assignable to type 'number'.
  }
}
Try
Getters / Setters
Classes can also have accessors:

class C {
  _length = 0;
  get length() {
    return this._length;
  }
  set length(value) {
    this._length = value;
  }
}
Try
Note that a field-backed get/set pair with no extra logic is very rarely useful in JavaScript. It’s fine to expose public fields if you don’t need to add additional logic during the get/set operations.

TypeScript has some special inference rules for accessors:

If get exists but no set, the property is automatically readonly
If the type of the setter parameter is not specified, it is inferred from the return type of the getter
Getters and setters must have the same Member Visibility
Since TypeScript 4.3, it is possible to have accessors with different types for getting and setting.

class Thing {
  _size = 0;
 
  get size(): number {
    return this._size;
  }
 
  set size(value: string | number | boolean) {
    let num = Number(value);
 
    // Don't allow NaN, Infinity, etc
 
    if (!Number.isFinite(num)) {
      this._size = 0;
      return;
    }
 
    this._size = num;
  }
}
Try
Index Signatures
Classes can declare index signatures; these work the same as Index Signatures for other object types:

class MyClass {
  [s: string]: boolean | ((s: string) => boolean);
 
  check(s: string) {
    return this[s] as boolean;
  }
}
Try
Because the index signature type needs to also capture the types of methods, it’s not easy to usefully use these types. Generally it’s better to store indexed data in another place instead of on the class instance itself.

Class Heritage
Like other languages with object-oriented features, classes in JavaScript can inherit from base classes.

implements
Clauses
You can use an implements clause to check that a class satisfies a particular interface. An error will be issued if a class fails to correctly implement it:

interface Pingable {
  ping(): void;
}
 
class Sonar implements Pingable {
  ping() {
    console.log("ping!");
  }
}
 
class Ball implements Pingable {
Class 'Ball' incorrectly implements interface 'Pingable'.
  Property 'ping' is missing in type 'Ball' but required in type 'Pingable'.
  pong() {
    console.log("pong!");
  }
}
Try
Classes may also implement multiple interfaces, e.g. class C implements A, B {.

Cautions
It’s important to understand that an implements clause is only a check that the class can be treated as the interface type. It doesn’t change the type of the class or its methods at all. A common source of error is to assume that an implements clause will change the class type - it doesn’t!

interface Checkable {
  check(name: string): boolean;
}
 
class NameChecker implements Checkable {
  check(s) {
Parameter 's' implicitly has an 'any' type.
    // Notice no error here
    return s.toLowercse() === "ok";
                 
any
  }
}
Try
In this example, we perhaps expected that s’s type would be influenced by the name: string parameter of check. It is not - implements clauses don’t change how the class body is checked or its type inferred.

Similarly, implementing an interface with an optional property doesn’t create that property:

interface A {
  x: number;
  y?: number;
}
class C implements A {
  x = 0;
}
const c = new C();
c.y = 10;
Property 'y' does not exist on type 'C'.
Try
extends
Clauses
Background Reading:
extends keyword (MDN)
Classes may extend from a base class. A derived class has all the properties and methods of its base class, and also define additional members.

class Animal {
  move() {
    console.log("Moving along!");
  }
}
 
class Dog extends Animal {
  woof(times: number) {
    for (let i = 0; i < times; i++) {
      console.log("woof!");
    }
  }
}
 
const d = new Dog();
// Base class method
d.move();
// Derived class method
d.woof(3);
Try
Overriding Methods
Background Reading:
super keyword (MDN)
A derived class can also override a base class field or property. You can use the super. syntax to access base class methods. Note that because JavaScript classes are a simple lookup object, there is no notion of a “super field”.

TypeScript enforces that a derived class is always a subtype of its base class.

For example, here’s a legal way to override a method:

class Base {
  greet() {
    console.log("Hello, world!");
  }
}
 
class Derived extends Base {
  greet(name?: string) {
    if (name === undefined) {
      super.greet();
    } else {
      console.log(`Hello, ${name.toUpperCase()}`);
    }
  }
}
 
const d = new Derived();
d.greet();
d.greet("reader");
Try
It’s important that a derived class follow its base class contract. Remember that it’s very common (and always legal!) to refer to a derived class instance through a base class reference:

// Alias the derived instance through a base class reference
const b: Base = d;
// No problem
b.greet();
Try
What if Derived didn’t follow Base’s contract?

class Base {
  greet() {
    console.log("Hello, world!");
  }
}
 
class Derived extends Base {
  // Make this parameter required
  greet(name: string) {
Property 'greet' in type 'Derived' is not assignable to the same property in base type 'Base'.
  Type '(name: string) => void' is not assignable to type '() => void'.
    console.log(`Hello, ${name.toUpperCase()}`);
  }
}
Try
If we compiled this code despite the error, this sample would then crash:

const b: Base = new Derived();
// Crashes because "name" will be undefined
b.greet();
Try
Type-only Field Declarations
When target >= ES2022 or useDefineForClassFields is true, class fields are initialized after the parent class constructor completes, overwriting any value set by the parent class. This can be a problem when you only want to re-declare a more accurate type for an inherited field. To handle these cases, you can write declare to indicate to TypeScript that there should be no runtime effect for this field declaration.

interface Animal {
  dateOfBirth: any;
}
 
interface Dog extends Animal {
  breed: any;
}
 
class AnimalHouse {
  resident: Animal;
  constructor(animal: Animal) {
    this.resident = animal;
  }
}
 
class DogHouse extends AnimalHouse {
  // Does not emit JavaScript code,
  // only ensures the types are correct
  declare resident: Dog;
  constructor(dog: Dog) {
    super(dog);
  }
}
Try
Initialization Order
The order that JavaScript classes initialize can be surprising in some cases. Let’s consider this code:

class Base {
  name = "base";
  constructor() {
    console.log("My name is " + this.name);
  }
}
 
class Derived extends Base {
  name = "derived";
}
 
// Prints "base", not "derived"
const d = new Derived();
Try
What happened here?

The order of class initialization, as defined by JavaScript, is:

The base class fields are initialized
The base class constructor runs
The derived class fields are initialized
The derived class constructor runs
This means that the base class constructor saw its own value for name during its own constructor, because the derived class field initializations hadn’t run yet.

Inheriting Built-in Types
Note: If you don’t plan to inherit from built-in types like Array, Error, Map, etc. or your compilation target is explicitly set to ES6/ES2015 or above, you may skip this section

In ES2015, constructors which return an object implicitly substitute the value of this for any callers of super(...). It is necessary for generated constructor code to capture any potential return value of super(...) and replace it with this.

As a result, subclassing Error, Array, and others may no longer work as expected. This is due to the fact that constructor functions for Error, Array, and the like use ECMAScript 6’s new.target to adjust the prototype chain; however, there is no way to ensure a value for new.target when invoking a constructor in ECMAScript 5. Other downlevel compilers generally have the same limitation by default.

For a subclass like the following:

class MsgError extends Error {
  constructor(m: string) {
    super(m);
  }
  sayHello() {
    return "hello " + this.message;
  }
}
Try
you may find that:

methods may be undefined on objects returned by constructing these subclasses, so calling sayHello will result in an error.
instanceof will be broken between instances of the subclass and their instances, so (new MsgError()) instanceof MsgError will return false.
As a recommendation, you can manually adjust the prototype immediately after any super(...) calls.

class MsgError extends Error {
  constructor(m: string) {
    super(m);
 
    // Set the prototype explicitly.
    Object.setPrototypeOf(this, MsgError.prototype);
  }
 
  sayHello() {
    return "hello " + this.message;
  }
}
Try
However, any subclass of MsgError will have to manually set the prototype as well. For runtimes that don’t support Object.setPrototypeOf, you may instead be able to use __proto__.

Unfortunately, these workarounds will not work on Internet Explorer 10 and prior. One can manually copy methods from the prototype onto the instance itself (i.e. MsgError.prototype onto this), but the prototype chain itself cannot be fixed.

Member Visibility
You can use TypeScript to control whether certain methods or properties are visible to code outside the class.

public
The default visibility of class members is public. A public member can be accessed anywhere:

class Greeter {
  public greet() {
    console.log("hi!");
  }
}
const g = new Greeter();
g.greet();
Try
Because public is already the default visibility modifier, you don’t ever need to write it on a class member, but might choose to do so for style/readability reasons.

protected
protected members are only visible to subclasses of the class they’re declared in.

class Greeter {
  public greet() {
    console.log("Hello, " + this.getName());
  }
  protected getName() {
    return "hi";
  }
}
 
class SpecialGreeter extends Greeter {
  public howdy() {
    // OK to access protected member here
    console.log("Howdy, " + this.getName());
  }
}
const g = new SpecialGreeter();
g.greet(); // OK
g.getName();
Property 'getName' is protected and only accessible within class 'Greeter' and its subclasses.
Try
Exposure of protected members
Derived classes need to follow their base class contracts, but may choose to expose a subtype of base class with more capabilities. This includes making protected members public:

class Base {
  protected m = 10;
}
class Derived extends Base {
  // No modifier, so default is 'public'
  m = 15;
}
const d = new Derived();
console.log(d.m); // OK
Try
Note that Derived was already able to freely read and write m, so this doesn’t meaningfully alter the “security” of this situation. The main thing to note here is that in the derived class, we need to be careful to repeat the protected modifier if this exposure isn’t intentional.

Cross-hierarchy protected access
Different OOP languages disagree about whether it’s legal to access a protected member through a base class reference:

class Base {
  protected x: number = 1;
}
class Derived1 extends Base {
  protected x: number = 5;
}
class Derived2 extends Base {
  f1(other: Derived2) {
    other.x = 10;
  }
  f2(other: Base) {
    other.x = 10;
Property 'x' is protected and only accessible through an instance of class 'Derived2'. This is an instance of class 'Base'.
  }
}
Try
Java, for example, considers this to be legal. On the other hand, C# and C++ chose that this code should be illegal.

TypeScript sides with C# and C++ here, because accessing x in Derived2 should only be legal from Derived2’s subclasses, and Derived1 isn’t one of them. Moreover, if accessing x through a Derived1 reference is illegal (which it certainly should be!), then accessing it through a base class reference should never improve the situation.

See also Why Can’t I Access A Protected Member From A Derived Class? which explains more of C#‘s reasoning.

private
private is like protected, but doesn’t allow access to the member even from subclasses:

class Base {
  private x = 0;
}
const b = new Base();
// Can't access from outside the class
console.log(b.x);
Property 'x' is private and only accessible within class 'Base'.
Try
class Derived extends Base {
  showX() {
    // Can't access in subclasses
    console.log(this.x);
Property 'x' is private and only accessible within class 'Base'.
  }
}
Try
Because private members aren’t visible to derived classes, a derived class can’t increase its visibility:

class Base {
  private x = 0;
}
class Derived extends Base {
Class 'Derived' incorrectly extends base class 'Base'.
  Property 'x' is private in type 'Base' but not in type 'Derived'.
  x = 1;
}
Try
Cross-instance private access
Different OOP languages disagree about whether different instances of the same class may access each others’ private members. While languages like Java, C#, C++, Swift, and PHP allow this, Ruby does not.

TypeScript does allow cross-instance private access:

class A {
  private x = 10;
 
  public sameAs(other: A) {
    // No error
    return other.x === this.x;
  }
}
Try
Caveats
Like other aspects of TypeScript’s type system, private and protected are only enforced during type checking.

This means that JavaScript runtime constructs like in or simple property lookup can still access a private or protected member:

class MySafe {
  private secretKey = 12345;
}
Try
// In a JavaScript file...
const s = new MySafe();
// Will print 12345
console.log(s.secretKey);
private also allows access using bracket notation during type checking. This makes private-declared fields potentially easier to access for things like unit tests, with the drawback that these fields are soft private and don’t strictly enforce privacy.

class MySafe {
  private secretKey = 12345;
}
 
const s = new MySafe();
 
// Not allowed during type checking
console.log(s.secretKey);
Property 'secretKey' is private and only accessible within class 'MySafe'.
 
// OK
console.log(s["secretKey"]);
Try
Unlike TypeScripts’s private, JavaScript’s private fields (#) remain private after compilation and do not provide the previously mentioned escape hatches like bracket notation access, making them hard private.

class Dog {
  #barkAmount = 0;
  personality = "happy";
 
  constructor() {}
}
Try
"use strict";
class Dog {
    #barkAmount = 0;
    personality = "happy";
    constructor() { }
}
 
Try
When compiling to ES2021 or less, TypeScript will use WeakMaps in place of #.

"use strict";
var _Dog_barkAmount;
class Dog {
    constructor() {
        _Dog_barkAmount.set(this, 0);
        this.personality = "happy";
    }
}
_Dog_barkAmount = new WeakMap();
 
Try
If you need to protect values in your class from malicious actors, you should use mechanisms that offer hard runtime privacy, such as closures, WeakMaps, or private fields. Note that these added privacy checks during runtime could affect performance.

Static Members
Background Reading:
Static Members (MDN)
Classes may have static members. These members aren’t associated with a particular instance of the class. They can be accessed through the class constructor object itself:

class MyClass {
  static x = 0;
  static printX() {
    console.log(MyClass.x);
  }
}
console.log(MyClass.x);
MyClass.printX();
Try
Static members can also use the same public, protected, and private visibility modifiers:

class MyClass {
  private static x = 0;
}
console.log(MyClass.x);
Property 'x' is private and only accessible within class 'MyClass'.
Try
Static members are also inherited:

class Base {
  static getGreeting() {
    return "Hello world";
  }
}
class Derived extends Base {
  myGreeting = Derived.getGreeting();
}
Try
Special Static Names
It’s generally not safe/possible to overwrite properties from the Function prototype. Because classes are themselves functions that can be invoked with new, certain static names can’t be used. Function properties like name, length, and call aren’t valid to define as static members:

class S {
  static name = "S!";
Static property 'name' conflicts with built-in property 'Function.name' of constructor function 'S'.
}
Try
Why No Static Classes?
TypeScript (and JavaScript) don’t have a construct called static class the same way as, for example, C# does.

Those constructs only exist because those languages force all data and functions to be inside a class; because that restriction doesn’t exist in TypeScript, there’s no need for them. A class with only a single instance is typically just represented as a normal object in JavaScript/TypeScript.

For example, we don’t need a “static class” syntax in TypeScript because a regular object (or even top-level function) will do the job just as well:

// Unnecessary "static" class
class MyStaticClass {
  static doSomething() {}
}
 
// Preferred (alternative 1)
function doSomething() {}
 
// Preferred (alternative 2)
const MyHelperObject = {
  dosomething() {},
};
Try
static
Blocks in Classes
Static blocks allow you to write a sequence of statements with their own scope that can access private fields within the containing class. This means that we can write initialization code with all the capabilities of writing statements, no leakage of variables, and full access to our class’s internals.

class Foo {
    static #count = 0;
 
    get count() {
        return Foo.#count;
    }
 
    static {
        try {
            const lastInstances = loadLastInstances();
            Foo.#count += lastInstances.length;
        }
        catch {}
    }
}
Try
Generic Classes
Classes, much like interfaces, can be generic. When a generic class is instantiated with new, its type parameters are inferred the same way as in a function call:

class Box<Type> {
  contents: Type;
  constructor(value: Type) {
    this.contents = value;
  }
}
 
const b = new Box("hello!");
     
const b: Box<string>
Try
Classes can use generic constraints and defaults the same way as interfaces.

Type Parameters in Static Members
This code isn’t legal, and it may not be obvious why:

class Box<Type> {
  static defaultValue: Type;
Static members cannot reference class type parameters.
}
Try
Remember that types are always fully erased! At runtime, there’s only one Box.defaultValue property slot. This means that setting Box<string>.defaultValue (if that were possible) would also change Box<number>.defaultValue - not good. The static members of a generic class can never refer to the class’s type parameters.

this
at Runtime in Classes
Background Reading:
this keyword (MDN)
It’s important to remember that TypeScript doesn’t change the runtime behavior of JavaScript, and that JavaScript is somewhat famous for having some peculiar runtime behaviors.

JavaScript’s handling of this is indeed unusual:

class MyClass {
  name = "MyClass";
  getName() {
    return this.name;
  }
}
const c = new MyClass();
const obj = {
  name: "obj",
  getName: c.getName,
};
 
// Prints "obj", not "MyClass"
console.log(obj.getName());
Try
Long story short, by default, the value of this inside a function depends on how the function was called. In this example, because the function was called through the obj reference, its value of this was obj rather than the class instance.

This is rarely what you want to happen! TypeScript provides some ways to mitigate or prevent this kind of error.

Arrow Functions
Background Reading:
Arrow functions (MDN)
If you have a function that will often be called in a way that loses its this context, it can make sense to use an arrow function property instead of a method definition:

class MyClass {
  name = "MyClass";
  getName = () => {
    return this.name;
  };
}
const c = new MyClass();
const g = c.getName;
// Prints "MyClass" instead of crashing
console.log(g());
Try
This has some trade-offs:

The this value is guaranteed to be correct at runtime, even for code not checked with TypeScript
This will use more memory, because each class instance will have its own copy of each function defined this way
You can’t use super.getName in a derived class, because there’s no entry in the prototype chain to fetch the base class method from
this
parameters
In a method or function definition, an initial parameter named this has special meaning in TypeScript. These parameters are erased during compilation:

// TypeScript input with 'this' parameter
function fn(this: SomeType, x: number) {
  /* ... */
}
Try
// JavaScript output
function fn(x) {
  /* ... */
}
TypeScript checks that calling a function with a this parameter is done so with a correct context. Instead of using an arrow function, we can add a this parameter to method definitions to statically enforce that the method is called correctly:

class MyClass {
  name = "MyClass";
  getName(this: MyClass) {
    return this.name;
  }
}
const c = new MyClass();
// OK
c.getName();
 
// Error, would crash
const g = c.getName;
console.log(g());
The 'this' context of type 'void' is not assignable to method's 'this' of type 'MyClass'.
Try
This method makes the opposite trade-offs of the arrow function approach:

JavaScript callers might still use the class method incorrectly without realizing it
Only one function per class definition gets allocated, rather than one per class instance
Base method definitions can still be called via super.
this
Types
In classes, a special type called this refers dynamically to the type of the current class. Let’s see how this is useful:

class Box {
  contents: string = "";
  set(value: string) {
  
(method) Box.set(value: string): this
    this.contents = value;
    return this;
  }
}
Try
Here, TypeScript inferred the return type of set to be this, rather than Box. Now let’s make a subclass of Box:

class ClearableBox extends Box {
  clear() {
    this.contents = "";
  }
}
 
const a = new ClearableBox();
const b = a.set("hello");
     
const b: ClearableBox
Try
You can also use this in a parameter type annotation:

class Box {
  content: string = "";
  sameAs(other: this) {
    return other.content === this.content;
  }
}
Try
This is different from writing other: Box — if you have a derived class, its sameAs method will now only accept other instances of that same derived class:

class Box {
  content: string = "";
  sameAs(other: this) {
    return other.content === this.content;
  }
}
 
class DerivedBox extends Box {
  otherContent: string = "?";
}
 
const base = new Box();
const derived = new DerivedBox();
derived.sameAs(base);
Argument of type 'Box' is not assignable to parameter of type 'DerivedBox'.
  Property 'otherContent' is missing in type 'Box' but required in type 'DerivedBox'.
Try
this
-based type guards
You can use this is Type in the return position for methods in classes and interfaces. When mixed with a type narrowing (e.g. if statements) the type of the target object would be narrowed to the specified Type.

class FileSystemObject {
  isFile(): this is FileRep {
    return this instanceof FileRep;
  }
  isDirectory(): this is Directory {
    return this instanceof Directory;
  }
  isNetworked(): this is Networked & this {
    return this.networked;
  }
  constructor(public path: string, private networked: boolean) {}
}
 
class FileRep extends FileSystemObject {
  constructor(path: string, public content: string) {
    super(path, false);
  }
}
 
class Directory extends FileSystemObject {
  children: FileSystemObject[];
}
 
interface Networked {
  host: string;
}
 
const fso: FileSystemObject = new FileRep("foo/bar.txt", "foo");
 
if (fso.isFile()) {
  fso.content;
  
const fso: FileRep
} else if (fso.isDirectory()) {
  fso.children;
  
const fso: Directory
} else if (fso.isNetworked()) {
  fso.host;
  
const fso: Networked & FileSystemObject
}
Try
A common use-case for a this-based type guard is to allow for lazy validation of a particular field. For example, this case removes an undefined from the value held inside box when hasValue has been verified to be true:

class Box<T> {
  value?: T;
 
  hasValue(): this is { value: T } {
    return this.value !== undefined;
  }
}
 
const box = new Box();
box.value = "Gameboy";
 
box.value;
     
(property) Box<unknown>.value?: unknown
 
if (box.hasValue()) {
  box.value;
       
(property) value: unknown
}
Try
Parameter Properties
TypeScript offers special syntax for turning a constructor parameter into a class property with the same name and value. These are called parameter properties and are created by prefixing a constructor argument with one of the visibility modifiers public, private, protected, or readonly. The resulting field gets those modifier(s):

class Params {
  constructor(
    public readonly x: number,
    protected y: number,
    private z: number
  ) {
    // No body necessary
  }
}
const a = new Params(1, 2, 3);
console.log(a.x);
             
(property) Params.x: number
console.log(a.z);
Property 'z' is private and only accessible within class 'Params'.
Try
Class Expressions
Background Reading:
Class expressions (MDN)
Class expressions are very similar to class declarations. The only real difference is that class expressions don’t need a name, though we can refer to them via whatever identifier they ended up bound to:

const someClass = class<Type> {
  content: Type;
  constructor(value: Type) {
    this.content = value;
  }
};
 
const m = new someClass("Hello, world");
     
const m: someClass<string>
Try
abstract
Classes and Members
Classes, methods, and fields in TypeScript may be abstract.

An abstract method or abstract field is one that hasn’t had an implementation provided. These members must exist inside an abstract class, which cannot be directly instantiated.

The role of abstract classes is to serve as a base class for subclasses which do implement all the abstract members. When a class doesn’t have any abstract members, it is said to be concrete.

Let’s look at an example:

abstract class Base {
  abstract getName(): string;
 
  printName() {
    console.log("Hello, " + this.getName());
  }
}
 
const b = new Base();
Cannot create an instance of an abstract class.
Try
We can’t instantiate Base with new because it’s abstract. Instead, we need to make a derived class and implement the abstract members:

class Derived extends Base {
  getName() {
    return "world";
  }
}
 
const d = new Derived();
d.printName();
Try
Notice that if we forget to implement the base class’s abstract members, we’ll get an error:

class Derived extends Base {
Non-abstract class 'Derived' does not implement inherited abstract member 'getName' from class 'Base'.
  // forgot to do anything
}
Try
Abstract Construct Signatures
Sometimes you want to accept some class constructor function that produces an instance of a class which derives from some abstract class.

For example, you might want to write this code:

function greet(ctor: typeof Base) {
  const instance = new ctor();
Cannot create an instance of an abstract class.
  instance.printName();
}
Try
TypeScript is correctly telling you that you’re trying to instantiate an abstract class. After all, given the definition of greet, it’s perfectly legal to write this code, which would end up constructing an abstract class:

// Bad!
greet(Base);
Try
Instead, you want to write a function that accepts something with a construct signature:

function greet(ctor: new () => Base) {
  const instance = new ctor();
  instance.printName();
}
greet(Derived);
greet(Base);
Argument of type 'typeof Base' is not assignable to parameter of type 'new () => Base'.
  Cannot assign an abstract constructor type to a non-abstract constructor type.
Try
Now TypeScript correctly tells you about which class constructor functions can be invoked - Derived can because it’s concrete, but Base cannot.

Relationships Between Classes
In most cases, classes in TypeScript are compared structurally, the same as other types.

For example, these two classes can be used in place of each other because they’re identical:

class Point1 {
  x = 0;
  y = 0;
}
 
class Point2 {
  x = 0;
  y = 0;
}
 
// OK
const p: Point1 = new Point2();
Try
Similarly, subtype relationships between classes exist even if there’s no explicit inheritance:

class Person {
  name: string;
  age: number;
}
 
class Employee {
  name: string;
  age: number;
  salary: number;
}
 
// OK
const p: Person = new Employee();
Try
This sounds straightforward, but there are a few cases that seem stranger than others.

Empty classes have no members. In a structural type system, a type with no members is generally a supertype of anything else. So if you write an empty class (don’t!), anything can be used in place of it:

class Empty {}
 
function fn(x: Empty) {
  // can't do anything with 'x', so I won't
}
 
// All OK!
fn(window);
fn({});
fn(fn);

