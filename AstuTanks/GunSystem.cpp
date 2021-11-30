/*
 * AstuTanks
 * 
 * Copyright (c) 2021 Tobias Filler. All rights reserved.
 */

#include "Constants.h"
#include "GunSystem.h"
#include "CBullet.h"
#include "CGun.h"
#include "EntityPrototypeService.h"
#include "CTank.h"

#include <AstuSuite2D.h>
#include <AstuECS.h>

using namespace astu2d;
using namespace astu;
using namespace std;

const EntityFamily GunSystem::FAMILY = EntityFamily::Create<CGun,CTank, CPose>();

GunSystem::GunSystem(int updatePriority)
	  : BaseService("Gun System")
	  , IteratingEntitySystem(FAMILY, updatePriority)    
{
	  // Intentionally left empty.
}

void GunSystem::OnStartup()
{
	fireAction1 = ASTU_SERVICE(InputMappingService).BindAction("FirePlayer1");
	fireAction1->SetDelegate([this](ActionBinding &binding){
		if (binding.IsPressed()) {
			trigger1 = true;
		}
	});
	trigger1 = false;

	fireAction2 = ASTU_SERVICE(InputMappingService).BindAction("FirePlayer2");
	fireAction2->SetDelegate([this](ActionBinding &binding){
		if (binding.IsPressed()) {
			trigger2 = true;
		}
	});
	trigger2 = false;
}

void GunSystem::OnShutdown()
{
	ASTU_SERVICE(InputMappingService).RemoveActionBinding(fireAction1);
	fireAction1 = nullptr;
	ASTU_SERVICE(InputMappingService).RemoveActionBinding(fireAction2);
	fireAction2 = nullptr;
}

void GunSystem::ProcessEntity(Entity& entity)
{
    auto& gun = entity.GetComponent<CGun>();
	gun.cooldownDelta -= GetElapsedTimeF();
	
    if (trigger1) {
		fireBullet(1, trigger1, entity);
	} else {
		if (trigger2) 
		{
			fireBullet(2, trigger2, entity);
		} else {
			return;
		}
	}
}

void GunSystem::fireBullet(int player, bool & trigger, Entity & entity){

	auto& gun = entity.GetComponent<CGun>();

	auto &tank = entity.GetComponent<CTank>();

	if (tank.player != player){
		return;
	}

	if (gun.cooldownDelta > 0) {
		return;
	}

	if (gun.currentBullets >= gun.bullets){
        trigger = false;
        return;
    }

	trigger = false;
    gun.currentBullets += 1;    

    gun.cooldownDelta = gun.cooldownMax;

    auto pose = entity.GetComponent<CPose>();	
	Vector2f spawnPoint = pose.transform.TransformPoint(gun.mountPoint);

    // Create and add bullet.
	//auto bulletEntity = AddEntity("Bullet", spawnPoint, pose.transform.GetRotation());

    auto bulletEntity = CreateBullet(spawnPoint, gun, pose.transform.GetRotation());
    ASTU_GET_SERVICE(EntityService)->AddEntity(bulletEntity);

    // Set velocity of bullet.
	auto& bulletBody = bulletEntity->GetComponent<CBody>();
	Vector2f velocity = pose.transform.TransformVector(gun.direction) * gun.muzzleVelocity;
	bulletBody.SetLinearVelocity(velocity);
}

void GunSystem::ResetTrigger1(){
	trigger1 = false;
}

shared_ptr<astu::Entity> GunSystem::CreateBullet(Vector2f spawnPoint, CGun& gunOrigen,float rotation){
  auto entity = make_shared<Entity>();

  entity->AddComponent(make_shared<CPose>(spawnPoint.x,spawnPoint.y,rotation));

	entity->AddComponent(make_shared<CScene>(NodeBuilder()
		.AttachChild(PolylineBuilder()
			.Color(BULLET_COLOR)
			.VertexBuffer(ShapeGenerator().GenCircle(BULLET_RADIUS))
			.Build())
		.Build())
	);

	entity->AddComponent(CBodyBuilder()
		.Type(CBody::Type::Dynamic)
		.LinearDamping(0)
		.AngularDamping(0)
		.Build()
	);

	entity->AddComponent(CCircleColliderBuilder()
		.Radius(BULLET_RADIUS)
		.Friction(0)
		.Restitution(1)
		.Build()
	);

  entity->AddComponent(make_shared<CBullet>(gunOrigen));

  return entity;
}