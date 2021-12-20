/*
    Overcooked OS
    Giovanni Shibaki Camargo        11796444
    Lucas Keiti Anbo Mihara         11796472
    Pedro Kenzo Muramatsu Carmo     11796451
*/

#ifndef ELECTRICITY_H
#define ELECTRICITY_H

// changing Library according to OS
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#define sleep usleep
#endif

#include <queue>
#include <iostream>
#include <thread>
#include "gadget.hpp"

#define MAX_ENERGY 100

using namespace std;

#include <semaphore.h>
#include <mutex>
#include <unistd.h>
#include <pthread.h>

// GLOBAL VARIABLES
sem_t notEmpty;
sem_t notFull;
mutex mtx;
int curEnergy = 0;
queue<Gadget*> gadgetQueue;
bool hasTimeElect = true;

static void* generateEnergy(void* arg)
{
    while(hasTimeElect)
    {
        sem_wait(&notFull);
        mtx.lock();
        curEnergy++; // insert_item()
        mtx.unlock();
        sem_post(&notEmpty);
        sleep(1000000);                                         
    }
}

static void* consumeEnergy(void* arg)
{
    while(hasTimeElect)
    {
        if(!gadgetQueue.empty())
        {
            Gadget *gt = gadgetQueue.front();
            gadgetQueue.pop();

            sem_wait(&notEmpty); // sem_wait -> decrease value
            mtx.lock();
            curEnergy--; // remove_item()
            mtx.unlock();
            sem_post(&notFull); // sem_post increase value
            gt->receiveEnergy(); // consume_item()
            sleep(500000);
        }
    }
}

class Electricity{
    private:
        pthread_t producer, consumer;

    public:
        void start()
        {

            pthread_attr_t attr;

            sem_init(&notEmpty,0 ,0);
            sem_init(&notFull, 0, 100);

            pthread_attr_init(&attr);
            pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

            int r;

            r = pthread_create(&this->producer, &attr, generateEnergy, NULL);
            if(r)
            {
                cout<<"Error in creating thread"<<endl;
                exit(-1);
            }
            
            r = pthread_create(&this->consumer, &attr, consumeEnergy, NULL);
            if(r)
            {
                cout<<"Error in creating thread"<<endl;
                exit(-1);
            }

            pthread_attr_destroy(&attr);
        }

        void stop()
        {
            hasTimeElect = false;
        }

        void insertGadgetQueue(Gadget *gt)
        {
            gadgetQueue.push(gt);
        }

        int getCurEnergy()
        {
            return curEnergy;
        }
};

#endif