#pragma once
#ifndef _BUILDINGS_H
#define _BUILDINGS_H

#include <system/application.h>
#include <graphics/mesh_instance.h>


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
	void Render(gef::Renderer3D* renderer);
	void InitBuildings(gef::Platform& platform);
private:
	
	gef::Scene* LoadSceneAssets(gef::Platform& platform, const char* filename);
	gef::Mesh* GetMeshFromSceneAssets(gef::Scene* scene);

	gef::Scene* samurai_Buildings[5];
	gef::Scene* military_Buildings[5];
	gef::Scene* apoc_Buildings[5];

	gef::MeshInstance  military_building_mesh_instance[NUMBER_OF_BUILDING_MESH_INSTANCE];
	gef::MeshInstance  samurai_building_mesh_instance[NUMBER_OF_BUILDING_MESH_INSTANCE];
	gef::MeshInstance  apoc_building_mesh_instance[NUMBER_OF_BUILDING_MESH_INSTANCE];
	gef::Vector4 military_Buildings_Mesh_Instance_V4[NUMBER_OF_BUILDING_MESH_INSTANCE];
	gef::Vector4 samurai_Buildings_Mesh_Instance_V4[NUMBER_OF_BUILDING_MESH_INSTANCE];
	gef::Vector4 apoc_Buildings_Mesh_Instance_V4[NUMBER_OF_BUILDING_MESH_INSTANCE];
	gef::Matrix44 military_Buildings_Mesh_Instance_M44[NUMBER_OF_BUILDING_MESH_INSTANCE];
	gef::Matrix44 samurai_Buildings_Mesh_Instance_M44[NUMBER_OF_BUILDING_MESH_INSTANCE];
	gef::Matrix44 apoc_Buildings_Mesh_Instance_M44[NUMBER_OF_BUILDING_MESH_INSTANCE];

	int military_Building_Distance_Counter;
	int samurai_Building_Distance_Counter;
	int apoc_Building_Distance_Counter;


};

#endif // _BUILDINGS_H
