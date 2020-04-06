#include <system/platform.h>
#include <graphics/renderer_3d.h>
#include <graphics/mesh.h>
#include <graphics/scene.h>
#include "Buildings.h"


void Buildings::Render(gef::Renderer3D* renderer)
{
	for (int i = 0; i < 12; i++)
	{
		renderer->DrawMesh(military_building_mesh_instance[i]);
		renderer->DrawMesh(samurai_building_mesh_instance[i]);
		renderer->DrawMesh(apoc_building_mesh_instance[i]);
	}

}

void Buildings::InitBuildings(gef::Platform& platform)
{

	//11 buildings
	military_Building_Distance_Counter = 20;
	//11 buildings
	samurai_Building_Distance_Counter = 420;
	//11 buildings
	apoc_Building_Distance_Counter = 820;
	const char* samuraibuilding_asset1 = "samurai/building/building1.scn";
	const char* samuraibuilding_asset2 = "samurai/building/building2.scn";
	const char* samuraibuilding_asset3 = "samurai/building/building3.scn";
	const char* samuraibuilding_asset4 = "samurai/building/building4.scn";
	const char* samuraibuilding_asset5 = "samurai/building/building5.scn";
	const char* milibuilding_asset1 = "military/building/building1.scn";
	const char* milibuilding_asset2 = "military/building/building2.scn";
	const char* milibuilding_asset3 = "military/building/building3.scn";
	const char* milibuilding_asset4 = "military/building/building4.scn";
	const char* milibuilding_asset5 = "military/building/building5.scn";
	const char* apocbuilding_asset1 = "apoc/building/building1.scn";
	const char* apocbuilding_asset2 = "apoc/building/building2.scn";
	const char* apocbuilding_asset3 = "apoc/building/building3.scn";
	const char* apocbuilding_asset4 = "apoc/building/building4.scn";
	const char* apocbuilding_asset5 = "apoc/building/building5.scn";

	military_Buildings[0] = LoadSceneAssets(platform, milibuilding_asset1);
	military_Buildings[1] = LoadSceneAssets(platform, milibuilding_asset2);
	military_Buildings[2] = LoadSceneAssets(platform, milibuilding_asset3);
	military_Buildings[3] = LoadSceneAssets(platform, milibuilding_asset4);
	military_Buildings[4] = LoadSceneAssets(platform, milibuilding_asset5);
	samurai_Buildings[0] = LoadSceneAssets(platform, samuraibuilding_asset1);
	samurai_Buildings[1] = LoadSceneAssets(platform, samuraibuilding_asset2);
	samurai_Buildings[2] = LoadSceneAssets(platform, samuraibuilding_asset3);
	samurai_Buildings[3] = LoadSceneAssets(platform, samuraibuilding_asset4);
	samurai_Buildings[4] = LoadSceneAssets(platform, samuraibuilding_asset5);
	apoc_Buildings[0] = LoadSceneAssets(platform, apocbuilding_asset1);
	apoc_Buildings[1] = LoadSceneAssets(platform, apocbuilding_asset2);
	apoc_Buildings[2] = LoadSceneAssets(platform, apocbuilding_asset3);
	apoc_Buildings[3] = LoadSceneAssets(platform, apocbuilding_asset4);
	apoc_Buildings[4] = LoadSceneAssets(platform, apocbuilding_asset5);

	for (int i = 0; i < 12; i++)
	{
		int building_Random_Number = rand() % 4;
		int building_Random_Disantce_Number = rand() % 10 + 30;

		military_building_mesh_instance[i].set_mesh(GetMeshFromSceneAssets(military_Buildings[building_Random_Number]));
		samurai_building_mesh_instance[i].set_mesh(GetMeshFromSceneAssets(samurai_Buildings[building_Random_Number]));
		apoc_building_mesh_instance[i].set_mesh(GetMeshFromSceneAssets(apoc_Buildings[building_Random_Number]));

		military_Buildings_Mesh_Instance_M44[i].SetIdentity();
		samurai_Buildings_Mesh_Instance_M44[i].SetIdentity();
		apoc_Buildings_Mesh_Instance_M44[i].SetIdentity();

		military_Buildings_Mesh_Instance_V4[i] = gef::Vector4(military_Building_Distance_Counter - 10, 0, 0, 0);
		samurai_Buildings_Mesh_Instance_V4[i] = gef::Vector4(samurai_Building_Distance_Counter, 0, 0, 0);
		apoc_Buildings_Mesh_Instance_V4[i] = gef::Vector4(apoc_Building_Distance_Counter, 0, 0, 0);

		military_Buildings_Mesh_Instance_M44[i].SetTranslation(military_Buildings_Mesh_Instance_V4[i]);
		samurai_Buildings_Mesh_Instance_M44[i].SetTranslation(samurai_Buildings_Mesh_Instance_V4[i]);
		apoc_Buildings_Mesh_Instance_M44[i].SetTranslation(apoc_Buildings_Mesh_Instance_V4[i]);

		military_building_mesh_instance[i].set_transform(military_Buildings_Mesh_Instance_M44[i]);
		samurai_building_mesh_instance[i].set_transform(samurai_Buildings_Mesh_Instance_M44[i]);
		apoc_building_mesh_instance[i].set_transform(apoc_Buildings_Mesh_Instance_M44[i]);

		military_Building_Distance_Counter = military_Building_Distance_Counter + building_Random_Disantce_Number;
		samurai_Building_Distance_Counter = samurai_Building_Distance_Counter + building_Random_Disantce_Number;
		apoc_Building_Distance_Counter = apoc_Building_Distance_Counter + building_Random_Disantce_Number;
	}
}

gef::Scene* Buildings::LoadSceneAssets(gef::Platform& platform, const char* filename)
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


gef::Mesh* Buildings::GetMeshFromSceneAssets(gef::Scene* scene)
{
	gef::Mesh* mesh = NULL;

	// if the scene data contains at least one mesh
	// return the first mesh
	if (scene && scene->meshes.size() > 0)
		mesh = scene->meshes.front();

	return mesh;
}

