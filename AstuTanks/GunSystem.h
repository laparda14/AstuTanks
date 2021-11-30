/*
 * AstuTanks
 * 
 * Copyright (c) 2021 Tobias Filler. All rights reserved.
 */

#pragma once

#include "CGun.h"

// AST Utilities includes
#include <AstuECS.h>
#include <AstuInput.h>
#include <AstuServices.h>

class GunSystem 
	: public astu::BaseService
	, private astu::IteratingEntitySystem
	, private astu::TimeClient
	, private astu::EntityFactoryClient
{
public:

	/**
	 * Constructor.
	 * 
	 * @param updatePriority	the update priority of this system
	 */
	GunSystem(int updatePriority = astu::Priority::Normal);

	std::shared_ptr<astu::Entity> CreateBullet(astu::Vector2f spawnPoint, CGun& gunOrigen,float rotation);

	void ResetTrigger1();

private:
	// The family of entities this system processes
	static const astu::EntityFamily FAMILY;

	/** Used to fire guns. */
	std::shared_ptr<astu::ActionBinding> fireAction1;
	std::shared_ptr<astu::ActionBinding> fireAction2;

	/** Whether the trigger of player 1 has been pulled. */
	bool trigger1;

	/** Whether the trigger of player 2 has been pulled. */
	bool trigger2;

	// Inherited via BaseService
	virtual void OnStartup() override;
	virtual void OnShutdown() override;

	// Inherited via IteratingEntitySystem
	virtual void ProcessEntity(astu::Entity& entity) override;

	void fireBullet(int player, bool & trigger, astu::Entity & entity);

	
};