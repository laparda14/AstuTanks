/*
 * AstuTanks
 * 
 * Copyright (c) 2021 Tobias Filler. All rights reserved.
 */

#pragma once

#include "CGun.h"

// AST Utilities includes
#include <AstuECS.h>

class CBullet : public astu::EntityComponent {
public:
	CGun& gunOrigen;

	int deflections;

	/**
	 * Constructor.
	 */
	CBullet(CGun& gunOrigen)
	: gunOrigen(gunOrigen)
	, deflections(0)
	{
		// Intentionally left empty.
	}

	void SetGunOrigen(CGun& gunOrigen){
		this->gunOrigen = gunOrigen;
	}

	// Inherited via EntityComponent
	virtual std::shared_ptr<EntityComponent> Clone() override {
		// Create copy using copy-constructor.
		return std::make_shared<CBullet>(*this);
	}    
};