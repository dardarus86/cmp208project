																		///////////////////////////////////////////
																		// CODE CREATED BY JAMIE HADDOW 0705082  //
																		//				COMMENTED                //
																		///////////////////////////////////////////

#ifndef _OBSTACLES_H
#define _OBSTACLES_H

#include <system/application.h>
#include <maths/vector2.h>
#include <graphics/mesh_instance.h>
#include <box2d/Box2D.h>
#include "Obstacles.h"
#include "Collision.h"


#define NUMBER_OF_OBSTACLE_MESH_INSTANCE 25
#define NUMBER_OF_OBJECT_B2B_BODIES 25
// FRAMEWORK FORWARD DECLARATIONS
namespace gef
{
	class Platform;
	class Renderer3D;
	class Scene;

}

class Obstacles : public collision
{
public:

	//  passing in renderer3D and the difficulty state as an int then static casting in sceneapp
	void Render(gef::Renderer3D *renderer, int difficultysstate);
	//creating the obstacles. gamestate by default is medium, however that can be changed in the options and init needs to be called once its been changed to update
	void InitObstacles(gef::Platform& platform, b2World* world, int difficultysstate);

private:


	//provided for CMP208
	gef::Scene* LoadSceneAssets(gef::Platform& platform, const char* filename);
	gef::Mesh* GetMeshFromSceneAssets(gef::Scene* scene);

	gef::Renderer3D* renderer_3d_;
	   
	//array of scene objects to hold the different obstacles that were created to give a more interesting graphical experience
	gef::Scene* samurai_obstacle[4];
	gef::Scene* military_obstacle[4];
	gef::Scene* apoc_obstacle[4];


	//mesh instance for the number of obstacles per genre. vectors and matrices used for translating the obsstacles
	gef::MeshInstance  military_obstacle_mesh_instance[NUMBER_OF_OBSTACLE_MESH_INSTANCE];
	gef::MeshInstance  samurai_obstacle_mesh_instance[NUMBER_OF_OBSTACLE_MESH_INSTANCE];
	gef::MeshInstance  apoc_obstacle_mesh_instance[NUMBER_OF_OBSTACLE_MESH_INSTANCE];
	gef::Vector4 military_obstacle_mesh_instance_V4[NUMBER_OF_OBSTACLE_MESH_INSTANCE];
	gef::Vector4 samurai_obstacle_mesh_instance_V4[NUMBER_OF_OBSTACLE_MESH_INSTANCE];
	gef::Vector4 apoc_obstacle_mesh_instance_V4[NUMBER_OF_OBSTACLE_MESH_INSTANCE];
	gef::Matrix44 military_obstacle_mesh_instance_M44[NUMBER_OF_OBSTACLE_MESH_INSTANCE];
	gef::Matrix44 samurai_obstacle_mesh_instance_M44[NUMBER_OF_OBSTACLE_MESH_INSTANCE];
	gef::Matrix44 apoc_obstacle_mesh_instance_M44[NUMBER_OF_OBSTACLE_MESH_INSTANCE];




	//B2Body below

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

	//float used to give the obstacles some height. this will be used in a rand if statement in obstacles.cpp to decide if an obstacle is 0 or 2.5 on Y
	float obstacle_height;
	
	//two vectors to decide the number of obstacles to render and also to decide the distance between each obstacle. 
	//easier is less obstacles and more distance, hard less distance nad more obstackes
	std::vector<int> game_obstacle_distance = { 30,20,15 };
	std::vector<int> game_objects_to_render = { 11, 16, 20 };

	//distance counter to calculate a base distance between each obstacle object
	int military_obstacle_distance_counter;
	int samurai_obstacle_distance_counter;
	int apoc_obstacle_distance_counter;


};

#endif // _OBSTACLES_H
