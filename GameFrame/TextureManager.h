#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <string>
#include <vector>
#include "Polygon.h"

class TextureManager
{
public:
   enum Textures
   {
      player,
      playerBullet,
      enemy,
      background,
      particle
   };

   TextureManager()
   {
      Init();

      AddTexture(player, "spaceship.png");
      AddTexture(playerBullet, "laser.png");
      AddTexture(enemy, "enemy.png");
      AddTexture(background, "background.png");
      AddTexture(particle, "Particle.png");
   }

   // This automatically sets default origin and dimensions based on texture
   //    size. Call SetOrigin() and AddPoint() to overwrite these values.
   void AddTexture(Textures textureId, std::string fileName);

   const sf::Texture& GetTexture(Textures textureId) const;

   // This will set the origin of the texture. Rotation, movement and scaling
   //    are relative to the origin. Default dimensions are automatically set
   //    based on the size of the texture. Call AddPoint() to overwrite these.
   void SetOrigin(Textures textureId, float x, float y);

   // The first time this is called, the dimensions will be a point.
   // The second time this is called, the dimensions will be a line.
   // Three or more calls will define a polygon with an automatic line drawn
   //    from the last point added to the first point that was added.
   void AddPoint(Textures textureId, float x, float y);

   const Polygon& GetPolygon(Textures textureId);

private:
   enum State
   {
      noTexture,
      textureWithDefaultDimensions,
      originSetDefaultDimensions,
      pointsAdded
   };

   void Init();
   void SetDefaultDimensions(Textures textureId);

   static const unsigned int MAX_TEXTURES = 100;
   sf::Texture mTexture[MAX_TEXTURES];
   Polygon mPolygon[MAX_TEXTURES];
   State mState[MAX_TEXTURES];
};
