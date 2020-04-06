#pragma once
#ifndef _ROADS_H
#define _ROADS_H

#include <system/application.h>
#include <graphics/mesh_instance.h>






#define NUMBER_OF_ROAD_MESH_INSTANCE 20

namespace gef
{
	class Platform;
	class mesh;
	class Scene;
	class Renderer3D;
}

class Roads
{
public:
	void InitRoad(gef::Platform& platform);
	void Cleanup();
	void Render(gef::Renderer3D* renderer);

private:

	gef::Scene* LoadSceneAssets(gef::Platform& platform, const char* filename);
	gef::Mesh* GetMeshFromSceneAssets(gef::Scene* scene);

	gef::Scene* samurai_Roads[5];
	gef::Scene* military_Roads[5];
	gef::Scene* apoc_Roads[5];
	
	gef::MeshInstance military_road_mesh_instance[NUMBER_OF_ROAD_MESH_INSTANCE];
	gef::MeshInstance samurai_road_mesh_instance[NUMBER_OF_ROAD_MESH_INSTANCE];
	gef::MeshInstance apoc_road_mesh_instance[NUMBER_OF_ROAD_MESH_INSTANCE];
	gef::Vector4 military_Road_Mesh_Instance_V4[NUMBER_OF_ROAD_MESH_INSTANCE];
	gef::Vector4 samurairoadmeshinstanceV4[NUMBER_OF_ROAD_MESH_INSTANCE];
	gef::Vector4 apocroadmeshinstanceV4[NUMBER_OF_ROAD_MESH_INSTANCE];
	gef::Matrix44 military_Road_Mesh_Instance_M44[NUMBER_OF_ROAD_MESH_INSTANCE];
	gef::Matrix44 samurai_Road_Mesh_Instance_M44[NUMBER_OF_ROAD_MESH_INSTANCE];
	gef::Matrix44 apoc_Road_MeshInstance_M44[NUMBER_OF_ROAD_MESH_INSTANCE];
	
	int military_Road_Distance_Counter;
	int samurai_Road_Distance_Counter;
	int apoc_Road_Distance_Counter;
};

#endif // _SCENE_APP_H
