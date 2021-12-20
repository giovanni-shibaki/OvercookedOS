/*
    Overcooked OS
    Giovanni Shibaki Camargo        11796444
    Lucas Keiti Anbo Mihara         11796472
    Pedro Kenzo Muramatsu Carmo     11796451
*/

#ifndef ORDER_MANAGER_H
#define ORDER_MANAGER_H

    #include <bits/stdc++.h>
    #include "order.hpp"

    // changing Library according to OS
    #ifdef _WIN32
    #include <Windows.h>
    #else
    #include <unistd.h>
    #define sleep usleep
    #endif

    using namespace std;

    bool hasTimeOrderMan = true;

    static void *addNewOrder(void *arg)
    {
        vector<Order> *ords = (vector<Order> *)arg;

        while (hasTimeOrderMan)
        {
            sleep(5000000 + rand() % 5000000);
            if (ords->size() < 6)
            {
                ords->emplace_back(Order());
            }
        }
    }

    class OrderManager
    {
    private:
        vector<Order> orderVec;
        pthread_t addOrder;

    public:
        vector<Order> getOrders()
        {
            return orderVec;
        }

        bool decreaseGrill()
        {
            if (!orderVec.empty())
            {
                bool flag = false;
                int aux = 0;
                while (!flag && aux < orderVec.size())
                {
                    if (orderVec.at(aux).getNumGrill() != 0)
                    {
                        orderVec.at(aux).decreaseNumGrill();
                        flag = true;
                        
                        // Check if
                        if(orderVec.at(aux).remainingItems() == 0)
                        {
                            orderVec.erase(orderVec.begin()+aux);
                            return true;
                        }
                    }
                    aux++;
                }
            }
            return false;
        }

        bool decreaseBlender()
        {
            if (!orderVec.empty())
            {
                bool flag = false;
                int aux = 0;
                while (!flag && aux < orderVec.size())
                {
                    if (orderVec.at(aux).getNumBlender() != 0)
                    {
                        orderVec.at(aux).decreaseNumBlender();
                        flag = true;

                        // Check if
                        if(orderVec.at(aux).remainingItems() == 0)
                        {
                            orderVec.erase(orderVec.begin()+aux);
                            return true;
                        }
                    }
                    aux++;
                }
            }
            return false;
        }

        bool decreaseIceCream()
        {
            if (!orderVec.empty())
            {
                bool flag = false;
                int aux = 0;
                while (!flag && aux < orderVec.size())
                {
                    if (orderVec.at(aux).getNumIceCream() != 0)
                    {
                        orderVec.at(aux).decreaseNumIceCream();
                        flag = true;

                        // Check if
                        if(orderVec.at(aux).remainingItems() == 0)
                        {
                            orderVec.erase(orderVec.begin()+aux);
                            return true;
                        }
                    }
                    aux++;
                }
            }
            return false;
        }

        void start()
        {
            
            pthread_attr_t attr;

            pthread_attr_init(&attr);
            pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

            pthread_create(&this->addOrder, &attr, addNewOrder, &(this->orderVec));

            pthread_attr_destroy(&attr);
        }

        void stop()
        {
            hasTimeOrderMan = false;
        }
    };

#endif