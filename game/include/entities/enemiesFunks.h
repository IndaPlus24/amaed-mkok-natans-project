#ifndef ENEMIES_F_H
#define ENEMIES_F_H

#include "gameData.h"

void EnemyMovement(Enemy *enemy, Vector2 target);
void EnemyAttack(Enemy *enemy, Player *player);
bool EnemyLineOfSight(Enemy *enemy, Player *player, Room *room);
void EnemyUpdate(Enemy *enemy, GameData *gameData);
void EnemyDraw(Enemy *enemy);
Enemies CreateEnemies(EnemySeeder *seeder);

EnemySeeder* CreateEnemySeeder(int count, Vector2* positions, EnemyType* type, EnemyBehavior* behavior);

#endif