/*
 * Astu Tanks
 * 
 * Copyright (c) 2021 Tobias Filler. All rights reserved.
 */

#include "BulletSystem.h"
#include "CBullet.h"
#include "CTank.h"
#include "Constants.h"
#include "CDestroyable.h"
#include "CObjective.h"

using namespace astu;
using namespace std;

const EntityFamily BulletSystem::FAMILY = EntityFamily::Create<CBullet, CDestroyable>();

BulletSystem::BulletSystem()
    : BaseService("Bullet System")
    , OneFamilyEntitySystem(FAMILY)    
{
    // Intentionally left empty.
}

void BulletSystem::OnStartup()
{
    // Intentionally left empty.
}

void BulletSystem::OnShutdown()
{
    // Intentionally left empty.
}

bool BulletSystem::OnCollision(astu::Entity& entityA, astu::Entity& entityB)
{
    if (entityA.HasComponent<CBullet>() && entityB.HasComponent<CDestroyable>()) {
        entityA.GetComponent<CBullet>().gunOrigen.BulletVanisched();
        HandelSignalsForEntity(entityB);
        GetEntityService().RemoveEntity(entityA);
        GetEntityService().RemoveEntity(entityB);
        return false;
    }
    if (entityB.HasComponent<CBullet>() && entityA.HasComponent<CDestroyable>()) {
        entityB.GetComponent<CBullet>().gunOrigen.BulletVanisched();
        HandelSignalsForEntity(entityA);
        GetEntityService().RemoveEntity(entityB);
        GetEntityService().RemoveEntity(entityA);
        return false;
    }

    if (entityA.HasComponent<CBullet>()){
        auto &bullet = entityA.GetComponent<CBullet>();
        if (bullet.deflections >= BULLET_MAX_DEFELTIONS){
            entityA.GetComponent<CBullet>().gunOrigen.BulletVanisched();
            GetEntityService().RemoveEntity(entityA);
        } else {
            bullet.deflections++;
        }
    }

    if (entityB.HasComponent<CBullet>()){
        auto &bullet = entityB.GetComponent<CBullet>();
        if (bullet.deflections >= BULLET_MAX_DEFELTIONS){
            entityB.GetComponent<CBullet>().gunOrigen.BulletVanisched();
            GetEntityService().RemoveEntity(entityB);
        } else {
            bullet.deflections++;
        }
    }

    return false;
}

void BulletSystem::HandelSignalsForEntity(Entity& entity)
{
    if (entity.HasComponent<CTank>()){
        EmitSignal(GameEvent(GameEvent::Type::LifeUpdate,-1,entity.GetComponent<CTank>().player));
    }

    if (entity.HasComponent<CDestroyable>()){
        EmitSignal(GameEvent(GameEvent::Type::ScoreUpdate, entity.GetComponent<CDestroyable>().points));
    }

    if (entity.HasComponent<CObjective>()){
        EmitSignal(GameEvent(GameEvent::Type::DestroyObjective));
    }
    
}