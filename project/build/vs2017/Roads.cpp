																		///////////////////////////////////////////
																		// CODE CREATED BY JAMIE HADDOW 0705082  //
																		//				COMMENTED                //
																		///////////////////////////////////////////

#include "Roads.h"
#include <system/platform.h>
#include <graphics/renderer_3d.h>
#include <graphics/mesh.h>
#include <graphics/scene.h>

//init funcction for spawning the roads
void Roads::InitRoad(gef::Platform& platform)
{	
	military_Road_Distance_Counter = 0;
	samurai_Road_Distance_Counter = 400;
	apoc_Road_Distance_Counter = 800;
	
	const char* samurairoad_asset[5] = {
	"samurai/road/road1.scn",
	"samurai/road/road2.scn",
	"samurai/road/road3.scn",
	"samurai/road/road4.scn",
	"samurai/road/road5.scn"
	};

	const char* miliroad_asset[5] = {
	"military/road/road1.scn",
	"military/road/road2.scn",
	"military/road/road3.scn",
	"military/road/road4.scn",
	"military/road/road5.scn"
	};

	const char* apocroad_asset[5] = {
	"apoc/road/road1.scn",
	"apoc/road/road2.scn",
	"apoc/road/road3.scn",
	"apoc/road/road4.scn",
	"apoc/road/road5.scn"
	};
	
	for (int i = 0; i < 5; i++)
	{
		military_Roads[i] = LoadSceneAssets(platform, miliroad_asset[i]);
		samurai_Roads[i] = LoadSceneAssets(platform, samurairoad_asset[i]);
		apoc_Roads[i] = LoadSceneAssets(platform, apocroad_asset[i]);
	}
	
	for (int i = 0; i < 20; i++)
	{
		//random number for deciding which array model to use
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

//becuase the roads are pointer objects, cleanup is needed. this also means the init function need to be recalled if the user restarts the game
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

//rendering 20 road assets
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
