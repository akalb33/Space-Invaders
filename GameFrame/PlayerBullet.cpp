#include "stdafx.h"
#include "Util.h"
#include "PlayerBullet.h"
#include "Particle.h"
#include "Enemy.h"
#include "ScoreManager.h"
#include "Character.h"

PlayerBullet::PlayerBullet()
   : Character(typeid(PlayerBullet))
{
   SetTexture(TextureManager::playerBullet);
   SetDisplayOrder(170);
   mScale = 0.1f;
	mVelocityY = -850.0f;
}

PlayerBullet::~PlayerBullet()
{
}

void PlayerBullet::Process(float deltaTime)
{
   ApplyInertia(deltaTime);


   if (mY < 0.0f)
   {
		Kill();
   }
   else
   {
      if (DetectCollisionAndKillBoth<Enemy>(false))
      {
         mpSoundManager->Play(SoundManager::bulletContact);
         Particle::GenerateParticles(20, mX, mY);
         ScoreManager::AddPoints(100);
      }

   }
 
}