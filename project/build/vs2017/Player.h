																		///////////////////////////////////////////
																		// CODE CREATED BY JAMIE HADDOW 0705082  //
																		//				COMMENTED                //
																		///////////////////////////////////////////

#ifndef _PLAYER_H
#define _PLAYER_H

#include <system/application.h>
#include <maths/vector2.h>
#include <graphics/mesh_instance.h>
#include <input/input_manager.h>
#include <box2d/Box2D.h>
#include <graphics\skinned_mesh_instance.h>
#include "motion_clip_player.h"


// FRAMEWORK FORWARD DECLARATIONS
namespace gef
{
	class Platform;
	class InputManager;
	class Renderer3D;
	class mesh;
	class Scene;
	class Skeleton;
	class Animation;
}

class Player
{
public:
	//initializing the player and B2Body elements
	void InitPlayer(b2World* world, gef::Platform& platform);
	//rendering the skinnedmesh for the player
	void Render(gef::Renderer3D* renderer);
	//main update for the player dealing with input, scores,a counter. i left the audio to be dealt with in scene.app so created some getting functions below
	void update(float frame_time, int* score, gef::InputManager* inputmanager);
	//useding a bool getting to check the state and ending the game
	bool GetGoalFinished();
	//uesd to setting it true or false and ending the game
	void SetGoalFinished(bool goalfinish);
	//simple function for reduction score
	void ReduceScore(int score);
	// three getters used in sceneapp
	float GetPlayerBodyX();
	float GetPlayerBodyY();
	float GetPlayerBodyXVecolity();
private:

	//5 below functions provided in framework
	gef::Skeleton* GetFirstSkeleton(gef::Scene* scene);
	gef::Mesh* GetFirstMesh(gef::Scene* scene, gef::Platform& platform);
	gef::Animation* LoadAnimation(const char* anim_scene_ilename, const char* anim_name, gef::Platform& platform);
	gef::Scene* LoadSceneAssets(gef::Platform& platform, const char* filename);
	gef::Mesh* GetMeshFromSceneAssets(gef::Scene* scene);

	//mesh object and scene object for the player
	gef::Mesh* player_mesh_;
	gef::Scene* ninja;

	//the player model needed to be scaled,rotated. slide vector for when the character rotates on the Z, 
	//he needs to be - on the x axis to the players standing position to avoid the impression of teleporting
	gef::Vector4 playerscalingV4;
	gef::Vector4 playerrotateV4;
	gef::Vector4 playerslideV4;
	
	gef::Matrix44 player_scaleM44;
	gef::Matrix44 player_rotateM44;
	gef::Matrix44 player_transformslideM44;
	gef::Matrix44 player_rotateslideM44;


	//animation
	MotionClipPlayer anim_player_;
	gef::Animation* walk_anim_;
	gef::Animation* slide_anim_;
	gef::Animation* up_anim_;
	gef::Animation* down_anim_;
	gef::SkinnedMeshInstance* playerskinned;

	//B2Body stuff
	b2Body* player_body_;
	b2Vec2 playeradjusty;
	b2BodyDef player_body_def;
	b2PolygonShape player_shape;
	b2Fixture* fixturelistplayer;
	b2FixtureDef player_fixture_def;


	bool issliding_;
	bool goalreached_ = false;
	int counter = 0;


};

#endif // _PLAYER_H
