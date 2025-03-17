#include <iostream>
#include <thread>
#include <vector>

class Philosopher {
public:
    Philosopher(int id) : id(id) {}
    void dine();

private:
    int id;
    void think();
    void eat();
};

void Philosopher::dine() {
    while (true) {
        think();
        eat();
    }
}

void Philosopher::think() {
    std::cout << "Philosopher " << id << " is thinking.\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

void Philosopher::eat() {
    std::cout << "Philosopher " << id << " is eating.\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <number_of_philosophers>\n";
        return 1;
    }

    int numPhilosophers = std::stoi(argv[1]);
    std::vector<std::thread> threads;
    std::vector<Philosopher> philosophers;

    for (int i = 0; i < numPhilosophers; ++i) {
        philosophers.emplace_back(i);
    }

    for (int i = 0; i < numPhilosophers; ++i) {
        threads.emplace_back(&Philosopher::dine, &philosophers[i]);
    }

    for (auto& t : threads) {
        t.join();
    }

    return 0;
}
