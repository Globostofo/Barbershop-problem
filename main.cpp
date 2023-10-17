#include <iostream>
#include <chrono>
#include <semaphore>
#include <thread>
#include <vector>
#include <mutex>

using namespace std;

mutex cout_mutex;
#define print(X) cout_mutex.lock(); cout << X << endl; cout_mutex.unlock();
#define N 25

// GLOBAL VARIABLES
int carsWaiting = 0;
mutex mutexCar;
binary_semaphore carArrived(0);
binary_semaphore bayFree(0);
binary_semaphore carWashed(0);
binary_semaphore washingDone(0);

void car(size_t id)
{
    // car arrives
    print("Car " << id << " arrives at the car wash");
    mutexCar.lock();
    if (carsWaiting == N) {
        mutexCar.unlock();
        print("Car " << id << " leaves due to no waiting space");
        return;
    }
    ++carsWaiting;
    mutexCar.unlock();

    // rendezvous 1
    carArrived.release();  // signal X
    bayFree.acquire();     // wait Y

    // getCarWash
    print("Car " << id << " is being washed");
    this_thread::sleep_for(chrono::seconds(1));
    print("Car " << id << " is washed");

    // rendezvous 2
    carWashed.release();  // signal X'
    washingDone.acquire(); // wait Y'

    // car leaves
    mutexCar.lock();
    --carsWaiting;
    mutexCar.unlock();
    print("Car " << id << " leaves the car wash");
}

void attendant()
{
    while (true) {
        // rendezvous 1
        carArrived.acquire();  // wait X
        bayFree.release();     // signal Y

        // washCar
        print("Attendant starts washing a car");
        this_thread::sleep_for(chrono::seconds(1));
        print("Attendant finishes washing");

        // rendezvous 2
        carWashed.acquire();   // wait X'
        washingDone.release(); // signal Y'
    }
}


int main()
{
    print("Car wash opening with " << N << " waiting spaces");

    thread attendantThread(attendant);
    attendantThread.detach();

    vector<thread> cars {};
    for(size_t i = 0; i < N+1; ++i)
    {
        thread t(car, i);
        cars.push_back(std::move(t));
    }

    for (auto &car: cars) {
        car.join();
    }

    return 0;
}
