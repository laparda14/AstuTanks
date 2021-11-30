/*
 * Astu Tanks
 * 
 * Copyright (c) 2021 Tobias Filler. All rights reserved.
 */

#pragma once

// AST-Utilities includes
#include <AstuServices.h>
#include <AstuECS.h>

class EntityPrototypeService : public astu::BaseService
{
public:

	// Constructor.
	EntityPrototypeService(bool debug = false);

private:
  // Whether debug mode is activated.
  bool debug;

  // Inherited via BaseService
  virtual void OnStartup() override;
  virtual void OnShutdown() override;

  std::shared_ptr<astu::Entity> CreateBoundary(float w, float h);
  std::shared_ptr<astu::Entity> CreateTank(int player, int color);
  // std::shared_ptr<astu::Entity> CreateBullet(std::shared_ptr<CGun>& gunOrigen);
  std::shared_ptr<astu::Entity> CreateWall(float h, float l);
  std::shared_ptr<astu::Entity> CreateBox();
  std::shared_ptr<astu::Entity> CreateTurret(float range);
};