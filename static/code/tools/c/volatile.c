#include <stdio.h>
#include <signal.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>

// the volatile qualifier tells the compiler that the value of a variable may change at any time without any action being taken by the code the compiler finds nearby
volatile bool done = false;

void *start(void *arg) {
  sleep(1);
  done = true;
  return NULL;
}

volatile bool arrived = false;

void handler(int sig) {
  if (sig == SIGTSTP) {
    arrived = true;
  }
}

int main() {
  pthread_t thread;
  pthread_create(&thread, NULL, start, NULL);

  printf("waiting for thread...\n");
  
  // if done is not volatile then compiling with -O2 may result in this blocking forever as the compiler notices that done is never updated in the loop and so replaces the loop with while (true);
  while (!done);

  
  signal(SIGTSTP, handler);

  printf("waiting for signal (^Z)...\n");

  while (!arrived);

  printf("finished!");
}

// https://www.youtube.com/watch?v=6tIWFEzzx9I

// ---

// // the volatile qualifier tells the compiler that the value of the variable may change at any time without any action being taken by the code the compiler finds nearby
// volatile uint16_t x; 

// // pointer to volatile variable
// volatile uint8_t *p_reg; 

// // pointer to non-volatile variable
// uint16_t *volatile p_x;

// // volatile pointer to a volatile variable
// uint16_t volatile * volatile p_y;


// // A variable should be declared volatile whenever its value could change unexpectedly. In practice, only three types of variables could change:

// // 1. Memory-mapped peripheral registers
// // 2. Global variables modified by an interrupt service routine
// // 3. Global variables accessed by multiple tasks within a multi-threaded application



// Peripheral Registers
// Embedded systems contain real hardware, usually with sophisticated peripherals. These peripherals contain registers whose values may change asynchronously to the program flow. As a very simple example, consider an 8-bit status register that is memory mapped at address 0x1234. It is required that you poll the status register until it becomes non-zero. The naive and incorrect implementation is as follows:

// uint8_t * p_reg = (uint8_t *) 0x1234;

// // Wait for register to read non-zero 
// do { ... } while (0 == *p_reg)
// This code will almost certainly fail as soon as you turn compiler optimization on.  That's because the compiler will generate assembly language (here for an 16-bit x86 processor) that looks something like this:

//   mov p_reg, #0x1234
//   mov a, @p_reg
// loop:
//   ...
//   bz loop
// The rationale of the optimizer is quite simple: having already read the variable's value into the accumulator (on the second line of assembly), there is no need to reread it, since the value will (duh!) always be the same. Thus, from the third line of assembly, we enter an infinite loop. To force the compiler to do what we want, we should modify the declaration to:

// uint8_t volatile * p_reg = (uint8_t volatile *) 0x1234;
// The assembly language now looks like this:

//   mov p_reg, #0x1234
// loop:
//   ...
//   mov a, @p_reg
//   bz loop
// The desired behavior is thus achieved.

// Subtler sorts of bugs tend to arise when registers with special properties are accessed without volatile declarations. For instance, a lot of peripherals contain registers that are cleared simply by reading them. Extra (or fewer) reads than you are intending could result in quite unexpected behavior in these cases.

// Interrupt Service Routines
// Interrupt service routines often set variables that are tested in mainline code. For example, a serial port interrupt may test each received character to see if it is an ETX character (presumably signifying the end of a message). If the character is an ETX, the ISR might set a global flag. An incorrect implementation of this might be:

// bool gb_etx_found = false;

// void main() 
// {
//     ... 
//     while (!gb_etx_found) 
//     {
//         // Wait
//     } 
//     ...
// }

// interrupt void rx_isr(void) 
// {
//     ... 
//     if (ETX == rx_char) 
//     {
//         gb_etx_found = true;
//     } 
//     ...
// }
// [NOTE: We're not advocating use of global variables; this code uses one to keep the example short/clear.]

// With compiler optimization turned off, this program might work. However, any half decent optimizer will "break" the program. The problem is that the compiler has no idea that gb_etx_found can be changed within the ISR function, which doesn't appear to be ever called.

// As far as the compiler is concerned, the expression !gb_ext_found will have the same result every time through the loop, and therefore, you must not ever want to exit the while loop. Consequently, all the code after the while loop may simply be removed by the optimizer. If you are lucky, your compiler will warn you about this. If you are unlucky (or you haven't yet learned to take compiler warnings seriously), your code will fail miserably. Naturally, the blame will be placed on a "lousy optimizer."

// The solution is to declare the variable gb_etx_found to be volatile. After which, this program will work as you intended.

// Multithreaded Applications
// Despite the presence of queues, pipes, and other scheduler-aware communications mechanisms in real-time operating systems, it is still possible that RTOS tasks will exchange information via a shared memory location (i.e., global storage). When you add a preemptive scheduler to your code, your compiler has no idea what a context switch is or when one might occur. Thus, a task asynchronously modifying a shared global is conceptually the same as the ISR scenario discussed above. Thus all shared global objects (variables, memory buffers, hardware registers, etc.) must also be declared volatile to prevent compiler optimization from introducing unexpected behaviors. For example, this code is asking for trouble:

// uint8_t gn_bluetask_runs = 0;

// void red_task (void) 
// {   
//     while (4 < gn_bluetask_runs) 
//     {
//         ...
//     } 
//     // Exit after 4 iterations of blue_task.
// }

// void blue_task (void) 
// {
//     for (;;)
//     {
//         ...
//         gn_bluetask_runs++;
//         ...
//     }
// }
// This program will likely fail once the compiler's optimizer is enabled. Declaring gn_bluetask_runs with volatile is the proper way to solve the problem.

// [NOTE: We're not advocating use of global variables; this code uses a global because it is explaining a relationship between volatile and global variables.]

// [WARNING: Global variables shared by tasks and ISRs will also need to be protected against race conditions, e.g. by a mutex.]

// Final Thoughts

// Some compilers allow you to implicitly declare all variables as volatile. Resist this temptation, since it is essentially a substitute for thought. It also leads to potentially less efficient code.

// Also, resist the temptation to blame the optimizer or turn it off when you encounter unexpected program behavior. Modern C/C++ optimizers are so good that I cannot remember the last time I came across an optimization bug. In contrast, I regularly come across failures by programmers to use volatile.

// If you are given a piece of flaky code to "fix," perform a grep for volatile. If grep comes up empty, the examples given here are probably good places to start looking for problems.


// ---


// Volatile in C: Volatile keyword in C lets the compiler know that the variable’s value might change at any point in time due to many external conditions, and the change is not only a result of the program’s control flow. The compiler associated will not rewrite anything that has to do with volatile variables.

// Volatile in C came into existence for not caching values of a variable automatically. It tells the computer system not to cache the value of this volatile variable. This generated code to obtain the value of the volatile variable from memory every time it confronts it. This mechanism is of great help because the value can be modified by the operating system or an interrupt, for that matter. Henceforth, using volatile helps in accessing the value every time it is needed.

// Syntax:


//  volatile int foo; /* Different methods in which we can declare a volatile variable */
//  int volatile foo;
//  volatile uint8_t * p_reg; /*initializing pointers in volatile variables*/
//  uint8_t volatile * p_reg; /*Pointer is not volatile, but the int the pointer points to is pointer */ 

// 1
// 2
// 3
// 4
// 5
// 6
 
//  volatile int foo; /* Different methods in which we can declare a volatile variable */
//  int volatile foo;
//  volatile uint8_t * p_reg; /*initializing pointers in volatile variables*/
//  uint8_t volatile * p_reg; /*Pointer is not volatile, but the int the pointer points to is pointer */ 
 
// The main reasons to use volatile variables:

// A variable should be declared as a volatile variable only when its value changes unexpectedly.

// To interface with the hardware that contains memory-mapped I/O registers, that is, the interface is a shared boundary between components present in a computer system. Volatile keyword helps in accessing the features by using addresses and pointers. In other words, memory-mapped peripheral registers lead to the volatile declaration.
// Volatile is used to modify variables outside the program control flow, that is, during an interrupt service routine. An interrupt service routine is a process that is invoked by an interrupt request from a hardware device, that is, when others modify global variables.
// Another thread running uses the same variable used by one compiler, and multiple tasks within a multi-threaded application are accessing global variables.
// When there is a signal handler, that may change the value of the variable. A Signal handler is one of the functions called by the target environment when a similar signal occurs.
// RTOS (Real-Time Operating System) will be working perfectly fine until some task is given, leading to some error. In such cases, volatile is used for debugging the error. RTOS is an operating system (OS) that is expected to serve real-time applications that process the data as it comes without any buffer delays.
// Not using volatile may lead to flaky hard drives constantly leading the computer system to become slow.
// Example


//  #include <stdio.h>
//  #include <signal.h>
//  #include <stdbool.h>
//  #include <unistd.h>
//  #include <pthread.h>
//  bool done = false;
//  void *tFunction (void * arg){
//              sleep(1);
//              done = true;
//              return NULL;
//  }
//  int main() {
//              pthread_t t1;
//              pthread_create(&t1, NULL, tfunc, NULL);
//              printf (" waiting \n ");
//              while (!done)
//              printf (" Move on \n ");
//  } 

// 1
// 2
// 3
// 4
// 5
// 6
// 7
// 8
// 9
// 10
// 11
// 12
// 13
// 14
// 15
// 16
// 17
// 18
// 19
// 20
 
//  #include <stdio.h>
//  #include <signal.h>
//  #include <stdbool.h>
//  #include <unistd.h>
//  #include <pthread.h>
//  bool done = false;
//  void *tFunction (void * arg){
//              sleep(1);
//              done = true;
//              return NULL;
//  }
//  int main() {
//              pthread_t t1;
//              pthread_create(&t1, NULL, tfunc, NULL);
//              printf (" waiting \n ");
//              while (!done)
//              printf (" Move on \n ");
//  } 
 
// When the above code is compiled and run, it usually works and gives the output. But for the sake of completeness, if the compiler optimization is turned on, it throws an error by not running some part of the code snippet. When the central part is viewed, there will be no evidence that will change; so the compiler does not realize that another thread modifies the global variable, breaking in the middle of the program. If the while loop is changed like shown below, the compiler sees the sleep function inside the while loop and thinks that there might be some more code left to compile. Hence it will run smoothly without any errors.


//              while (!done) {
//                          sleep(1);
//              } 

// 1
// 2
// 3
// 4
// 5
 
//              while (!done) {
//                          sleep(1);
//              } 
 
// The program’s correctness is at the whim of the compiler as the set of heuristics that are used to optimize the code. Because different compilers might have different heuristics, and it might be broken in both or might work correctly in both cases. Therefore, adding sleep is not the solution. Here we can make use of volatile by declaring it in the beginning as shown below;


//             volatile bool done = false;

// 1
// 2
// 3
 
//             volatile bool done = false;
 
// Doing so, the program tells the compiler that the variable might not be apparent to the compiler. For example, it might be a thread or a signal handler or an interrupt service. The main point is when we make a variable volatile, we are indicating the compiler not to make any assumptions about when this variable can be changed.

// ---

// volatile tells the compiler not to optimize anything that has to do with the volatile variable.

// There are at least three common reasons to use it, all involving situations where the value of the variable can change without action from the visible code: When you interface with hardware that changes the value itself; when there's another thread running that also uses the variable; or when there's a signal handler that might change the value of the variable.

// Let's say you have a little piece of hardware that is mapped into RAM somewhere and that has two addresses: a command port and a data port:

// typedef struct
// {
//   int command;
//   int data;
//   int isBusy;
// } MyHardwareGadget;
// Now you want to send some command:

// void SendCommand (MyHardwareGadget * gadget, int command, int data)
// {
//   // wait while the gadget is busy:
//   while (gadget->isbusy)
//   {
//     // do nothing here.
//   }
//   // set data first:
//   gadget->data    = data;
//   // writing the command starts the action:
//   gadget->command = command;
// }
// Looks easy, but it can fail because the compiler is free to change the order in which data and commands are written. This would cause our little gadget to issue commands with the previous data-value. Also take a look at the wait while busy loop. That one will be optimized out. The compiler will try to be clever, read the value of isBusy just once and then go into an infinite loop. That's not what you want.

// The way to get around this is to declare the pointer gadget as volatile. This way the compiler is forced to do what you wrote. It can't remove the memory assignments, it can't cache variables in registers and it can't change the order of assignments either

// This is the correct version:

// void SendCommand (volatile MyHardwareGadget * gadget, int command, int data)
// {
//   // wait while the gadget is busy:
//   while (gadget->isBusy)
//   {
//     // do nothing here.
//   }
//   // set data first:
//   gadget->data    = data;
//   // writing the command starts the action:
//   gadget->command = command;
// }

// ---

// Imagine that you are writing code that controls a hardware device by placing appropriate values in hardware registers at known absolute addresses.

// Let's imagine that the device has two registers, each 16 bits long, at ascending memory addresses; the first one is the control and status register (csr) and the second is a data port. The traditional way of accessing such a device is like this:

// /* Standard C example but without const or volatile */
// /*
// * Declare the device registers
// * Whether to use int or short
// * is implementation dependent
// */

// struct devregs{
//         unsigned short  csr;    /* control & status */
//         unsigned short  data;   /* data port */
// };

// /* bit patterns in the csr */
// #define ERROR   0x1
// #define READY   0x2
// #define RESET   0x4

// /* absolute address of the device */
// #define DEVADDR ((struct devregs *)0xffff0004)

// /* number of such devices in system */
// #define NDEVS   4

// /*
// * Busy-wait function to read a byte from device n.
// * check range of device number.
// * Wait until READY or ERROR
// * if no error, read byte, return it
// * otherwise reset error, return 0xffff
// */
// unsigned int read_dev(unsigned devno){

//         struct devregs *dvp = DEVADDR + devno;

//         if(devno >= NDEVS)
//                 return(0xffff);

//         while((dvp->csr & (READY | ERROR)) == 0)
//                 ; /* NULL - wait till done */

//         if(dvp->csr & ERROR){
//                 dvp->csr = RESET;
//                 return(0xffff);
//         }

//         return((dvp->data) & 0xff);
// }
// Example 8.4
// The technique of using a structure declaration to describe the device register layout and names is very common practice. Notice that there aren't actually any objects of that type defined, so the declaration simply indicates the structure without using up any store.

// To access the device registers, an appropriately cast constant is used as if it were pointing to such a structure, but of course it points to memory addresses instead.

// However, a major problem with previous C compilers would be in the while loop which tests the status register and waits for the ERROR or READY bit to come on. Any self-respecting optimizing compiler would notice that the loop tests the same memory address over and over again. It would almost certainly arrange to reference memory once only, and copy the value into a hardware register, thus speeding up the loop. This is, of course, exactly what we don't want; this is one of the few places where we must look at the place where the pointer points, every time around the loop.

// Because of this problem, most C compilers have been unable to make that sort of optimization in the past. To remove the problem (and other similar ones to do with when to write to where a pointer points), the keyword volatile was introduced. It tells the compiler that the object is subject to sudden change for reasons which cannot be predicted from a study of the program itself, and forces every reference to such an object to be a genuine reference.

// Here is how you would rewrite the example, making use of const and volatile to get what you want.

// /*
// * Declare the device registers
// * Whether to use int or short
// * is implementation dependent
// */

// struct devregs{
//         unsigned short volatile csr;
//         unsigned short const volatile data;
// };

// /* bit patterns in the csr */
// #define ERROR   0x1
// #define READY   0x2
// #define RESET   0x4

// /* absolute address of the device */
// #define DEVADDR ((struct devregs *)0xffff0004)

// /* number of such devices in system */
// #define NDEVS   4

// /*
// * Busy-wait function to read a byte from device n.
// * check range of device number.
// * Wait until READY or ERROR
// * if no error, read byte, return it
// * otherwise reset error, return 0xffff
// */
// unsigned int read_dev(unsigned devno){

//         struct devregs * const dvp = DEVADDR + devno;

//         if(devno >= NDEVS)
//                 return(0xffff);

//         while((dvp->csr & (READY | ERROR)) == 0)
//                 ; /* NULL - wait till done */

//         if(dvp->csr & ERROR){
//                 dvp->csr = RESET;
//                 return(0xffff);
//         }

//         return((dvp->data) & 0xff);
// }
// Example 8.5
// The rules about mixing volatile and regular types resemble those for const. A pointer to a volatile object can be assigned the address of a regular object with safety, but it is dangerous (and needs a cast) to take the address of a volatile object and put it into a pointer to a regular object. Using such a derived pointer results in undefined behaviour.

// If an array, union or structure is declared with const or volatile attributes, then all of the members take on that attribute too. This makes sense when you think about it—how could a member of a const structure be modifiable?

// That means that an alternative rewrite of the last example would be possible. Instead of declaring the device registers to be volatile in the structure, the pointer could have been declared to point to a volatile structure instead, like this:

// struct devregs{
//       unsigned short  csr;    /* control & status */
//       unsigned short  data;   /* data port */
// };
// volatile struct devregs *const dvp=DEVADDR+devno;
// Since dvp points to a volatile object, it not permitted to optimize references through the pointer. Our feeling is that, although this would work, it is bad style. The volatile declaration belongs in the structure: it is the device registers which are volatile and that is where the information should be kept; it reinforces the fact for a human reader.

// So, for any object likely to be subject to modification either by hardware or asynchronous interrupt service routines, the volatile type qualifier is important.

// Now, just when you thought that you understood all that, here comes the final twist. A declaration like this:

// volatile struct devregs{
//       /* stuff */
// }v_decl;
// declares the type struct devregs and also a volatile-qualified object of that type, called v_decl. A later declaration like this

// struct devregs nv_decl;
// declares nv_decl which is not qualified with volatile! The qualification is not part of the type of struct devregs but applies only to the declaration of v_decl. Look at it this way round, which perhaps makes the situation more clear (the two declarations are the same in their effect):

// struct devregs{
//       /* stuff */
// }volatile v_decl;
// If you do want to get a shorthand way of attaching a qualifier to another type, you can use typedef to do it:

// struct x{
//       int a;
// };
// typedef const struct x csx;

// csx const_sx;
// struct x non_const_sx = {1};

// const_sx = non_const_sx;        /* error - attempt to modify a const */
// 8.4.2.1. Indivisible Operations
// Those of you who are familiar with techniques that involve hardware interrupts and other ‘real time’ aspects of programming will recognise the need for volatile types. Related to this area is the need to ensure that accesses to data objects are ‘atomic’, or uninterruptable. To discuss this is any depth would take us beyond the scope of this book, but we can at least outline some of the issues.

// Be careful not to assume that any operations written in C are uninterruptable. For example,

// extern const volatile unsigned long realtimeclock;
// could be a counter which is updated by a clock interrupt routine. It is essential to make it volatile because of the asynchronous updates to it, and it is marked const because it should not be changed by anything other than the interrupt routine. If the program accesses it like this:

// unsigned long int time_of_day;

// time_of_day = real_time_clock;
// there may be a problem. What if, to copy one long into another, it takes several machine instructions to copy the two words making up real_time_clock and time_of_day? It is possible that an interrupt will occur in the middle of the assignment and that in the worst case, when the low-order word of real_time_clock is 0xffff and the high-order word is 0x0000, then the low-order word of time_of_day will receive 0xffff. The interrupt arrives and increments the low-order word of real_time_clock to 0x0 and then the high-order word to 0x1, then returns. The rest of the assignment then completes, with time_of_day ending up containing 0x0001ffff and real_time_clock containing the correct value, 0x00010000.

// This whole class of problem is what is known as a critical region, and is well understood by those who regularly work in asynchronous environments. It should be understood that Standard C takes no special precautions to avoid these problems, and that the usual techniques should be employed.

// The header ‘signal.h’ declares a type called sig_atomic_t which is guaranteed to be modifiable safely in the presence of asynchronous events. This means only that it can be modified by assigning a value to it; incrementing or decrementing it, or anything else which produces a new value depending on its previous value, is not safe.

