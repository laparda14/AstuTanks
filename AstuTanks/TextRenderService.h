/*
 * Asteroids Clone
 * 
 * Copyright (c) 2021 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "MeshData.h"

// AST-Utilities includes
#include <AstuMath.h>
#include <AstuServices.h>
#include <AstuSuite2D.h>

// C++ Standard library includes
#include <map>
#include <string>

class TextRenderService 
    : public astu::BaseService
    , private astu2d::LineRendererClient2f
    , private astu2d::CameraClient
    , private astu::MouseButtonListener
    , private astu::KeystrokeListener
{
public:

    /**
     * The different types of text alingments.
     */
    enum class Alignment {
        Left, Center, Right
    };

    /**
     * Constructor.
     */
    TextRenderService();

    /**
     * Draws a text at a certain position using the current color and alignment.
     * 
     * @param p the position of the text
     * @param s a string containing the text to draw
     */
    void DrawString(const astu::Vector2f& p, const std::string& s) {
        DrawString(p.x, p.y, s);
    }

    /**
     * Draws a text at a certain position using the current color and alignment.
     * 
     * @param x the x-coordinate of the text
     * @param y the y-coordinate of the text
     * @param s a string containing the text to draw
     */
    void DrawString(float x, float y, const std::string& s);

    /**
     * Sets the alignment of the text.
     * 
     * @param align the alingment
     */
    void SetAlingment(Alignment align) {
        alignment = align;
    }

    /**
     * Sets the current drawing color used for all subsequent draw calls.
     * 
     * @param c the new drawing color 
     */
    void SetDrawColor(const astu::Color4f& c) {
        color = c;
    }

    /**
     * Sets the view tranformation. 
     * 
     * The view transformation matrix is typically provieded by a camera.
     * 
     * @param m the view transformation matrix
     */
    void SetViewTransform(const astu::Matrix3f& m);

private:
    /** Maps characters to mesh data for quick lookup. */
    std::map<char, const IndexedMeshData*> charToMesh;

    /* The space between characters. */
    float characterSpacing;

    /** The color used to render characters. */
    astu::Color4f color;

    /** The horizontal text alignment. */
    Alignment alignment;

    void DrawCharacter(const astu::Vector2f& p, const IndexedMeshData& mesh);

    // Inherited via BaseService
    virtual void OnStartup() override;
    virtual void OnShutdown() override;
};