																		///////////////////////////////////////////
																		// CODE CREATED BY JAMIE HADDOW 0705082  //
																		//				COMMENTED                //
																		///////////////////////////////////////////

#ifndef _BUILDINGS_H
#define _BUILDINGS_H

#include <system/application.h>
#include <graphics/mesh_instance.h>

//simple define for the number of mesh instances
#define NUMBER_OF_BUILDING_MESH_INSTANCE 12

// FRAMEWORK FORWARD DECLARATIONS
namespace gef
{
	class Platform;
	class Renderer3D;
	class mesh;
	class Scene;
}

class Buildings
{
public:
	// only need a render and init functions for the buildings as they do not require to be updated
	// passing in renderer3D and platform so ican call the function in SceneApp
	void Render(gef::Renderer3D* renderer);
	void InitBuildings(gef::Platform& platform);
private:
	
	// two functions provided for CMP208
	gef::Scene* LoadSceneAssets(gef::Platform& platform, const char* filename);
	gef::Mesh* GetMeshFromSceneAssets(gef::Scene* scene);

	//array of 5 for each genres buildings as there are 5 buildings for each area. Military,samurai,apoc. These arrays are used in a rand() to choose which element to use
	gef::Scene* samurai_Buildings[5];
	gef::Scene* military_Buildings[5];
	gef::Scene* apoc_Buildings[5];

	//mesh instance for the buildings. Vectors and matrixs used to translate the buildings if required
	gef::MeshInstance  military_building_mesh_instance[NUMBER_OF_BUILDING_MESH_INSTANCE];
	gef::MeshInstance  samurai_building_mesh_instance[NUMBER_OF_BUILDING_MESH_INSTANCE];
	gef::MeshInstance  apoc_building_mesh_instance[NUMBER_OF_BUILDING_MESH_INSTANCE];
	gef::Vector4 military_Buildings_Mesh_Instance_V4[NUMBER_OF_BUILDING_MESH_INSTANCE];
	gef::Vector4 samurai_Buildings_Mesh_Instance_V4[NUMBER_OF_BUILDING_MESH_INSTANCE];
	gef::Vector4 apoc_Buildings_Mesh_Instance_V4[NUMBER_OF_BUILDING_MESH_INSTANCE];
	gef::Matrix44 military_Buildings_Mesh_Instance_M44[NUMBER_OF_BUILDING_MESH_INSTANCE];
	gef::Matrix44 samurai_Buildings_Mesh_Instance_M44[NUMBER_OF_BUILDING_MESH_INSTANCE];
	gef::Matrix44 apoc_Buildings_Mesh_Instance_M44[NUMBER_OF_BUILDING_MESH_INSTANCE];


	//building counter used for calculating the distance between building objects
	int military_Building_Distance_Counter;
	int samurai_Building_Distance_Counter;
	int apoc_Building_Distance_Counter;


};

#endif // _BUILDINGS_H
