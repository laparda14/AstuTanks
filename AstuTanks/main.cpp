/*
 * Astu Tanks
 * 
 * Copyright (c) 2021 Tobias Filler. All rights reserved.
 */

// Local includes
#include "EntityPrototypeService.h"	
#include "GameModeService.h"
#include "Constants.h"
#include "TankSystem.h"
#include "GunSystem.h"
#include "BulletSystem.h"
#include "TurretSystem.h"
#include "TextRenderService.h"
#include "GameEvent.h"

// ASTU/Box2D includes
#include <AstuBox2D.h>

// AST-Utilities includes
#include <AstuSuiteSDL.h>
#include <AstuInput.h>
#include <AstuECS.h>
#include <AstuSuite2D.h>

using namespace astu2d;
using namespace astu;
using namespace std;

// Main entry point of the application.
int main()
{
	SdlApplication app;

	// Cameras do the world-to-screen transform and used by the scene graph.
	ASTU_CREATE_AND_ADD_SERVICE(CameraService);
	//ASTU_CREATE_AND_ADD_SERVICE(CameraControlService);

	// Add Services and Systems required ECS.
	ASTU_CREATE_AND_ADD_SERVICE(EntityService);
	ASTU_CREATE_AND_ADD_SERVICE(EntityFactoryService);	
	ASTU_CREATE_AND_ADD_SERVICE(Box2DPhysicsSystem);
	ASTU_CREATE_AND_ADD_SERVICE(CollisionSignalService);
	ASTU_CREATE_AND_ADD_SERVICE(SceneSystem);

	// adding stolen services ;)
	ASTU_CREATE_AND_ADD_SERVICE(TextRenderService);

	// Let's add our main service that runs the application.
	ASTU_CREATE_AND_ADD_SERVICE(EntityPrototypeService);
	ASTU_CREATE_AND_ADD_SERVICE(GameModeService);
	ASTU_CREATE_AND_ADD_SERVICE(TankSystem);
	ASTU_CREATE_AND_ADD_SERVICE(GunSystem);
	ASTU_CREATE_AND_ADD_SERVICE(BulletSystem);
	ASTU_CREATE_AND_ADD_SERVICE(TurretSystem);
	ASTU_CREATE_AND_ADD_SERVICE(GameEventService);

	// Configure physics environment.
	ASTU_SERVICE(PhysicsSystem).SetGravityVector(0, 0);

	// Configure input controls (Axis and Actions)
	// Player 2
	ASTU_SERVICE(InputMappingService).AddAxisMapping("TurningPlayer1", Keys::A, -1.0f);
	ASTU_SERVICE(InputMappingService).AddAxisMapping("TurningPlayer1", Keys::D, 1.0f);

	ASTU_SERVICE(InputMappingService).AddAxisMapping("ThrustPlayer1", Keys::W, -1.0f);
	ASTU_SERVICE(InputMappingService).AddAxisMapping("ThrustPlayer1", Keys::S, 1.0f);

	ASTU_SERVICE(InputMappingService).AddActionMapping("FirePlayer1", Keys::SpaceBar);

	// Player 1
	ASTU_SERVICE(InputMappingService).AddAxisMapping("TurningPlayer2", Keys::Left, -1.0f);
	ASTU_SERVICE(InputMappingService).AddAxisMapping("TurningPlayer2", Keys::Right, 1.0f);

	ASTU_SERVICE(InputMappingService).AddAxisMapping("ThrustPlayer2", Keys::Up, -1.0f);
	ASTU_SERVICE(InputMappingService).AddAxisMapping("ThrustPlayer2", Keys::Down, 1.0f);

	ASTU_SERVICE(InputMappingService).AddActionMapping("FirePlayer2", Keys::P);
	// Controller Mappings
	ASTU_SERVICE(InputMappingService).AddAxisMapping("TurningPlayer2", Keys::GamepadLeftThumbstickX);

	//ASTU_SERVICE(InputMappingService).AddAxisMapping("ThrustPlayer2", Keys::GamepadLeftShoulder);
	ASTU_SERVICE(InputMappingService).AddAxisMapping("ThrustPlayer2", Keys::GamepadRightShoulder,-1.0f);

	ASTU_SERVICE(InputMappingService).AddActionMapping("FirePlayer2", Keys::GamepadFaceButtonBottom);


	// Configure application.
	app.SetApplicationName("Astu Tanks");
	app.SetVersionString("1.0.3");
	app.SetResolution(Resolution::WXGA);
	app.SetFullscreen(false);

	// Run the application.
	return app.Run();
} 
