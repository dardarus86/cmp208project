
#ifndef _PLAYER_H
#define _PLAYER_H

#include <system/application.h>
#include <maths/vector2.h>
#include <graphics/sprite.h>
#include "primitive_builder.h"
#include <graphics/mesh_instance.h>
#include <audio/audio_manager.h>
#include <input/input_manager.h>
#include <box2d/Box2D.h>
#include "game_object.h"
#include <graphics\skinned_mesh_instance.h>
#include "motion_clip_player.h"
#include <string.h>
#include "Roads.h"
#include "Buildings.h"
#include "Obstacles.h"
#define NUMBER_OF_DIFFICULTY 3
#define START_MENU_CHOICES 3
#define OPTION_MENU_CHOICES 5
#define NUMBER_OF_SONGS 3
#define NUMBER_OF_VOLUME 5
#define NUMBER_OF_ROAD_MESH_INSTANCE 20
#define NUMBER_OF_BUILDING_MESH_INSTANCE 12
#define NUMBER_OF_OBSTACLE_MESH_INSTANCE 25
#define NUMBER_OF_OBJECT_B2B_BODIES 25
// FRAMEWORK FORWARD DECLARATIONS
namespace gef
{
	class Platform;
	class SpriteRenderer;
	class Font;
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
	void InitPlayer(b2World* world, gef::Platform& platform);
	void Render(gef::Renderer3D* renderer);
	void update(float frame_time, int score, gef::InputManager* inputmanager);
	bool GetGoalFinished();
	float GetPlayerBodyX();
	float GetPlayerBodyY();
private:

	void SetGoalFinished(bool goalfinish);
	

	void UpdateSimulation(float frame_time);

	gef::Skeleton* GetFirstSkeleton(gef::Scene* scene);
	gef::Mesh* GetFirstMesh(gef::Scene* scene, gef::Platform& platform);
	gef::Animation* LoadAnimation(const char* anim_scene_ilename, const char* anim_name, gef::Platform& platform);
	gef::Scene* LoadSceneAssets(gef::Platform& platform, const char* filename);
	gef::Mesh* GetMeshFromSceneAssets(gef::Scene* scene);

	gef::Mesh* player_mesh_;
	gef::Scene* ninja;

	gef::Vector4 playerscalingV4;
	gef::Vector4 playerrotateV4;
	gef::Vector4 playerslideV4;

	gef::Matrix44 player_scaleM44;
	gef::Matrix44 player_rotateM44;
	gef::Matrix44 player_transformslideM44;
	gef::Matrix44 player_rotateslideM44;
	gef::Matrix44 endplayerpos;

	//animation
	MotionClipPlayer anim_player_;
	gef::Animation* walk_anim_;
	gef::Animation* slide_anim_;
	gef::Animation* up_anim_;
	gef::Animation* down_anim_;
	gef::Animation* dance_anim_;
	gef::SkinnedMeshInstance* playerskinned;

	b2Body* player_body_;
	b2Vec2 playeradjusty;

	b2Fixture* fixturelistplayer;
	b2BodyDef player_body_def;
	b2PolygonShape player_shape;

	b2FixtureDef player_fixture_def;


	bool issliding_;
	bool goalreached_ = false;
	int counter = 0;


};

#endif // _PLAYER_H
