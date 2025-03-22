#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <cstdio>

using namespace std;

const int MAX_PHILOSOPHERS = 10;
int num_philosophers;

int state[MAX_PHILOSOPHERS]; // 0 - thinking, 1 - waiting, 2 - eating
bool forks[MAX_PHILOSOPHERS];

/**
 * @brief Prints philosopher status in a readable format.
 */
void print_status(int id, const char* status) {
    printf("[Philosopher %d] %s\n", id, status);
    fflush(stdout);
}

/**
 * @brief Checks if philosopher can start eating.
 */
void try_to_eat(int id) {
    int left = id;
    int right = (id + 1) % num_philosophers;

    if (state[id] == 1 && !forks[left] && !forks[right]) {
        forks[left] = true;
        forks[right] = true;
        state[id] = 2;
        print_status(id, "is eating.");
        this_thread::sleep_for(chrono::milliseconds(500));

        // Release forks after eating
        forks[left] = false;
        forks[right] = false;
        state[id] = 0;
        print_status(id, "finished eating and is thinking.");
    }
}

/**
 * @brief Philosopher behavior with proper fork checking.
 */
void philosopher(int id) {
    for (int i = 0; i < 5; ++i) { // Each philosopher eats 5 times
        print_status(id, "is thinking.");
        this_thread::sleep_for(chrono::milliseconds(500));

        state[id] = 1; // Mark as waiting for forks

        // Keep trying to eat until successful
        while (state[id] == 1) {
            try_to_eat(id);
            this_thread::sleep_for(chrono::milliseconds(10)); // Small delay to prevent CPU overload
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <number_of_philosophers>\n";
        return 1;
    }

    num_philosophers = stoi(argv[1]);
    if (num_philosophers > MAX_PHILOSOPHERS || num_philosophers < 2) {
        cerr << "Number of philosophers must be between 2 and " << MAX_PHILOSOPHERS << "\n";
        return 1;
    }

    // Initialize state and forks
    for (int i = 0; i < num_philosophers; ++i) {
        state[i] = 0;
        forks[i] = false;
    }

    vector<thread> philosophers;
    for (int i = 0; i < num_philosophers; ++i) {
        philosophers.emplace_back(philosopher, i);
    }

    for (auto& phil : philosophers) {
        phil.join();
    }

    return 0;
}
