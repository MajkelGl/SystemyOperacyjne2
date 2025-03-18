#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <chrono>

using namespace std;

const int MAX_PHILOSOPHERS = 10;
atomic<bool> flag[MAX_PHILOSOPHERS];
atomic<int> turn;
int num_philosophers;

void philosopher(int id) {
    for (int i = 0; i < 5; ++i) { // Each philosopher is trying to each 5 times
        // Entrance to critical section
        flag[id] = true;
        turn = id;
        for (int j = 0; j < num_philosophers; ++j) {
            if (j != id) {
                while (flag[j] && turn == id);
            }
        }

        // Critical section - eating
        cout << "Philosopher " << id << " is eating.\n";
        this_thread::sleep_for(chrono::milliseconds(500));

        // Exit from critical section
        flag[id] = false;
        cout << "Philosopher " << id << " finished eating and is thinking.\n";
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

    vector<thread> philosophers;
    for (int i = 0; i < num_philosophers; ++i) {
        flag[i] = false;
    }

    for (int i = 0; i < num_philosophers; ++i) {
        philosophers.emplace_back(philosopher, i);
    }

    for (auto& phil : philosophers) {
        phil.join();
    }

    return 0;
}
