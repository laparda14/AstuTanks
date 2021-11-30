/*
 * AstuTanks
 * 
 * Copyright (c) 2021 Tobias Filler. All rights reserved.
 */


#pragma once

// AST Utilities includes
#include <AstuECS.h>

class CObjective : public astu::EntityComponent {
public: 
    // Add component data member here...

    CObjective() {
        // Intentionally left empty.        
    }
    
    // Inherited via EntityComponent
    virtual std::shared_ptr<astu::EntityComponent> Clone() override {
        // Create copy using copy-constructor.
        return std::make_shared<CObjective>(*this);
    }    
};