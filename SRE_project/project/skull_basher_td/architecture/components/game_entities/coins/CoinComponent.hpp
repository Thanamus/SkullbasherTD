//
// Created by kb on 31/01/2021.
//

#include "../../Component.hpp"

class Pathfinder;

class CoinComponent :  public Component {
public:
    explicit CoinComponent(GameObject* gameObject);
    ~CoinComponent();

private:
    float money = 0;
public:
    float getMoney() const;

    void setMoney(float money);
};
