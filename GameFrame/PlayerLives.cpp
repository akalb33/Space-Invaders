#include "stdafx.h"
#include "Util.h"
#include "PlayerLives.h"
#include "Enemy.h"
#include "PlayerBullet.h"
#include "Character.h"

PlayerLives::PlayerLives()
   : Character(typeid(PlayerLives))
{
   SetTexture(TextureManager::player);
   SetDisplayOrder(180);
   mScale = 0.05f;
}

PlayerLives::~PlayerLives()
{
}

void PlayerLives::Process(float deltaTime)
{
  
}