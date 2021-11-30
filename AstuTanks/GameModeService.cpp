/*
 * Astu Tanks
 * 
 * Copyright (c) 2021 Tobias Filler. All rights reserved.
 */

// Local includes
#include "GameModeService.h"
#include "Constants.h"
#include "GunSystem.h"

#include <AstuSuite2D.h>
#include <AstuECS.h>
#include <AstuMath.h>

using namespace astu2d;
using namespace astu;
using namespace std;

GameModeService::GameModeService()
  : BaseService("Game Mode Service")
{
	  // Intentionally left empty.    
}

void GameModeService::OnStartup()
{
  textRenderer = ASTU_GET_SERVICE(TextRenderService);

  // setting up menu Controlls
  upDown = ASTU_SERVICE(InputMappingService).BindAxis("ThrustPlayer1");
  enterAction = ASTU_SERVICE(InputMappingService).BindAction("FirePlayer1");
  enterAction->SetDelegate([this](ActionBinding &binding){
		if (binding.IsPressed()) {
			enter = true;
		}
	});
	enter = false;

  menuInt = 0;
  wait = true;
  deltaWaitSec = 0;
  waitSec = 0.2f;

  objectivs = 0;
  currentLevel = 0;
  lifesPlayerOne = STARTING_LIFES;
  lifesPlayerTwo = STARTING_LIFES;
  score = 0;

  SwitchLevel(currentLevel);
}

void GameModeService::OnShutdown()
{
  ASTU_SERVICE(InputMappingService).RemoveActionBinding(enterAction);
  ASTU_SERVICE(InputMappingService).RemoveAxisBinding(upDown);
	upDown = nullptr;
	enterAction = nullptr;

  ASTU_SERVICE(EntityService).RemoveAll();
}

void GameModeService::SwitchLevel(int level){
  showMenu = false;

  ASTU_GET_SERVICE(GunSystem)->ResetTrigger1();
  switch (level)
  {
    case 0:
      Menu();
      break;
    case 1:
      currentLevel = level;
      SPLevel1();
      break;
    case 2:
      currentLevel = level;
      SPLevel2();
      break;
    case 30:
      currentLevel = level;
      COOPLevel1();
      break;
    case 31:
      currentLevel = level;
      COOPLevel2();
      break;
    case 50:
      currentLevel = level;
      VSArena1();
      break;
    case 99:
      currentLevel = level;
      DemoLevel();
      break;
    default:
      currentLevel = 0;
      Menu();
      break;
  }
}

shared_ptr<Entity> GameModeService::AddEntity(const string& proto, float x, float y, float phiDeg)
{
  auto entity = ASTU_SERVICE(EntityFactoryService).CreateEntity(proto);
  auto& pose = entity->GetComponent<CPose>();
  pose.transform.SetTranslation(x, y);
  if (phiDeg != 0) {
      pose.transform.SetRotationDeg(phiDeg);
  }

  ASTU_SERVICE(EntityService).AddEntity(entity);
  return entity;
}

void GameModeService::OnUpdate(){

  if (showMenu){
    //render
    textRenderer->SetViewTransform(GetCamera().GetMatrix());
    textRenderer->SetDrawColor(RalColors::TrafficGreyA);
    textRenderer->SetAlingment(TextRenderService::Alignment::Center);
    textRenderer->DrawString(0, -2.5, "ASTU TANKS");
    textRenderer->SetViewTransform(GetCamera().GetMatrix()
    .operator*(Matrix3f(0.5, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0, 0.0, 0.5)));
    textRenderer->DrawString(8, 4, "SINGE PLAYER");
    textRenderer->DrawString(8, 6, "CO-OP");
    textRenderer->DrawString(8, 8, "VS MODE");
    textRenderer->DrawString(8, 10, "END GAME");

    //controll
    if (wait){
    deltaWaitSec -= GetElapsedTimeF();
    if (deltaWaitSec <= 0){
      deltaWaitSec = waitSec;
      wait = false;
    }
  } else {

    if (upDown->GetValue() > 0){
      menuInt++;
      pos->transform.SetTranslation(0, GetMenuPos());
      wait = true;
    } else if (upDown->GetValue() < 0){
      menuInt--;
      pos->transform.SetTranslation(0, GetMenuPos());
      wait = true;
    }
    
    if (enter){
      enter = false;
      switch (menuInt)
      {
      case 0:
        // Single Player
        currentLevel = 1;
        SwitchLevel(1);
        break;
      case 1:
        // COOP Modus
        currentLevel = 30;
        SwitchLevel(30);
        break;
      case 2:
        // VS Modus
        currentLevel = 50;
        SwitchLevel(50);
        break;
      case 3:
        // End Game
        exit(EXIT_SUCCESS);
        break;
      }
    }
    }
  } else {
    if (!isMultiplayer){
      if (lifesPlayerOne == 0){
        currentLevel = 0;
        SwitchLevel(0);
      }
    } else {
      if (lifesPlayerOne == 0 || lifesPlayerTwo == 0){
        currentLevel = 0;
        SwitchLevel(0);
      }
    }
    // show HUD
    textRenderer->SetViewTransform(GetCamera().GetMatrix()*GetScalingMatrix(levelHudScaling));

    textRenderer->SetDrawColor(TEXT_COLOR);
    textRenderer->SetAlingment(TextRenderService::Alignment::Center);
    textRenderer->DrawString(levelHudScoreX, levelHudY, "SCORE: " + to_string(score));
     textRenderer->SetDrawColor(PLAYER1_COLOR);
    textRenderer->SetAlingment(TextRenderService::Alignment::Center);
    textRenderer->DrawString(levelHudPlayer1X, levelHudY, "LIFES: " + to_string(lifesPlayerOne));

    if (isMultiplayer){
      textRenderer->SetDrawColor(PLAYER2_COLOR);
      textRenderer->SetAlingment(TextRenderService::Alignment::Center);
      textRenderer->DrawString(levelHudPlayer2X, levelHudY, "LIFES: " + to_string(lifesPlayerTwo));
    }
    
    // objectivs
    if (objectivs == 0){
      EmitSignal(GameEvent(GameEvent::Type::ScoreUpdate, FINISH_LEVEL_POINTS));
      SwitchLevel(++currentLevel);
    }
  }
  }

float GameModeService::GetMenuPos(){
  switch (menuInt)
  {
  case 0:
    return -0.25f;
    break;
  case 1:
    return 0.75f;
  case 2:
    return 1.75f;
  case 3:
    return 2.75f;
  default:
    menuInt = 0;
    return -0.25f;
    break;
  }
  return -0.25f;
}

Matrix3f GameModeService::GetScalingMatrix(float s){
  return Matrix3f(s, 0.0, 0.0, 0.0, s, 0.0, 0.0, 0.0, s);
}

void GameModeService::RespawnPlayer(int player){
  switch (player)
  {
  case 1:
    AddEntity("Player1", respawnPointPlayerOne.x, respawnPointPlayerOne.y, respawnRotationPlayerOne);
    break;
  
  case 2:
    AddEntity("Player2", respawnPointPlayerTwo.x, respawnPointPlayerTwo.y, respawnRotationPlayerTwo);
    break;
  }
}

bool GameModeService::OnSignal(const GameEvent & signal){
  switch (signal.type)
  {
  case GameEvent::Type::ScoreUpdate:
    score += signal.value;
    break;

  case GameEvent::Type::LifeUpdate:
    RespawnPlayer(signal.player);
    if (signal.player == 1){
      lifesPlayerOne += signal.value;
    } else if (signal.player == 2){
      lifesPlayerTwo += signal.value;
    }
    break;

  case GameEvent::Type::DestroyObjective:
    objectivs--;
    break;
  }
  return false;
}

// levelId = 0
void GameModeService::Menu(){

  ASTU_SERVICE(EntityService).RemoveAll();

  GetCamera().ShowFitting(VIEW_WIDTH, VIEW_HEIGHT);
  showMenu = true;
  lifesPlayerOne = STARTING_LIFES;
  lifesPlayerTwo = STARTING_LIFES;
  score = 0;
  enter = false;

  auto menuHighlight = make_shared<Entity>();
  pos = make_shared<CPose>();

  menuHighlight->AddComponent(pos);

  menuHighlight->AddComponent(make_shared<CScene>(PolylineBuilder()
    .Color(RalColors::TrafficGreyA)
    .VertexBuffer(ShapeGenerator().GenRectangle(9,1))
    .Build()));

  pos->transform.SetTranslation(0, -0.25f);

  ASTU_SERVICE(EntityService).AddEntity(menuHighlight);
}

// levelId = 50
void GameModeService::VSArena1(){

  ASTU_SERVICE(EntityService).RemoveAll();

  // Configure level varibles
  isMultiplayer = true;
  objectivs = 1;

  levelHudScaling = 0.2f;
  levelHudY = -3.2f;
  levelHudScoreX = 32;
  levelHudPlayer1X = 0.0f;
  levelHudPlayer2X = 64;
  

  respawnPointPlayerOne = Vector2f(-VIEW_WIDTH/3,VIEW_HEIGHT/3);
  respawnRotationPlayerOne = 180;

  respawnPointPlayerTwo = Vector2f(VIEW_WIDTH/3,-VIEW_HEIGHT/3);
  respawnRotationPlayerTwo = 0;

  // Configure camera.
  GetCamera().ShowFitting(VIEW_WIDTH, VIEW_HEIGHT);

  // Add outer Walls.
  AddEntity("LargeWall", 0, -VIEW_HEIGHT/2 + WALL_THICKNESS/2);
  AddEntity("LargeWall", 0, VIEW_HEIGHT/2 - WALL_THICKNESS/2);
  AddEntity("MediumLargeWall", -VIEW_WIDTH/2 + WALL_THICKNESS/2, 0,90);
  AddEntity("MediumLargeWall", VIEW_WIDTH/2 - WALL_THICKNESS/2, 0,90);

  // Add level walls
   AddEntity("MediumLargeWall", 0, 0,0);
   AddEntity("SmallWall", -VIEW_HEIGHT/4, WALL_THICKNESS/2 + WALL_SMALL/2,90);
   AddEntity("SmallWall", VIEW_HEIGHT/4, -(WALL_THICKNESS/2 +WALL_SMALL/2),90);

  // Add initial entities.
  AddEntity("Player1", -VIEW_WIDTH/3,VIEW_HEIGHT/3, 180);
  AddEntity("Player2", VIEW_WIDTH/3,-VIEW_HEIGHT/3, 0);
}

// levelId = 99
void GameModeService::DemoLevel(){

  ASTU_SERVICE(EntityService).RemoveAll();

  // Configure level varibles
  isMultiplayer = false;
  objectivs = 1;

  levelHudScaling = 0.2f;
  levelHudY = -3.2f;
  levelHudScoreX = 32;
  levelHudPlayer1X = 0.0f;
  
  respawnPointPlayerOne = Vector2f(-VIEW_WIDTH * (3.0f / 8.0f), 0);
  respawnRotationPlayerOne = 180;

  // Configure camera.
  GetCamera().ShowFitting(VIEW_WIDTH, VIEW_HEIGHT);

  // Add outer Walls.
  AddEntity("LargeWall", 0, -VIEW_HEIGHT/2 + WALL_THICKNESS/2);
  AddEntity("LargeWall", 0, VIEW_HEIGHT/2 - WALL_THICKNESS/2);
  AddEntity("MediumLargeWall", -VIEW_WIDTH/2 + WALL_THICKNESS/2, 0,90);
  AddEntity("MediumLargeWall", VIEW_WIDTH/2 - WALL_THICKNESS/2, 0,90);

  // Add level walls


  // Add initial entities.
  AddEntity("Player1", respawnPointPlayerOne.x,respawnPointPlayerOne.y,respawnRotationPlayerOne);

}

// levelId = 1
void GameModeService::SPLevel1(){

  ASTU_SERVICE(EntityService).RemoveAll();

  // Configure level varibles
  isMultiplayer = false;
  objectivs = 1;

  levelHudScaling = 0.2f;
  levelHudY = -3.2f;
  levelHudScoreX = 32;
  levelHudPlayer1X = 0.0f;
  

  respawnPointPlayerOne = Vector2f(-VIEW_WIDTH/2 + 3*WALL_THICKNESS, 0);
  respawnRotationPlayerOne = 270;

  // Configure camera.
  GetCamera().ShowFitting(VIEW_WIDTH, VIEW_HEIGHT);

  // Add outer Walls.
  AddEntity("LargeWall", 0, -VIEW_HEIGHT/2 + WALL_THICKNESS/2);
  AddEntity("LargeWall", 0, VIEW_HEIGHT/2 - WALL_THICKNESS/2);
  AddEntity("MediumLargeWall", -VIEW_WIDTH/2 + WALL_THICKNESS/2, 0,90);
  AddEntity("MediumLargeWall", VIEW_WIDTH/2 - WALL_THICKNESS/2, 0,90);

  // Add level walls
  AddEntity("SmallWall", -6.5 * WALL_THICKNESS, -3 * WALL_THICKNESS, 90);
  AddEntity("SmallWall", -6.5 * WALL_THICKNESS, 3 * WALL_THICKNESS, 90);
  AddEntity("SmallWall", 2.5 * WALL_THICKNESS, -4 * WALL_THICKNESS, 90);
  AddEntity("SmallWall", 2.5 * WALL_THICKNESS, 4 * WALL_THICKNESS, 90);

  // Add initial entities.
  AddEntity("Box", 2.5 * WALL_THICKNESS, -WALL_THICKNESS / 2);
  AddEntity("Box", 2.5 * WALL_THICKNESS, -1.5 * WALL_THICKNESS);
  AddEntity("Box", 2.5 * WALL_THICKNESS, WALL_THICKNESS / 2);
  AddEntity("Box", 2.5 * WALL_THICKNESS, 1.5 * WALL_THICKNESS);

  AddEntity("Turret", 9.5 * WALL_THICKNESS, 0);

  AddEntity("Player1", respawnPointPlayerOne.x, respawnPointPlayerOne.y, respawnRotationPlayerOne);
}

// levelId = 2
void GameModeService::SPLevel2(){

  ASTU_SERVICE(EntityService).RemoveAll();

  // Configure level varibles
  isMultiplayer = false;
  objectivs = 1;

  levelHudScaling = 0.2f;
  levelHudY = -3.2f;
  levelHudScoreX = 32;
  levelHudPlayer1X = 0.0f;
  

  respawnPointPlayerOne = Vector2f(-VIEW_WIDTH * (3.0f / 8.0f), 0);
  respawnRotationPlayerOne = 180;

  // Configure camera.
  GetCamera().ShowFitting(VIEW_WIDTH, VIEW_HEIGHT);

  // Add outer Walls.
  AddEntity("LargeWall", 0, -VIEW_HEIGHT/2 + WALL_THICKNESS/2);
  AddEntity("LargeWall", 0, VIEW_HEIGHT/2 - WALL_THICKNESS/2);
  AddEntity("MediumLargeWall", -VIEW_WIDTH/2 + WALL_THICKNESS/2, 0,90);
  AddEntity("MediumLargeWall", VIEW_WIDTH/2 - WALL_THICKNESS/2, 0,90);

  // Add level walls
  AddEntity("MediumWall", -VIEW_WIDTH / 4, 0, 90);
  AddEntity("MediumWall", VIEW_WIDTH / 4, 0, 90);

  // Add initial entities.
  AddEntity("Player1", respawnPointPlayerOne.x,respawnPointPlayerOne.y,respawnRotationPlayerOne);

  AddEntity("Turret", 0, 0, 0);

  for (int i = 0; i < 4;++i){
    AddEntity("Box", -VIEW_WIDTH / 4, WALL_MEDIUM/2+BOX_SIZE* (i + 0.5f));
  }

  for (int i = 0; i < 4;++i){
    AddEntity("Box", VIEW_WIDTH / 4, WALL_MEDIUM/2+BOX_SIZE*(i + 0.5f));
  }

  for (int i = 0; i < 4;++i){
   AddEntity("Box", -VIEW_WIDTH / 4, -(WALL_MEDIUM/2+BOX_SIZE*(i + 0.5f)));
  }

  for (int i = 0; i < 4;++i){
    AddEntity("Box", VIEW_WIDTH / 4, -(WALL_MEDIUM/2+BOX_SIZE*(i + 0.5f)));
  }
}

// levelId = 3
void GameModeService::SPLevel3(){
  ASTU_SERVICE(EntityService).RemoveAll();

  // Configure level varibles
  isMultiplayer = false;
  objectivs = 1;

  levelHudScaling = 0.2f;
  levelHudY = -3.2f;
  levelHudScoreX = 32;
  levelHudPlayer1X = 0.0f;
  

  respawnPointPlayerOne = Vector2f(-VIEW_WIDTH/2 + 3*WALL_THICKNESS, VIEW_HEIGHT/2 - 3*WALL_THICKNESS);
  respawnRotationPlayerOne = 270;

  // Configure camera.
  GetCamera().ShowFitting(VIEW_WIDTH, VIEW_HEIGHT);

  // Add outer Walls.
  AddEntity("LargeWall", 0, -VIEW_HEIGHT/2 + WALL_THICKNESS/2);
  AddEntity("LargeWall", 0, VIEW_HEIGHT/2 - WALL_THICKNESS/2);
  AddEntity("MediumLargeWall", -VIEW_WIDTH/2 + WALL_THICKNESS/2, 0,90);
  AddEntity("MediumLargeWall", VIEW_WIDTH/2 - WALL_THICKNESS/2, 0,90);

  // Add level walls
  AddEntity("MediumWall", 0, -2.5 * WALL_THICKNESS);
  AddEntity("SmallWall", -6 * WALL_THICKNESS, -2.5 * WALL_THICKNESS);

  AddEntity("Box",  4.5 * WALL_THICKNESS, -2.5 * WALL_THICKNESS);
  AddEntity("Box",  5.5 * WALL_THICKNESS, -2.5 * WALL_THICKNESS);
  AddEntity("Box",  6.5 * WALL_THICKNESS, -2.5 * WALL_THICKNESS);
  AddEntity("Box",  7.5 * WALL_THICKNESS, -2.5 * WALL_THICKNESS);

  AddEntity("MediumWall", 0, 2.5 * WALL_THICKNESS);
  AddEntity("SmallWall", 6 * WALL_THICKNESS, 2.5 * WALL_THICKNESS);

  AddEntity("Box",  -4.5 * WALL_THICKNESS, 2.5 * WALL_THICKNESS);
  AddEntity("Box",  -5.5 * WALL_THICKNESS, 2.5 * WALL_THICKNESS);
  AddEntity("Box",  -6.5 * WALL_THICKNESS, 2.5 * WALL_THICKNESS);
  AddEntity("Box",  -7.5 * WALL_THICKNESS, 2.5 * WALL_THICKNESS);

  AddEntity("CornerWall", 3.5 * WALL_THICKNESS, -5.5 * WALL_THICKNESS);
  AddEntity("CornerWall", 3.5 * WALL_THICKNESS, -4.5 * WALL_THICKNESS);
  AddEntity("CornerWall", 7.5 * WALL_THICKNESS, 7.5* WALL_THICKNESS);
  AddEntity("CornerWall", 7.5 * WALL_THICKNESS, 6.5* WALL_THICKNESS);
  AddEntity("MediumWall", VIEW_WIDTH/2 - 5 * WALL_THICKNESS, VIEW_HEIGHT/2 - 3.5 * WALL_THICKNESS);

  // Add initial entities.
  AddEntity("Turret", VIEW_WIDTH / 2 - 5 * WALL_THICKNESS, -VIEW_HEIGHT/2 + (4 * WALL_THICKNESS));

  AddEntity("Player1", respawnPointPlayerOne.x, respawnPointPlayerOne.y, respawnRotationPlayerOne);
}

// levelId = 30
void GameModeService::COOPLevel1(){
  ASTU_SERVICE(EntityService).RemoveAll();

  // Configure level varibles
  isMultiplayer = true;
  objectivs = 4;

  levelHudScaling = 0.2f;
  levelHudY = -3.2f;
  levelHudScoreX = 32;
  levelHudPlayer1X = 0.0f;
  levelHudPlayer2X = 64;
  
  respawnPointPlayerOne = Vector2f(-VIEW_WIDTH/2 + WALL_THICKNESS * 4, 6 * WALL_THICKNESS);
  respawnRotationPlayerOne = 90;

  respawnPointPlayerTwo = Vector2f(+VIEW_WIDTH/2 - WALL_THICKNESS * 4, -6 * WALL_THICKNESS);
  respawnRotationPlayerTwo = 270;

  // Configure camera.
  GetCamera().ShowFitting(VIEW_WIDTH, VIEW_HEIGHT);

  // Add outer Walls.
  AddEntity("LargeWall", 0, -VIEW_HEIGHT/2 + WALL_THICKNESS/2);
  AddEntity("LargeWall", 0, VIEW_HEIGHT/2 - WALL_THICKNESS/2);
  AddEntity("MediumLargeWall", -VIEW_WIDTH/2 + WALL_THICKNESS/2, 0,90);
  AddEntity("MediumLargeWall", VIEW_WIDTH/2 - WALL_THICKNESS/2, 0,90);

  // Add level walls
  AddEntity("SmallWall", -VIEW_WIDTH / 2 + 5 * WALL_THICKNESS, 3.5 * WALL_THICKNESS);
  AddEntity("SmallWall", -VIEW_WIDTH / 2 + 6.5 * WALL_THICKNESS, 6 * WALL_THICKNESS,90);
  AddEntity("SmallWall", -3.5 * WALL_THICKNESS, -6 * WALL_THICKNESS,90);

  AddEntity("Box", -3.5 * WALL_THICKNESS, -0.5 * WALL_THICKNESS);
  AddEntity("Box", -3.5 * WALL_THICKNESS, -1.5 * WALL_THICKNESS);
  AddEntity("Box", -3.5 * WALL_THICKNESS, -2.5 * WALL_THICKNESS);
  AddEntity("Box", -3.5 * WALL_THICKNESS, -3.5 * WALL_THICKNESS);

  AddEntity("SmallWall", VIEW_WIDTH / 2 - 5 * WALL_THICKNESS, -3.5 * WALL_THICKNESS);
  AddEntity("SmallWall", VIEW_WIDTH / 2 - 6.5 * WALL_THICKNESS, -6 * WALL_THICKNESS,90);
  AddEntity("SmallWall", 3.5 * WALL_THICKNESS, 6 * WALL_THICKNESS,90);

  AddEntity("Box", 3.5 * WALL_THICKNESS, 0.5 * WALL_THICKNESS);
  AddEntity("Box", 3.5 * WALL_THICKNESS, 1.5 * WALL_THICKNESS);
  AddEntity("Box", 3.5 * WALL_THICKNESS, 2.5 * WALL_THICKNESS);
  AddEntity("Box", 3.5 * WALL_THICKNESS, 3.5 * WALL_THICKNESS);

  // Add initial entities.
  AddEntity("Player1", respawnPointPlayerOne.x,respawnPointPlayerOne.y,respawnRotationPlayerOne);
  AddEntity("Player2", respawnPointPlayerTwo.x,respawnPointPlayerTwo.y,respawnRotationPlayerTwo);

  AddEntity("Turret", -VIEW_WIDTH / 2 + 5.5 * WALL_THICKNESS, -1.5 * WALL_THICKNESS);
  AddEntity("Turret", -3 * WALL_THICKNESS, 4 * WALL_THICKNESS);
  AddEntity("Turret", VIEW_WIDTH / 2 - 5.5 * WALL_THICKNESS, 1.5 * WALL_THICKNESS);
  AddEntity("Turret", 3 * WALL_THICKNESS, -4 * WALL_THICKNESS);
}

// levelId = 31
void GameModeService::COOPLevel2(){
  ASTU_SERVICE(EntityService).RemoveAll();

  // Configure level varibles
  isMultiplayer = true;
  objectivs = 5;

  levelHudScaling = 0.2f;
  levelHudY = -3.2f;
  levelHudScoreX = 32;
  levelHudPlayer1X = 0.0f;
  levelHudPlayer2X = 64;
  
  respawnPointPlayerOne = Vector2f(-VIEW_WIDTH/2 + WALL_THICKNESS * 3.5, 5.5 * WALL_THICKNESS);
  respawnRotationPlayerOne = 270;

  respawnPointPlayerTwo = Vector2f(-VIEW_WIDTH/2 + WALL_THICKNESS * 3.5, 2.5 * WALL_THICKNESS);
  respawnRotationPlayerTwo = 270;

  // Configure camera.
  GetCamera().ShowFitting(VIEW_WIDTH, VIEW_HEIGHT);

  // Add outer Walls.
  AddEntity("LargeWall", 0, -VIEW_HEIGHT/2 + WALL_THICKNESS/2);
  AddEntity("LargeWall", 0, VIEW_HEIGHT/2 - WALL_THICKNESS/2);
  AddEntity("MediumLargeWall", -VIEW_WIDTH/2 + WALL_THICKNESS/2, 0,90);
  AddEntity("MediumLargeWall", VIEW_WIDTH/2 - WALL_THICKNESS/2, 0,90);

  // Add level walls
  AddEntity("SmallWall", -VIEW_WIDTH / 2 + 3 * WALL_THICKNESS, 0.5 * WALL_THICKNESS);
  AddEntity("SmallWall",-6.5 * WALL_THICKNESS, -4 * WALL_THICKNESS,90);
  AddEntity("SmallWall",2.5 * WALL_THICKNESS, -1 * WALL_THICKNESS,90);
  AddEntity("SmallWall",7.5 * WALL_THICKNESS, -6 * WALL_THICKNESS,90);
  AddEntity("SmallWall",4.5 * WALL_THICKNESS, 6 * WALL_THICKNESS,90);
  AddEntity("MediumWall", -6.5 * WALL_THICKNESS, 4 * WALL_THICKNESS, 90);
  AddEntity("MediumWall", -2 * WALL_THICKNESS, 1.5 * WALL_THICKNESS);

  AddEntity("SmallWall", 7 * WALL_THICKNESS, 1.5 * WALL_THICKNESS);
  AddEntity("SmallWall", 7.5 * WALL_THICKNESS, 4 * WALL_THICKNESS, 90);
  AddEntity("CornerWall", 5.5 * WALL_THICKNESS, 0.5 * WALL_THICKNESS);

  AddEntity("Box", 3.5 * WALL_THICKNESS, -0.5 * WALL_THICKNESS);
  AddEntity("Box", 4.5 * WALL_THICKNESS, -0.5 * WALL_THICKNESS);
  AddEntity("Box", 5.5 * WALL_THICKNESS, -0.5 * WALL_THICKNESS);
  AddEntity("Box", 3.5 * WALL_THICKNESS, 0.5 * WALL_THICKNESS);
  AddEntity("Box", 4.5 * WALL_THICKNESS, 0.5 * WALL_THICKNESS);
  AddEntity("Box", 2.5 * WALL_THICKNESS, 1.5 * WALL_THICKNESS);
  AddEntity("Box", 3.5 * WALL_THICKNESS, 1.5 * WALL_THICKNESS);
  AddEntity("Box", 4.5 * WALL_THICKNESS, 1.5 * WALL_THICKNESS);

  AddEntity("Box", 5.5 * WALL_THICKNESS, -4.5 * WALL_THICKNESS);
  AddEntity("Box", 6.5 * WALL_THICKNESS, -4.5 * WALL_THICKNESS);

  AddEntity("Box", -6.5 * WALL_THICKNESS, -0.5 * WALL_THICKNESS);
  AddEntity("Box", -6.5 * WALL_THICKNESS, -1.5 * WALL_THICKNESS);
  

  // Add initial entities.
  AddEntity("Player1", respawnPointPlayerOne.x,respawnPointPlayerOne.y,respawnRotationPlayerOne);
  AddEntity("Player2", respawnPointPlayerTwo.x,respawnPointPlayerTwo.y,respawnRotationPlayerTwo);

  AddEntity("Turret", -11 * WALL_THICKNESS, -4 * WALL_THICKNESS);
  AddEntity("TurretSmallRange", -1.5 * WALL_THICKNESS, -2.5 * WALL_THICKNESS);
  AddEntity("Turret", 1.5 * WALL_THICKNESS, -6.5 * WALL_THICKNESS);
  AddEntity("Turret", 11.5 * WALL_THICKNESS, -2.5 * WALL_THICKNESS);
  AddEntity("TurretSmallRange", -0.5 * WALL_THICKNESS, 5.5 * WALL_THICKNESS);
}

