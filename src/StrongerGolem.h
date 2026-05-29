#ifndef SGOLEM_H
#define SGOLEM_H

#include "Enemy.h"
#include <string>
#include "Map.h"
#include "golem.h"

class StrongerGolem : public Golem
{
    public:
        StrongerGolem(Vector2 position);
        ~StrongerGolem();
        void TakeDamage(int amount) override;

};

#endif