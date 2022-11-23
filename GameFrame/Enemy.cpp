#include "stdafx.h"
#include "Util.h"
#include "Enemy.h"
#include "Character.h"

Enemy::direction Enemy::mDirection = Enemy::right;
const float Enemy::mIncrementDownwards = 100.0f;
float Enemy::mSpeed = 75.0f; 
float Enemy::mGravity = 75.0f;
Enemy::direction Enemy::mNewDirection = Enemy::right;
float Enemy::mNewSpeed = 75.0f;

Enemy::Enemy()
   : Character(typeid(Enemy))
   , mDestinationY(0.0f)
{
   SetTexture(TextureManager::enemy);
   SetDisplayOrder(180);
   mScale = 0.1f;
}

Enemy::~Enemy()
{
}



void Enemy::Reset()
{
   mDirection = right;
   mNewDirection = right;
   mAngularVelocity = 0.0f;
   mAngle = 0.0f;
}

void Enemy::Process(float deltaTime)
{
   if (GetStatus() == dying)
   {
      ProcessDying(deltaTime);
   }
   else
   {
      if (mDirection == left)
      {
         mVelocityX = mSpeed * -1.0f;
         mVelocityY = 0.0f;
         mDestinationY = 0.0f;
      }
      else if (mDirection == right)
      {
         mVelocityX = mSpeed;
         mVelocityY = 0.0f;
         mDestinationY = 0.0f;
      }
      else 
      {
         mVelocityX = 0.0f;
         mVelocityY = mSpeed * 4.0f;
      
         if (mDestinationY == 0.0f)
           mDestinationY = mY + mIncrementDownwards;
      }
      ApplyInertia(deltaTime);

      if ((mDirection == right) && (mX > 1720.0f))
      {
         mDestinationY = mY + mIncrementDownwards;
         mNewDirection = downL;
      }
      else if (mY > 1070.0f)
      {
         SetStatus(StatusManager::dead);
      }
      else if ((mDirection == left) && (mX < 200.0f))
      {
         mDestinationY = mY + mIncrementDownwards;
         mNewDirection = downR;
      }
      else if ((mDestinationY != 0.0f) && (mY > mDestinationY))
      {
         mY = mDestinationY;

         if(mDirection == downL)
            mNewDirection = left;
         else
            mNewDirection = right;
      }
   }
}

void Enemy::Kill()
{
   SetStatus(StatusManager::dying);
   mVelocityY = -500.0f;
   mVelocityX = Util::RandomFloat(-200.0f, 200.0f);
   mAngularVelocity = Util::RandomFloat(-1500.0f, 1500.0f);
   unsigned int alive = mStatusManager.GetAliveCount(typeid(Enemy));
   if (alive == 12)
   {
      mNewSpeed *= 2.0f;
   }
   else if (alive == 4)
   {
      mNewSpeed *= 2.0f;
   }
   else if (alive == 1)
   {
      mNewSpeed *= 2.0f;
   }
}

void Enemy::ProcessDying(float deltaTime)
{
   ApplyInertia(deltaTime);
   mVelocityY += 20.0f * mGravity * deltaTime;
   if (mY > 1080.0f)
   {
      SetStatus(StatusManager::dead);
   }

}

void Enemy::SetSpeed(float speed)
{
   mSpeed = speed;
   mNewSpeed = speed;
}

void Enemy::SetGravity(float gravity)
{
   mGravity = gravity;
}

void Enemy::UpdateSpeedAndDirection()
{
   mDirection = mNewDirection;
   mSpeed = mNewSpeed;
}