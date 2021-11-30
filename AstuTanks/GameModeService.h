/*
 * ASTU Tanks
 * 
 * Copyright (c) 2021 Tobias Filler. All rights reserved.
 */

#pragma once

#include "TextRenderService.h"
#include "GameEvent.h"

// AST-Utilities includes
#include <AstuServices.h>
#include <AstuSuite2D.h>
#include <AstuECS.h>
#include <AstuInput.h>
#include <AstuMath.h>

class GameModeService 
  : public astu::BaseService
  , private astu2d::CameraClient
  , private astu::Updatable
  , private astu::TimeClient
  , private astu::SignalListener<GameEvent>
  , private astu::SignalEmitter<GameEvent>
{
public:
		
	/**
	 * Constructor.
	 */
  GameModeService();

  /**
   * Switches To the desiered level
   * 
   * @param level     the int of the desiered level
   */
  void SwitchLevel(int level);

  int currentLevel;

    bool showMenu;

private:

  // general data
  int score;
  int lifesPlayerOne;
  int lifesPlayerTwo;

  // level data
  bool isMultiplayer;
  int objectivs;

  float levelHudScaling;
  float levelHudY;
  float levelHudScoreX;
  float levelHudPlayer1X;
  float levelHudPlayer2X;

  astu::Vector2f respawnPointPlayerOne;
  astu::Vector2f respawnPointPlayerTwo;
  float respawnRotationPlayerOne;
  float respawnRotationPlayerTwo;

  // Menu variablen
  std::shared_ptr<astu2d::CPose> pos;
  int menuInt;
  std::shared_ptr<astu::AxisBinding> upDown;
  std::shared_ptr<astu::ActionBinding> enterAction;
  bool enter;

  // waite funtions variablen
  bool wait;
  float deltaWaitSec;
  float waitSec;

  /** Used to render HUD elements. */
	std::shared_ptr<TextRenderService> textRenderer;

  /**
   * Adds an entity to the game world.
   * 
   * @param proto     the name of the entity prototype
   * @param x         the x-coordinate of the spawn point
   * @param y         the y-coordinate of the spawn point
   * @param phiDeg    the orientation angle in degrees
   * @return the newly added entity
   */
  std::shared_ptr<astu::Entity> AddEntity(
      const std::string& proto, 
      float x, 
      float y, 
      float phiDeg = 0); 

  // Inherited via BaseService
  virtual void OnStartup() override;
  virtual void OnShutdown() override;

  // Inherited via Updatable
  virtual void OnUpdate() override;

  // Inherited via private astu::SignalListener<GameEvent>
	virtual bool OnSignal(const GameEvent & signal) override;

  void Menu();
  void VSArena1();
  void DemoLevel();

  void SPLevel1();
  void SPLevel2();
  void SPLevel3();

  void COOPLevel1();
  void COOPLevel2();

  void RespawnPlayer(int player);

  float GetMenuPos();
  astu::Matrix3f GetScalingMatrix(float s);
};