#include "stdafx.h"
#include "Util.h"
#include "Player.h"
#include "Enemy.h"
#include "Particle.h"
#include "PlayerBullet.h"
#include "Character.h"

Player::Player()
   : Character(typeid(Player))
   , mSpacebarPreviouslyPressed(false)
{
   SetTexture(TextureManager::player);
   SetDisplayOrder(180);
   mScale = 0.1f;
   mPolygon.SetOrigin(356.0f, 406.0f);
   mPolygon.AddPoint(360.0f, 14.0f);
   mPolygon.AddPoint(261.0f, 329.0f);
   mPolygon.AddPoint(2.0f, 600.0f);
   mPolygon.AddPoint(712.0f, 604.0f);
   mPolygon.AddPoint(459.0f, 330.0f);
}

Player::~Player()
{
}

void Player::Reset()
{ 
   mAngularVelocity = 0.0f;
   mAngle = 0.0f; 
   mX = Character::GetScreen()->GetViewWidth() / 2.0f;
   mY = 1000.0f;
}

void Player::Process(float deltaTime)
{
   if (GetStatus() == dying)
   {
      ProcessDying(deltaTime);
   }
  else
   {
      // Movement Logic
	   mVelocityX = 0.0f;
	   mVelocityY = 0.0f;
	   if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		   mVelocityX = -350.0f;
	   if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		   mVelocityX = 350.0f;
      
      ApplyInertia(deltaTime);

      if (mX < 200.0f)
      {
		   mX = 200.0f;
      }
      else if (mX > 1720.0f)
      {
         mX = 1720.0f;
      }


      // Firing Logic
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
      {
         if (!mSpacebarPreviouslyPressed)
         {
            PlayerBullet* pBullet = Resurrect<PlayerBullet>();
            if (pBullet != 0)
            {
               //GetSoundManager()->Stop(SoundManager::bulletNoise);
               GetSoundManager()->Play(SoundManager::bulletNoise);
               pBullet->mX = mX;
               pBullet->mY = mPolygon.GetAdjustedMinY();
            }
         }
         mSpacebarPreviouslyPressed = true;
      }
      else
      {
         mSpacebarPreviouslyPressed = false;
      }
   
      static_cast<void> (DetectCollisionAndKillBoth<Enemy>(true));
   }
}

void Player::Kill()
{
   GetSoundManager()->Play(SoundManager::explosion);
   SetStatus(StatusManager::dying);
   mAngularVelocity = Util::RandomFloat(-500.0f, 500.0f);
   Particle::GenerateParticles(100, mX, mY);
}

void Player::ProcessDying(float deltaTime)
{
   ApplyInertia(deltaTime);
   mVelocityY += 1500.0f * deltaTime;
   if (mY > 5080.0f)
   {
      SetStatus(StatusManager::dead);
   }

}
