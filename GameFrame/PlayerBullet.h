#pragma once

#include "Character.h"

class PlayerBullet : public Character
{
public:
   PlayerBullet();
   virtual ~PlayerBullet();
   
   virtual void Process(float deltaTime);

public:

private:
 
};
