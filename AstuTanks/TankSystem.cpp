/*
 * AstuTanks
 * 
 * Copyright (c) 2021 Tobias Filler. All rights reserved.
 */

// Local includes
#include "TankSystem.h"
#include "CTank.h"

// AST Utilities includes
#include <AstuSuite2D.h>

using namespace astu2d;
using namespace astu;
using namespace std;

// TODO add types of entity components to the family e.g.,
// EntityFamily::Create<astu2d::CPose, astu2d::CBody>();

const EntityFamily TankSystem::FAMILY = EntityFamily::Create<CTank, CBody, CPose>();

TankSystem::TankSystem(int updatePriority)
  : BaseService("Tank controll system")
  , IteratingEntitySystem(FAMILY, updatePriority)    
{
  // Intentionally left empty.
}

void TankSystem::OnStartup()
{
  auto& imps = ASTU_SERVICE(InputMappingService);
	turnAxis1 = imps.BindAxis("TurningPlayer1");
  thrustAxis1 = imps.BindAxis("ThrustPlayer1");
  turnAxis2 = imps.BindAxis("TurningPlayer2");
  thrustAxis2 = imps.BindAxis("ThrustPlayer2");
}

void TankSystem::OnShutdown()
{
  auto& imps = ASTU_SERVICE(InputMappingService);
	imps.RemoveAxisBinding(thrustAxis1);
	thrustAxis1 = nullptr;
	imps.RemoveAxisBinding(turnAxis1);
	turnAxis1 = nullptr;
  imps.RemoveAxisBinding(thrustAxis2);
	thrustAxis2 = nullptr;
	imps.RemoveAxisBinding(turnAxis2);
	turnAxis2 = nullptr;
}

void TankSystem::ProcessEntity(Entity & entity)
{
  
    auto& tank = entity.GetComponent<CTank>();

    if (tank.player == 1)
    {
      ManageMovement(turnAxis1, thrustAxis1, entity);
    } else if (tank.player == 2)
    {
      ManageMovement(turnAxis2, thrustAxis2, entity);
    }
}

void TankSystem::ManageMovement(std::shared_ptr<astu::AxisBinding> turnAxis, std::shared_ptr<astu::AxisBinding> thrustAxis, astu::Entity &entity){

  auto& body = entity.GetComponent<CBody>();
  auto& tank = entity.GetComponent<CTank>();
	
	// Update current thrust, used for thrust visualization etc.
	tank.curThrust = thrustAxis->GetValue() * tank.maxThrust;

	// Apply current torque.
  body.ApplyTorque(turnAxis->GetValue() * tank.maxTorque);

	// Apply current thrust.
	body.ApplyForce( body.GetWorldVector(0, -tank.curThrust));

}