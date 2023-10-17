# The Barbershop problem
## Introduction
The original barbershop problem was proposed by Dijkstra. A variation of it
appears in Silberschatz and Galvin’s Operating Systems Concepts.


A barbershop consists of a waiting room with n chairs, and the
barber room containing the barber chair. If there are no customers
to be served, the barber goes to sleep. If a customer enters the
barbershop and all chairs are occupied, then the customer leaves
the shop. If the barber is busy, but chairs are available, then the
customer sits in one of the free chairs. If the barber is asleep, the
customer wakes up the barber. Write a program to coordinate the
barber and the customers.

## Similar implementation
We implemented the following problem, which is similar to the barbershop problem
### Car Wash Problem
Imagine a car wash service with the following constraints:

* **One Car Wash Bay**: There's only one car wash bay where cars are washed. If the bay is empty, a car can directly go into the bay for a wash. If the bay is occupied, the car has to wait.

* **Waiting Area:** There's a waiting area that can accommodate *n* cars. When the bay is occupied, arriving cars will park in the waiting area. If the waiting area is full, the car has to come back later.

* **Attendant**: An attendant is responsible for signaling cars to move into the bay when it's free. If there are no cars in the waiting area and the bay is free, the attendant can take a break.

## Purpose
The purpose of this project is to implement a semaphore from the [little book of semaphores](https://greenteapress.com/semaphores/LittleBookOfSemaphores.pdf) for the school subject R.5.A.10. 

## Group
CAPELIER Marla, CECCARELLI Luca, CLEMENT Romain, SIMAILA Djalim
