/*
 * Asteroids Clone
 * 
 * Copyright (c) 2021 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "TextRenderService.h"
#include "MeshData.h"

// AST-Utilities includes
#include <AstuGraphics.h>

// C++ Standard Library includes
#include <cassert>

using namespace astu;
using namespace std;

TextRenderService::TextRenderService()
    : BaseService("Text Redern Service")
    , characterSpacing(0.5f)
    , color(WebColors::Aqua)
    , alignment(Alignment::Left)
{
    // Intentionally left empty.    
}

void TextRenderService::SetViewTransform(const Matrix3f& m)
{
    LineRendererClient::SetViewTransform(m);
}

void TextRenderService::OnStartup()
{
    for (const auto& mesh : kFont) {
        if (mesh.name.empty())
            continue;

        charToMesh[mesh.name[0]] = &mesh;
    }
}

void TextRenderService::OnShutdown()
{
    charToMesh.clear();
}

void TextRenderService::DrawCharacter(const Vector2f& p, const IndexedMeshData& mesh)
{
    assert(mesh.indices.size() % 2 == 0);
    for (auto it = mesh.indices.begin(); it != mesh.indices.end();) {
        const Vector2f p1 = mesh.vertices[*it++] + p;
        const Vector2f p2 = mesh.vertices[*it++] + p;
        DrawLine(p1, p2);
    }
}

void TextRenderService::DrawString(float x, float y, const string& s)
{
    LineRendererClient::SetDrawColor(color);

    Vector2f pos;
    int n = static_cast<int>(s.length());
    float deltaX = kFontWidth + characterSpacing;

    switch (alignment) {
    case Alignment::Left:
        pos.Set(x + kFontWidth / 2, y);
        break;

    case Alignment::Center:
        pos.Set(x - ((kFontWidth + characterSpacing) * (n - 1)) / 2, y);
        break;

    case Alignment::Right:
        pos.Set(x - ((kFontWidth + characterSpacing) * (n - 1)) + kFontWidth / 2, y);
        break;
    }

    for (const char& ch : s) {
        auto it = charToMesh.find(ch);
        if (it == charToMesh.end()) {
            continue;
        }

        DrawCharacter(pos, *it->second);
        pos.x += deltaX;
    }
}
