/*
    Overcooked OS
    Giovanni Shibaki Camargo        11796444
    Lucas Keiti Anbo Mihara         11796472
    Pedro Kenzo Muramatsu Carmo     11796451
*/

#ifndef ORDER_H
#define ORDER_H

// changing Library according to OS
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#define sleep usleep
#endif

#include <stdlib.h>
#include <bits/stdc++.h>

class Order
{
    private:
        int numGrill;
        int numIceCream;
        int numBlender;

    public:
        Order()
        {
            numGrill = rand() % 4; // 0 to 3 grill
            numIceCream = rand() % 4;
            numBlender = rand() % 4;
        }

        Order(int numGrill, int numIceCream, int numBlender)
        {
            this->numGrill = numGrill;
            this->numIceCream = numIceCream;
            this->numBlender = numBlender;
        }

        int remainingItems()
        {
            return (numGrill + numIceCream + numBlender);
        }

        int getNumGrill()
        {
            return numGrill;
        }

        int getNumBlender()
        {
            return numBlender;
        }

        int getNumIceCream()
        {
            return numIceCream;
        }

        void decreaseNumGrill()
        {
            numGrill--;
        }

        void decreaseNumBlender()
        {
            numBlender--;
        }

        void decreaseNumIceCream()
        {
            numIceCream--;
        }

        void receiveProduct(/*???*/)
        {

        }
};

#endif