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
int customers = 0;
mutex mutexCustomer;
counting_semaphore customer(0);
counting_semaphore barber(0);
counting_semaphore customerDone(0);
counting_semaphore barberDone(0);

void client(size_t id)
{
    // customer enters
    print("Customer " << id << " enters the barbershop");
    mutexCustomer.lock();
    if (customers == N) {
        mutexCustomer.unlock();
        print("Customer " << id << " bulks");
        return;
    }
    ++customers;
    mutexCustomer.unlock();

    // rendezvous 1
    customer.release();         // signal X
    barber.acquire();           // wait Y

    // getHairCut
    print("Customer " << id << " get his hair cut");
    this_thread::sleep_for(chrono::seconds(1));
    print("Customer " << id << " got a haircut");

    // rendezvous 2
    customerDone.release();     // signal X'
    barberDone.acquire();       // wait Y'

    // customer leaves
    mutexCustomer.lock();
    --customers;
    mutexCustomer.unlock();
    print("Customer " << id << " leaves the barbershop");
}

void barber()
{
    while (true) {
        // rendezvous 1
        customer.acquire();     // wait X
        barber.release();       // signal Y

        // cutHair
        print("Barber starts to cut hair");
        this_thread::sleep_for(chrono::seconds(1));
        print("Barber stops to cut hair");

        // rendezvous 2
        customerDone.acquire(); // wait X'
        barberDone.release();   // signal Y'
    }
}


int main()
{
    print("Barbershop opening with " << N << " chairs");
    thread barberThread(barberProcedure);
    vector<thread> clients {};
    for(size_t i = 0; i < N+1; ++i)
    {
        thread t(client, i);
        clients.push_back(std::move(t));
    }

    barber_thread.join();
    for (auto &client: clients) {
        client.join();
    }

    return 0;
}
