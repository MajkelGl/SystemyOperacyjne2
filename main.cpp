#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <mutex>

using namespace std;

const int MAX_PHILOSOPHERS = 10;
int num_philosophers;

int state[MAX_PHILOSOPHERS]; // 0 - thinking, 1 - hungry, 2 - eating
bool forks[MAX_PHILOSOPHERS];

mutex fork_mutex[MAX_PHILOSOPHERS]; // Mutexy dla widełców
mutex print_mutex; // Mutex do synchronizacji wypisywania

void print_status(int id, const char* status) {
    lock_guard<mutex> lock(print_mutex); // Blokada mutexa na czas wypisywania
    printf("[Philosopher %d] %s\n", id, status);
    fflush(stdout);
}

void pick_up_forks(int id) {
    int left = id;
    int right = (id + 1) % num_philosophers;

    state[id] = 1; // Hungry
    print_status(id, "is hungry.");

    while (true) {
        lock_guard<mutex> left_lock(fork_mutex[left]); // Zablokowanie lewego widelca
        if (!forks[left]) {
            forks[left] = true;

            lock_guard<mutex> right_lock(fork_mutex[right]); // Zablokowanie prawego widelca
            if (!forks[right]) {
                forks[right] = true;

                state[id] = 2; // Eating
                print_status(id, "is eating.");
                return;
            }
            // Jeśli prawy widelec nie jest dostępny, zwolnij lewy
            forks[left] = false;
        }
        this_thread::sleep_for(chrono::milliseconds(rand() % 50 + 10));
    }
}

void put_down_forks(int id) {
    int left = id;
    int right = (id + 1) % num_philosophers;

    lock_guard<mutex> left_lock(fork_mutex[left]);
    lock_guard<mutex> right_lock(fork_mutex[right]);

    forks[left] = false;
    forks[right] = false;
    state[id] = 0;

    print_status(id, "finished eating and is thinking.");
}

void philosopher(int id) {
    while (true) {
        print_status(id, "is thinking.");
        this_thread::sleep_for(chrono::milliseconds(rand() % 200 + 100));

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
