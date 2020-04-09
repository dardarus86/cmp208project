																		///////////////////////////////////////////
																		// CODE CREATED BY JAMIE HADDOW 0705082  //
																		//				COMMENTED                //
																		///////////////////////////////////////////

#ifndef _ZOMBIES_H
#define _ZOMBIES_H

#include <system/application.h>
#include <graphics/mesh_instance.h>
#include "game_object.h"
#include <graphics/skinned_mesh_instance.h>
#include "motion_clip_player.h"

// FRAMEWORK FORWARD DECLARATIONS
namespace gef
{
	class Platform;
	class Renderer3D;
	class mesh;
	class Scene;
	class Skeleton;
	class Animation;
}

class Zombies
{
public:

	//initializing the paul zombies for their epic danceoff
	void InitZombies(gef::Platform& platform);
	//passing in the renderer3d to be used in sceneapp
	void Render(gef::Renderer3D* renderer);
	//update needed as it is needed for animations
	void update(float frame_time);

private:
	//functions provided by framework
	gef::Skeleton* GetFirstSkeleton(gef::Scene* scene);
	gef::Mesh* GetFirstMesh(gef::Scene* scene, gef::Platform& platform);
	gef::Animation* LoadAnimation(const char* anim_scene_ilename, const char* anim_name, gef::Platform& platform);
	gef::Scene* LoadSceneAssets(gef::Platform& platform, const char* filename);
	gef::Mesh* GetMeshFromSceneAssets(gef::Scene* scene);

	//mesh objects for all the models. I left it named to see my mindset with 3 models originally planned, but unfortunatly only the paul model was textured here
	gef::Mesh* zombie_chrisT_mesh_1;
	gef::Mesh* zombie_chrisT_mesh_2;
	gef::Mesh* zombie_chrisA_mesh_1;
	gef::Mesh* zombie_chrisA_mesh_2;
	gef::Mesh* zombie_paul_mesh_1;
	gef::Mesh* zombie_paul_mesh_2;


	gef::Scene* zombie_chrisT_scene_1;
	gef::Scene* zombie_chrisT_scene_2;
	gef::Scene* zombie_chrisA_scene_1;
	gef::Scene* zombie_chrisA_scene_2;
	gef::Scene* zombie_paul_scene_1;
	gef::Scene* zombie_paul_scene_2;

	//vectors and matrixs here to translate the models
	gef::Vector4 zombie_chrisT_1_V4 = {-1,0,-2,0};
	gef::Vector4 zombie_chrisT_2_V4 = { 1,0,-2,0 };
	gef::Vector4 zombie_chrisA_1_V4 = { 3,0,0,0 };
	gef::Vector4 zombie_chrisA_2_V4 = { -3,0,0,0 };
	gef::Vector4 zombie_paul_1_V4 = { 5,0,2,0 };
	gef::Vector4 zombie_paul_2_V4 = {-5,0,2,0 };
	gef::Vector4 zombie_scale_V4;
	gef::Matrix44 zombie_chrisT_translate_1;
	gef::Matrix44 zombie_chrisT_translate_2;
	gef::Matrix44 zombie_chrisA_translate_1;
	gef::Matrix44 zombie_chrisA_translate_2;
	gef::Matrix44 zombie_paul_translate_1;
	gef::Matrix44 zombie_paul_translate_2;

	//this matrix was used to set the scale oherwise if i did scale*translate my -1 would turn into 0.009 so i made 2 seperate matrixes
	gef::Matrix44 zombie_scaleM44;
	gef::Matrix44 zombie_rotateM44;
	gef::Matrix44 zombie_chrisT_1_M44;
	gef::Matrix44 zombie_chrisT_2_M44;
	gef::Matrix44 zombie_chrisA_1_M44;
	gef::Matrix44 zombie_chrisA_2_M44;
	gef::Matrix44 zombie_paul_1_M44;
	gef::Matrix44 zombie_paul_2_M44;

	//animation
	MotionClipPlayer zombie_chrisT_anim_player_1_;
	MotionClipPlayer zombie_chrisT_anim_player_2_;
	MotionClipPlayer zombie_chrisA_anim_player_1_;
	MotionClipPlayer zombie_chrisA_anim_player_2_;
	MotionClipPlayer zombie_paul_anim_player_1_;
	MotionClipPlayer zombie_paul_anim_player_2_;
	gef::Animation* chrisT_dance_anim_;
	gef::Animation* chrisA_dance_anim_;
	gef::Animation* paul_dance_anim_;
	gef::SkinnedMeshInstance* zombie_chrisT_player_skinned_1;
	gef::SkinnedMeshInstance* zombie_chrisT_player_skinned_2;
	gef::SkinnedMeshInstance* zombie_chrisA_player_skinned_1;
	gef::SkinnedMeshInstance* zombie_chrisA_player_skinned_2;
	gef::SkinnedMeshInstance* zombie_paul_player_skinned_1;
	gef::SkinnedMeshInstance* zombie_paul_player_skinned_2;

};

#endif // _PLAYER_H
