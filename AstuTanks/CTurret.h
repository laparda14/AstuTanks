/*
 * Astu Tanks
 * 
 * Copyright (c) 2021 Tobias Filler. All rights reserved.
 */

#pragma once

// AST Utilities includes
#include <AstuECS.h>

class CTurret : public astu::EntityComponent {
public:
	float range;
	float offset;

	/**
	 * Constructor.
	 */
	CTurret(float ragne, float offset)
	:range(ragne)
	,offset(offset)
	{
		// Intentionally left empty.
	}

	// Inherited via EntityComponent
	virtual std::shared_ptr<EntityComponent> Clone() override {
		// Create copy using copy-constructor.
		return std::make_shared<CTurret>(*this);
	}    
};