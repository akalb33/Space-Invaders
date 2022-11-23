#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "Enemy.h"
#include "PlayerLives.h"
#include "PlayerBullet.h"
#include "Particle.h"
#include "ScoreManager.h"
#include "Background.h"
#include "Util.h"
#include <assert.h>
#include <SFML/System/Time.hpp>
#include <random>

Game::Game()
   : mScreen(0.0f, 0.0f, 1920.0f, 1080.0f, false, "Space Invaders")
   , mTextureManager()
   , mSoundManager()
   , mMenu()
   , mLevelSpeed(0.0f)
   , mLives(0)
{
   Util::InitRandomizer();
   mScreen.setVerticalSyncEnabled(true);
   mScreen.SetMouseCursorVisible(false);

   Character::SetTextureManager(&mTextureManager);
   Character::SetSoundManager(&mSoundManager);
   Character::SetScreen(&mScreen);
   SetMaxCharacters();

   sf::Font font;
   assert(font.loadFromFile("arial.ttf"));
   sf::Text text;
   text.setFont(font);

   ScoreManager::Load();
   sf::Text score;
   score.setString("Score");
   score.setPosition(75.0f, 40.0f);
   score.setFont(font);
   ScoreManager::AddPlayer(150.0f, 100.0f, text, 50.0f);
   ScoreManager::SelectPlayer(0);
   sf::Text highScore;
   highScore.setString("High Score");
   highScore.setPosition(3.0f, 340.0f);
   highScore.setFont(font);
   ScoreManager::SetHighScorePosition(150.0f, 400.0f, text, 50.0f);

   mMenu.SetMenuTitle("Space Invaders");
   mMenu.SetText(text);
   mMenu.AddSetting("Start New Game");
   mMenu.AddSetting("Toggle Fullscreen");
   mMenu.AddSetting("Exit");
   mMenu.Load();

   bool goToMenu = true;
   bool gameStarted = false;

   sf::Clock clock;
   while (mScreen.isOpen())
   {
      sf::Event event;
      while (mScreen.pollEvent(event) || goToMenu)
      {
         if (event.type == sf::Event::Closed)
            mScreen.close();
         else if (event.type == sf::Event::Resized)
            mScreen.HandleResizeEvent();
         else if (event.type == sf::Event::KeyReleased || goToMenu)
         {
            if (event.key.code == sf::Keyboard::F11)
            {
               mScreen.ToggleFullScreen();
            }
            else if (event.key.code == sf::Keyboard::Escape || goToMenu)
            {
               goToMenu = false;
               std::string selection = mMenu.ShowMenu(mScreen);
               if((selection == "Exit") || (selection == "WINDOW_CLOSED_EVENT"))
               {
                  mScreen.close();
               }
               else if(selection == "Start New Game")
               {
                  StartNewGame();
                  gameStarted = true;
               }
               else if (selection == "Toggle Fullscreen")
               {
                  mScreen.ToggleFullScreen();
                  goToMenu = true;
               }
               else
               {
                  if (!gameStarted)
                  {
                     goToMenu = true;
                  }
               }

               clock.restart();
            }
         }
      }

      mScreen.clear();
      Character::ProcessAll(clock.restart().asSeconds());
      Character::DisplayAll();
      ScoreManager::DrawScores(mScreen);
      mScreen.draw(score);
      mScreen.draw(highScore);


      mScreen.DisplayNewScreen();
      
      Enemy::UpdateSpeedAndDirection();

      if (Character::FindCharacter<Player>(Character::dead) != 0)
      {
         if (mLives > 0)
         {
            mLives--;
            UpdatePlayerLives();
            Player* pPlayer = Character::Resurrect<Player>();
            pPlayer->Reset();
         }
      }

      if ((Character::FindCharacter<Enemy>(Character::alive) == 0) &&
          (Character::FindCharacter<Enemy>(Character::dying) == 0) &&
          (Character::FindCharacter<Player>(Character::alive) != 0))
      {
         mLevelSpeed *= 1.2f;
         StartNewLevel();
      }
   }

   mMenu.Save();
   ScoreManager::Save();
}

Game::~Game()
{

}

void Game::SetMaxCharacters()
{
   Character::SetMaxLimit<Player>(1);
   Character::SetMaxLimit<PlayerBullet>(2);
   Character::SetMaxLimit<Enemy>(50);
   Character::SetMaxLimit<PlayerLives>(2);
   Character::SetMaxLimit<Background>(1);
   Character::SetMaxLimit<Particle>(500);
}

void Game::UpdateMenuOptions()
{
}

void Game::StartNewGame()
{
   ScoreManager::ResetScore();
   Character::KillAll();
   Background* pBackground = Character::Resurrect<Background>();
   assert(pBackground != 0);
   pBackground->mX = 960.0f;
   pBackground->mY = 540.0f;
   mLives = 2;
   UpdatePlayerLives();
	Player* pPlayer = Character::Resurrect<Player>();
   assert(pPlayer != 0);
   pPlayer->Reset();
   mLevelSpeed = 50.0f;
   StartNewLevel();
}
void Game::StartNewLevel()
{
   Enemy::SetSpeed(mLevelSpeed);
   Enemy::SetGravity(mLevelSpeed);
   for (int x = 0; x < 10; x++)
   {
      for (int y = 0; y < 5; y++)
      {
         Enemy* pEnemy = Character::Resurrect<Enemy>();
         assert(pEnemy != 0);
         pEnemy->mX = 200.0f + x * 100.0f;
         pEnemy->mY = 10.0f + Enemy::mIncrementDownwards * y;
         pEnemy->Reset();
      }
   }
}

void Game::UpdatePlayerLives()
{
   PlayerLives* pPlayerLives = 0;
   do
   {
      pPlayerLives = Character::FindCharacter<PlayerLives>(Character::alive);
      if (Character::FindCharacter<PlayerLives>(Character::alive) != 0)
      {
         pPlayerLives->Kill();
      }

   }while(pPlayerLives != 0);

   for (int x = 0; x < mLives; x++)
   {
      pPlayerLives = Character::Resurrect<PlayerLives>();
      assert(pPlayerLives != 0);
      pPlayerLives->mX = 50.0f + x * 50.0f;
      pPlayerLives->mY = 1040.0f;
   }
}