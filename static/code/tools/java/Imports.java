// types that have been imported into the namespace with an import declaration may be used without their package name
import java.io.File;
import java.util.*;  // import on-demand
import java.util.zip.ZipInputStream;


// it illegal to import classes that cause name conflicts
// import java.util.List;
// import java.awt.List;

// using on-demand imports is fine though actually trying to use List will produce a compilation error
import java.util.*;
import java.awt.*;

// the workaround is to explicitly specify the package name you want
import java.util.*;
import java.awt.*;
import java.util.List;


// you can import the static members of types using the keywords
import static java.lang.System.out;
import static java.lang.Math.*;

// a single static member import declaration may actually import more than one member due to overloading and the compiler will choose the appropriate method
import static java.util.Arrays.sort;

// we can even import static methods with the same name from different packages as long as they have different signatures
import static java.util.Collections.sort;


public class Imports {

  // $ javac Imports.java && java Imports
  public static void main(String[] args) {
    
    // the types from java.lang and the current package are automatically imported so they can 
    Object obj = new Object();

    // we can omit the package name
    out.println(sqrt(abs(sin(45))));  // 0.9224...

    // compiler determines the appropriate sort method
    int[] arr = new int[]{2, 1, 3, 4, 5};
    sort(arr);
    System.out.println(Arrays.toString(arr));  // [1, 2, 3, 4, 5]
  }
}