# SystemyOperacyjne2
Documentation for project


Problem Description

The Dining Philosophers problem is a classic synchronization problem in computer science. It involves multiple philosophers who alternate between thinking and eating while sharing limited resources (forks). The challenge is to design a system where philosophers can eat without leading to deadlock or starvation.

How to Run the Project

Compilation

This program is written in C++ and uses std::thread for multithreading.

# Compile the program
g++ -std=c++20 -o main main.cpp -lpthread

Execution

To run the program, specify the number of philosophers as a command-line argument:

main <number_of_philosophers>

For example, to run with 5 philosophers:

dining_philosophers 5

Threads Used

The program creates one thread per philosopher. Each thread represents an individual philosopher who cycles between thinking, being hungry, and eating.

Philosopher Threads: Each philosopher follows an infinite loop of thinking, becoming hungry, picking up forks, eating, and putting down forks.

Critical Sections and Solution

There are two critical sections in the program:

Accessing Forks:

Before a philosopher can eat, they must acquire two forks.

The program ensures that forks are picked up one at a time.

If the second fork is unavailable, the first fork is put back to avoid deadlocks.

Printing to Console:

Since multiple threads print status messages, a global flag (print_lock) ensures that only one thread prints at a time, preventing message interleaving.

Synchronization Mechanism

Instead of using mutex or atomic variables, a simple busy-wait flag mechanism is used:

Global access_control flag ensures that only one philosopher at a time modifies fork availability.

Global print_lock flag ensures orderly output to stdout.

Philosophers retry in a loop with a short delay (sleep_for) if they fail to acquire both forks.

Expected Output Example

[Philosopher 0] is thinking.
[Philosopher 1] is thinking.
[Philosopher 4] is thinking.
[Philosopher 3] is thinking.
[Philosopher 2] is thinking.
[Philosopher 0] is hungry.
[Philosopher 0] is eating.
[Philosopher 1] is hungry.
[Philosopher 3] is hungry.
[Philosopher 3] is eating.
[Philosopher 2] is hungry.
[Philosopher 4] is hungry.
[Philosopher 0] finished eating and is thinking.
[Philosopher 0] is thinking.
[Philosopher 3] finished eating and is thinking.

This output shows philosophers transitioning between states while ensuring synchronization.

Notes

The program prevents deadlocks by enforcing a rule where a philosopher only takes both forks when both are available.

The busy-waiting approach is simple but can be optimized further using more sophisticated synchronization techniques.
