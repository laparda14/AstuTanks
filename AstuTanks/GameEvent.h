/*
 * ASTU Tanks
 * 
 * Copyright (c) 2021 Tobias Filler. All rights reserved.
 */

#pragma once

// AST Utilities includes
#include <AstuMath.h>
#include <AstuServices.h>

/**
 * A signal which represents a game events.
 */
class GameEvent {
public:

    /** The various types of game events. */
    enum class Type {
        DestroyObjective,
        ShipDestroyed,
        LifeUpdate,
        ScoreUpdate
    };

    /** The type of this game event. */
    Type type;

    /** The value off the game event. */
    int value;


    int player;

    /**
     * Constructor.
     * 
     * @param t the type of this event
     * @param p the position of this event
     */
    GameEvent(Type t, int p = 0, int player = 1)
        : type(t), value(p), player(player)
    {
        // Intentionally left empty.        
    }
};

using GameEventService = astu::SignalService<GameEvent>;