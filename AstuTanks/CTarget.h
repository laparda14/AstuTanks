/*
 * Astu Tanks
 * 
 * Copyright (c) 2021 Tobias Filler. All rights reserved.
 */

#pragma once

// AST Utilities includes
#include <AstuECS.h>

class CTarget : public astu::EntityComponent {
public: 


	/**
	 * Constructor.
	 */
	CTarget() 

	{
		// Intentionally left empty.
	}

	// Inherited via EntityComponent
	virtual std::shared_ptr<EntityComponent> Clone() override {
		// Create copy using copy-constructor.
		return std::make_shared<CTarget>(*this);
	}    
};