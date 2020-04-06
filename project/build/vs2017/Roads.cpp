#include "Roads.h"
#include "scene_app.h"
#include <system/platform.h>
#include <graphics/sprite_renderer.h>
#include <graphics/font.h>
#include <system/debug_log.h>
#include <graphics/renderer_3d.h>
#include <graphics/mesh.h>
#include <maths/math_utils.h>
#include <input/sony_controller_input_manager.h>
#include <input/input_manager.h>
#include <input/keyboard.h>
#include <graphics/scene.h>
#include <animation/skeleton.h>
#include <animation/animation.h>
#include <stdlib.h>
#include <time.h>  
void Roads::InitRoad(gef::Platform& platform)
{	
	//20 roads
	military_Road_Distance_Counter = 0;
	//20roads
	samurai_Road_Distance_Counter = 400;
	//20 roads
	apoc_Road_Distance_Counter = 800;
	
	//random number for picking road elements at random out of 5 choices
	
	
	
	const char* samurairoad_asset1 = "samurai/road/road1.scn";
	const char* samurairoad_asset2 = "samurai/road/road2.scn";
	const char* samurairoad_asset3 = "samurai/road/road3.scn";
	const char* samurairoad_asset4 = "samurai/road/road4.scn";
	const char* samurairoad_asset5 = "samurai/road/road5.scn";
	const char* miliroad_asset1 = "military/road/road1.scn";
	const char* miliroad_asset2 = "military/road/road2.scn";
	const char* miliroad_asset3 = "military/road/road3.scn";
	const char* miliroad_asset4 = "military/road/road4.scn";
	const char* miliroad_asset5 = "military/road/road5.scn";
	const char* apocroad_asset1 = "apoc/road/road1.scn";
	const char* apocroad_asset2 = "apoc/road/road2.scn";
	const char* apocroad_asset3 = "apoc/road/road3.scn";
	const char* apocroad_asset4 = "apoc/road/road4.scn";
	const char* apocroad_asset5 = "apoc/road/road5.scn";
	
	//military
	military_Roads[0] = LoadSceneAssets(platform, miliroad_asset1);
	military_Roads[1] = LoadSceneAssets(platform, miliroad_asset2);
	military_Roads[2] = LoadSceneAssets(platform, miliroad_asset3);
	military_Roads[3] = LoadSceneAssets(platform, miliroad_asset4);
	military_Roads[4] = LoadSceneAssets(platform, miliroad_asset5);
	samurai_Roads[0] = LoadSceneAssets(platform, samurairoad_asset1);
	samurai_Roads[1] = LoadSceneAssets(platform, samurairoad_asset2);
	samurai_Roads[2] = LoadSceneAssets(platform, samurairoad_asset3);
	samurai_Roads[3] = LoadSceneAssets(platform, samurairoad_asset4);
	samurai_Roads[4] = LoadSceneAssets(platform, samurairoad_asset5);
	apoc_Roads[0] = LoadSceneAssets(platform, apocroad_asset1);
	apoc_Roads[1] = LoadSceneAssets(platform, apocroad_asset2);
	apoc_Roads[2] = LoadSceneAssets(platform, apocroad_asset3);
	apoc_Roads[3] = LoadSceneAssets(platform, apocroad_asset4);
	apoc_Roads[4] = LoadSceneAssets(platform, apocroad_asset5);
	
	
	
	for (int i = 0; i < 20; i++)
	{
		int road_Random_Number = rand() % 4;
		military_Road_Mesh_Instance_M44[i].SetIdentity();
		samurai_Road_Mesh_Instance_M44[i].SetIdentity();
		apoc_Road_MeshInstance_M44[i].SetIdentity();
	
		military_road_mesh_instance[i].set_mesh(GetMeshFromSceneAssets(military_Roads[road_Random_Number]));
		samurai_road_mesh_instance[i].set_mesh(GetMeshFromSceneAssets(samurai_Roads[road_Random_Number]));
		apoc_road_mesh_instance[i].set_mesh(GetMeshFromSceneAssets(apoc_Roads[road_Random_Number]));
	
		military_Road_Mesh_Instance_V4[i] = gef::Vector4(military_Road_Distance_Counter, 0, 0, 0);
		samurairoadmeshinstanceV4[i] = gef::Vector4(samurai_Road_Distance_Counter, -0.4, 0, 0);
		apocroadmeshinstanceV4[i] = gef::Vector4(apoc_Road_Distance_Counter, 0, 0, 0);
	
		military_Road_Mesh_Instance_M44[i].SetTranslation(military_Road_Mesh_Instance_V4[i]);
		samurai_Road_Mesh_Instance_M44[i].SetTranslation(samurairoadmeshinstanceV4[i]);
		apoc_Road_MeshInstance_M44[i].SetTranslation(apocroadmeshinstanceV4[i]);
	
		military_road_mesh_instance[i].set_transform(military_Road_Mesh_Instance_M44[i]);
		samurai_road_mesh_instance[i].set_transform(samurai_Road_Mesh_Instance_M44[i]);
		apoc_road_mesh_instance[i].set_transform(apoc_Road_MeshInstance_M44[i]);
	
		military_Road_Distance_Counter = military_Road_Distance_Counter + 20;
		samurai_Road_Distance_Counter = samurai_Road_Distance_Counter + 20;
		apoc_Road_Distance_Counter = apoc_Road_Distance_Counter + 20;
	}
}

void Roads::Cleanup()
{
	for (int i = 0; i < 5; i++)
	{
		delete samurai_Roads[i];
		delete military_Roads[i];
		delete apoc_Roads[i];
		samurai_Roads[i] = NULL;
		military_Roads[i] = NULL;
		apoc_Roads[i] = NULL;
	}
}

void Roads::Render(gef::Renderer3D* renderer)
{
	for (int i = 0; i < 20; i++)
	{
		renderer->DrawMesh(military_road_mesh_instance[i]);
		renderer->DrawMesh(samurai_road_mesh_instance[i]);
		renderer->DrawMesh(apoc_road_mesh_instance[i]);
	}

}

gef::Scene* Roads::LoadSceneAssets(gef::Platform& platform, const char* filename)
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
gef::Mesh* Roads::GetMeshFromSceneAssets(gef::Scene* scene)
{
	gef::Mesh* mesh = NULL;

	// if the scene data contains at least one mesh
	// return the first mesh
	if (scene && scene->meshes.size() > 0)
		mesh = scene->meshes.front();

	return mesh;
}
