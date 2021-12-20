/*
    Overcooked OS
    Giovanni Shibaki Camargo        11796444
    Lucas Keiti Anbo Mihara         11796472
    Pedro Kenzo Muramatsu Carmo     11796451
*/

#ifndef ICE_CREAM_H
#define ICE_CREAM_H

#include "electricity.hpp"

using namespace std;

class IceCream: public Gadget
{
    private:
        Electricity elect;

    public:
        IceCream(Electricity elect) : Gadget(2, 6)
        {
            this->energyGadget = this->defaultEnergy;
            this->elect = elect;
            this->isDone = 0;
        }
        
        bool activate()
        {
            if(this->isDone == 0)
            {
                this->energyGadget = this->defaultEnergy;
                this->isDone = 1;
                elect.insertGadgetQueue(this);
                return false;
            }
            else if(this->isDone == 2)
            {
                // Coleta
                this->isDone = 0;
                this->energyGadget = this->defaultEnergy;
                return true;
            }
            return false;
        }

        void receiveEnergy()
        {
            this->energyGadget--;
            if(this->energyGadget > 0)
            {
                elect.insertGadgetQueue(this);
            }
            else
            {
                this->isDone = 2;
            }
        }

        vector<string> print()
        {
            vector<string> sprite;
            sprite.push_back("    /´-''''-`\\  ");
            sprite.push_back("   |          | ");
            sprite.push_back("   \\_.--._.-._/ ");
            sprite.push_back("     \\=-=-=-/   ");
            sprite.push_back("      \\=-=-/    ");
            sprite.push_back("       \\=-/     ");
            sprite.push_back("        \\/      ");
            return sprite;
        }
};

#endif