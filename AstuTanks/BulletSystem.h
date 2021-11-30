/*
 * AstuTanks
 * 
 * Copyright (c) 2021 Tobias Filler. All rights reserved.
 */


#pragma once

#include "CGun.h"
#include "GameEvent.h"

// AST Utilities includes
#include <AstUtils.h>
#include <AstuECS.h>
#include <AstuSuite2D.h>

class BulletSystem 
    : public astu::BaseService
    , private astu::OneFamilyEntitySystem
    , private astu2d::CollisionListener
    , private astu::SignalEmitter<GameEvent>
{
public:
    
    /** Constructor. */
    BulletSystem();

private:
    // The family of entities this system processes
    static const astu::EntityFamily FAMILY;

    // Inherited via BaseService
    virtual void OnStartup() override;
    virtual void OnShutdown() override;

    // Inherited via CollisionListener
    virtual bool OnCollision(astu::Entity& entityA, astu::Entity& entityB) override;

    void HandelSignalsForEntity(astu::Entity& entity);
};