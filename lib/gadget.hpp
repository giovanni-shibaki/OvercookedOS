/*
    Overcooked OS
    Giovanni Shibaki Camargo        11796444
    Lucas Keiti Anbo Mihara         11796472
    Pedro Kenzo Muramatsu Carmo     11796451
*/

#ifndef GADGET_H
#define GADGET_H

#include <thread>
#include <vector>
#include <iostream>
#include <ncurses.h>

#include "colours.hpp"

using namespace std;

class Gadget
{
    protected:
        int idGadget;
        int energyGadget; // consumed energy by unit of time
        int isDone; // 0: parado, 1: trabalhando, 2: concluido
        int defaultEnergy;

    public:
        Gadget(int id, int defaultEnergy)
        {
            this->idGadget = id;
            this->defaultEnergy = defaultEnergy;
        }
        // SETTERS
        void setIdGadget(int id)
        {
            this->idGadget = id;
        }
        // GETTERS

        int getDefaultEnergy()
        {
            return this->defaultEnergy;
        }

        int getIdGadget()
        {
            return this->idGadget;
        }

        int getEnergyGadget()
        {
            return this->energyGadget;
        }

        int getIsDone()
        {
            return this->isDone;
        }

        virtual bool activate()
        {
        }

        virtual void receiveEnergy()
        {
        }

        virtual vector<string> print()
        {
            vector<string> sprite;
            sprite.push_back("Erro2");
            return sprite;
        }
};

#endif