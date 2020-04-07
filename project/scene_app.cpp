#include "scene_app.h"
#include <system/platform.h>
#include <graphics/sprite_renderer.h>
#include <graphics/font.h>
#include <system/debug_log.h>
#include <graphics/renderer_3d.h>
#include <graphics/mesh.h>
#include <maths/math_utils.h>
#include <input/sony_controller_input_manager.h>
#include <input/input_manager.h>
#include <input/keyboard.h>
#include <graphics/scene.h>
#include <animation/skeleton.h>
#include <animation/animation.h>
#include <graphics/sprite.h>
#include <stdlib.h>
#include <time.h>  
#include "load_texture.h"




SceneApp::SceneApp(gef::Platform& platform) :
	Application(platform)
	, sprite_renderer_(NULL)
	, renderer_3d_(NULL)
	, primitive_builder_(NULL)
	, input_manager_(NULL)
	, audio_manager_(NULL)
	, font_(NULL)
	, world_(NULL)
{
}

void SceneApp::Init()
{
	game_state_ = GAMESTATE::SPLASH;
	difficulty_state_ = GAMEDIFFICULTY::DIFF_MEDIUM;
	volume_state_ = VOLUMELEVEL::VOL_MEDIUM;
	current_start_menu_choice_ = STARTMENUCHOICE::START;
	current_option_menu_choice_ = OPTIONMENUCHOICE::VOLUME;
	camera_state_ = CameraOptions::Behind;
	textcolors[0] = 0xffffffff;
	textcolors[1] = 0xff0000dd;
	textcolors[2] = 0xffabcdef;
	sprite_renderer_ = gef::SpriteRenderer::Create(platform_);
	renderer_3d_ = gef::Renderer3D::Create(platform_);
	primitive_builder_ = new PrimitiveBuilder(platform_);
	input_manager_ = gef::InputManager::Create(platform_);
	audio_manager_ = gef::AudioManager::Create();

	//This was my original code for working out how many obstancles were present depending on the difficulty.
	//gameobstacledifficulty = new int[NUMBER_OF_DIFFICULTY] { 500/35, 500/25, 500/20 };
	audio_manager_->LoadMusic("audio/music/start.wav", platform_);
	GameInit();

}

void SceneApp::GameInit()
{
	EndInit();
	InitFont();
	InitBackgroundImages();
	audio_manager_->PlayMusic();
	audio_manager_->LoadSample("audio/soundeffects/menu/select1.wav", platform_);//0
	audio_manager_->LoadSample("audio/soundeffects/good/bravo.wav", platform_);//1
	audio_manager_->LoadSample("audio/soundeffects/good/good.wav", platform_);//2
	audio_manager_->LoadSample("audio/soundeffects/good/great.wav", platform_);//3
	audio_manager_->LoadSample("audio/soundeffects/good/marvelous.wav", platform_);//4
	audio_manager_->LoadSample("audio/soundeffects/good/nicemoves.wav", platform_);//5
	audio_manager_->LoadSample("audio/soundeffects/good/prettygood.wav", platform_);//6
	audio_manager_->LoadSample("audio/soundeffects/good/perfect.wav", platform_);//7
	audio_manager_->LoadSample("audio/soundeffects/bad/bad.wav", platform_);//8
	audio_manager_->LoadSample("audio/soundeffects/bad/boo1.wav", platform_);//9
	audio_manager_->LoadSample("audio/soundeffects/bad/boo2.wav", platform_);//10
	audio_manager_->LoadSample("audio/soundeffects/bad/boo3.wav", platform_);//11

	audio_manager_->SetMasterVolume(volumelevel.at(static_cast<int>(volume_state_)));
	srand(time(NULL));
	b2Vec2 gravity(0.0f, -9.81f);
	world_ = new b2World(gravity);
	score = 0;
	boocounter = 30;
	player.InitPlayer(world_, platform_);
	InitGround();
	buildings.InitBuildings(platform_);
	roads.InitRoad(platform_);
	zombies.InitZombies(world_, platform_);
	//obstacles.InitObstacles(platform_, world_, static_cast<int>(difficulty_state_));
	SetupLights();
	SetupCamera();

}
void SceneApp::InitGround()
{
	// ground dimensions
	gef::Vector4 ground_half_dimensions(1000.0f, 0.1f, 50.0f);
	b2Vec2 ground_start_position(990.0f, -0.77f);
	// setup the mesh for the ground
	ground_mesh_ = primitive_builder_->CreateBoxMesh(ground_half_dimensions);
	ground_.set_mesh(ground_mesh_);

	// create a physics body
	b2BodyDef body_def;
	body_def.type = b2_staticBody;
	body_def.position = ground_start_position;

	ground_body_ = world_->CreateBody(&body_def);

	// create the shape
	b2PolygonShape shape;
	shape.SetAsBox(ground_half_dimensions.x(), ground_half_dimensions.y());


	// create the fixture
	b2FixtureDef fixture_def;
	fixture_def.shape = &shape;

	// create the fixture on the rigid body
	ground_body_->CreateFixture(&fixture_def);

	// update visuals from simulation data
	ground_.UpdateFromSimulation(ground_body_);



}

void SceneApp::InitFont()
{
	font_ = new gef::Font(platform_);
	font_->Load("Bubblegum.ttf");
	
}

void SceneApp::InitBackgroundImages()
{
	splash_screen__texture_ = CreateTextureFromPNG("images/splash.png", platform_);
	options_texture_ = CreateTextureFromPNG("images/option.png", platform_);
	menu_texture_ = CreateTextureFromPNG("images/main.png", platform_);

	splash_background_.set_texture(splash_screen__texture_);
	splash_background_.set_position(gef::Vector4(platform_.width() * 0.5f, platform_.height() * 0.5f, -0.99f));
	splash_background_.set_height(960.0f);
	splash_background_.set_width(1280.0f);
	main_menu_background_.set_texture(menu_texture_);
	main_menu_background_.set_position(gef::Vector4(platform_.width() * 0.5f, platform_.height() * 0.5f, -0.99f));
	main_menu_background_.set_height(960.0f);
	main_menu_background_.set_width(1280.0f);
	options_background_.set_texture(options_texture_);
	options_background_.set_position(gef::Vector4(platform_.width() * 0.5f, platform_.height() * 0.5f, -0.99f));
	options_background_.set_height(960.0f);
	options_background_.set_width(1280.0f);
	
}


bool SceneApp::Update(float frame_time)
{
	fps_ = 1.0f / frame_time;


	input_manager_->Update();

	switch (game_state_)
	{
	case GAMESTATE::SPLASH:
		SplashUpdate(frame_time);
		break;
	case GAMESTATE::INIT:
		FrontendUpdate(frame_time);
		break;
	case GAMESTATE::OPTIONS:
		OptionsUpdate(frame_time);
		break;
	case GAMESTATE::GAME:
		GameUpdate(frame_time);
		break;
	case GAMESTATE::END:
		EndUpdate(frame_time);
		break;
	}

	if (input_manager_->keyboard()->IsKeyPressed(gef::Keyboard::KC_RETURN))
	{
		switch (current_start_menu_choice_)
		{
		case STARTMENUCHOICE::QUIT:
			return false;
			break;
		}
	}

	return true;
}
void SceneApp::Render()
{
	switch (game_state_)
	{
	case GAMESTATE::SPLASH:
		SplashRender();
		break;
	case GAMESTATE::INIT:
		FrontendRender();
		break;
	case GAMESTATE::OPTIONS:
		OptionsRender();
		break;
	case GAMESTATE::GAME:
		GameRender();
		break;
	case GAMESTATE::END:
		EndRender();
		break;

	}
}


void SceneApp::SplashUpdate(float frame_time)
{
	if (input_manager_->keyboard()->IsKeyPressed(gef::Keyboard::KC_RETURN))
	{
		game_state_ = GAMESTATE::INIT;
	}
}

void SceneApp::SplashRender()
{
	sprite_renderer_->Begin();
	   
	sprite_renderer_->DrawSprite(splash_background_);


	sprite_renderer_->End();
}


void SceneApp::FrontendInit()
{

	//const char* start_model_asset = "start/startmodel.scn";
	//start_model = LoadSceneAssets(platform_, start_model_asset);
	//startmodelM44.SetIdentity();
	//startmodel.set_mesh(GetMeshFromSceneAssets(start_model));
	//startmodelV4 = gef::Vector4(0, 0, 0, 0);
	//startmodelM44.SetTranslation(startmodelV4);
	//startmodel.set_transform(startmodelM44);


}
void SceneApp::FrontendUpdate(float frame_time)
{



	if (input_manager_->keyboard()->IsKeyPressed(gef::Keyboard::KC_DOWN))
	{
		audio_manager_->PlaySample(0);
		current_start_menu_choice_ = SceneApp::STARTMENUCHOICE((static_cast<int>( current_start_menu_choice_) + 1) % START_MENU_CHOICES);
	}


	if (input_manager_->keyboard()->IsKeyPressed(gef::Keyboard::KC_UP))
	{
		audio_manager_->PlaySample(0);
		current_start_menu_choice_ = SceneApp::STARTMENUCHOICE((static_cast<int>(current_start_menu_choice_) -1 + START_MENU_CHOICES) % START_MENU_CHOICES);
	}

	if (input_manager_->keyboard()->IsKeyPressed(gef::Keyboard::KC_RETURN))
	{
		switch (current_start_menu_choice_)
		{
		case STARTMENUCHOICE::START:
			obstacles.InitObstacles(platform_, world_, static_cast<int>(difficulty_state_));
			switch (game_music_state)
			{
			case GAMEMUSICCHOICE::BREEZY:
				audio_manager_->LoadMusic("audio/music/breezy.wav", platform_);
				break;
			case GAMEMUSICCHOICE::RUNBOY:
				audio_manager_->LoadMusic("audio/music/runboy.wav", platform_);
				break;
			case GAMEMUSICCHOICE::MIAMI:
				audio_manager_->LoadMusic("audio/music/miami.wav", platform_);
				break;

			}
			
			audio_manager_->PlayMusic();
			game_state_ = GAMESTATE::GAME;
			break;
		case STARTMENUCHOICE::OPTION:
			game_state_ = GAMESTATE::OPTIONS;
			break;
		}
	}


}
void SceneApp::FrontendRender()
{

	sprite_renderer_->Begin();

	sprite_renderer_->DrawSprite(main_menu_background_);

	font_->RenderText(
		sprite_renderer_,
		gef::Vector4(platform_.width() * 0.5f, platform_.height() * 0.5f - 56.0f, -0.99f),
		1.0f,
		textcolors[current_start_menu_choice_ == STARTMENUCHOICE::START],
		gef::TJ_CENTRE,
		"Start");

	font_->RenderText(
		sprite_renderer_,
		gef::Vector4(platform_.width() * 0.5f, platform_.height() * 0.55f - 56.0f, -0.99f),
		1.0f,
		textcolors[current_start_menu_choice_ == STARTMENUCHOICE::OPTION],
		gef::TJ_CENTRE,
		"Options");

	font_->RenderText(
		sprite_renderer_,
		gef::Vector4(platform_.width() * 0.5f, platform_.height() * 0.60f - 56.0f, -0.99f),
		1.0f,
		textcolors[current_start_menu_choice_ == STARTMENUCHOICE::QUIT],
		gef::TJ_CENTRE,
		"Quit");

	sprite_renderer_->End();
}


void SceneApp::OptionsUpdate(float frame_time)
{
	if (input_manager_->keyboard()->IsKeyPressed(gef::Keyboard::KC_DOWN))
	{
		audio_manager_->PlaySample(0);
		current_option_menu_choice_ = SceneApp::OPTIONMENUCHOICE((static_cast<int>(current_option_menu_choice_) + 1) % OPTION_MENU_CHOICES);
	}

	if (input_manager_->keyboard()->IsKeyPressed(gef::Keyboard::KC_UP))
	{
		audio_manager_->PlaySample(0);
		current_option_menu_choice_ = SceneApp::OPTIONMENUCHOICE((static_cast<int>(current_option_menu_choice_) - 1 + OPTION_MENU_CHOICES) % OPTION_MENU_CHOICES);
	}

	if (input_manager_->keyboard()->IsKeyPressed(gef::Keyboard::KC_RIGHT))
	{
		if (current_option_menu_choice_ == OPTIONMENUCHOICE::VOLUME)
		{
			volume_state_ = SceneApp::VOLUMELEVEL((static_cast<int>(volume_state_) + 1) % volumelevel.size());
			audio_manager_->SetMasterVolume(volumelevel.at(static_cast<int>(volume_state_)));
		}
		else if (current_option_menu_choice_ == OPTIONMENUCHOICE::DIFFICULTY)
		{
			difficulty_state_ = SceneApp::GAMEDIFFICULTY((static_cast<int>(difficulty_state_) + 1) % difficultytext.size());
		}
		else if (current_option_menu_choice_ == OPTIONMENUCHOICE::MUSIC)
		{
			game_music_state = SceneApp::GAMEMUSICCHOICE((static_cast<int>(game_music_state) + 1) % gamemusictext.size());
		}
		else if (current_option_menu_choice_ == OPTIONMENUCHOICE::CAMERA)
		{
			camera_state_ = SceneApp::CameraOptions((static_cast<int>(camera_state_) + 1) % cameraoptiontext.size());
		}

	}

	if (input_manager_->keyboard()->IsKeyPressed(gef::Keyboard::KC_LEFT))
	{
		if (current_option_menu_choice_ == OPTIONMENUCHOICE::VOLUME)
		{
			volume_state_ = SceneApp::VOLUMELEVEL((static_cast<int>(volume_state_)  - 1 + volumelevel.size()) % volumelevel.size());
			audio_manager_->SetMasterVolume(volumelevel.at(static_cast<int>(volume_state_)));
		}
		else if (current_option_menu_choice_ == OPTIONMENUCHOICE::DIFFICULTY)
		{
			difficulty_state_ = SceneApp::GAMEDIFFICULTY((static_cast<int>(difficulty_state_) - 1 + difficultytext.size()) % difficultytext.size());
		}
		else if (current_option_menu_choice_ == OPTIONMENUCHOICE::MUSIC)
		{
			game_music_state = SceneApp::GAMEMUSICCHOICE((static_cast<int>(game_music_state) - 1 + gamemusictext.size()) % gamemusictext.size());
		}
		else if (current_option_menu_choice_ == OPTIONMENUCHOICE::CAMERA)
		{
			camera_state_ = SceneApp::CameraOptions((static_cast<int>(camera_state_) -1 + cameraoptiontext.size()) % cameraoptiontext.size());
		}
	}

	if (input_manager_->keyboard()->IsKeyPressed(gef::Keyboard::KC_RETURN))
	{
		switch (current_option_menu_choice_)
		{
		case OPTIONMENUCHOICE::BACK:
			
			game_state_ = GAMESTATE::INIT;
			break;
		}

	}
}
void SceneApp::OptionsRender()
{

	sprite_renderer_->Begin();

	sprite_renderer_->DrawSprite(options_background_);

	font_->RenderText(
		sprite_renderer_,
		gef::Vector4(platform_.width() * 0.3f, platform_.height() * 0.4f - 56.0f, -0.99f),
		1.5f,
		textcolors[current_option_menu_choice_ == OPTIONMENUCHOICE::VOLUME],
		gef::TJ_LEFT,
		"      VOLUME: %i", volumelevel.at(static_cast<int>(volume_state_)));


	font_->RenderText(
		sprite_renderer_,
		gef::Vector4(platform_.width() * 0.225f, platform_.height() * 0.48f - 56.0f, -0.99f),
		1.5f,
		textcolors[current_option_menu_choice_ == OPTIONMENUCHOICE::DIFFICULTY],
		gef::TJ_LEFT,
		"        DIFFICULTY: %s", difficultytext.at(static_cast<int>(difficulty_state_)));

	font_->RenderText(
		sprite_renderer_,
		gef::Vector4(platform_.width() * 0.36f, platform_.height() * 0.56f - 56.0f, -0.99f),
		1.5f,
		textcolors[current_option_menu_choice_ == OPTIONMENUCHOICE::MUSIC],
		gef::TJ_LEFT,
		"     SONG: %s", gamemusictext.at(static_cast<int>(game_music_state)));

	font_->RenderText(
		sprite_renderer_,
		gef::Vector4(platform_.width() * 0.29f, platform_.height() * 0.64f - 56.0f, -0.99f),
		1.5f,
		textcolors[current_option_menu_choice_ == OPTIONMENUCHOICE::CAMERA],
		gef::TJ_LEFT,
		"       Camera:%s", cameraoptiontext.at(static_cast<int>(camera_state_)));

	font_->RenderText(
		sprite_renderer_,
		gef::Vector4(platform_.width() * 0.5f, platform_.height() * 0.72f - 56.0f, -0.99f),
		1.5f,
		textcolors[current_option_menu_choice_ == OPTIONMENUCHOICE::BACK],
		gef::TJ_CENTRE,
		"  BACK");

	sprite_renderer_->End();
}



void SceneApp::GameUpdate(float frame_time)
{
	fps_ = 1.0f / frame_time;
	input_manager_->Update();
	score++;
	boocounter--;

	cameraoptionchoice =
	{
		gef::Vector4(player.GetPlayerBodyX(), player.GetPlayerBodyY() + 10.0f,20.0f,0.0f),
		gef::Vector4(player.GetPlayerBodyX() - 20, player.GetPlayerBodyY() + 10,20.0f,0.0f),
		gef::Vector4(player.GetPlayerBodyX() - 20, player.GetPlayerBodyY() + 10,0.0f,0.0f)
	};

	world_->Step(frame_time, 8, 3);
	gef::Vector4 playerPosition2(player.GetPlayerBodyX(), player.GetPlayerBodyY(), 0.0f);
	camera_eye_transformV4 = cameraoptionchoice.at((static_cast<int>(camera_state_)));
	camera_lookat_transformV4 = gef::Vector4{ player.GetPlayerBodyX(), player.GetPlayerBodyY(),0,0 };
	camera_eye_V4 = camera_eye_transformV4;
	camera_lookat_V4 = camera_lookat_transformV4;


	player.update(frame_time, &score, input_manager_);
	if (score % 300 == 0 && boocounter <= 0)
	{
		audio_manager_->PlaySample(rand() % 6 + 1);
		boocounter = 50;
	}

	if (score  < 4 && boocounter <= 0)
	{
		audio_manager_->PlaySample(rand() % 3 + 8);
		boocounter = 50;
	}

	if (player.GetGoalFinished())
	{
		audio_manager_->LoadMusic("audio/music/end.wav", platform_);
		audio_manager_->PlayMusic();
		game_state_ = GAMESTATE::END;
	}

	if (input_manager_->keyboard()->IsKeyDown(gef::Keyboard::KC_E))
	{
		audio_manager_->LoadMusic("audio/music/end.wav", platform_);
		audio_manager_->PlayMusic();
		game_state_ = GAMESTATE::END;
	}

	if (player.GetPlayerBodyXVecolity() < 4)
	{
		score -= 5;
	}


	if (boocounter < 0)
	{
		boocounter = 0;
	}

	if (score < 0)
	{
		score = 0;
	}



	

	UpdateSimulation(frame_time);

	
}
void SceneApp::GameRender()
{
	
	// projection
	gef::Matrix44 projection_matrix;
	gef::Matrix44 view_matrix;
	projection_matrix = platform_.PerspectiveProjectionFov(camera_fov_, (float)platform_.width() / (float)platform_.height(), near_plane_, far_plane_);
	view_matrix.LookAt(camera_eye_V4, camera_lookat_V4, camera_up_V4);
	renderer_3d_->set_projection_matrix(projection_matrix);
	renderer_3d_->set_view_matrix(view_matrix);

	renderer_3d_->Begin();
		player.Render(renderer_3d_);
		roads.Render(renderer_3d_);
		buildings.Render(renderer_3d_);
		obstacles.Render(renderer_3d_, static_cast<int>(difficulty_state_));
		renderer_3d_->DrawMesh(background_Mesh_Instance);
		renderer_3d_->set_override_material(&primitive_builder_->green_material());
		renderer_3d_->DrawMesh(ground_);
		renderer_3d_->set_override_material(NULL);
	renderer_3d_->End();

	// start drawing sprites, but don't clear the frame buffer
	sprite_renderer_->Begin(false);
	DrawHUD();
	sprite_renderer_->End();
}

void SceneApp::EndUpdate(float frame_time)
{

	if (input_manager_->keyboard()->IsKeyPressed(gef::Keyboard::KC_RETURN))
	{
		switch (game_music_state)
		{
		case GAMEMUSICCHOICE::BREEZY:
			CleanUp();
			Init();
			audio_manager_->LoadMusic("audio/music/breezy.wav", platform_);
			audio_manager_->PlayMusic();
			score = 0;

			game_state_ = GAMESTATE::GAME;
			break;
		case GAMEMUSICCHOICE::RUNBOY:
			CleanUp();
			Init();
			audio_manager_->LoadMusic("audio/music/runboy.wav", platform_);
			audio_manager_->PlayMusic();
			score = 0;
			game_state_ = GAMESTATE::GAME;
			break;
		case GAMEMUSICCHOICE::MIAMI:
			CleanUp();
			Init();
			audio_manager_->LoadMusic("audio/music/miami.wav", platform_);
			audio_manager_->PlayMusic();
			score = 0;
			game_state_ = GAMESTATE::GAME;
			break;

		}
		
	}

	if (input_manager_->keyboard()->IsKeyPressed(gef::Keyboard::KC_ESCAPE))
	{
		CleanUp();
		Init();
		audio_manager_->LoadMusic("audio/music/start.wav", platform_);
		audio_manager_->PlayMusic();
		game_state_ = GAMESTATE::INIT;
	}


	UpdateSimulation(frame_time);

	//if (playerskinned)
	//{
	//	// update the pose in the anim player from the animation
	//	anim_player_.Update(frame_time, playerskinned->bind_pose());

	//	// update the bone matrices that are used for rendering the character
	//	// from the newly updated pose in the anim player
	//	playerskinned->UpdateBoneMatrices(anim_player_.pose());
	//}

}
void SceneApp::EndRender()
{
	gef::Vector4 playerPosition(0.0, 0.0, -200.0f);
	//endplayerpos.SetIdentity();
	//endplayerpos.SetTranslation(playerPosition);
	//playerskinned->set_transform(endplayerpos);

	//camera_eye_V4 = gef::Vector4(0.0f, 0.0f, 300.0f);
	//player_body_->SetTransform(b2Vec2(0.0, 0.0), 0);
	//camera_lookat_V4 = gef::Vector4(0, 0, 0.0f);
	//camera_up_V4 = gef::Vector4(0.0f, 1.0f, 0.0f, 0.0f);

	gef::Matrix44 projection_matrix;
	gef::Matrix44 view_matrix;
	projection_matrix = platform_.PerspectiveProjectionFov(camera_fov_, (float)platform_.width() / (float)platform_.height(), near_plane_, far_plane_);
	view_matrix.LookAt(camera_eye_V4, camera_lookat_V4, camera_up_V4);
	renderer_3d_->set_projection_matrix(projection_matrix);
	renderer_3d_->set_view_matrix(view_matrix);

	renderer_3d_->Begin();

	zombies.Render(renderer_3d_);

	renderer_3d_->End();

	sprite_renderer_->Begin(false);
	font_->RenderText(
		sprite_renderer_,
		gef::Vector4(platform_.width() * 0.5f, platform_.height() * 0.8f - 56.0f, -0.99f),
		2.0f,
		0xff20a5da,
		gef::TJ_CENTRE,
		"Your final score: %i", score);

	font_->RenderText(
		sprite_renderer_,
		gef::Vector4(platform_.width() * 0.5f, platform_.height() * 0.88f - 56.0f, -0.99f),
		1.0f,
		0xff20a5da,
		gef::TJ_CENTRE,
		"Press ENTER to go to a new game", score);

	font_->RenderText(
		sprite_renderer_,
		gef::Vector4(platform_.width() * 0.5f, platform_.height() * 0.92f - 56.0f, -0.99f),
		1.0f,
		0xff20a5da,
		gef::TJ_CENTRE,
		"Press ESCAPE to go to the start menu", score);

	sprite_renderer_->End();



}

void SceneApp::EndInit()
{
	bool endmusicplaying_ = false;

}



void SceneApp::DrawHUD()
{
	if (font_)
	{
		// display frame rate
		font_->RenderText(sprite_renderer_, gef::Vector4(1075.0f, 10.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "Score: %i", score);
		font_->RenderText(sprite_renderer_, gef::Vector4(1075.0f, 900.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "FPS: %.1f", fps_);
		
	}
}
void SceneApp::SetupLights()
{
	// grab the data for the default shader used for rendering 3D geometry
	gef::Default3DShaderData& default_shader_data = renderer_3d_->default_shader_data();

	// set the ambient light
	default_shader_data.set_ambient_light_colour(gef::Colour(0.25f, 0.25f, 0.25f, 1.0f));

	// add a point light that is almost white, but with a blue tinge
	// the position of the light is set far away so it acts light a directional light
	gef::PointLight default_point_light;
	default_point_light.set_colour(gef::Colour(1.0f, 1.0f, 1.0f, 1.0f));
	default_point_light.set_position(gef::Vector4(-500.0f, 400.0f, 700.0f));
	default_shader_data.AddPointLight(default_point_light);


}
void SceneApp::SetupCamera()
{
	// initialise the camera settings
	camera_eye_V4 = gef::Vector4(10.0f, 10.0f, 0.0f);
	camera_lookat_V4 = gef::Vector4(0.0f, 0.0f, 0.0f);
	camera_up_V4 = gef::Vector4(0.0f, 1.0f, 0.0f, 0.0f);
	camera_fov_ = gef::DegToRad(45.0f);
	near_plane_ = 0.01f;
	far_plane_ = 500.f;

	const char* background_asset1 = "background/background.scn";
	background = LoadSceneAssets(platform_, background_asset1);
	background_Mesh_Instance.set_mesh(GetMeshFromSceneAssets(background));
	backgroundM44.SetIdentity();
	backgroundV4 = gef::Vector4(0, 0, -80, 0);
	backgroundM44.SetTranslation(backgroundV4);
	background_Mesh_Instance.set_transform(backgroundM44);
}
void SceneApp::UpdateSimulation(float frame_time)
{
	// update physics world
	float timeStep = 1.0f / 60.0f;

	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	world_->Step(timeStep, velocityIterations, positionIterations);

	// update object visuals from simulation data
	//player_.UpdateFromSimulation(player_body_);

	// don't have to update the ground visuals as it is static

	// collision detection
	// get the head of the contact list
	b2Contact* contact = world_->GetContactList();
	// get contact count
	int contact_count = world_->GetContactCount();

	//for (int contact_num = 0; contact_num < contact_count; ++contact_num)
	//{
	//	if (contact->IsTouching())
	//	{
	//		// get the colliding bodies
	//		b2Body* bodyA = contact->GetFixtureA()->GetBody();
	//		b2Body* bodyB = contact->GetFixtureB()->GetBody();

	//		// DO COLLISION RESPONSE HERE
	//		Player* player = NULL;

	//		GameObject* gameObjectA = NULL;
	//		GameObject* gameObjectB = NULL;

	//		gameObjectA = (GameObject*)bodyA->GetUserData();
	//		gameObjectB = (GameObject*)bodyB->GetUserData();

	//		if (gameObjectA)
	//		{
	//			if (gameObjectA->type() == PLAYER)
	//			{
	//				player = (Player*)bodyA->GetUserData();
	//			}
	//		}

	//		if (gameObjectB)
	//		{
	//			if (gameObjectB->type() == PLAYER)
	//			{
	//				player = (Player*)bodyB->GetUserData();
	//			}
	//		}

	//		if (player)
	//		{
	//			player->DecrementHealth();
	//		}
	//	}

	//	// Get next contact point
	//	contact = contact->GetNext();
	//}
}
void SceneApp::CameraRotateAroundObject(float rotate)
{
	gef::Vector4 offset = camera_lookat_V4 - camera_eye_V4;
	camera_eye_transformM44.RotationY(rotate);
	offset = offset.Transform(camera_eye_transformM44);
	//camera_eye_rotateV4.Transform(camera_eye_transformM44);
	camera_eye_V4 = camera_lookat_V4 - offset;
}

gef::Mesh* SceneApp::GetMeshFromSceneAssets(gef::Scene* scene)
{
	gef::Mesh* mesh = NULL;

	// if the scene data contains at least one mesh
	// return the first mesh
	if (scene && scene->meshes.size() > 0)
		mesh = scene->meshes.front();

	return mesh;
}

gef::Scene* SceneApp::LoadSceneAssets(gef::Platform& platform, const char* filename)
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
void SceneApp::CleanUp()
{
	// destroying the physics world also destroys all the objects within it
	delete world_;
	world_ = NULL;

	delete ground_mesh_;
	ground_mesh_ = NULL;






	CleanUpFont();

	delete audio_manager_;
	audio_manager_ = NULL;

	delete input_manager_;
	input_manager_ = NULL;

	delete primitive_builder_;
	primitive_builder_ = NULL;

	delete renderer_3d_;
	renderer_3d_ = NULL;

	delete sprite_renderer_;
	sprite_renderer_ = NULL;

	player.SetGoalFinished(false);
}
void SceneApp::CleanUpFont()
{
	delete font_;
	font_ = NULL;
}