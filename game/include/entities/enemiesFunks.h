#ifndef ENEMIES_F_H
#define ENEMIES_F_H

#include "gameData.h"

void EnemyMovement(Enemy *enemy, Vector2 target, GameData *gameData);
void EnemyAttack(Enemy *enemy, GameData *gameData);
bool EnemyLineOfSight(Enemy *enemy, Player *player, Room *room);
void EnemyUpdate(Enemy *enemy, GameData *gameData);
void EnemyDraw(Enemy *enemy);
Enemies CreateEnemies(EnemySeeder *seeder);
void EnemyGetHit(Enemy *enemy, float damage, Vector2 force);



EnemySeeder* CreateEnemySeeder(int count, Vector2* positions, EnemyType* type, EnemyBehavior* behavior);

#endif