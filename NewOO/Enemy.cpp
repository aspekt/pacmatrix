#include <cstdlib>
#include <iostream>
#include <vector>

#include "inc/Enemy.h"

void Enemy::pursuitPlayer(Player* p)
{
	float dx = p->pos.x - pos.x;
	float dy = p->pos.y - pos.y;

	float dir_x = 1;
	float dir_y = 1;

	if (dx < 0)
		dir_x = -1;
	if (dy < 0)
		dir_y = -1;

	// calculate new values
	if (abs((long)dy) < abs((long)dx) || abs((long)dx) == abs((long)dy)) {
		pos.x += (Enemy::ENEMY_MOVEMENT * dir_x);
	} else {
		pos.y += (Enemy::ENEMY_MOVEMENT * dir_y);
	}
}

/* move the enemy towards the player */
void Enemy::move(Player* p)
{
    pursuitPlayer(p);
    cout << m_name << " is chasing player " << p->getName() << " at " << pos.x << "," << pos.y << "," << pos.z << endl;
}
