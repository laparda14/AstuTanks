/*
 * AstuTanks
 * a small and simple implementation of the Classic Tanks Game
 * 
 * Copyright (c) 2021 Tobias Filler. All rights reserved.
 */

// Local includes
#include "EntityPrototypeService.h"
#include "Constants.h"
#include "CTank.h"
#include "CGun.h"
#include "CBullet.h"
#include "CDestroyable.h"
#include "CTurret.h"
#include "CTarget.h"
#include "CObjective.h"

// AST-Utilities includes
#include <AstuSuite2D.h>
#include <AstuECS.h>

using namespace astu2d;
using namespace astu;
using namespace std;

EntityPrototypeService::EntityPrototypeService(bool debug)
  : BaseService("Entity Prototype Service")
  , debug(debug)
{
	// Intentionally left empty. 
}

void EntityPrototypeService::OnStartup()
{
	// Create and register entity prototypes.
	auto &entityFactory = ASTU_SERVICE(EntityFactoryService);

  //register entity prototypes.
  entityFactory.RegisterPrototype("Player1", CreateTank(1,PLAYER1_COLOR));
  entityFactory.RegisterPrototype("Player2", CreateTank(2,PLAYER2_COLOR));
  entityFactory.RegisterPrototype("LargeWall", CreateWall(WALL_THICKNESS, WALL_LARGE));
  entityFactory.RegisterPrototype("MediumLargeWall", CreateWall(WALL_THICKNESS, WALL_MEDIUM_LARGE));
  entityFactory.RegisterPrototype("MediumWall", CreateWall(WALL_THICKNESS, WALL_MEDIUM));
  entityFactory.RegisterPrototype("SmallWall", CreateWall(WALL_THICKNESS, WALL_SMALL));
  entityFactory.RegisterPrototype("CornerWall", CreateWall(WALL_THICKNESS, WALL_CORNER));
  entityFactory.RegisterPrototype("Box", CreateBox());
  entityFactory.RegisterPrototype("Turret", CreateTurret(TURRET_RANGE));
  entityFactory.RegisterPrototype("TurretSmallRange", CreateTurret(TURRET_SMALL_RANGE));
  
}

void EntityPrototypeService::OnShutdown()
{
  // Cleanup.
  ASTU_SERVICE(EntityFactoryService).DeregisterAllPrototypes();
}

shared_ptr<Entity> EntityPrototypeService::CreateTank(int player,int color)
{
  auto entity = make_shared<Entity>();
  
    entity->AddComponent(make_shared<CPose>());

    entity->AddComponent(make_shared<CScene>(NodeBuilder()
        .AttachChild(PolylineBuilder()
            .Color(color)
            .Name("Core")
            .Translation(0, 0)
            .VertexBuffer(ShapeGenerator().GenRectangle(0.4f,0.6f))
            .Build())
        .AttachChild(PolylineBuilder()
            .Color(color)
            .Name("chain_left")
            .Translation(-0.2f, 0)
            .VertexBuffer(ShapeGenerator().GenRectangle(0.1f, 0.7f))
            .Build())
        .AttachChild(PolylineBuilder()
            .Color(color)
            .Name("chain_left")
            .Translation(0.2f, 0)
            .VertexBuffer(ShapeGenerator().GenRectangle(0.1f, 0.7f))
            .Build())
        .AttachChild(PolylineBuilder()
            .Color(color)
            .Name("canon")
            .Translation(0, 0.25f)
            .VertexBuffer(ShapeGenerator().GenRectangle(0.1f, 0.5f))
            .Build())
        .Build()));

      entity->AddComponent(CBodyBuilder()
      .Type(CBody::Type::Dynamic)
      .LinearDamping(TANK_LINEAR_DAMPING)
		  .AngularDamping(TANK_ANGULAR_DAMPING)
      .Build());

      entity->AddComponent(CPolygonColliderBuilder()
        .Polygon(ShapeGenerator().GenRectangle(0.5f, 0.7f))
        .Build());

      entity->AddComponent(make_shared<CTank>(TANK_MAX_THRUST, TANK_MAX_TORQUE, player));

      entity->AddComponent(make_shared<CGun>(
        GUN_COOLDOWN, 
        1, 
        GUN_MUZZLE_VELOCITY, 
		    GUN_MOUNT_POINT, 
		    GUN_DIRECTION));

      entity->AddComponent(make_shared<CDestroyable>(TANK_POINTS));

      entity->AddComponent(make_shared<CTarget>());
      //add aditional Components etc. CArmor

      return entity;
}

shared_ptr<Entity> EntityPrototypeService::CreateWall(float h, float l)
{
  auto entity = make_shared<Entity>();
  entity->AddComponent(make_shared<CPose>());

  
  entity->AddComponent(make_shared<CScene>(PolylineBuilder()
      .Color(WALL_COLOR)
      .VertexBuffer(ShapeGenerator().GenRectangle(l, h))
      .Build()));
  

  entity->AddComponent(CBodyBuilder()
      .Type(CBody::Type::Static)
      .Build());

  entity->AddComponent(CPolygonColliderBuilder()
      .Polygon(ShapeGenerator().GenRectangle(l, h))
      .Friction(GENERAL_FRICTION)
      .Restitution(GENERAL_RESTITUTION)
      .Build());

  return entity;
}

shared_ptr<Entity> EntityPrototypeService::CreateBox(){
  auto entity = make_shared<Entity>();
  entity->AddComponent(make_shared<CPose>());

  
  entity->AddComponent(make_shared<CScene>(NodeBuilder()
     .AttachChild(PolylineBuilder()
        .Color(BOX_COLOR)
        .VertexBuffer(ShapeGenerator().GenRectangle(BOX_SIZE,BOX_SIZE))
        .Build())
      .AttachChild(PolylineBuilder()
        .Color(BOX_COLOR)
        .VertexBuffer(ShapeGenerator().GenRectangle(BOX_SIZE*0.8f,BOX_SIZE*0.8f))
        .Build())
      .AttachChild(PolylineBuilder()
        .Color(BOX_COLOR)
        .VertexBuffer(ShapeGenerator().GenLine(-BOX_SIZE*0.8f/2.0f,-BOX_SIZE*0.8f/2.0f,BOX_SIZE*0.8f/2.0f,BOX_SIZE*0.8f/2.0f))
        .Build())
      .AttachChild(PolylineBuilder()
        .Color(BOX_COLOR)
        .VertexBuffer(ShapeGenerator().GenLine(-BOX_SIZE*0.8f/2.0f,BOX_SIZE*0.8f/2.0f,BOX_SIZE*0.8f/2.0f,-BOX_SIZE*0.8f/2.0f))
        .Build())
      .Build()));
  

  entity->AddComponent(CBodyBuilder()
      .Type(CBody::Type::Static)
      .Build());

  entity->AddComponent(CPolygonColliderBuilder()
      .Polygon(ShapeGenerator().GenRectangle(BOX_SIZE, BOX_SIZE))
      .Friction(GENERAL_FRICTION)
      .Restitution(GENERAL_RESTITUTION)
      .Build());

  entity->AddComponent(make_shared<CDestroyable>(BOX_POINTS));

  return entity;
}

shared_ptr<Entity> EntityPrototypeService::CreateTurret(float range){
  auto entity = make_shared<Entity>();

  entity->AddComponent(make_shared<CPose>());

  entity->AddComponent(make_shared <CDestroyable>(TURRET_POINTS));

  entity->AddComponent(make_shared<CScene>(NodeBuilder()
     .AttachChild(PolylineBuilder()
        .Color(TURRET_COLOR)
        .Name("base")
        .VertexBuffer(ShapeGenerator().GenCircle(0.3f))
        .Build())
      .AttachChild(PolylineBuilder()
        .Color(TURRET_COLOR)
        .Name("canon")
        .Translation(0, 0.25f)
        .VertexBuffer(ShapeGenerator().GenRectangle(0.1f, 0.5f))
        .Build())
      // .AttachChild(PolylineBuilder()
      //   .Name("trigger")
      //   .Color(RalColors::TrafficGreyA)
      //   .VertexBuffer(ShapeGenerator().GenCircle(TURRET_RANGE))
      //   .Build())  
      .Build()));

  entity->AddComponent(CBodyBuilder()
      .Type(CBody::Type::Static)
      .Build());

  entity->AddComponent(CCircleColliderBuilder()
      .Radius(0.3f)
      .Friction(GENERAL_FRICTION)
      .Restitution(GENERAL_RESTITUTION)
      .Build());

  entity->AddComponent(make_shared<CGun>(
      GUN_COOLDOWN, 
      1, 
      GUN_MUZZLE_VELOCITY, 
		  GUN_MOUNT_POINT, 
		  GUN_DIRECTION));

  entity->AddComponent(make_shared<CTurret>(range, TURRET_OFFSET));

  entity->AddComponent(make_shared<CObjective>());

  return entity;
}