#ifndef _SCENE_APP_H
#define _SCENE_APP_H

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

class SceneApp : public gef::Application
{
public:
	SceneApp(gef::Platform& platform);
	void Init();
	void CleanUp();
	bool Update(float frame_time);
	void Render();
	
	gef::Skeleton* GetFirstSkeleton(gef::Scene* scene);
	gef::Mesh* GetFirstMesh(gef::Scene* scene);
	void CameraRotateAroundObject(float rotate);
private:
	void InitPlayer();
	void InitGround();
	void InitRoad();
	void InitBuildings();
	void InitObstacles();
	void InitFont();
	void CleanUpFont();
	void DrawHUD();
	void SetupLights();
	void SetupCamera();
	void UpdateSimulation(float frame_time);

	gef::Animation* LoadAnimation(const char* anim_scene_ilename, const char* anim_name);
	gef::Scene* LoadSceneAssets(gef::Platform& platform, const char* filename);
	gef::Mesh* GetMeshFromSceneAssets(gef::Scene* scene);


	gef::SpriteRenderer* sprite_renderer_;
	gef::Font* font_;
	gef::Renderer3D* renderer_3d_;
	gef::InputManager* input_manager_;
	gef::AudioManager* audio_manager_;
	PrimitiveBuilder* primitive_builder_;

	
	

	gef::Mesh* road_mesh;
	gef::Mesh* player_mesh_;
	gef::Mesh* ground_mesh_;
	

	gef::Scene* ninja;
	gef::Scene* road;

	gef::Scene* samurairoads[5];
	gef::Scene* militaryroads[5];
	gef::Scene* apocroads[5];
	gef::MeshInstance samurairoadmeshinstance[20];
	gef::Vector4 samurairoadmeshinstanceV4[20];
	gef::Matrix44 samurairoadmeshinstanceM44[20];
	gef::MeshInstance militaryroadmeshinstance[20];
	gef::Vector4 militaryroadmeshinstanceV4[20];
	gef::Matrix44 militaryroadmeshinstanceM44[20];
	gef::MeshInstance apocroadmeshinstance[20];
	gef::Vector4 apocroadmeshinstanceV4[20];
	gef::Matrix44 apocroadmeshinstanceM44[20];

	gef::Scene* samuraibuildings[5];
	gef::Scene* militarybuildings[5];
	gef::Scene* apocbuildings[5];
	gef::Vector4 samuraibuildingsmeshinstanceV4[12];
	gef::Matrix44 samuraibuildingsmeshinstanceM44[12];
	gef::Vector4 militarybuildingsmeshinstanceV4[12];
	gef::Matrix44 militarybuildingsmeshinstanceM44[12];
	gef::Vector4 apocbuildingsmeshinstanceV4[12];
	gef::Matrix44 apocbuildingsmeshinstanceM44[12];
	gef::MeshInstance  samuraibuildingmeshinstance[12];
	gef::MeshInstance  militarybuildingmeshinstance[12];
	gef::MeshInstance  apocbuildingmeshinstance[12];

	gef::Scene* samuraiobstacle[2];
	gef::Scene* militaryobstacle[2];
	gef::Scene* apocobstacle[2];
	gef::Vector4 militaryobstaclemeshinstanceV4[10];
	gef::Matrix44 militaryobstaclemeshinstanceM44[10];
	gef::Vector4 samuraiobstaclemeshinstanceV4[10];
	gef::Matrix44 samuraiobstaclemeshinstanceM44[10];
	gef::Vector4 apocobstaclemeshinstanceV4[10];
	gef::Matrix44 apocobstaclemeshinstanceM44[10];
	gef::MeshInstance  samuraiobstaclemeshinstance[10];
	gef::MeshInstance  militaryobstaclemeshinstance[10];
	gef::MeshInstance  apocoobstaclemeshinstance[10];





	//Vectors/matrixes
	gef::Vector4 camera_eye_V4;
	gef::Vector4 camera_eye_transformV4;
	gef::Matrix44 camera_eye_transformM44;
	gef::Vector4 camera_eye_rotationV4;
	gef::Vector4 camera_lookat_V4;
	gef::Vector4 camera_lookat_transformV4;
	gef::Vector4 camera_up_V4;
	gef::Vector4 playerscalingV4;
	gef::Vector4 playerrotateV4;
	gef::Vector4 playerslideV4;
	gef::Matrix44 player_scaleM44;
	gef::Matrix44 player_transformM44;
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

	

	//B2 stuff
	GameObject ground_;
	b2World* world_;
	b2Vec2 vel;
	b2Body* ground_body_;
	b2Body* player_body_;

	b2Body* militaryobstacleB2B[10];
	b2Body* samuraiobstacleB2B[10];
	b2Body* apocobstacleB2B[10];
	b2Vec2 militaryobstaclepositionb2vec2[10];
	b2Vec2 samuraiobstaclepositionb2vec2[10];
	b2Vec2 apocobstaclepositionb2vec2[10];

	b2BodyDef militarybodydef[10];
	b2BodyDef samuraibodydef[10];
	b2BodyDef apocbodydef[10];

	b2PolygonShape player_shape;
	b2PolygonShape militaryshape[10];
	b2PolygonShape samuraishape[10];
	b2PolygonShape apocshape[10];

	b2FixtureDef militaryobstaclefixturedef[10];
	b2FixtureDef samuraiobstaclefixturedef[10];
	b2FixtureDef apocobstaclefixturedef[10];

	// audio variables

	float fps_;
	float camera_fov_;
	float near_plane_;
	float far_plane_;

	bool issliding_;
	int counter;
	int militaryroaddistancecounter;
	int samurairoaddistancecounter;
	int apocroaddistancecounter;
	int militarybuildingdistancecounter;
	int samuraibuildingdistancecounter;
	int apocbuildingdistancecounter;

	int militaryobstacledistancecounter;
	int samuraiobstacledistancecounter;
	int apocobstacledistancecounter;

	float  obstacleheight;
};

#endif // _SCENE_APP_H
