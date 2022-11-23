#pragma once

#include "Character.h"

class Enemy : public Character
{
public:
   Enemy();
   virtual ~Enemy();
   void Reset();
   virtual void Process(float deltaTime);
   virtual void Kill();
   static void SetSpeed(float speed);
   static void SetGravity(float gravity);
   static void UpdateSpeedAndDirection();
   
public:
   static const float mIncrementDownwards;

private:
   enum direction
   {
      left,
      right,
      downL,
      downR
   };
   static direction mDirection;
   float mDestinationY;
   void ProcessDying(float deltaTime);
   static float mSpeed;
   static float mGravity;
   static direction mNewDirection;
   static float mNewSpeed;
};
