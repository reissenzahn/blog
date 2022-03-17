// Console is a singleton with an instance that always applies to the current shell
Console theConsole = System.console();

// readLine() returns a string containing the line read from the console not including any line-termination characters or null if an end of stream has been reached


public String readLine(String prompt, Object... formatting)

The prompt string is interpreted like any printf( ) string and filled with arguments to
its right. All this does is format the prompt. This is not a scanf( ) equivalent. The
return value is the same as for the no-args readLine( ) method.

Console also has two readPassword( ) methods:
public char[] readPassword( )
public char[] readPassword(String prompt, Object... formatting)

public Console format(String format, Object... arguments)
public Console printf(String format, Object... arguments)

There is no difference between these two methods. They are synonyms. For example, this code fragment prints a three-column table of the angles between 0
and 360 degrees in degrees, radians, and grads on the console using only printf( ).
Each number is exactly five characters wide with one digit after the decimal point.
Console console = System.console( );
for (double degrees = 0.0; degrees < 360.0; degrees++) {
double radians = Math.PI * degrees / 180.0;
double grads = 400 * degrees / 360;
console.printf("%5.1f %5.1f %5.1f\n", degrees, radians, grads);
}
Here's the start of the output:
0.0 0.0 0.0
1.0 0.0 1.1
2.0 0.0 2.2
3.0 0.1 3.3
...

The console normally buffers all output until a line break is seen. You can force data
to be written to the screen even before a line break by invoking the flush( ) method:
formatter.flush( );
formatter.close( );