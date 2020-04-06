#pragma once
#ifndef _OBSTACLES_H
#define _OBSTACLES_H

#include <system/application.h>
#include <maths/vector2.h>
#include <graphics/mesh_instance.h>
#include <box2d/Box2D.h>
#include "Obstacles.h"


#define NUMBER_OF_OBSTACLE_MESH_INSTANCE 25
#define NUMBER_OF_OBJECT_B2B_BODIES 25
// FRAMEWORK FORWARD DECLARATIONS
namespace gef
{
	class Platform;
	class Renderer3D;
	class Scene;

}

class Obstacles
{
public:
//	enum GAMEDIFFICULTY { DIFF_EASY = 0, DIFF_MEDIUM, DIFF_HARD };

	void Render(gef::Renderer3D *renderer, int difficultysstate);
	void InitObstacles(gef::Platform& platform, b2World* world, int difficultysstate);

private:
	//GAMEDIFFICULTY difficulty_state_;


	gef::Scene* LoadSceneAssets(gef::Platform& platform, const char* filename);
	gef::Mesh* GetMeshFromSceneAssets(gef::Scene* scene);


	gef::Renderer3D* renderer_3d_;



	gef::Scene* samurai_obstacle[4];
	gef::Scene* military_obstacle[4];
	gef::Scene* apoc_obstacle[4];



	gef::MeshInstance  military_obstacle_mesh_instance[NUMBER_OF_OBSTACLE_MESH_INSTANCE];
	gef::MeshInstance  samurai_obstacle_mesh_instance[NUMBER_OF_OBSTACLE_MESH_INSTANCE];
	gef::MeshInstance  apoc_obstacle_mesh_instance[NUMBER_OF_OBSTACLE_MESH_INSTANCE];
	gef::Vector4 military_obstacle_mesh_instance_V4[NUMBER_OF_OBSTACLE_MESH_INSTANCE];
	gef::Vector4 samurai_obstacle_mesh_instance_V4[NUMBER_OF_OBSTACLE_MESH_INSTANCE];
	gef::Vector4 apoc_obstacle_mesh_instance_V4[NUMBER_OF_OBSTACLE_MESH_INSTANCE];
	gef::Matrix44 military_obstacle_mesh_instance_M44[NUMBER_OF_OBSTACLE_MESH_INSTANCE];
	gef::Matrix44 samurai_obstacle_mesh_instance_M44[NUMBER_OF_OBSTACLE_MESH_INSTANCE];
	gef::Matrix44 apoc_obstacle_mesh_instance_M44[NUMBER_OF_OBSTACLE_MESH_INSTANCE];





	b2Body* military_obstacle_B2B[NUMBER_OF_OBJECT_B2B_BODIES];
	b2Body* samurai_obstacle_B2B[NUMBER_OF_OBJECT_B2B_BODIES];
	b2Body* apoc_obstacle_B2B[NUMBER_OF_OBJECT_B2B_BODIES];
	b2Vec2 military_obstacle_position_B2vec2[NUMBER_OF_OBJECT_B2B_BODIES];
	b2Vec2 samurai_obstacle_position_B2vec2[NUMBER_OF_OBJECT_B2B_BODIES];
	b2Vec2 apoc_obstacle_position_B2vec2[NUMBER_OF_OBJECT_B2B_BODIES];
	b2BodyDef military_body_def[NUMBER_OF_OBJECT_B2B_BODIES];
	b2BodyDef samurai_body_def[NUMBER_OF_OBJECT_B2B_BODIES];
	b2BodyDef apoc_body_def[NUMBER_OF_OBJECT_B2B_BODIES];
	b2PolygonShape military_shape;
	b2PolygonShape samurai_shape;
	b2PolygonShape apoc_shape;
	b2FixtureDef military_obstacle_fixture_def[NUMBER_OF_OBJECT_B2B_BODIES];
	b2FixtureDef samurai_obstacle_fixture_def[NUMBER_OF_OBJECT_B2B_BODIES];
	b2FixtureDef apoc_obstacle_fixture_def[NUMBER_OF_OBJECT_B2B_BODIES];

	float obstacle_height;
	
	std::vector<int> game_obstacle_distance = { 30,20,15 };
	std::vector<int> game_objects_to_render = { 11, 16, 20 };
	int military_obstacle_distance_counter;
	int samurai_obstacle_distance_counter;
	int apoc_obstacle_distance_counter;


};

#endif // _OBSTACLES_H
