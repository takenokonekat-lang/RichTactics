#pragma once
#include "MapParameter.h"
#include "../Collision/CollisionParameter.h"

void InitMap();
void LoadMap();
void StartMap();
void DrawMap();
void FinMap();

void MoveWithMapCollision(Body* body);
