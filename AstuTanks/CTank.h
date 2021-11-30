/*
 * Astu Tanks
 * 
 * Copyright (c) 2021 Tobias Filler. All rights reserved.
 */

#pragma once

// AST Utilities includes
#include <AstuECS.h>

class CTank : public astu::EntityComponent {
public: 
	/** The maximum torque the player can use to steer the ship. */
	float maxTorque;

	/** The maximum thrust the player can use to accelerate the ship. */
	float maxThrust;

	/** Describes the amount of currently active thrust. */
	float curThrust;

	/** Player index (0 = NPC, 1 = Player 1, 2 = Player 2) */
	int player;

	/**
	 * Constructor.
	 */
	CTank(float maxThrust, float maxTorque, int player) 
		: maxThrust(maxThrust)
		, maxTorque(maxTorque)
		, player(player)
		, curThrust(0)
	{
		// Intentionally left empty.
	}

	// Inherited via EntityComponent
	virtual std::shared_ptr<EntityComponent> Clone() override {
		// Create copy using copy-constructor.
		return std::make_shared<CTank>(*this);
	}    
};