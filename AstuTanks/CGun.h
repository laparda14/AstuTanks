/*
 * Astu Tanks
 * 
 * Copyright (c) 2021 Tobias Filler. All rights reserved.
 */

#pragma once

// AST Utilities includes
#include <AstuECS.h>
#include <AstuMath.h>

class CGun : public astu::EntityComponent {
public: 
	/** The minimal cooldown between shots */
	float cooldownMax;

	float cooldownDelta;

	/** The number of bullets that the Player can fire at once. */
	int bullets;

	/** the number of bullets that are currently on in the gameword from this gun */
	int currentBullets;

	/** The initial velocity of projectiles this gun fires. */
	float muzzleVelocity;

	/** The determines the spawn point of projectiles in local space. */
	astu::Vector2f mountPoint;

	/** The direction the gun fires projectiles in local space. */
	astu::Vector2f direction;

	/**
	 * Constructor.
	 */
	CGun(float cooldownMax, int bullets, float speed, const astu::Vector2f& pos, const astu::Vector2f& dir) 
		: cooldownMax(cooldownMax)
		, bullets(bullets)
		, muzzleVelocity(speed)
		, mountPoint(pos)
		, direction (dir)
		, currentBullets(0)
		, cooldownDelta(0)
	{
		// Intentionally left empty.
	}

	// Inherited via EntityComponent
	virtual std::shared_ptr<EntityComponent> Clone() override {
		// Create copy using copy-constructor.
		return std::make_shared<CGun>(*this);
	}

	void BulletVanisched(){
		currentBullets = currentBullets == 0? currentBullets: --currentBullets;
	}
};