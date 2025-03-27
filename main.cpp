#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <cstdio>
#include <cstdlib>

using namespace std;

const int MAX_PHILOSOPHERS = 10;
int num_philosophers;

int state[MAX_PHILOSOPHERS]; // 0 - thinking, 1 - hungry, 2 - eating
bool forks[MAX_PHILOSOPHERS];
bool access_control = false; // Simple lock
bool print_lock = false; // Lock for printing

void print_status(int id, const char* status) {
    while (print_lock) {} // Wait until print access is free
    print_lock = true; // Lock print access
    printf("[Philosopher %d] %s\n", id, status);
    fflush(stdout);
    print_lock = false; // Unlock print access
}

void pick_up_forks(int id) {
    int left = id;
    int right = (id + 1) % num_philosophers;

    state[id] = 1; // Hungry

    while (true) {
        while (access_control) {} // Wait until global access is free
        access_control = true; // Lock global access

        if (!forks[left]) {
            forks[left] = true;
            if (!forks[right]) {
                forks[right] = true;
                state[id] = 2; // Now eating
                access_control = false; // Unlock global access
                print_status(id, "is eating.");
                return;
            } else {
                forks[left] = false; // Release left fork if right is unavailable
            }
        }
        access_control = false; // Unlock global access
        this_thread::sleep_for(chrono::milliseconds(rand() % 50 + 10));
    }
}

void put_down_forks(int id) {
    int left = id;
    int right = (id + 1) % num_philosophers;

    while (access_control) {} // Wait until global access is free
    access_control = true; // Lock global access

    forks[left] = false;
    forks[right] = false;
    state[id] = 0;
    access_control = false; // Unlock global access

    print_status(id, "finished eating and is thinking.");
}

void philosopher(int id) {
    while (true) {
        print_status(id, "is thinking.");
        this_thread::sleep_for(chrono::milliseconds(rand() % 200 + 100));

        print_status(id, "is hungry.");
        pick_up_forks(id);

        this_thread::sleep_for(chrono::milliseconds(rand() % 500 + 200)); // Eating time
        put_down_forks(id);
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
