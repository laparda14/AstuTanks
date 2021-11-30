/*
 * Astu Tanks
 * 
 * Copyright (c) 2021 Tobias Filler. All rights reserved.
 */

#pragma once

// AST Utilities includes
#include <AstuECS.h>

class CDestroyable : public astu::EntityComponent {
public: 
	/** The points that are stored by this entity */
	int points;

	/**
	 * Constructor.
	 */
	CDestroyable(int points) 
		: points(points)
	{
		// Intentionally left empty.
	}

	// Inherited via EntityComponent
	virtual std::shared_ptr<EntityComponent> Clone() override {
		// Create copy using copy-constructor.
		return std::make_shared<CDestroyable>(*this);
	}    
};