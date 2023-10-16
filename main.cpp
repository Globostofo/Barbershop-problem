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
counting_semaphore free_chairs(N);
binary_semaphore barber_free(1);
binary_semaphore barber_sem(1);

void get_haircut(size_t id)
{
    this_thread::sleep_for(chrono::seconds(10));
    print("Client " << id << " got a haircut");
}

void cutHair()
{
    this_thread::sleep_for(chrono::seconds(10));
    print("Barber cut hair");
}

void client(size_t id)
{
    print("Client " << id << " arrived");
    bool isAChairFree = free_chairs.try_acquire();
    if (!isAChairFree){
        print("no chairs available");
        print("Client " << id << " left");
        return; //Bye bye  
    } 

    //wait for barber    
    barber_free.acquire();
    print("Client " << id << " woke the barber up");
    barber_sem.release();
    // Get a haircut
    get_haircut(id);
    // Bye bye;
    free_chairs.release();
    print("Client " << id << " left");
}

void barber()
{
    while (true) {
        print("Barber is sleeping");
        barber_sem.acquire();
        print("Barber woke up");
        cutHair();
        barber_free.release();
    }
}


int main()
{
    barber_sem.acquire();
    thread barber_thread(barber);
    print("Client will arrive");
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
