/*
 * Astu Tanks
 * 
 * Copyright (c) 2021 Tobias Filler. All rights reserved.
 */

#pragma once

// AST Utilities includes
#include <AstuECS.h>
#include <AstuSuite2D.h>

class TurretSystem 
  : public astu::BaseService
  , private astu::IteratingEntitySystem
  , private astu::TimeClient
{
public:

  // Constructor.
  TurretSystem(int updatePriority = astu::Priority::Normal);

private:
  // The family of entities this system processes
  static const astu::EntityFamily TURRETFAMILY;
  static const astu::EntityFamily TARGETFAMILY;

  std::shared_ptr<astu::EntityView> turrets;
  std::shared_ptr<astu::EntityView> targets;

  std::vector<astu::Vector2f> Targets;
  bool nerastTargetInRange;

  // Inherited via BaseService
  virtual void OnStartup() override;
  virtual void OnShutdown() override;

  // Inherited via IteratingEntitySystem
  virtual void ProcessEntity(astu::Entity& entity) override;

  // utility Methods
  astu::Vector2f GetNerastTarget(astu::Entity &turret);

  float VectorToRad(astu::Vector2f v);

  float TranslateRotation(float r);

  float GetRotationDir(float rotationA, float rotationB);

  // State Methods
  void Idle(astu::Entity& entity);

  void RotateToTarget(astu::Entity& entity, astu::Vector2f targetPos);

  void FireBullet(astu::Entity& entity);
};