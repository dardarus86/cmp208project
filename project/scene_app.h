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
#include "Roads.h"
#include "Buildings.h"
#include "Obstacles.h"
#include "Player.h"
#define NUMBER_OF_DIFFICULTY 3
#define START_MENU_CHOICES 3
#define OPTION_MENU_CHOICES 5
#define NUMBER_OF_SONGS 3
#define NUMBER_OF_VOLUME 5

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

}

class SceneApp : public gef::Application
{
public:

	enum class STARTMENUCHOICE { START = 0, OPTION, QUIT };
	enum class GAMEMUSICCHOICE { BREEZY = 0, RUNBOY, MIAMI };
	enum class GOODSCOREEFFECT { BRAVO = 0, GOOD, PRETTYGOOD, GREAT, MARVELOUS, NICE, PERFECT };
	enum class OPTIONMENUCHOICE { VOLUME = 0, DIFFICULTY, MUSIC, CAMERA, BACK };
	enum class GAMESTATE { INIT = 0, OPTIONS, GAME, END };
	enum class GAMEDIFFICULTY { DIFF_EASY = 0, DIFF_MEDIUM, DIFF_HARD };
	enum class VOLUMELEVEL { VOL_OFF = 0, VOL_LOW, VOL_MEDIUM, VOL_HIGH, VOL_FULL };
	enum class CameraOptions { Behind = 0, Deg_45, Side };

	SceneApp(gef::Platform& platform);
	void Init();
	void CleanUp();
	bool Update(float frame_time);
	void Render();


	void CameraRotateAroundObject(float rotate);
private:

	STARTMENUCHOICE current_start_menu_choice_;
	OPTIONMENUCHOICE current_option_menu_choice_;
	GAMEMUSICCHOICE game_music_state;
	GAMESTATE game_state_;
	GAMEDIFFICULTY difficulty_state_;
	VOLUMELEVEL volume_state_;
	CameraOptions camera_state_;

	void GameInit();
	void FrontendInit();

	void InitGround();
	void InitFont();

	void FrontendUpdate(float frame_time);
	void FrontendRender();

	void OptionsUpdate(float frame_time);
	void OptionsRender();

	void GameUpdate(float frame_time);
	void GameRender();

	void EndInit();
	void EndUpdate(float frame_time);
	void EndRender();

	void CleanUpFont();
	void DrawHUD();
	void SetupLights();
	void SetupCamera();
	void UpdateSimulation(float frame_time);

	gef::Scene* LoadSceneAssets(gef::Platform& platform, const char* filename);
	gef::Mesh* GetMeshFromSceneAssets(gef::Scene* scene);

	Roads roads;
	Buildings buildings;
	Obstacles obstacles;
	Player player;

	gef::SpriteRenderer* sprite_renderer_;
	gef::Font* font_;
	gef::Renderer3D* renderer_3d_;
	gef::InputManager* input_manager_;
	gef::AudioManager* audio_manager_;
	PrimitiveBuilder* primitive_builder_;

	gef::Mesh* ground_mesh_;


	gef::Scene* background;

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
	
	gef::Matrix44 camera_eye_transformM44;

	//B2 stuff
	GameObject ground_;
	b2World* world_;
	b2Body* ground_body_;

	

	// audio variables

	float fps_;
	float camera_fov_;
	float near_plane_;
	float far_plane_;

	//std::vector<int> game_obstacle_distance = { 30,20,15 };
	std::vector<int> volumelevel = { 0, 25, 50, 75, 100 };
	std::vector<std::string> difficultytext = { "EASY","MEDIUM","HARD" };
	std::vector<std::string> gamemusictext = { "BREEZY", "RUN BOY", "MIAMI" };
	std::vector<std::string> cameraoptiontext = { " Side on " , " 45 Degree" , " Behind " };
	std::vector<gef::Vector4> cameraoptionchoice;

	uint32 textcolors[2];

	int score;



};

#endif // _SCENE_APP_H
