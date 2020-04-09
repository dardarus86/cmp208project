																		///////////////////////////////////////////
																		//      CODE PROVIDED FOR CMP208         //
																		//				COMMENTED                //
																		///////////////////////////////////////////


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
#include "Zombies.h"

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
	//Enums used for the project to control specific states. last one was lower case just under suggestion of Chris Trewartha so i wanted to use both versions for clarity
	enum class STARTMENUCHOICE { START = 0, OPTION, QUIT };
	enum class GAMEMUSICCHOICE { BREEZY = 0, RUNBOY, MIAMI };
	enum class OPTIONMENUCHOICE { VOLUME = 0, DIFFICULTY, MUSIC, CAMERA, BACK };
	enum class GAMESTATE { SPLASH = 0,INIT, OPTIONS, GAME, END };
	enum class GAMEDIFFICULTY { DIFF_EASY = 0, DIFF_MEDIUM, DIFF_HARD };
	enum class VOLUMELEVEL { VOL_OFF = 0, VOL_LOW, VOL_MEDIUM, VOL_HIGH, VOL_FULL };
	enum class CameraOptions { Behind = 0, Deg_45, Side };

	SceneApp(gef::Platform& platform);
	//basic game state functions for the game
	void Init();
	void CleanUp();
	bool Update(float frame_time);
	void Render();
	
private:

	//initalizing the enums above
	STARTMENUCHOICE current_start_menu_choice_;
	OPTIONMENUCHOICE current_option_menu_choice_;
	GAMEMUSICCHOICE game_music_state;
	GAMESTATE game_state_;
	GAMEDIFFICULTY difficulty_state_;
	VOLUMELEVEL volume_state_;
	CameraOptions camera_state_;

	//all inits together
	void GameInit();
	void EndInit();
	void InitGround();
	void InitFont();
	void InitBackgroundImages();

	//update and render functions for each game state
	void SplashUpdate(float frame_time);
	void SplashRender();

	void FrontendUpdate(float frame_time);
	void FrontendRender();

	void OptionsUpdate(float frame_time);
	void OptionsRender();

	void GameUpdate(float frame_time);
	void GameRender();
	
	void EndUpdate(float frame_time);
	void EndRender();

	//functions that dont fit in any of the above catagories
	void CleanUpFont();
	void DrawHUD();
	void SetupLights();
	void SetupCamera();
	void UpdateSimulation(float frame_time);

	//functions provided by framework
	gef::Scene* LoadSceneAssets(gef::Platform& platform, const char* filename);
	gef::Mesh* GetMeshFromSceneAssets(gef::Scene* scene);

	//creating objects of the classes needed for game
	Roads roads;
	Buildings buildings;
	Obstacles obstacles;
	Player player;
	Zombies zombies;

	//pointer objects for classes needed for game
	gef::SpriteRenderer* sprite_renderer_;
	gef::Font* font_;
	gef::Renderer3D* renderer_3d_;
	gef::InputManager* input_manager_;
	gef::AudioManager* audio_manager_;
	PrimitiveBuilder* primitive_builder_;

	//mesh used for the ground b2b body that is placed below the ground model
	gef::Mesh* ground_mesh_;

	//texture and sprites use  for the background images on splash,options and main menu
	gef::Texture* splash_screen__texture_;
	gef::Texture* options_texture_;
	gef::Texture* menu_texture_;
	gef::Sprite splash_background_;
	gef::Sprite options_background_;
	gef::Sprite main_menu_background_;

	//model used for the ground and mountains in the background loaded as a single model and the vector and matrix used  for translating
	gef::Scene* background;
	gef::MeshInstance  background_Mesh_Instance;
	gef::Vector4 backgroundV4;
	gef::Matrix44 backgroundM44;


	//Vectors/matrixes for camera use
	gef::Vector4 camera_eye_V4;
	gef::Vector4 camera_eye_transformV4;
	gef::Vector4 camera_eye_rotationV4;
	gef::Vector4 camera_lookat_V4;
	gef::Vector4 camera_lookat_transformV4;
	gef::Vector4 camera_up_V4;
	gef::Matrix44 camera_eye_transformM44;

	//B2body objects for the ground and creating the world physics
	GameObject ground_;
	b2World* world_;
	b2Body* ground_body_;

	//floats used for fps and camera
	float fps_;
	float camera_fov_;
	float near_plane_;
	float far_plane_;

	//vectors used for the menu selection text and volume control in options
	std::vector<int> volumelevel = { 0, 25, 50, 75, 100 };
	std::vector<std::string> difficultytext = { "EASY","MEDIUM","HARD" };
	std::vector<std::string> gamemusictext = { "BREEZY", "RUN BOY", "MIAMI" };
	std::vector<std::string> cameraoptiontext = { " Side on " , " 45 Degree" , " Behind " };
	std::vector<gef::Vector4> cameraoptionchoice;

	//used for the font text
	uint32 textcolors[3];

	//Two ints for score and a boocounter used when the score reaches a certain threshold it plays an audio sample
	int score;
	int boocounter;
};

#endif // _SCENE_APP_H
