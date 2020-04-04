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
#include <string.h>

#define NUMBER_OF_DIFFICULTY 3
#define START_MENU_CHOICES 3
#define OPTION_MENU_CHOICES 4
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

class SceneApp : public gef::Application
{
public:

	enum STARTMENUCHOICE { START = 0, OPTION, QUIT };
	enum GAMEMUSICCHOICE {BREEZY = 0, RUNBOY, MIAMI };
	enum GOODSCOREEFFECT {BRAVO = 0, GOOD, PRETTYGOOD, GREAT, MARVELOUS, NICE, PERFECT };
	enum OPTIONMENUCHOICE { VOLUME = 0, DIFFICULTY, MUSIC, BACK };
	enum GAMESTATE { INIT, OPTIONS, GAME, END };
	enum GAMEDIFFICULTY { DIFF_EASY = 0, DIFF_MEDIUM, DIFF_HARD };
	enum VOLUMELEVEL { VOL_OFF = 0, VOL_LOW, VOL_MEDIUM, VOL_HIGH, VOL_FULL };

	SceneApp(gef::Platform& platform);
	void Init();
	void CleanUp();
	bool Update(float frame_time);
	void Render();

	gef::Skeleton* GetFirstSkeleton(gef::Scene* scene);
	gef::Mesh* GetFirstMesh(gef::Scene* scene);
	void CameraRotateAroundObject(float rotate);
private:
	void GameInit();
	void FrontendInit();
	void FrontendUpdate(float frame_time);
	void FrontendRender();

	void OptionsUpdate(float frame_time);
	void OptionsRender();
	void OptionsRelease();

	void GameUpdate(float frame_time);
	void GameRender();
	void GameRelease();

	void EndInit();
	void EndUpdate(float frame_time);
	void EndRender();
	void EndRelease();
	void InitOptions();
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

	STARTMENUCHOICE current_start_menu_choice_;
	OPTIONMENUCHOICE current_option_menu_choice_;
	GAMEMUSICCHOICE game_music_state;
	GAMESTATE game_state_;
	GAMEDIFFICULTY difficulty_state_;
	VOLUMELEVEL volume_state_;

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
	gef::Scene* samurai_Roads[5];
	gef::Scene* military_Roads[5];
	gef::Scene* apoc_Roads[5];
	gef::Scene* samurai_Buildings[5];
	gef::Scene* military_Buildings[5];
	gef::Scene* apoc_Buildings[5];
	gef::Scene* samurai_Obstacle[4];
	gef::Scene* military_Obstacle[4];
	gef::Scene* apoc_Obstacle[4];
	gef::Scene* background;
	gef::Scene* start_model;



	///test
	gef::Scene* startscreen;
	gef::MeshInstance startscreen_Mesh_Instance;
	gef::Vector4 startscreen_Mesh_Instance_V4;
	gef::Matrix44 startscreen_Mesh_Instance_M44;


	gef::MeshInstance startmodel;
	gef::Vector4 startmodelV4;
	gef::Matrix44 startmodelM44;



	gef::MeshInstance militaryRoad_Mesh_Instance[NUMBER_OF_ROAD_MESH_INSTANCE];
	gef::MeshInstance samurai_Road_Mesh_Instance[NUMBER_OF_ROAD_MESH_INSTANCE];
	gef::MeshInstance apoc_Road_Mesh_Instance[NUMBER_OF_ROAD_MESH_INSTANCE];
	gef::Vector4 military_Road_Mesh_Instance_V4[NUMBER_OF_ROAD_MESH_INSTANCE];
	gef::Vector4 samurairoadmeshinstanceV4[NUMBER_OF_ROAD_MESH_INSTANCE];
	gef::Vector4 apocroadmeshinstanceV4[NUMBER_OF_ROAD_MESH_INSTANCE];
	gef::Matrix44 military_Road_Mesh_Instance_M44[NUMBER_OF_ROAD_MESH_INSTANCE];
	gef::Matrix44 samurai_Road_Mesh_Instance_M44[NUMBER_OF_ROAD_MESH_INSTANCE];
	gef::Matrix44 apoc_Road_MeshInstance_M44[NUMBER_OF_ROAD_MESH_INSTANCE];

	gef::MeshInstance  military_Building_Mesh_Instance[NUMBER_OF_BUILDING_MESH_INSTANCE];
	gef::MeshInstance  samurai_Building_Mesh_Instance[NUMBER_OF_BUILDING_MESH_INSTANCE];
	gef::MeshInstance  apoc_Building_Mesh_Instance[NUMBER_OF_BUILDING_MESH_INSTANCE];
	gef::Vector4 military_Buildings_Mesh_Instance_V4[NUMBER_OF_BUILDING_MESH_INSTANCE];
	gef::Vector4 samurai_Buildings_Mesh_Instance_V4[NUMBER_OF_BUILDING_MESH_INSTANCE];
	gef::Vector4 apoc_Buildings_Mesh_Instance_V4[NUMBER_OF_BUILDING_MESH_INSTANCE];
	gef::Matrix44 military_Buildings_Mesh_Instance_M44[NUMBER_OF_BUILDING_MESH_INSTANCE];
	gef::Matrix44 samurai_Buildings_Mesh_Instance_M44[NUMBER_OF_BUILDING_MESH_INSTANCE];
	gef::Matrix44 apoc_Buildings_Mesh_Instance_M44[NUMBER_OF_BUILDING_MESH_INSTANCE];

	gef::MeshInstance  military_Obstacle_Mesh_Instance[NUMBER_OF_OBSTACLE_MESH_INSTANCE];
	gef::MeshInstance  samurai_Obstacle_Mesh_Instance[NUMBER_OF_OBSTACLE_MESH_INSTANCE];
	gef::MeshInstance  apoc_Obstacle_Mesh_Instance[NUMBER_OF_OBSTACLE_MESH_INSTANCE];
	gef::Vector4 militaryobstaclemeshinstanceV4[NUMBER_OF_OBSTACLE_MESH_INSTANCE];
	gef::Vector4 samuraiobstaclemeshinstanceV4[NUMBER_OF_OBSTACLE_MESH_INSTANCE];
	gef::Vector4 apocobstaclemeshinstanceV4[NUMBER_OF_OBSTACLE_MESH_INSTANCE];
	gef::Matrix44 military_Obstacle_Mesh_Instance_M44[NUMBER_OF_OBSTACLE_MESH_INSTANCE];
	gef::Matrix44 samurai_Obstacle_Mesh_Instance_M44[NUMBER_OF_OBSTACLE_MESH_INSTANCE];
	gef::Matrix44 apoc_Obstacle_Mesh_Instance_M44[NUMBER_OF_OBSTACLE_MESH_INSTANCE];

	gef::MeshInstance  background_Mesh_Instance;
	gef::Vector4 backgroundV4;
	gef::Matrix44 backgroundM44;




	//Vectors/matrixes
	gef::Vector4 camera_eye_V4;
	gef::Vector4 camera_eye_transformV4;

	gef::Vector4 camera_eye_rotationV4;
	gef::Vector4 camera_lookat_V4;
	gef::Vector4 camera_lookat_transformV4;
	gef::Vector4 camera_up_V4;
	gef::Vector4 playerscalingV4;
	gef::Vector4 playerrotateV4;
	gef::Vector4 playerslideV4;
	gef::Matrix44 camera_eye_transformM44;
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



	//B2 stuff
	GameObject ground_;
	GameObject ground2_;
	b2World* world_;
	b2Vec2 vel;
	b2Body* ground_body_;
	b2Body* ground2_body_;
	b2Body* player_body_;

	b2Fixture* fixturelistplayer;

	b2Body* military_Obstacle_B2B[NUMBER_OF_OBJECT_B2B_BODIES];
	b2Body* samurai_Obstacle_B2B[NUMBER_OF_OBJECT_B2B_BODIES];
	b2Body* apoc_Obstacle_B2B[NUMBER_OF_OBJECT_B2B_BODIES];
	b2Vec2 military_Obstacle_Position_B2vec2[NUMBER_OF_OBJECT_B2B_BODIES];
	b2Vec2 samurai_Obstacle_Position_B2vec2[NUMBER_OF_OBJECT_B2B_BODIES];
	b2Vec2 apoc_Obstacle_Position_V2vec2[NUMBER_OF_OBJECT_B2B_BODIES];
	b2BodyDef military_Body_Def[NUMBER_OF_OBJECT_B2B_BODIES];
	b2BodyDef samurai_Body_Def[NUMBER_OF_OBJECT_B2B_BODIES];
	b2BodyDef apoc_Body_Def[NUMBER_OF_OBJECT_B2B_BODIES];
	b2BodyDef player_body_def;
	b2PolygonShape player_shape;
	b2PolygonShape military_Shape;
	b2PolygonShape samurai_Shape;
	b2PolygonShape apoc_Shape;
	b2FixtureDef player_fixture_def;
	b2FixtureDef military_Obstacle_Fixture_Def[NUMBER_OF_OBJECT_B2B_BODIES];
	b2FixtureDef samurai_Obstacle_Fixture_Def[NUMBER_OF_OBJECT_B2B_BODIES];
	b2FixtureDef apoc_Obstacle_Fixture_Def[NUMBER_OF_OBJECT_B2B_BODIES];

	// audio variables

	float fps_;
	float camera_fov_;
	float near_plane_;
	float far_plane_;
	float obstacle_Height;

	bool issliding_;

	int* gameobstacledifficulty;
	int* gameobstacledistance;
	int* volumelevel;
	
	int audio_sample_id;

	std::string* difficultytext;
	std::string* gamemusictext;
	uint32 textcolors[2];

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


};

#endif // _SCENE_APP_H
