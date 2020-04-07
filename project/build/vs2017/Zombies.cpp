#include <system/platform.h>
#include <graphics/renderer_3d.h>
#include <graphics/mesh.h>
#include <maths/math_utils.h>
#include <input/keyboard.h>
#include <graphics/scene.h>
#include <animation/skeleton.h>
#include <animation/animation.h>
#include <stdlib.h>
#include <system/debug_log.h>
#include "Zombies.h"

void Zombies::InitZombies(b2World* world, gef::Platform& platform)
{
	zombie_scale_V4 = gef::Vector4{ 0.01f,0.01f,0.01f,0 };

	zombie_scaleM44.Scale(zombie_scale_V4);

	zombie_chrisT_scene_1->ReadSceneFromFile(platform, "zombies/chrisT.scn");
	zombie_chrisT_scene_2->ReadSceneFromFile(platform, "zombies/chrisT2.scn");
	zombie_chrisA_scene_1->ReadSceneFromFile(platform, "zombies/chrisA.scn");
	zombie_chrisA_scene_1->ReadSceneFromFile(platform, "zombies/chrisA2.scn");
	zombie_paul_scene_1->ReadSceneFromFile(platform, "zombies/paul.scn");
	zombie_paul_scene_1->ReadSceneFromFile(platform, "zombies/paul2.scn");

	zombie_chrisT_scene_1->CreateMaterials(platform);
	zombie_chrisT_scene_2->CreateMaterials(platform);
	zombie_chrisA_scene_1->CreateMaterials(platform);
	zombie_chrisA_scene_2->CreateMaterials(platform);
	zombie_paul_scene_1->CreateMaterials(platform);
	zombie_paul_scene_2->CreateMaterials(platform);

	zombie_chrisT_mesh_1 = GetFirstMesh(zombie_chrisT_scene_1, platform);
	zombie_chrisT_mesh_2 = GetFirstMesh(zombie_chrisT_scene_2, platform);
	zombie_chrisA_mesh_1 = GetFirstMesh(zombie_chrisA_scene_1, platform);
	zombie_chrisA_mesh_2 = GetFirstMesh(zombie_chrisA_scene_2, platform);
	zombie_paul_mesh_1 = GetFirstMesh(zombie_paul_scene_1, platform);
	zombie_paul_mesh_2 = GetFirstMesh(zombie_paul_scene_2, platform);

	gef::Skeleton* zombie_chrisT_skeleton_1 = GetFirstSkeleton(zombie_chrisT_scene_1);
	gef::Skeleton* zombie_chrisT_skeleton_2 = GetFirstSkeleton(zombie_chrisT_scene_2);
	gef::Skeleton* zombie_chrisA_skeleton_1 = GetFirstSkeleton(zombie_chrisA_scene_1);
	gef::Skeleton* zombie_chrisA_skeleton_2 = GetFirstSkeleton(zombie_chrisA_scene_2);
	gef::Skeleton* zombie_paul_skeleton_1 = GetFirstSkeleton(zombie_paul_scene_1);
	gef::Skeleton* zombie_paul_skeleton_2 = GetFirstSkeleton(zombie_paul_scene_2);

	if (zombie_chrisT_skeleton_1)
	{
		zombie_chrisT_player_skinned_1 = new gef::SkinnedMeshInstance(*zombie_chrisT_skeleton_1);
		zombie_chrisT_anim_player_1_.Init(zombie_chrisT_player_skinned_1->bind_pose());
		zombie_chrisT_player_skinned_1->set_mesh(zombie_chrisT_mesh_1);
	}
	if (zombie_chrisT_skeleton_2)
	{
		zombie_chrisT_player_skinned_2 = new gef::SkinnedMeshInstance(*zombie_chrisT_skeleton_2);
		zombie_chrisT_anim_player_2_.Init(zombie_chrisT_player_skinned_2->bind_pose());
		zombie_chrisT_player_skinned_2->set_mesh(zombie_chrisT_mesh_2);
	}
	if (zombie_chrisA_skeleton_1)
	{
		zombie_chrisA_player_skinned_1 = new gef::SkinnedMeshInstance(*zombie_chrisA_skeleton_1);
		zombie_chrisA_anim_player_1_.Init(zombie_chrisA_player_skinned_1->bind_pose());
		zombie_chrisA_player_skinned_1->set_mesh(zombie_chrisA_mesh_1);
	}
	if (zombie_chrisA_skeleton_2)
	{
		zombie_chrisA_player_skinned_2 = new gef::SkinnedMeshInstance(*zombie_chrisA_skeleton_2);
		zombie_chrisA_anim_player_2_.Init(zombie_chrisA_player_skinned_2->bind_pose());
		zombie_chrisA_player_skinned_2->set_mesh(zombie_chrisA_mesh_2);
	}	
	if (zombie_paul_skeleton_1)
	{
		zombie_paul_player_skinned_1 = new gef::SkinnedMeshInstance(*zombie_paul_skeleton_1);
		zombie_paul_anim_player_1_.Init(zombie_paul_player_skinned_1->bind_pose());
		zombie_paul_player_skinned_1->set_mesh(zombie_paul_mesh_1);
	}
	if (zombie_paul_skeleton_2)
	{
		zombie_paul_player_skinned_2 = new gef::SkinnedMeshInstance(*zombie_paul_skeleton_2);
		zombie_paul_anim_player_2_.Init(zombie_paul_player_skinned_2->bind_pose());
		zombie_paul_player_skinned_2->set_mesh(zombie_chrisT_mesh_2);
	}

	gef::Matrix44 zombie_chrisT_translate_1 = zombie_scaleM44;
	gef::Matrix44 zombie_chrisT_translate_2 = zombie_scaleM44;
	gef::Matrix44 zombie_chrisA_translate_1 = zombie_scaleM44;
	gef::Matrix44 zombie_chrisA_translate_2 = zombie_scaleM44;
	gef::Matrix44 zombie_paul_translate_1 = zombie_scaleM44;
	gef::Matrix44 zombie_paul_translate_2 = zombie_scaleM44;

	zombie_chrisT_translate_1.SetTranslation(zombie_chrisT_1_V4);
	zombie_chrisT_translate_2.SetTranslation(zombie_chrisT_2_V4);
	zombie_chrisA_translate_1.SetTranslation(zombie_chrisA_1_V4);
	zombie_chrisA_translate_2.SetTranslation(zombie_chrisA_2_V4);
	zombie_paul_translate_1.SetTranslation(zombie_paul_1_V4);
	zombie_paul_translate_2.SetTranslation(zombie_paul_2_V4);

	zombie_chrisT_player_skinned_1->set_transform(zombie_chrisT_translate_1);
	zombie_chrisT_player_skinned_2->set_transform(zombie_chrisT_translate_2);
	zombie_chrisT_player_skinned_1->set_transform(zombie_chrisA_translate_1);
	zombie_chrisT_player_skinned_2->set_transform(zombie_chrisA_translate_2);
	zombie_paul_player_skinned_1->set_transform(zombie_paul_translate_1);
	zombie_paul_player_skinned_2->set_transform(zombie_paul_translate_2);

	chrisT_dance_anim_ = LoadAnimation("player/chrisT.scn", "", platform);
	chrisA_dance_anim_ = LoadAnimation("player/chrisA.scn", "", platform);
	paul_dance_anim_ = LoadAnimation("player/paul.scn", "", platform);

	if (chrisT_dance_anim_)
	{
		zombie_chrisT_anim_player_1_.set_clip(chrisT_dance_anim_);
		zombie_chrisT_anim_player_2_.set_clip(chrisT_dance_anim_);
		zombie_chrisA_anim_player_1_.set_clip(chrisA_dance_anim_);
		zombie_chrisA_anim_player_2_.set_clip(chrisA_dance_anim_);
		zombie_paul_anim_player_1_.set_clip(paul_dance_anim_);
		zombie_paul_anim_player_2_.set_clip(paul_dance_anim_);

		zombie_chrisT_anim_player_1_.set_looping(true);
		zombie_chrisT_anim_player_2_.set_looping(true);
		zombie_chrisA_anim_player_1_.set_looping(true);
		zombie_chrisA_anim_player_2_.set_looping(true);
		zombie_paul_anim_player_1_.set_looping(true);
		zombie_paul_anim_player_2_.set_looping(true);

		zombie_chrisT_anim_player_1_.set_anim_time(0.0f);
		zombie_chrisT_anim_player_2_.set_anim_time(0.0f);
		zombie_chrisA_anim_player_1_.set_anim_time(0.0f);
		zombie_chrisA_anim_player_2_.set_anim_time(0.0f);
		zombie_paul_anim_player_1_.set_anim_time(0.0f);
		zombie_paul_anim_player_2_.set_anim_time(0.0f);
	}
}

void Zombies::Render(gef::Renderer3D* renderer)
{
	
	renderer->DrawSkinnedMesh(*zombie_chrisT_player_skinned_1, zombie_chrisT_player_skinned_1->bone_matrices());
	renderer->DrawSkinnedMesh(*zombie_chrisT_player_skinned_2, zombie_chrisT_player_skinned_2->bone_matrices());
	renderer->DrawSkinnedMesh(*zombie_chrisA_player_skinned_1, zombie_chrisA_player_skinned_1->bone_matrices());
	renderer->DrawSkinnedMesh(*zombie_chrisA_player_skinned_2, zombie_chrisA_player_skinned_2->bone_matrices());
	renderer->DrawSkinnedMesh(*zombie_paul_player_skinned_1, zombie_paul_player_skinned_1->bone_matrices());
	renderer->DrawSkinnedMesh(*zombie_paul_player_skinned_2, zombie_paul_player_skinned_2->bone_matrices());
}

void Zombies::update(float frame_time, int* score, gef::InputManager* inputmanager)
{

}

gef::Skeleton* Zombies::GetFirstSkeleton(gef::Scene* scene)
{
	gef::Skeleton* skeleton = NULL;
	if (scene)
	{
		// check to see if there is a skeleton in the the scene file
		// if so, pull out the bind pose and create an array of matrices
		// that wil be used to store the bone transformations
		if (scene->skeletons.size() > 0)
			skeleton = scene->skeletons.front();
	}

	return skeleton;
}

gef::Mesh* Zombies::GetFirstMesh(gef::Scene* scene, gef::Platform& platform)
{
	gef::Mesh* mesh = NULL;

	if (scene)
	{
		// now check to see if there is any mesh data in the file, if so lets create a mesh from it
		if (scene->mesh_data.size() > 0)
			mesh = scene->CreateMesh(platform, scene->mesh_data.front());
	}

	return mesh;
}

gef::Animation* Zombies::LoadAnimation(const char* anim_scene_filename, const char* anim_name, gef::Platform& platform)
{
	gef::Animation* anim = NULL;

	gef::Scene anim_scene;
	if (anim_scene.ReadSceneFromFile(platform, anim_scene_filename))
	{
		// if the animation name is specified then try and find the named anim
		// otherwise return the first animation if there is one
		std::map<gef::StringId, gef::Animation*>::const_iterator anim_node_iter;
		if (anim_name)
			anim_node_iter = anim_scene.animations.find(gef::GetStringId(anim_name));
		else
			anim_node_iter = anim_scene.animations.begin();

		if (anim_node_iter != anim_scene.animations.end())
			anim = new gef::Animation(*anim_node_iter->second);
	}

	return anim;
}

gef::Scene* Zombies::LoadSceneAssets(gef::Platform& platform, const char* filename)
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

gef::Mesh* Zombies::GetMeshFromSceneAssets(gef::Scene* scene)
{
	gef::Mesh* mesh = NULL;

	// if the scene data contains at least one mesh
	// return the first mesh
	if (scene && scene->meshes.size() > 0)
		mesh = scene->meshes.front();

	return mesh;
}
