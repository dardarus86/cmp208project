																		///////////////////////////////////////////
																		// CODE CREATED BY JAMIE HADDOW 0705082  //
																		//				COMMENTED                //
																		///////////////////////////////////////////

#include <system/platform.h>
#include <graphics/renderer_3d.h>
#include <graphics/scene.h>
#include <stdlib.h>
#include <time.h> 
#include "Obstacles.h"

void Obstacles::InitObstacles(gef::Platform& platform, b2World* world, int difficultysstate)
{
	// the first obstacle starts at 30 but then for samurai and apoc starts at he beginning of the area
	military_obstacle_distance_counter = 30;
	samurai_obstacle_distance_counter = 400;
	apoc_obstacle_distance_counter = 800;

	//taking the obstacle scns created using the model loader provided and manually edited in maya for positions
	const char* samurai_obstacle_asset[4] = {
	"samurai/obstacles/small.scn",
	"samurai/obstacles/large.scn",
	"samurai/obstacles/gong.scn",
	"samurai/obstacles/lion.scn"
	};

	const char* military_obstacle_asset[4] = {
	"military/obstacles/boxes.scn",
	"military/obstacles/apc.scn",
	"military/obstacles/medical.scn",
	"military/obstacles/rockets.scn"
	};

	const char* apoc_obstacle_asset[4] =
	{
	"apoc/obstacles/car.scn",
	"apoc/obstacles/truck.scn",
	"apoc/obstacles/bodies.scn",
	"apoc/obstacles/billboard.scn"
	};

	for (int i = 0; i < 4; i++)
	{

		military_obstacle[i] = LoadSceneAssets(platform, military_obstacle_asset[i]);
		samurai_obstacle[i] = LoadSceneAssets(platform, samurai_obstacle_asset[i]);
		apoc_obstacle[i] = LoadSceneAssets(platform, apoc_obstacle_asset[i]);
	}

	
	for (int i = 0, total_distance = 0; ; i++)
	{
		//two random numbers used in the code below. Random number decides which obstacle model to use,  distance number decides the spacing of the obstacles
		int obstacle_Random_Number = rand() % 4;
		int obstacle_Random_Distance = rand() % 10 + game_obstacle_distance.at(difficultysstate);
		total_distance += obstacle_Random_Distance;
		if (total_distance > 480)
			break;
		int obstacle_Up_Or_Down = rand() % 100;
		if (obstacle_Up_Or_Down > 50)
		{
			obstacle_height = 2.5;
		}
		else
		{
			obstacle_height = 0;
		}
		//setting the meshs,vectors/matrixes for transforma nd the B2Body parameters
		apoc_obstacle_mesh_instance[i].set_mesh(GetMeshFromSceneAssets(apoc_obstacle[obstacle_Random_Number]));
		apoc_obstacle_mesh_instance_M44[i].SetIdentity();
		apoc_obstacle_mesh_instance_V4[i] = gef::Vector4(apoc_obstacle_distance_counter, obstacle_height, 0, 0);
		apoc_obstacle_mesh_instance_M44[i].SetTranslation(apoc_obstacle_mesh_instance_V4[i]);
		apoc_obstacle_mesh_instance[i].set_transform(apoc_obstacle_mesh_instance_M44[i]);
		apoc_obstacle_position_B2vec2[i] = b2Vec2(apoc_obstacle_distance_counter, obstacle_height + 1);
		apoc_body_def[i].type = b2_staticBody;
		apoc_body_def[i].position = apoc_obstacle_position_B2vec2[i];
		apoc_obstacle_B2B[i] = world->CreateBody(&apoc_body_def[i]);
		apoc_shape.SetAsBox(1.0, 3.4);
		apoc_obstacle_fixture_def[i].shape = &apoc_shape;
		apoc_obstacle_B2B[i]->CreateFixture(&apoc_obstacle_fixture_def[i]);
		apoc_obstacle_distance_counter = apoc_obstacle_distance_counter + obstacle_Random_Distance;

	}
	// same as previous for loop
	for (int i = 0, total_distance = 0; ; i++)
	{
		int obstacle_Random_Number = rand() % 4;
		int obstacle_Random_Distance = rand() % 10 + game_obstacle_distance.at(difficultysstate);
		total_distance += obstacle_Random_Distance;
		if (total_distance > 380)
			break;
		int obstacle_Up_Or_Down = rand() % 100;
		if (obstacle_Up_Or_Down > 50)
		{
			obstacle_height = 2.5;
		}
		else
		{
			obstacle_height = 0;
		}
		military_obstacle_mesh_instance[i].set_mesh(GetMeshFromSceneAssets(military_obstacle[obstacle_Random_Number]));
		military_obstacle_mesh_instance_M44[i].SetIdentity();

		military_obstacle_mesh_instance_V4[i] = gef::Vector4(military_obstacle_distance_counter, obstacle_height, 0, 0);

		military_obstacle_mesh_instance_M44[i].SetTranslation(military_obstacle_mesh_instance_V4[i]);

		military_obstacle_mesh_instance[i].set_transform(military_obstacle_mesh_instance_M44[i]);

		military_obstacle_position_B2vec2[i] = b2Vec2(military_obstacle_distance_counter, obstacle_height + 1);
		military_body_def[i].type = b2_staticBody;

		military_body_def[i].position = military_obstacle_position_B2vec2[i];
		military_obstacle_B2B[i] = world->CreateBody(&military_body_def[i]);
		military_shape.SetAsBox(1.0, 3.25);
		military_obstacle_fixture_def[i].shape = &military_shape;
		military_obstacle_B2B[i]->CreateFixture(&military_obstacle_fixture_def[i]);


		military_obstacle_distance_counter = military_obstacle_distance_counter + obstacle_Random_Distance;


	}
	// same as previous for loop
	for (int i = 0, total_distance = 0; ; i++)
	{
		int obstacle_Random_Number = rand() % 4;
		int obstacle_Random_Distance = rand() % 10 + game_obstacle_distance.at(difficultysstate);
		total_distance += obstacle_Random_Distance;
		if (total_distance > 380)
			break;
		int obstacle_Up_Or_Down = rand() % 100;
		if (obstacle_Up_Or_Down > 50)
		{
			obstacle_height = 2.5;
		}
		else
		{
			obstacle_height = 0;
		}
		samurai_obstacle_mesh_instance[i].set_mesh(GetMeshFromSceneAssets(samurai_obstacle[obstacle_Random_Number]));
		samurai_obstacle_mesh_instance_M44[i].SetIdentity();
		samurai_obstacle_mesh_instance_V4[i] = gef::Vector4(samurai_obstacle_distance_counter, obstacle_height, 0, 0);
		samurai_obstacle_mesh_instance_M44[i].SetTranslation(samurai_obstacle_mesh_instance_V4[i]);
		samurai_obstacle_mesh_instance[i].set_transform(samurai_obstacle_mesh_instance_M44[i]);
		samurai_obstacle_position_B2vec2[i] = b2Vec2(samurai_obstacle_distance_counter, obstacle_height + 1);
		samurai_body_def[i].type = b2_staticBody;
		samurai_body_def[i].position = samurai_obstacle_position_B2vec2[i];
		samurai_obstacle_B2B[i] = world->CreateBody(&samurai_body_def[i]);
		samurai_shape.SetAsBox(1.0, 3.9);
		samurai_obstacle_fixture_def[i].shape = &military_shape;
		samurai_obstacle_B2B[i]->CreateFixture(&samurai_obstacle_fixture_def[i]);
		samurai_obstacle_distance_counter = samurai_obstacle_distance_counter + obstacle_Random_Distance;

	}
}

//render function that iterates through the obstacle mesh instances
void Obstacles::Render(gef::Renderer3D* renderer, int difficultysstate)
{
	for (int i = 0; i < game_objects_to_render.at(difficultysstate); i++)
	{
		renderer->DrawMesh(military_obstacle_mesh_instance[i]);
		renderer->DrawMesh(samurai_obstacle_mesh_instance[i]);
		renderer->DrawMesh(apoc_obstacle_mesh_instance[i]);
	}
}

gef::Scene* Obstacles::LoadSceneAssets(gef::Platform& platform, const char* filename)
{
	gef::Scene* scene = new gef::Scene();

	if (scene->ReadSceneFromFile(platform, filename))
	{
		// if scene file loads successful
		// create material and mesh resources from the scene data
		scene->CreateMaterials(platform);
		scene->CreateMeshes(platform);
	}
	else
	{
		delete scene;
		scene = NULL;
	}

	return scene;
}

gef::Mesh* Obstacles::GetMeshFromSceneAssets(gef::Scene* scene)
{
	gef::Mesh* mesh = NULL;

	// if the scene data contains at least one mesh
	// return the first mesh
	if (scene && scene->meshes.size() > 0)
		mesh = scene->meshes.front();

	return mesh;
}
