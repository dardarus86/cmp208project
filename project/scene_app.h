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
	gef::Mesh* ground2_mesh_;

	gef::Scene* ninja;
	gef::Scene* road;

	gef::Scene* samurai_Roads[5];
	gef::Scene* military_Roads[5];
	gef::Scene* apoc_Roads[5];
	gef::MeshInstance samurai_Road_Mesh_Instance[20];
	gef::Vector4 samurairoadmeshinstanceV4[20];
	gef::Matrix44 samurai_Road_Mesh_Instance_M44[20];
	gef::MeshInstance militaryRoad_Mesh_Instance[20];
	gef::Vector4 military_Road_Mesh_Instance_V4[20];
	gef::Matrix44 military_Road_Mesh_Instance_M44[20];
	gef::MeshInstance apoc_Road_Mesh_Instance[20];
	gef::Vector4 apocroadmeshinstanceV4[20];
	gef::Matrix44 apoc_Road_MeshInstance_M44[20];

	gef::Scene* samurai_Buildings[5];
	gef::Scene* military_Buildings[5];
	gef::Scene* apoc_Buildings[5];
	gef::Vector4 samurai_Buildings_Mesh_Instance_V4[12];
	gef::Matrix44 samurai_Buildings_Mesh_Instance_M44[12];
	gef::Vector4 military_Buildings_Mesh_Instance_V4[12];
	gef::Matrix44 military_Buildings_Mesh_Instance_M44[12];
	gef::Vector4 apoc_Buildings_Mesh_Instance_V4[12];
	gef::Matrix44 apoc_Buildings_Mesh_Instance_M44[12];
	gef::MeshInstance  samurai_Building_Mesh_Instance[12];
	gef::MeshInstance  military_Building_Mesh_Instance[12];
	gef::MeshInstance  apoc_Building_Mesh_Instance[12];

	gef::Scene* samurai_Obstacle[4];
	gef::Scene* military_Obstacle[4];
	gef::Scene* apoc_Obstacle[4];
	gef::Vector4 militaryobstaclemeshinstanceV4[20];
	gef::Matrix44 military_Obstacle_Mesh_Instance_M44[20];
	gef::Vector4 samuraiobstaclemeshinstanceV4[20];
	gef::Matrix44 samurai_Obstacle_Mesh_Instance_M44[20];
	gef::Vector4 apocobstaclemeshinstanceV4[20];
	gef::Matrix44 apoc_Obstacle_Mesh_Instance_M44[20];
	gef::MeshInstance  samurai_Obstacle_Mesh_Instance[20];
	gef::MeshInstance  military_Obstacle_Mesh_Instance[20];
	gef::MeshInstance  apoc_Obstacle_Mesh_Instance[20];

	gef::Scene* background;
	gef::Vector4 backgroundV4;
	gef::Matrix44 backgroundM44;
	gef::MeshInstance  background_Mesh_Instance;



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
	GameObject ground2_;
	b2World* world_;
	b2Vec2 vel;
	b2Body* ground_body_;
	b2Body* ground2_body_;
	b2Body* player_body_;

	b2Fixture* fixturelistplayer;

	b2Body* military_Obstacle_B2B[20];
	b2Body* samurai_Obstacle_B2B[20];
	b2Body* apoc_Obstacle_B2B[20];
	b2Vec2 military_Obstacle_Position_B2vec2[20];
	b2Vec2 samurai_Obstacle_Position_B2vec2[20];
	b2Vec2 apoc_Obstacle_Position_V2vec2[20];

	b2BodyDef military_Body_Def[20];
	b2BodyDef samurai_Body_Def[20];
	b2BodyDef apoc_Body_Def[20];

	b2PolygonShape player_fixture_shape;
	b2PolygonShape player_shape;
	b2FixtureDef player_fixture_def;
	b2PolygonShape military_Shape[20];
	b2PolygonShape samurai_Shape[20];
	b2PolygonShape apoc_Shape[20];

	b2FixtureDef military_Obstacle_Fixture_Def[20];
	b2FixtureDef samurai_Obstacle_Fixture_Def[20];
	b2FixtureDef apoc_Obstacle_Fixture_Def[20];

	// audio variables

	float fps_;
	float camera_fov_;
	float near_plane_;
	float far_plane_;

	bool issliding_;
	int counter;
	int score;
	int military_Road_Distance_Counter;
	int samurai_Road_Distance_Counter;
	int apoc_Road_Distance_Counter;
	int military_Building_Distance_Counter;
	int samurai_Building_Distance_Counter;
	int apoc_Building_Distance_Counter;

	int military_Obstacle_Distance_Counter;
	int samurai_Obstacle_Distance_Counter;
	int apoc_Obstacle_Distance_Counter;

	float  obstacle_Height;
};

#endif // _SCENE_APP_H
