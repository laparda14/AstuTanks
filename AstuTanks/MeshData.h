/*
 * Astu Tanks
 * 
 * Copyright (c) 2021 Roman Divotkey. All rights reserved.
 * only uses the Font Data of the original
 *
 * Code generated with SWORD 1.8.15
 * Last modified: Thu Aug 05 19:18:45 CEST 2021
 */
#pragma once

// AST-Utilities includes
#include <AstuMath.h>

// C++ Standard Library includes
#include <cstdint>
#include <array>
#include <vector>
#include <string>

/**
 * Colletion of mesh-related data.
 */
class MeshData {
public:
	/** The name of the mesh data. */
	std::string name;

	/** The vertices of the polygon-data. */
	std::vector<astu::Vector2f> vertices;

	/** The radius of this mesh. */
	float radius;

	/**
	 * Constructor.
	 * 
	 * @param name	the name 
	 * @param r			the radius
	 * @param vtx		the vertices
	 */
	MeshData(const std::string & name, float r, const std::vector<astu::Vector2f>& vtx)
		: name(name), vertices(vtx), radius(r) {}
};

/**
 * Contains indxed mesh data.
 */
class IndexedMeshData : public MeshData {
public:
	/** The vertex indexes. */
	std::vector<uint32_t> indices;

	IndexedMeshData(
		const std::string & name,
		float r, 
		const std::vector<astu::Vector2f>& vtx,
		const std::vector<uint32_t>& idx
	)
		: MeshData(name, r, vtx)
		, indices(idx)
	{
		// Intentionally left empty.
	}
};

// Contains the font data.
extern const float kFontWidth;
extern const float kFontHeight;
extern const std::array<IndexedMeshData, 42> kFont;