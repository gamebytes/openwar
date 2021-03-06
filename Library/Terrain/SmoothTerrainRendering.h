// Copyright (C) 2013 Felix Ungman
//
// This file is part of the openwar platform (GPL v3 or later), see LICENSE.txt

#ifndef SMOOTHTERRAINRENDERING_H
#define SMOOTHTERRAINRENDERING_H

#include "SmoothTerrainModel.h"
#include "framebuffer.h"
#include "renderbuffer.h"
#include "vertexbuffer.h"
#include "renderer.h"


struct terrain_renderers;

struct terrain_address
{
	int _level; // level 0 has an unsplit chunk (1x1), level 1 is split 2x2, level 2 is split (2x2)x(2x2) and so on.
	int _x; // 0 <= _x <= 2 ^ _level
	int _y; // 0 <= _x <= 2 ^ _level

	terrain_address();
	terrain_address(int level, int x, int y);

	static bool is_valid(int level, int x, int z);
	terrain_address get_parent();
	void foreach_neighbor(std::function<void (terrain_address)> action);
	void foreach_child(std::function<void (terrain_address)> action);
	void foreach_ancestor(std::function<void (terrain_address)> action);

	bool all_children(std::function<bool (terrain_address)> predicate);
	bool all_ancestors(std::function<bool (terrain_address)> predicate);
};

bool operator ==(terrain_address chunk1, terrain_address chunk2);
bool operator !=(terrain_address chunk1, terrain_address chunk2);
bool operator <(terrain_address chunk1, terrain_address chunk2);



struct terrain_vertex
{
	glm::vec3 _position;
	glm::vec3 _normal;

	terrain_vertex(glm::vec3 p, glm::vec3 n) : _position(p), _normal(n) {}
};


struct terrain_edge_vertex
{
	glm::vec3 _position;
	float _height;

	terrain_edge_vertex() {}
	terrain_edge_vertex(glm::vec3 p, float h) : _position(p), _height(h) { }
};


struct terrain_uniforms
{
	glm::mat4x4 _transform;
	glm::vec3 _light_normal;
	const texture* _colors;
	const texture* _map;
};


class terrain_chunk
{
public:
	terrain_address _address;
	terrain_chunk* _parent;
	terrain_chunk* _neighbors[4];
	terrain_chunk* _children[4];
	bool _is_split;
	int _lod;
	shape<color_vertex3> _lines;
	shape<terrain_vertex> _inside;
	shape<terrain_vertex> _border;
	bounds3f _bounds;

	terrain_chunk(terrain_address address);
	bool has_children() const;

	shape<terrain_vertex>* triangle_shape(int inside);
};


struct sobel_uniforms
{
	glm::mat4x4 _transform;
	const texture* _depth;
};



class SmoothTerrainRendering
{
	SmoothTerrainModel* _terrainModel;
	image* _mapImage;

	int _framebuffer_width;
	int _framebuffer_height;
	framebuffer* _framebuffer;
	renderbuffer* _colorbuffer;
	texture* _depth;
	texture* _colors;
	texture* _mapTexture;

	std::map<terrain_address, terrain_chunk*> _chunks;
	std::map<terrain_address, bool> _split;
	std::map<terrain_address, float> _lod;

	shape<terrain_edge_vertex> _shape_terrain_edge;
	terrain_renderers* _renderers;

public:
	SmoothTerrainRendering(SmoothTerrainModel* terrainModel, image* map, bool render_edges);
	~SmoothTerrainRendering();

	SmoothTerrainModel* GetTerrainModel() const { return _terrainModel; }

	void UpdateHeights(bounds2f bounds);
	void UpdateMapTexture();

	void UpdateDepthTextureSize();
	void InitializeEdge();

	void Render(const glm::mat4x4& transform, const glm::vec3 lightNormal);
	void ForEachLeaf(terrain_address chunk, std::function<void(terrain_chunk&)> f);

	bool IsLoaded(terrain_address chunk);
	void LoadChunk(terrain_address chunk, float priority);
	void UnloadChunk(terrain_address chunk);

	void LoadChildren(terrain_address chunk, float priority);
	void RequestLoadChildrenUnloadGrandChildren(terrain_address chunk, float priority);
	void RequestUnloadChildren(terrain_address chunk);

	terrain_chunk* CreateNode(terrain_address chunk);

	bool IsSplit(terrain_address chunk);
	void SetSplit(terrain_address chunk);
	void ClearSplit(terrain_address chunk);
	bool CanChunkBeSplitted(terrain_address chunk);
	bool CanGrandParentBeSplitted(terrain_address chunk);

	void SetLod(terrain_address chunk, float lod);
	float GetLod(terrain_address chunk);

	bounds3f GetBounds(terrain_address chunk) const;

	void BuildLines(shape<color_vertex3>& shape, terrain_address chunk);
	void BuildTriangles(terrain_chunk* chunk);

	terrain_vertex MakeTerrainVertex(float x, float y);
	color_vertex3 MakeColorVertex(float x, float y);
};


struct terrain_viewpoint
{
	SmoothTerrainRendering* _terrainRendering;
	glm::vec3 _viewpoint;
	float _near;
	float _far;
	int _near_lod;
	float _distance_lod_max;

	terrain_viewpoint(SmoothTerrainRendering* terrainRendering);

	void set_parameters(float errorLodMax, float maxPixelError, float screenWidth, float horizontalFOVDegrees);

	float compute_lod(bounds3f boundingBox) const;
	float compute_lod(float distance) const;

	void update();
	void update(terrain_address chunk);


};


#endif
