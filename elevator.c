#include "hw6.h"
#include <stdio.h>
#include<pthread.h>
#include <time.h>
#include <stdlib.h>



struct Elevator {
	pthread_mutex_t lock;

	int current_floor;
	int direction;
	int destination_floor;
	int occupancy;
	enum { ELEVATOR_ARRIVED, ELEVATOR_OPEN, ELEVATOR_CLOSED } state;
	pthread_barrier_t door_open, door_close;
}elevators[ELEVATORS];

void scheduler_init() {
	for (int i = 0; i < ELEVATORS; i++) {
		elevators[i].current_floor = 0;
		elevators[i].direction = -1;
		elevators[i].occupancy = 0;
		elevators[i].state = ELEVATOR_ARRIVED;
		pthread_mutex_init(&elevators[i].lock, 0);
		pthread_barrier_init(&elevators[i].door_open, 0, 2);
		pthread_barrier_init(&elevators[i].door_close, 0, 2);
		elevators[i].destination_floor = 0;
	}
}


void passenger_request(int passenger, int from_floor, int to_floor,
	void(*enter)(int, int),
	void(*exit)(int, int))
{
	int elevator = random() % ELEVATORS;
	while (elevators[elevator].occupancy == 1)
	{
		elevator = random() % ELEVATORS;
		//printf("\n**************");

	}
	// wait for the elevator to arrive at our origin floor, then get in
	//int waiting = 1;
	//while(waiting) {
	pthread_mutex_lock(&elevators[elevator].lock);

	elevators[elevator].destination_floor = from_floor;
	//if(current_floor == from_floor && state == ELEVATOR_OPEN && occupancy==0) {

	pthread_barrier_wait(&elevators[elevator].door_open);
	if (elevators[elevator].occupancy == 0)
	{
		enter(passenger, elevator);
		elevators[elevator].occupancy++;
	}
	elevators[elevator].destination_floor = to_floor;

	//waiting=0;
//}

//pthread_mutex_unlock(&lock);
//}
	pthread_barrier_wait(&elevators[elevator].door_close);

	// wait for the elevator at our destination floor, then get out
	//int riding=1;
	//while(riding) {
		//pthread_mutex_lock(&lock);

		//if(current_floor == to_floor && state == ELEVATOR_OPEN) {
	pthread_barrier_wait(&elevators[elevator].door_open);
	if (elevators[elevator].occupancy == 1)
	{
		exit(passenger, elevator);
		elevators[elevator].occupancy--;
	}
	elevators[elevator].destination_floor = -1;


	//riding=0;
//}
	pthread_barrier_wait(&elevators[elevator].door_close);

	pthread_mutex_unlock(&elevators[elevator].lock);
	//}
}

void elevator_ready(int elevator, int at_floor,
	void(*move_direction)(int, int),
	void(*door_open)(int), void(*door_close)(int)) {
	//if(elevator!=0) return;

	//pthread_mutex_lock(&lock);
	if (elevators[elevator].destination_floor == at_floor && elevators[elevator].state == ELEVATOR_ARRIVED) {
		door_open(elevator);
		elevators[elevator].state = ELEVATOR_OPEN;
		pthread_barrier_wait(&elevators[elevator].door_open);
		pthread_barrier_wait(&elevators[elevator].door_close);
	}
	else if (elevators[elevator].state == ELEVATOR_OPEN) {
		door_close(elevator);
		elevators[elevator].state = ELEVATOR_CLOSED;
	}
	else {
		if (at_floor == 0 || at_floor == FLOORS - 1)
			elevators[elevator].direction *= -1;
		if (elevators[elevator].destination_floor != -1) {
			move_direction(elevator, elevators[elevator].direction);
			elevators[elevator].current_floor = at_floor + elevators[elevator].direction;
		}
		elevators[elevator].state = ELEVATOR_ARRIVED;
		
	}
	//pthread_mutex_unlock(&lock);
}
