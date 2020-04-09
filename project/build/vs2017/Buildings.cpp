																		///////////////////////////////////////////
																		// CODE CREATED BY JAMIE HADDOW 0705082  //
																		//				COMMENTED                //
																		///////////////////////////////////////////


#include <system/platform.h>
#include <graphics/renderer_3d.h>
#include <graphics/mesh.h>
#include <graphics/scene.h>
#include "Buildings.h"


void Buildings::InitBuildings(gef::Platform& platform)
{
	//the buildings are slightly offset from t0 so the first building is not on the edge of the area type
	military_Building_Distance_Counter = 20;
	samurai_Building_Distance_Counter = 420;
	apoc_Building_Distance_Counter = 820;

	//taking the building scns created using the model loader provided and manually edited in maya for positions
	const char* samuraibuilding_asset[5] = {
	"samurai/building/building1.scn",
	"samurai/building/building2.scn",
	"samurai/building/building3.scn",
	"samurai/building/building4.scn",
	"samurai/building/building5.scn"
	};

	const char* milibuilding_asset[5] = {
	"military/building/building1.scn",
	"military/building/building2.scn",
	"military/building/building3.scn",
	"military/building/building4.scn",
	"military/building/building5.scn"
	};

	const char* apocbuilding_asset[5] = {
	"apoc/building/building1.scn",
	"apoc/building/building2.scn",
	"apoc/building/building3.scn",
	"apoc/building/building4.scn",
	"apoc/building/building5.scn"
	};

	for (int i = 0; i < 5; i++)
	{
		military_Buildings[i] = LoadSceneAssets(platform, milibuilding_asset[i]);
		samurai_Buildings[i] = LoadSceneAssets(platform, samuraibuilding_asset[i]);
		apoc_Buildings[i] = LoadSceneAssets(platform, apocbuilding_asset[i]);
	}

	//going through the building instances, setting the meshs and positions using vectors and matrixes
	for (int i = 0; i < 12; i++)
	{
		//two random numbers used in the code below. Random number decides which building model to use,  distance number decides the spacing of the buildings
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

		//for a bit of randomness, the distance between each building is calculated using the random number created above
		military_Building_Distance_Counter = military_Building_Distance_Counter + building_Random_Disantce_Number;
		samurai_Building_Distance_Counter = samurai_Building_Distance_Counter + building_Random_Disantce_Number;
		apoc_Building_Distance_Counter = apoc_Building_Distance_Counter + building_Random_Disantce_Number;
	}
}

//render function that iterates through the building mesh instances
void Buildings::Render(gef::Renderer3D* renderer)
{
	for (int i = 0; i < 12; i++)
	{
		renderer->DrawMesh(military_building_mesh_instance[i]);
		renderer->DrawMesh(samurai_building_mesh_instance[i]);
		renderer->DrawMesh(apoc_building_mesh_instance[i]);
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

