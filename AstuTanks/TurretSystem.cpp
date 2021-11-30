/*
 * AstuTanks
 * 
 * Copyright (c) 2021 Tobias Filler. All rights reserved.
 */

// Local includes
#include "TurretSystem.h"
#include "CTurret.h"
#include "CTarget.h"
#include "GunSystem.h"
#include "Constants.h"
#include "CGun.h"

// AST Utilities includes
#include <AstuSuite2D.h>
#include <AstuECS.h>

// C++ Standart libary includes
#include "math.h"

using namespace astu2d;
using namespace astu;
using namespace std;


const EntityFamily TurretSystem::TURRETFAMILY = EntityFamily::Create<CTurret, CBody, CPose>();
const EntityFamily TurretSystem::TARGETFAMILY = EntityFamily::Create<CTarget, CPose>();

TurretSystem::TurretSystem(int updatePriority)
  : BaseService("Turret System")
  , IteratingEntitySystem(TURRETFAMILY, updatePriority)   
{
  nerastTargetInRange = false;
}

void TurretSystem::OnStartup()
{
  targets = ASTU_SERVICE(EntityService).GetEntityView(TARGETFAMILY);  
  turrets = ASTU_SERVICE(EntityService).GetEntityView(TURRETFAMILY);
}

void TurretSystem::OnShutdown()
{
  turrets = nullptr;
  targets = nullptr;
}

void TurretSystem::ProcessEntity(Entity & entity)
{
  auto& gun = entity.GetComponent<CGun>();
	gun.cooldownDelta -= GetElapsedTimeF();

  Vector2f nerastTarget = GetNerastTarget(entity);
  if (nerastTarget == Vector2f(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity())){
    Idle(entity);
    return;
  }

  if (nerastTarget.Distance(entity.GetComponent<CPose>().transform.GetTranslation()) < entity.GetComponent<CTurret>().range){
    RotateToTarget(entity, nerastTarget);
    return;
  } else {
    Idle(entity);
    return;
  }
}

void TurretSystem::RotateToTarget(astu::Entity& entity, Vector2f target)
{

  auto &pose = entity.GetComponent<CPose>().transform;
  Vector2f lookVector = target - pose.GetTranslation();

  float offset = entity.GetComponent<CTurret>().offset;

  float lookRotation = VectorToRad(lookVector) + MathUtils().PIf *1.5f;

  float rotation = TranslateRotation(pose.GetRotation());

  if (rotation < lookRotation + offset && rotation > lookRotation - offset){
    FireBullet(entity);
  } else {

    float dir = GetRotationDir(rotation, lookRotation);
    pose.Rotate(dir * TURRET_ROTATION_SPEED * GetElapsedTimeF());
  }
}

void TurretSystem::Idle(Entity& entity)
{
  auto& pose = entity.GetComponent<CPose>();
  pose.transform.Rotate( TURRET_IDLE_SPEED * GetElapsedTimeF() );
}

void TurretSystem::FireBullet(Entity& entity)
{
  auto& gun = entity.GetComponent<CGun>();

  if (gun.cooldownDelta > 0)
  {
		return;
	}

	if (gun.currentBullets >= gun.bullets){
        return;
    }

    gun.currentBullets += 1;    

    gun.cooldownDelta = gun.cooldownMax;

    auto pose = entity.GetComponent<CPose>();	
	Vector2f spawnPoint = pose.transform.TransformPoint(gun.mountPoint);

    // Create and add bullet.
	//auto bulletEntity = AddEntity("Bullet", spawnPoint, pose.transform.GetRotation());

    auto bulletEntity = ASTU_SERVICE(GunSystem).CreateBullet(spawnPoint, gun, pose.transform.GetRotation());
    ASTU_GET_SERVICE(EntityService)->AddEntity(bulletEntity);

    // Set velocity of bullet.
	auto& bulletBody = bulletEntity->GetComponent<CBody>();
	Vector2f velocity = pose.transform.TransformVector(gun.direction) * gun.muzzleVelocity;
	bulletBody.SetLinearVelocity(velocity);
}

astu::Vector2f TurretSystem::GetNerastTarget(astu::Entity &turret)
{
  auto pos = turret.GetComponent<CPose>().transform.GetTranslation();

  Vector2f nerastTarget = Vector2f(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity());
  float nerastDistance = std::numeric_limits<float>::infinity();

  for (auto & entity : *targets){
    auto v = entity->GetComponent<CPose>().transform.GetTranslation();
    auto d = v.Distance(pos);

    if (d < nerastDistance){
      nerastDistance = d;
      nerastTarget = v;
    }
  }

  return nerastTarget;
}

float TurretSystem::VectorToRad(Vector2f v)
{
  return atan2f(v.y, v.x);
}

float TurretSystem::TranslateRotation(float r)
{
  if (r > MathUtils().PIf * 2.5f){
    r = TranslateRotation(r - (MathUtils().PI2f));
  } else if (r < MathUtils().PIf * 0.5f){
    r = TranslateRotation(r + (MathUtils().PI2f));
  }
  return r;
}

float TurretSystem::GetRotationDir(float rotationA, float rotationB)
{
  float distOne = rotationA - rotationB;
  float distTwo = rotationA - rotationB + MathUtils().PI2f;
  float distThree = rotationB - rotationA + MathUtils().PI2f;
  distThree *= -1.0f;

  float dist = abs(distOne) >= abs(distTwo) ? distTwo : distOne;
  dist = abs(dist) >= abs(distThree) ? distThree : dist;

  return dist >= 0 ? -1.0f : 1.0f;
}