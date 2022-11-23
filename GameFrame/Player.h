#pragma once

#include "Character.h"

class Player : public Character
{
public:
   Player();
   virtual ~Player();
   void Reset();
   virtual void Process(float deltaTime);
   virtual void Kill();
public:

private:
   bool mSpacebarPreviouslyPressed;
   void ProcessDying(float deltaTime);
};
