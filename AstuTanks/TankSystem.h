/*
 * Astu Tanks
 * 
 * Copyright (c) 2021 Tobias Filler. All rights reserved.
 */

#pragma once

// AST Utilities includes
#include <AstuECS.h>
#include <AstuSuite2D.h>
#include <AstuInput.h>

class TankSystem 
  : public astu::BaseService
  , private astu::IteratingEntitySystem
{
public:

  // Constructor.
  TankSystem(int updatePriority = astu::Priority::Normal);

private:
  // The family of entities this system processes
  static const astu::EntityFamily FAMILY;


  /** Used to turn the Tank. */
  std::shared_ptr<astu::AxisBinding> turnAxis1; // player 1
  std::shared_ptr<astu::AxisBinding> turnAxis2; // player 2

  /** Used to steer the ship. */
  std::shared_ptr<astu::AxisBinding> thrustAxis1; // player 1
  std::shared_ptr<astu::AxisBinding> thrustAxis2; // player 2

  // Inherited via BaseService
  virtual void OnStartup() override;
  virtual void OnShutdown() override;

  // Inherited via IteratingEntitySystem
  virtual void ProcessEntity(astu::Entity& entity) override;

  void ManageMovement(std::shared_ptr<astu::AxisBinding> turnAxis, std::shared_ptr<astu::AxisBinding> thrustAxis, astu::Entity &entity);
};