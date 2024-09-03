# dynamic_structures

These examples are an attempt to take a previously static structure that used defines to set the sizes of its members and change it to a dynamic structure that uses environment variables at run-time.

The structure in question has many different flavors of members, and all must be working to match the output of the "static" example.

To run static to see the expected behavior, first run "make" from the complex directory. This will create a "first_program" and "second_program" in both the static and dynamic folders.

Then, open two terminals side-by-side and cd to "static".

In one terminal run "first_program" and then in another terminal run "second_program" while "first_program" is still running. You should see that same structure values printed on both screens.

When you kill "first_program" with ctrl+c the allocated shared memory on the system will be destroyed, so don't kill it until after you have run "second_program".

The dynamic folder is an attempt to mimic the behavior of static, but using environment variables.

The two conditions of this example is that the envars must be used at run-time, NOT compile-time, the original structure members must stay intact (ie we add offset members), and of course the structure members using the envars must be dynamic.

There are various functions called in main() that do different things such as set test values, zero out the structure, print the structure, detach the structure and destroy the structure.
