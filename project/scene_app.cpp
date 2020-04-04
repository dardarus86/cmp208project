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
#include <stdlib.h>
#include <time.h>  

SceneApp::SceneApp(gef::Platform& platform) :
	Application(platform)
	, sprite_renderer_(NULL)
	, renderer_3d_(NULL)
	, primitive_builder_(NULL)
	, input_manager_(NULL)
	, audio_manager_(NULL)
	, font_(NULL)
	, world_(NULL)
	, player_body_(NULL)
	, ninja(NULL)
	, road(NULL)
	, walk_anim_(NULL)
	
{
}

void SceneApp::Init()
{
	game_state_ = INIT;
	difficulty_state_ = DIFF_MEDIUM;
	volume_state_ = VOL_MEDIUM;
	current_start_menu_choice_ = START;
	current_option_menu_choice_ = VOLUME;
	textcolors[0] = 0xffcc0000;
	textcolors[1] = 0xffffffff;
	difficultytext = new std::string[3]{ "EASY","MEDIUM","HARD" };
	gamemusictext = new std::string[3]{ "BREEZY", "RUN BOY", "MIAMI" };
	volumelevel = new int[NUMBER_OF_VOLUME] { 0, 25, 50, 75, 100 };
	gameobstacledistance = new int[NUMBER_OF_DIFFICULTY] { 30, 20, 15 };

	//This was my original code for working out how many obstancles were present depending on the difficulty.
	//gameobstacledifficulty = new int[NUMBER_OF_DIFFICULTY] { 500/35, 500/25, 500/20 };
	   
	GameInit();
	EndInit();
	InitFont();
}

void SceneApp::GameInit()
{

	sprite_renderer_ = gef::SpriteRenderer::Create(platform_);
	renderer_3d_ = gef::Renderer3D::Create(platform_);
	primitive_builder_ = new PrimitiveBuilder(platform_);
	input_manager_ = gef::InputManager::Create(platform_);
	audio_manager_ = gef::AudioManager::Create();
	audio_manager_->LoadMusic("audio/music/start.wav", platform_);
	audio_manager_->LoadSample("audio/soundeffects/menu/select1.wav", platform_);
	audio_manager_->LoadSample("audio/soundeffects/menu/select2.wav", platform_);
	audio_sample_id = -1;
	audio_manager_->PlayMusic();
	audio_manager_->SetMasterVolume(volumelevel[volume_state_]);
	srand(time(NULL));
	b2Vec2 gravity(0.0f, -9.81f);
	world_ = new b2World(gravity);
	score = 0;
	counter = 0;

	InitPlayer();
	InitGround();
	InitRoad();
	InitBuildings();

	SetupLights();
	SetupCamera();

	issliding_ = false;

	volume_state_ = VOL_MEDIUM;
	difficulty_state_ = DIFF_MEDIUM;



}
void SceneApp::EndInit()
{
	bool endmusicplaying_ = false;
}
void SceneApp::InitPlayer()
{
	ninja = new gef::Scene();
	playerscalingV4 = gef::Vector4{ 0.01f,0.01f,0.01f,0 };

	player_scaleM44.Scale(playerscalingV4);


	ninja->ReadSceneFromFile(platform_, "player/ninja.scn");
	ninja->CreateMaterials(platform_);

	player_mesh_ = GetFirstMesh(ninja);

	gef::Skeleton* skeleton = GetFirstSkeleton(ninja);

	if (skeleton)
	{
		playerskinned = new gef::SkinnedMeshInstance(*skeleton);
		anim_player_.Init(playerskinned->bind_pose());
		playerskinned->set_mesh(player_mesh_);

	}
	playerskinned->set_transform(player_scaleM44);

	walk_anim_ = LoadAnimation("player/ninjarun.scn", "");
	slide_anim_ = LoadAnimation("player/ninjafly.scn", "");
	up_anim_ = LoadAnimation("player/ninjaup.scn", "");
	down_anim_ = LoadAnimation("player/ninjadown.scn", "");
	dance_anim_ = LoadAnimation("player/ninjadance.scn", "");


	if (walk_anim_)
	{
		anim_player_.set_clip(walk_anim_);
		anim_player_.set_looping(true);
		anim_player_.set_anim_time(0.0f);
	}

	// create a physics body for the player

	player_body_def.type = b2_dynamicBody;
	player_body_def.position = b2Vec2(0.0f, 0.0f);
	player_body_ = world_->CreateBody(&player_body_def);

	// create the shape for the player

	player_shape.SetAsBox(0.37f, 0.40f);

	// create the fixture

	player_fixture_def.shape = &player_shape;
	player_fixture_def.density = 0.8f;
	player_fixture_def.friction = 0.2f;

	// create the fixture on the rigid body
	player_body_->CreateFixture(&player_fixture_def);








	// update visuals from simulation data
	//player_.UpdateFromSimulation(player_body_);
	vel = player_body_->GetLinearVelocity();
	// create a connection between the rigid body and GameObject
	player_body_->SetUserData(&playerskinned);

}
void SceneApp::InitGround()
{
	// ground dimensions
	gef::Vector4 ground_half_dimensions(1000.0f, 0.1f, 50.0f);
	b2Vec2 ground_start_position(990.0f, -0.55f);
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
void SceneApp::InitRoad()
{
	//20 roads
	military_Road_Distance_Counter = 0;
	//20roads
	samurai_Road_Distance_Counter = 400;
	//20 roads
	apoc_Road_Distance_Counter = 800;

	//random number for picking road elements at random out of 5 choices



	const char* samurairoad_asset1 = "samurai/road/road1.scn";
	const char* samurairoad_asset2 = "samurai/road/road2.scn";
	const char* samurairoad_asset3 = "samurai/road/road3.scn";
	const char* samurairoad_asset4 = "samurai/road/road4.scn";
	const char* samurairoad_asset5 = "samurai/road/road5.scn";
	const char* miliroad_asset1 = "military/road/road1.scn";
	const char* miliroad_asset2 = "military/road/road2.scn";
	const char* miliroad_asset3 = "military/road/road3.scn";
	const char* miliroad_asset4 = "military/road/road4.scn";
	const char* miliroad_asset5 = "military/road/road5.scn";
	const char* apocroad_asset1 = "apoc/road/road1.scn";
	const char* apocroad_asset2 = "apoc/road/road2.scn";
	const char* apocroad_asset3 = "apoc/road/road3.scn";
	const char* apocroad_asset4 = "apoc/road/road4.scn";
	const char* apocroad_asset5 = "apoc/road/road5.scn";

	//military
	military_Roads[0] = LoadSceneAssets(platform_, miliroad_asset1);
	military_Roads[1] = LoadSceneAssets(platform_, miliroad_asset2);
	military_Roads[2] = LoadSceneAssets(platform_, miliroad_asset3);
	military_Roads[3] = LoadSceneAssets(platform_, miliroad_asset4);
	military_Roads[4] = LoadSceneAssets(platform_, miliroad_asset5);
	samurai_Roads[0] = LoadSceneAssets(platform_, samurairoad_asset1);
	samurai_Roads[1] = LoadSceneAssets(platform_, samurairoad_asset2);
	samurai_Roads[2] = LoadSceneAssets(platform_, samurairoad_asset3);
	samurai_Roads[3] = LoadSceneAssets(platform_, samurairoad_asset4);
	samurai_Roads[4] = LoadSceneAssets(platform_, samurairoad_asset5);
	apoc_Roads[0] = LoadSceneAssets(platform_, apocroad_asset1);
	apoc_Roads[1] = LoadSceneAssets(platform_, apocroad_asset2);
	apoc_Roads[2] = LoadSceneAssets(platform_, apocroad_asset3);
	apoc_Roads[3] = LoadSceneAssets(platform_, apocroad_asset4);
	apoc_Roads[4] = LoadSceneAssets(platform_, apocroad_asset5);



	for (int i = 0; i < 20; i++)
	{
		int road_Random_Number = rand() % 4;
		military_Road_Mesh_Instance_M44[i].SetIdentity();
		samurai_Road_Mesh_Instance_M44[i].SetIdentity();
		apoc_Road_MeshInstance_M44[i].SetIdentity();

		militaryRoad_Mesh_Instance[i].set_mesh(GetMeshFromSceneAssets(military_Roads[road_Random_Number]));
		samurai_Road_Mesh_Instance[i].set_mesh(GetMeshFromSceneAssets(samurai_Roads[road_Random_Number]));
		apoc_Road_Mesh_Instance[i].set_mesh(GetMeshFromSceneAssets(apoc_Roads[road_Random_Number]));

		military_Road_Mesh_Instance_V4[i] = gef::Vector4(military_Road_Distance_Counter, 0, 0, 0);
		samurairoadmeshinstanceV4[i] = gef::Vector4(samurai_Road_Distance_Counter, -0.4, 0, 0);
		apocroadmeshinstanceV4[i] = gef::Vector4(apoc_Road_Distance_Counter, 0, 0, 0);

		military_Road_Mesh_Instance_M44[i].SetTranslation(military_Road_Mesh_Instance_V4[i]);
		samurai_Road_Mesh_Instance_M44[i].SetTranslation(samurairoadmeshinstanceV4[i]);
		apoc_Road_MeshInstance_M44[i].SetTranslation(apocroadmeshinstanceV4[i]);

		militaryRoad_Mesh_Instance[i].set_transform(military_Road_Mesh_Instance_M44[i]);
		samurai_Road_Mesh_Instance[i].set_transform(samurai_Road_Mesh_Instance_M44[i]);
		apoc_Road_Mesh_Instance[i].set_transform(apoc_Road_MeshInstance_M44[i]);

		military_Road_Distance_Counter = military_Road_Distance_Counter + 20;
		samurai_Road_Distance_Counter = samurai_Road_Distance_Counter + 20;
		apoc_Road_Distance_Counter = apoc_Road_Distance_Counter + 20;
	}

	/*militaryroadmeshinstance[1].set_mesh(GetMeshFromSceneAssets(militaryroads[1]));
	militaryroadmeshinstance[2].set_mesh(GetMeshFromSceneAssets(militaryroads[2]));
	militaryroadmeshinstance[3].set_mesh(GetMeshFromSceneAssets(militaryroads[3]));
	militaryroadmeshinstance[4].set_mesh(GetMeshFromSceneAssets(militaryroads[4]));
	militaryroadmeshinstanceM44[1].SetIdentity();
	militaryroadmeshinstanceM44[2].SetIdentity();
	militaryroadmeshinstanceM44[3].SetIdentity();
	militaryroadmeshinstanceM44[4].SetIdentity();
	militaryroadmeshinstanceV4[1] = gef::Vector4(0, 0, 0, 0);
	militaryroadmeshinstanceV4[1] = gef::Vector4(20, 0, 0, 0);
	militaryroadmeshinstanceV4[2] = gef::Vector4(40, 0, 0, 0);
	militaryroadmeshinstanceV4[3] = gef::Vector4(60, 0, 0, 0);
	militaryroadmeshinstanceV4[4] = gef::Vector4(80, 0, 0, 0);
	militaryroadmeshinstanceM44[1].SetTranslation(militaryroadmeshinstanceV4[1]);
	militaryroadmeshinstanceM44[2].SetTranslation(militaryroadmeshinstanceV4[2]);
	militaryroadmeshinstanceM44[3].SetTranslation(militaryroadmeshinstanceV4[3]);
	militaryroadmeshinstanceM44[4].SetTranslation(militaryroadmeshinstanceV4[4]);
	militaryroadmeshinstance[1].set_transform(militaryroadmeshinstanceM44[1]);
	militaryroadmeshinstance[2].set_transform(militaryroadmeshinstanceM44[2]);
	militaryroadmeshinstance[3].set_transform(militaryroadmeshinstanceM44[3]);
	militaryroadmeshinstance[4].set_transform(militaryroadmeshinstanceM44[4]);

	//samurai
	samurairoadmeshinstance[0].set_mesh(GetMeshFromSceneAssets(samurairoads[0]));
	samurairoadmeshinstance[1].set_mesh(GetMeshFromSceneAssets(samurairoads[1]));
	samurairoadmeshinstance[2].set_mesh(GetMeshFromSceneAssets(samurairoads[2]));
	samurairoadmeshinstance[3].set_mesh(GetMeshFromSceneAssets(samurairoads[3]));
	samurairoadmeshinstance[4].set_mesh(GetMeshFromSceneAssets(samurairoads[4]));
	samurairoadmeshinstanceM44[0].SetIdentity();
	samurairoadmeshinstanceM44[1].SetIdentity();
	samurairoadmeshinstanceM44[2].SetIdentity();
	samurairoadmeshinstanceM44[3].SetIdentity();
	samurairoadmeshinstanceM44[4].SetIdentity();
	samurairoadmeshinstanceV4[0] = gef::Vector4(100,0,0,0);
	samurairoadmeshinstanceV4[1] = gef::Vector4(120,0,0,0);
	samurairoadmeshinstanceV4[2] = gef::Vector4(140,0,0,0);
	samurairoadmeshinstanceV4[3] = gef::Vector4(160,0,0,0);
	samurairoadmeshinstanceV4[4] = gef::Vector4(180,0,0,0);
	samurairoadmeshinstanceM44[0].SetTranslation(samurairoadmeshinstanceV4[0]);
	samurairoadmeshinstanceM44[1].SetTranslation(samurairoadmeshinstanceV4[1]);
	samurairoadmeshinstanceM44[2].SetTranslation(samurairoadmeshinstanceV4[2]);
	samurairoadmeshinstanceM44[3].SetTranslation(samurairoadmeshinstanceV4[3]);
	samurairoadmeshinstanceM44[4].SetTranslation(samurairoadmeshinstanceV4[4]);
	samurairoadmeshinstance[0].set_transform(samurairoadmeshinstanceM44[0]);
	samurairoadmeshinstance[1].set_transform(samurairoadmeshinstanceM44[1]);
	samurairoadmeshinstance[2].set_transform(samurairoadmeshinstanceM44[2]);
	samurairoadmeshinstance[3].set_transform(samurairoadmeshinstanceM44[3]);
	samurairoadmeshinstance[4].set_transform(samurairoadmeshinstanceM44[4]);


	//apoc

	apocroadmeshinstance[0].set_mesh(GetMeshFromSceneAssets(apocroads[0]));
	apocroadmeshinstance[1].set_mesh(GetMeshFromSceneAssets(apocroads[1]));
	apocroadmeshinstance[2].set_mesh(GetMeshFromSceneAssets(apocroads[2]));
	apocroadmeshinstance[3].set_mesh(GetMeshFromSceneAssets(apocroads[3]));
	apocroadmeshinstance[4].set_mesh(GetMeshFromSceneAssets(apocroads[4]));
	//apocroadmeshinstanceM44[0].SetIdentity();
	//apocroadmeshinstanceM44[1].SetIdentity();
	//apocroadmeshinstanceM44[2].SetIdentity();
	//apocroadmeshinstanceM44[3].SetIdentity();
	//apocroadmeshinstanceM44[4].SetIdentity();
	//apocroadmeshinstanceV4[0] = gef::Vector4(200, 0, 0, 0);
	//apocroadmeshinstanceV4[1] = gef::Vector4(220, 0, 0, 0);
	//apocroadmeshinstanceV4[2] = gef::Vector4(240, 0, 0, 0);
	//apocroadmeshinstanceV4[3] = gef::Vector4(260, 0, 0, 0);
	//apocroadmeshinstanceV4[4] = gef::Vector4(280, 0, 0, 0);
	apocroadmeshinstanceM44[0].SetTranslation(apocroadmeshinstanceV4[0]);
	apocroadmeshinstanceM44[1].SetTranslation(apocroadmeshinstanceV4[1]);
	apocroadmeshinstanceM44[2].SetTranslation(apocroadmeshinstanceV4[2]);
	apocroadmeshinstanceM44[3].SetTranslation(apocroadmeshinstanceV4[3]);
	apocroadmeshinstanceM44[4].SetTranslation(apocroadmeshinstanceV4[4]);
	apocroadmeshinstance[0].set_transform(apocroadmeshinstanceM44[0]);
	apocroadmeshinstance[1].set_transform(apocroadmeshinstanceM44[1]);
	apocroadmeshinstance[2].set_transform(apocroadmeshinstanceM44[2]);
	apocroadmeshinstance[3].set_transform(apocroadmeshinstanceM44[3]);
	apocroadmeshinstance[4].set_transform(apocroadmeshinstanceM44[4]);*/
}
void SceneApp::InitBuildings()
{
	//11 buildings
	military_Building_Distance_Counter = 20;
	//11 buildings
	samurai_Building_Distance_Counter = 420;
	//11 buildings
	apoc_Building_Distance_Counter = 820;
	const char* samuraibuilding_asset1 = "samurai/building/building1.scn";
	const char* samuraibuilding_asset2 = "samurai/building/building2.scn";
	const char* samuraibuilding_asset3 = "samurai/building/building3.scn";
	const char* samuraibuilding_asset4 = "samurai/building/building4.scn";
	const char* samuraibuilding_asset5 = "samurai/building/building5.scn";
	const char* milibuilding_asset1 = "military/building/building1.scn";
	const char* milibuilding_asset2 = "military/building/building2.scn";
	const char* milibuilding_asset3 = "military/building/building3.scn";
	const char* milibuilding_asset4 = "military/building/building4.scn";
	const char* milibuilding_asset5 = "military/building/building5.scn";
	const char* apocbuilding_asset1 = "apoc/building/building1.scn";
	const char* apocbuilding_asset2 = "apoc/building/building2.scn";
	const char* apocbuilding_asset3 = "apoc/building/building3.scn";
	const char* apocbuilding_asset4 = "apoc/building/building4.scn";
	const char* apocbuilding_asset5 = "apoc/building/building5.scn";

	military_Buildings[0] = LoadSceneAssets(platform_, milibuilding_asset1);
	military_Buildings[1] = LoadSceneAssets(platform_, milibuilding_asset2);
	military_Buildings[2] = LoadSceneAssets(platform_, milibuilding_asset3);
	military_Buildings[3] = LoadSceneAssets(platform_, milibuilding_asset4);
	military_Buildings[4] = LoadSceneAssets(platform_, milibuilding_asset5);
	samurai_Buildings[0] = LoadSceneAssets(platform_, samuraibuilding_asset1);
	samurai_Buildings[1] = LoadSceneAssets(platform_, samuraibuilding_asset2);
	samurai_Buildings[2] = LoadSceneAssets(platform_, samuraibuilding_asset3);
	samurai_Buildings[3] = LoadSceneAssets(platform_, samuraibuilding_asset4);
	samurai_Buildings[4] = LoadSceneAssets(platform_, samuraibuilding_asset5);
	apoc_Buildings[0] = LoadSceneAssets(platform_, apocbuilding_asset1);
	apoc_Buildings[1] = LoadSceneAssets(platform_, apocbuilding_asset2);
	apoc_Buildings[2] = LoadSceneAssets(platform_, apocbuilding_asset3);
	apoc_Buildings[3] = LoadSceneAssets(platform_, apocbuilding_asset4);
	apoc_Buildings[4] = LoadSceneAssets(platform_, apocbuilding_asset5);

	for (int i = 0; i < 12; i++)
	{
		int building_Random_Number = rand() % 4;
		int building_Random_Disantce_Number = rand() % 10 + 30;

		military_Building_Mesh_Instance[i].set_mesh(GetMeshFromSceneAssets(military_Buildings[building_Random_Number]));
		samurai_Building_Mesh_Instance[i].set_mesh(GetMeshFromSceneAssets(samurai_Buildings[building_Random_Number]));
		apoc_Building_Mesh_Instance[i].set_mesh(GetMeshFromSceneAssets(apoc_Buildings[building_Random_Number]));

		military_Buildings_Mesh_Instance_M44[i].SetIdentity();
		samurai_Buildings_Mesh_Instance_M44[i].SetIdentity();
		apoc_Buildings_Mesh_Instance_M44[i].SetIdentity();

		military_Buildings_Mesh_Instance_V4[i] = gef::Vector4(military_Building_Distance_Counter - 10, 0, 0, 0);
		samurai_Buildings_Mesh_Instance_V4[i] = gef::Vector4(samurai_Building_Distance_Counter, 0, 0, 0);
		apoc_Buildings_Mesh_Instance_V4[i] = gef::Vector4(apoc_Building_Distance_Counter, 0, 0, 0);

		military_Buildings_Mesh_Instance_M44[i].SetTranslation(military_Buildings_Mesh_Instance_V4[i]);
		samurai_Buildings_Mesh_Instance_M44[i].SetTranslation(samurai_Buildings_Mesh_Instance_V4[i]);
		apoc_Buildings_Mesh_Instance_M44[i].SetTranslation(apoc_Buildings_Mesh_Instance_V4[i]);

		military_Building_Mesh_Instance[i].set_transform(military_Buildings_Mesh_Instance_M44[i]);
		samurai_Building_Mesh_Instance[i].set_transform(samurai_Buildings_Mesh_Instance_M44[i]);
		apoc_Building_Mesh_Instance[i].set_transform(apoc_Buildings_Mesh_Instance_M44[i]);

		military_Building_Distance_Counter = military_Building_Distance_Counter + building_Random_Disantce_Number;
		samurai_Building_Distance_Counter = samurai_Building_Distance_Counter + building_Random_Disantce_Number;
		apoc_Building_Distance_Counter = apoc_Building_Distance_Counter + building_Random_Disantce_Number;
	}


	/*militarybuildingmeshinstance[1].set_mesh(GetMeshFromSceneAssets(militarybuildings[1]));
	militarybuildingmeshinstance[2].set_mesh(GetMeshFromSceneAssets(militarybuildings[2]));
	militarybuildingmeshinstance[3].set_mesh(GetMeshFromSceneAssets(militarybuildings[3]));
	militarybuildingmeshinstance[4].set_mesh(GetMeshFromSceneAssets(militarybuildings[4]));
	militarybuildingsmeshinstanceM44[0].SetIdentity();
	militarybuildingsmeshinstanceM44[1].SetIdentity();
	militarybuildingsmeshinstanceM44[2].SetIdentity();
	militarybuildingsmeshinstanceM44[3].SetIdentity();
	militarybuildingsmeshinstanceM44[4].SetIdentity();
	militarybuildingsmeshinstanceV4[0] = gef::Vector4(10, 0, 0, 0);
	militarybuildingsmeshinstanceV4[1] = gef::Vector4(30, 0, 0, 0);
	militarybuildingsmeshinstanceV4[2] = gef::Vector4(50, 0, 0, 0);
	militarybuildingsmeshinstanceV4[3] = gef::Vector4(70, 0, 0, 0);
	militarybuildingsmeshinstanceV4[4] = gef::Vector4(90, 0, 0, 0);
	militarybuildingsmeshinstanceM44[0].SetTranslation(militarybuildingsmeshinstanceV4[0]);
	militarybuildingsmeshinstanceM44[1].SetTranslation(militarybuildingsmeshinstanceV4[1]);
	militarybuildingsmeshinstanceM44[2].SetTranslation(militarybuildingsmeshinstanceV4[2]);
	militarybuildingsmeshinstanceM44[3].SetTranslation(militarybuildingsmeshinstanceV4[3]);
	militarybuildingsmeshinstanceM44[4].SetTranslation(militarybuildingsmeshinstanceV4[4]);
	militarybuildingmeshinstance[0].set_transform(militarybuildingsmeshinstanceM44[0]);
	militarybuildingmeshinstance[1].set_transform(militarybuildingsmeshinstanceM44[1]);
	militarybuildingmeshinstance[2].set_transform(militarybuildingsmeshinstanceM44[2]);
	militarybuildingmeshinstance[3].set_transform(militarybuildingsmeshinstanceM44[3]);
	militarybuildingmeshinstance[4].set_transform(militarybuildingsmeshinstanceM44[4]);
	samuraibuildingmeshinstance[0].set_mesh(GetMeshFromSceneAssets(samuraibuildings[0]));
	samuraibuildingmeshinstance[1].set_mesh(GetMeshFromSceneAssets(samuraibuildings[1]));
	samuraibuildingmeshinstance[2].set_mesh(GetMeshFromSceneAssets(samuraibuildings[2]));
	samuraibuildingmeshinstance[3].set_mesh(GetMeshFromSceneAssets(samuraibuildings[3]));
	samuraibuildingmeshinstance[4].set_mesh(GetMeshFromSceneAssets(samuraibuildings[4]));
	samuraibuildingsmeshinstanceM44[0].SetIdentity();
	samuraibuildingsmeshinstanceM44[1].SetIdentity();
	samuraibuildingsmeshinstanceM44[2].SetIdentity();
	samuraibuildingsmeshinstanceM44[3].SetIdentity();
	samuraibuildingsmeshinstanceM44[4].SetIdentity();
	samuraibuildingsmeshinstanceV4[0] = gef::Vector4(110, 0, 0, 0);
	samuraibuildingsmeshinstanceV4[1] = gef::Vector4(130, 0, 0, 0);
	samuraibuildingsmeshinstanceV4[2] = gef::Vector4(150, 0, 0, 0);
	samuraibuildingsmeshinstanceV4[3] = gef::Vector4(170, 0, 0, 0);
	samuraibuildingsmeshinstanceV4[4] = gef::Vector4(190, 0, 0, 0);
	samuraibuildingsmeshinstanceM44[0].SetTranslation(samuraibuildingsmeshinstanceV4[0]);
	samuraibuildingsmeshinstanceM44[1].SetTranslation(samuraibuildingsmeshinstanceV4[1]);
	samuraibuildingsmeshinstanceM44[2].SetTranslation(samuraibuildingsmeshinstanceV4[2]);
	samuraibuildingsmeshinstanceM44[3].SetTranslation(samuraibuildingsmeshinstanceV4[3]);
	samuraibuildingsmeshinstanceM44[4].SetTranslation(samuraibuildingsmeshinstanceV4[4]);
	samuraibuildingmeshinstance[0].set_transform(samuraibuildingsmeshinstanceM44[0]);
	samuraibuildingmeshinstance[1].set_transform(samuraibuildingsmeshinstanceM44[1]);
	samuraibuildingmeshinstance[2].set_transform(samuraibuildingsmeshinstanceM44[2]);
	samuraibuildingmeshinstance[3].set_transform(samuraibuildingsmeshinstanceM44[3]);
	samuraibuildingmeshinstance[4].set_transform(samuraibuildingsmeshinstanceM44[4]);
	apocbuildingmeshinstance[0].set_mesh(GetMeshFromSceneAssets(apocbuildings[0]));
	apocbuildingmeshinstance[1].set_mesh(GetMeshFromSceneAssets(apocbuildings[1]));
	apocbuildingmeshinstance[2].set_mesh(GetMeshFromSceneAssets(apocbuildings[2]));
	apocbuildingmeshinstance[3].set_mesh(GetMeshFromSceneAssets(apocbuildings[3]));
	apocbuildingmeshinstance[4].set_mesh(GetMeshFromSceneAssets(apocbuildings[4]));
	apocbuildingsmeshinstanceM44[0].SetIdentity();
	apocbuildingsmeshinstanceM44[1].SetIdentity();
	apocbuildingsmeshinstanceM44[2].SetIdentity();
	apocbuildingsmeshinstanceM44[3].SetIdentity();
	apocbuildingsmeshinstanceM44[4].SetIdentity();
	apocbuildingsmeshinstanceV4[0] = gef::Vector4(210, 0, 0, 0);
	apocbuildingsmeshinstanceV4[1] = gef::Vector4(230, 0, 0, 0);
	apocbuildingsmeshinstanceV4[2] = gef::Vector4(250, 0, 0, 0);
	apocbuildingsmeshinstanceV4[3] = gef::Vector4(270, 0, 0, 0);
	apocbuildingsmeshinstanceV4[4] = gef::Vector4(290, 0, 0, 0);
	apocbuildingsmeshinstanceM44[0].SetTranslation(apocbuildingsmeshinstanceV4[0]);
	apocbuildingsmeshinstanceM44[1].SetTranslation(apocbuildingsmeshinstanceV4[1]);
	apocbuildingsmeshinstanceM44[2].SetTranslation(apocbuildingsmeshinstanceV4[2]);
	apocbuildingsmeshinstanceM44[3].SetTranslation(apocbuildingsmeshinstanceV4[3]);
	apocbuildingsmeshinstanceM44[4].SetTranslation(apocbuildingsmeshinstanceV4[4]);
	apocbuildingmeshinstance[0].set_transform(apocbuildingsmeshinstanceM44[0]);
	apocbuildingmeshinstance[1].set_transform(apocbuildingsmeshinstanceM44[1]);
	apocbuildingmeshinstance[2].set_transform(apocbuildingsmeshinstanceM44[2]);
	apocbuildingmeshinstance[3].set_transform(apocbuildingsmeshinstanceM44[3]);
	apocbuildingmeshinstance[4].set_transform(apocbuildingsmeshinstanceM44[4]);*/

}
void SceneApp::InitObstacles()
{
	const char* startscreen_asset = "test/startsscreenmodel.scn";
	startscreen = LoadSceneAssets(platform_, startscreen_asset);
	startscreen_Mesh_Instance.set_mesh(GetMeshFromSceneAssets(startscreen));
	startscreen_Mesh_Instance_M44.SetIdentity();
	startscreen_Mesh_Instance_V4 = gef::Vector4(0, 0, 0, 0);
	startscreen_Mesh_Instance_M44.SetTranslation(startscreen_Mesh_Instance_V4);
	startscreen_Mesh_Instance.set_transform(startscreen_Mesh_Instance_M44);

	military_Obstacle_Distance_Counter = 30;
	samurai_Obstacle_Distance_Counter = 400;
	apoc_Obstacle_Distance_Counter = 800;

	const char* samurai_obstacle_asset[4] = {
	"samurai/obstacles/small.scn",
	"samurai/obstacles/large.scn",
	"samurai/obstacles/gong.scn",
	"samurai/obstacles/lion.scn"
	};

	const char* military_obstacle_asset[4] = {
	"military/obstacles/boxes.scn",
	"military/obstacles/apc.scn",
	"military/obstacles/medical.scn",
	"military/obstacles/rockets.scn"
	};

	const char* apoc_obstacle_asset[4] =
	{
	"apoc/obstacles/car.scn",
	"apoc/obstacles/truck.scn",
	"apoc/obstacles/bodies.scn",
	"apoc/obstacles/billboard.scn"
	};

	for (int i = 0; i < 4; i++)
	{

		military_Obstacle[i] = LoadSceneAssets(platform_, military_obstacle_asset[i]);
		samurai_Obstacle[i] = LoadSceneAssets(platform_, samurai_obstacle_asset[i]);
		apoc_Obstacle[i] = LoadSceneAssets(platform_, apoc_obstacle_asset[i]);
	}
	
	for (int i = 0, total_distance = 0; ; i++)
	{
		int obstacle_Random_Number = rand() % 4;
		int obstacle_Random_Distance = rand() % 10 + gameobstacledistance[difficulty_state_];
		total_distance += obstacle_Random_Distance;
		if (total_distance > 380)
			break;
		int obstacle_Up_Or_Down = rand() % 100;
		if (obstacle_Up_Or_Down > 50)
		{
			obstacle_Height = 2;
		}
		else
		{
			obstacle_Height = 0;
		}
		apoc_Obstacle_Mesh_Instance[i].set_mesh(GetMeshFromSceneAssets(apoc_Obstacle[obstacle_Random_Number]));
		apoc_Obstacle_Mesh_Instance_M44[i].SetIdentity();
		apocobstaclemeshinstanceV4[i] = gef::Vector4(apoc_Obstacle_Distance_Counter, obstacle_Height, 0, 0);
		apoc_Obstacle_Mesh_Instance_M44[i].SetTranslation(apocobstaclemeshinstanceV4[i]);
		apoc_Obstacle_Mesh_Instance[i].set_transform(apoc_Obstacle_Mesh_Instance_M44[i]);
		apoc_Obstacle_Position_V2vec2[i] = b2Vec2(apoc_Obstacle_Distance_Counter, obstacle_Height + 1);
		apoc_Body_Def[i].type = b2_staticBody;
		apoc_Body_Def[i].position = apoc_Obstacle_Position_V2vec2[i];
		apoc_Obstacle_B2B[i] = world_->CreateBody(&apoc_Body_Def[i]);
		apoc_Shape.SetAsBox(1.0, 2.7);
		apoc_Obstacle_Fixture_Def[i].shape = &apoc_Shape;
		apoc_Obstacle_B2B[i]->CreateFixture(&apoc_Obstacle_Fixture_Def[i]);
		apoc_Obstacle_Distance_Counter = apoc_Obstacle_Distance_Counter + obstacle_Random_Distance;

	}
	for (int i = 0, total_distance = 0; ; i++)
	{
		int obstacle_Random_Number = rand() % 4;
		int obstacle_Random_Distance = rand() % 10 + gameobstacledistance[difficulty_state_];
		total_distance += obstacle_Random_Distance;
		if (total_distance > 380)
			break;
		int obstacle_Up_Or_Down = rand() % 100;
		if (obstacle_Up_Or_Down > 50)
		{
			obstacle_Height = 2;
		}
		else
		{
			obstacle_Height = 0;
		}
		military_Obstacle_Mesh_Instance[i].set_mesh(GetMeshFromSceneAssets(military_Obstacle[obstacle_Random_Number]));
		military_Obstacle_Mesh_Instance_M44[i].SetIdentity();

		militaryobstaclemeshinstanceV4[i] = gef::Vector4(military_Obstacle_Distance_Counter, obstacle_Height, 0, 0);

		military_Obstacle_Mesh_Instance_M44[i].SetTranslation(militaryobstaclemeshinstanceV4[i]);

		military_Obstacle_Mesh_Instance[i].set_transform(military_Obstacle_Mesh_Instance_M44[i]);

		military_Obstacle_Position_B2vec2[i] = b2Vec2(military_Obstacle_Distance_Counter, obstacle_Height + 1);
		military_Body_Def[i].type = b2_staticBody;

		military_Body_Def[i].position = military_Obstacle_Position_B2vec2[i];
		military_Obstacle_B2B[i] = world_->CreateBody(&military_Body_Def[i]);
		military_Shape.SetAsBox(1.0, 2.7);
		military_Obstacle_Fixture_Def[i].shape = &military_Shape;
		military_Obstacle_B2B[i]->CreateFixture(&military_Obstacle_Fixture_Def[i]);


		military_Obstacle_Distance_Counter = military_Obstacle_Distance_Counter + obstacle_Random_Distance;


	}
	for (int i = 0, total_distance = 0; ; i++)
	{
		int obstacle_Random_Number = rand() % 4;
		int obstacle_Random_Distance = rand() % 10 + gameobstacledistance[difficulty_state_];
		total_distance += obstacle_Random_Distance;
		if (total_distance > 380)
			break;
		int obstacle_Up_Or_Down = rand() % 100;
		if (obstacle_Up_Or_Down > 50)
		{
			obstacle_Height = 2;
		}
		else
		{
			obstacle_Height = 0;
		}
		samurai_Obstacle_Mesh_Instance[i].set_mesh(GetMeshFromSceneAssets(samurai_Obstacle[obstacle_Random_Number]));
		samurai_Obstacle_Mesh_Instance_M44[i].SetIdentity();
		samuraiobstaclemeshinstanceV4[i] = gef::Vector4(samurai_Obstacle_Distance_Counter, obstacle_Height, 0, 0);
		samurai_Obstacle_Mesh_Instance_M44[i].SetTranslation(samuraiobstaclemeshinstanceV4[i]);
		samurai_Obstacle_Mesh_Instance[i].set_transform(samurai_Obstacle_Mesh_Instance_M44[i]);
		samurai_Obstacle_Position_B2vec2[i] = b2Vec2(samurai_Obstacle_Distance_Counter, obstacle_Height + 1);
		samurai_Body_Def[i].type = b2_staticBody;
		samurai_Body_Def[i].position = samurai_Obstacle_Position_B2vec2[i];
		samurai_Obstacle_B2B[i] = world_->CreateBody(&samurai_Body_Def[i]);
		samurai_Shape.SetAsBox(1.0, 2.7);
		samurai_Obstacle_Fixture_Def[i].shape = &military_Shape;
		samurai_Obstacle_B2B[i]->CreateFixture(&samurai_Obstacle_Fixture_Def[i]);
		samurai_Obstacle_Distance_Counter = samurai_Obstacle_Distance_Counter + obstacle_Random_Distance;

	}
}
void SceneApp::InitFont()
{
	font_ = new gef::Font(platform_);
	font_->Load("comic_sans");
}

bool SceneApp::Update(float frame_time)
{
	fps_ = 1.0f / frame_time;


	input_manager_->Update();

	switch (game_state_)
	{
	case INIT:
		FrontendUpdate(frame_time);
		break;
	case OPTIONS:
		OptionsUpdate(frame_time);
		break;
	case GAME:
		GameUpdate(frame_time);
		break;
	case END:
		EndUpdate(frame_time);
		break;
	}

	return true;
}
void SceneApp::Render()
{
	switch (game_state_)
	{
	case INIT:
		FrontendRender();
		break;
	case OPTIONS:
		OptionsRender();
		break;
	case GAME:
		GameRender();
		break;
	case END:
		EndRender();
		break;

	}
}

void SceneApp::FrontendInit()
{

	const char* start_model_asset = "start/startmodel.scn";
	start_model = LoadSceneAssets(platform_, start_model_asset);
	startmodelM44.SetIdentity();
	startmodel.set_mesh(GetMeshFromSceneAssets(start_model));
	startmodelV4 = gef::Vector4(0, 0, 0, 0);
	startmodelM44.SetTranslation(startmodelV4);
	startmodel.set_transform(startmodelM44);


}

void SceneApp::FrontendUpdate(float frame_time)
{



	if (input_manager_->keyboard()->IsKeyPressed(gef::Keyboard::KC_DOWN))
	{
		audio_manager_->PlaySample(0);
		current_start_menu_choice_ = SceneApp::STARTMENUCHOICE((current_start_menu_choice_ + 1) % START_MENU_CHOICES);
	}


	if (input_manager_->keyboard()->IsKeyPressed(gef::Keyboard::KC_UP))
	{
		audio_manager_->PlaySample(0);
		current_start_menu_choice_ = SceneApp::STARTMENUCHOICE((current_start_menu_choice_ -1 + START_MENU_CHOICES) % START_MENU_CHOICES);
	}

	if (input_manager_->keyboard()->IsKeyPressed(gef::Keyboard::KC_RETURN))
	{
		switch (current_start_menu_choice_)
		{
		case START:
			InitObstacles();
			switch (game_music_state)
			{
			case BREEZY:
				audio_manager_->LoadMusic("audio/music/breezy.wav", platform_);
				break;
			case RUNBOY:
				audio_manager_->LoadMusic("audio/music/runboy.wav", platform_);
				break;
			case MIAMI:
				audio_manager_->LoadMusic("audio/music/miami.wav", platform_);
				break;

			}
			
			audio_manager_->PlayMusic();
			game_state_ = GAME;
			break;
		case OPTION:
			game_state_ = OPTIONS;
			break;
		case QUIT:
			//to quit app
			break;
		}
	}


}
void SceneApp::FrontendRender()
{

	sprite_renderer_->Begin();

	font_->RenderText(
		sprite_renderer_,
		gef::Vector4(platform_.width() * 0.5f, platform_.height() * 0.5f - 56.0f, -0.99f),
		1.0f,
		textcolors[current_start_menu_choice_ == START],
		gef::TJ_CENTRE,
		"Start");

	font_->RenderText(
		sprite_renderer_,
		gef::Vector4(platform_.width() * 0.5f, platform_.height() * 0.6f - 56.0f, -0.99f),
		1.0f,
		textcolors[current_start_menu_choice_ == OPTIONS],
		gef::TJ_CENTRE,
		"Options");

	font_->RenderText(
		sprite_renderer_,
		gef::Vector4(platform_.width() * 0.5f, platform_.height() * 0.7f - 56.0f, -0.99f),
		1.0f,
		textcolors[current_start_menu_choice_ == QUIT],
		gef::TJ_CENTRE,
		"Quit");

	sprite_renderer_->End();
}

void SceneApp::OptionsUpdate(float frame_time)
{
	if (input_manager_->keyboard()->IsKeyPressed(gef::Keyboard::KC_DOWN))
	{
		audio_manager_->PlaySample(0);
		current_option_menu_choice_ = SceneApp::OPTIONMENUCHOICE((current_option_menu_choice_ + 1) % OPTION_MENU_CHOICES);
	}

	if (input_manager_->keyboard()->IsKeyPressed(gef::Keyboard::KC_UP))
	{
		audio_manager_->PlaySample(0);
		current_option_menu_choice_ = SceneApp::OPTIONMENUCHOICE((current_option_menu_choice_ - 1 + OPTION_MENU_CHOICES) % OPTION_MENU_CHOICES);
	}

	if (input_manager_->keyboard()->IsKeyPressed(gef::Keyboard::KC_RIGHT))
	{
		if (current_option_menu_choice_ == VOLUME)
		{
			volume_state_ = SceneApp::VOLUMELEVEL((volume_state_ + 1) % NUMBER_OF_VOLUME);
			audio_manager_->SetMasterVolume(volumelevel[volume_state_]);
		}
		else if (current_option_menu_choice_ == DIFFICULTY)
		{
			difficulty_state_ = SceneApp::GAMEDIFFICULTY((difficulty_state_ + 1) % NUMBER_OF_DIFFICULTY);
		}
		else if (current_option_menu_choice_ == MUSIC)
		{
			game_music_state = SceneApp::GAMEMUSICCHOICE((game_music_state + 1) % NUMBER_OF_SONGS);
		}

	}

	if (input_manager_->keyboard()->IsKeyPressed(gef::Keyboard::KC_LEFT))
	{
		if (current_option_menu_choice_ == VOLUME)
		{
			volume_state_ = SceneApp::VOLUMELEVEL((volume_state_  - 1 + NUMBER_OF_VOLUME) % NUMBER_OF_VOLUME);
			audio_manager_->SetMasterVolume(volumelevel[volume_state_]);
		}
		else if (current_option_menu_choice_ == DIFFICULTY)
		{
			difficulty_state_ = SceneApp::GAMEDIFFICULTY((difficulty_state_ - 1 + NUMBER_OF_DIFFICULTY) % NUMBER_OF_DIFFICULTY);
		}
		else if (current_option_menu_choice_ == MUSIC)
		{
			game_music_state = SceneApp::GAMEMUSICCHOICE((game_music_state - 1 + NUMBER_OF_SONGS) % NUMBER_OF_SONGS);
		}
	}

	if (input_manager_->keyboard()->IsKeyPressed(gef::Keyboard::KC_RETURN))
	{
		switch (current_option_menu_choice_)
		{
		case BACK:
			
			game_state_ = INIT;
			break;
		}

	}
}
void SceneApp::OptionsRender()
{

	sprite_renderer_->Begin();



	font_->RenderText(
		sprite_renderer_,
		gef::Vector4(platform_.width() * 0.265f, platform_.height() * 0.4f - 56.0f, -0.99f),
		1.5f,
		textcolors[current_option_menu_choice_ == VOLUME],
		gef::TJ_LEFT,
		"       VOLUME:  %i", volumelevel[volume_state_]);


	font_->RenderText(
		sprite_renderer_,
		gef::Vector4(platform_.width() * 0.265f, platform_.height() * 0.5f - 56.0f, -0.99f),
		1.5f,
		textcolors[current_option_menu_choice_ == DIFFICULTY],
		gef::TJ_LEFT,
		"DIFFICULTY: %s", difficultytext[difficulty_state_]);

	font_->RenderText(
		sprite_renderer_,
		gef::Vector4(platform_.width() * 0.265f, platform_.height() * 0.6f - 56.0f, -0.99f),
		1.5f,
		textcolors[current_option_menu_choice_ == MUSIC],
		gef::TJ_LEFT,
		"           SONG: %s", gamemusictext[game_music_state]);

	font_->RenderText(
		sprite_renderer_,
		gef::Vector4(platform_.width() * 0.5f, platform_.height() * 0.7f - 56.0f, -0.99f),
		1.5f,
		textcolors[current_option_menu_choice_ == BACK],
		gef::TJ_CENTRE,
		"BACK");

	sprite_renderer_->End();
}
void SceneApp::OptionsRelease()
{
}

void SceneApp::GameUpdate(float frame_time)
{
	counter--;
	score++;

	if (player_body_->GetPosition().x > 1190)
	{
		audio_manager_->LoadMusic("audio/music/end.wav", platform_);
		audio_manager_->PlayMusic();
		game_state_ = END;
	}

	if (input_manager_->keyboard()->IsKeyDown(gef::Keyboard::KC_E))
	{
		audio_manager_->LoadMusic("audio/music/end.wav", platform_);
		audio_manager_->PlayMusic();
		game_state_ = END;
	}

	vel = b2Vec2(2.0f, 0.0f);
	fps_ = 1.0f / frame_time;
	input_manager_->Update();
	//const gef::SonyController* controller = input_manager_->controller_input()->GetController(0);

	world_->Step(frame_time, 8, 3);
	gef::Vector4 playerPosition2(player_body_->GetPosition().x, player_body_->GetPosition().y, 0.0f);
	camera_eye_transformV4 = gef::Vector4{ player_body_->GetPosition().x, player_body_->GetPosition().y + 5,16,0 };
	camera_lookat_transformV4 = gef::Vector4{ player_body_->GetPosition().x, player_body_->GetPosition().y,0,0 };
	camera_eye_V4 = camera_eye_transformV4;
	camera_lookat_V4 = camera_lookat_transformV4;

	gef::Vector4 playerPosition(player_body_->GetPosition().x, player_body_->GetPosition().y, 0.0f);



	player_rotateM44.RotationY(gef::DegToRad(90.0f));
	gef::Matrix44 combined = player_scaleM44 * player_rotateM44;
	combined.SetTranslation(playerPosition);

	playerskinned->set_transform(combined);



	//players speed
	if (player_body_->GetLinearVelocity().x < 7)
	{
		player_body_->ApplyForce(b2Vec2(5.0f, 0.0f), b2Vec2(player_body_->GetPosition().x, player_body_->GetPosition().y), true);
	}
	//when player slows down, decriment score
	if (player_body_->GetLinearVelocity().x < 4)
	{
		score -= 5;
	}

	if (input_manager_->keyboard()->IsKeyDown(gef::Keyboard::KC_S))
	{

		if (issliding_)
		{
			fixturelistplayer = player_body_->GetFixtureList();
			b2Shape* shape = fixturelistplayer->GetShape();
			b2PolygonShape* polygon = dynamic_cast<b2PolygonShape*>(shape);
			polygon->SetAsBox(0.3f, 0.1f);
			anim_player_.set_clip(slide_anim_);
			playerslideV4 = gef::Vector4(-1.0f, 0.3f, 0, 0) + playerPosition;
			player_transformslideM44.SetIdentity();
			player_transformslideM44.SetTranslation(playerslideV4);
			player_rotateslideM44.RotationZ(gef::DegToRad(-90.0f));
			gef::Matrix44 combined2 = (player_scaleM44 * player_rotateM44 * player_rotateslideM44 * player_transformslideM44);
			playerskinned->set_transform(combined2);
		}
		issliding_ = true;

	}

	else
	{
		player_shape.SetAsBox(0.37f, 0.40f);
		player_fixture_def.shape = &player_shape;
		player_fixture_def.density = 0.8f;
		player_fixture_def.friction = 0.2f;
		player_body_->DestroyFixture(player_body_->GetFixtureList());
		player_body_->CreateFixture(&player_fixture_def);
		issliding_ = false;
		anim_player_.set_clip(walk_anim_);
		playerslideV4 = gef::Vector4(0.5f, 0, 0, 0);
		player_transformslideM44.SetIdentity();
		player_transformslideM44.SetTranslation(playerslideV4);
		player_rotateslideM44.RotationZ(gef::DegToRad(0.0f));
		gef::Matrix44 combined3 = (player_scaleM44 * player_rotateM44 * player_rotateslideM44 * player_transformslideM44);
		combined3.SetTranslation(playerPosition);
		playerskinned->set_transform(combined3);
	}
	//if (input_manager_->keyboard()->IsKeyReleased(gef::Keyboard::KC_S))
	//{
	//	player_shape.SetAsBox(0.37f, 0.40f);
	//	player_fixture_def.shape = &player_shape;
	//	player_fixture_def.density = 0.8f;
	//	player_fixture_def.friction = 0.2f;
	//	player_body_->DestroyFixture(player_body_->GetFixtureList());
	//	player_body_->CreateFixture(&player_fixture_def);
	//	issliding_ = false;
	//	anim_player_.set_clip(walk_anim_);
	//	playerslideV4 = gef::Vector4(0.5f, 0, 0, 0);
	//	player_transformslideM44.SetIdentity();
	//	player_transformslideM44.SetTranslation(playerslideV4);
	//	player_rotateslideM44.RotationZ(gef::DegToRad(90.0f));
	//	gef::Matrix44 combined3 = (player_scaleM44 * player_rotateM44 * player_rotateslideM44 * player_transformslideM44);
	//	combined3.SetTranslation(playerPosition);
	//	playerskinned->set_transform(combined3);
	//}
	if (input_manager_->keyboard()->IsKeyDown(gef::Keyboard::KC_W) && counter <= 0)
	{
		counter = 50;
		player_body_->ApplyLinearImpulse(b2Vec2(0.0f, 5.0f), b2Vec2(player_body_->GetPosition().x, player_body_->GetPosition().y), true);
	}
	if (counter < 0)
	{
		counter = 0;
	}
	if (score < 0)
	{
		score = 0;
	}
	if (player_body_->GetLinearVelocity().y > 0.05)
	{
		anim_player_.set_clip(up_anim_);
		anim_player_.set_looping(false);
	}
	else if (player_body_->GetLinearVelocity().y < -0.05)
	{
		anim_player_.set_clip(down_anim_);
		anim_player_.set_looping(false);
		player_body_->ApplyForce(b2Vec2(2.0f, 0.0f), b2Vec2(player_body_->GetPosition().x, player_body_->GetPosition().y), true);
	}

	if (player_body_->GetLinearVelocity().y < 0.01 && player_body_->GetLinearVelocity().y > -0.01)
	{
		if (!issliding_)
		{

			anim_player_.set_clip(walk_anim_);
			anim_player_.set_looping(true);
		}
	}

	gef::DebugOut("Player: %0.1f %0.1f %0.1f %0.01f\n", player_body_->GetPosition().x, player_body_->GetPosition().y, player_body_->GetLinearVelocity().x, player_body_->GetLinearVelocity().y);

	UpdateSimulation(frame_time);

	if (playerskinned)
	{
		// update the pose in the anim player from the animation
		anim_player_.Update(frame_time, playerskinned->bind_pose());

		// update the bone matrices that are used for rendering the character
		// from the newly updated pose in the anim player
		playerskinned->UpdateBoneMatrices(anim_player_.pose());
	}


}
void SceneApp::GameRender()
{
	// setup camera

	// projection
	gef::Matrix44 projection_matrix;
	gef::Matrix44 view_matrix;
	projection_matrix = platform_.PerspectiveProjectionFov(camera_fov_, (float)platform_.width() / (float)platform_.height(), near_plane_, far_plane_);
	view_matrix.LookAt(camera_eye_V4, camera_lookat_V4, camera_up_V4);
	renderer_3d_->set_projection_matrix(projection_matrix);
	renderer_3d_->set_view_matrix(view_matrix);


	// draw 3d geometry
	renderer_3d_->Begin();

	// draw ground


	// draw player

	renderer_3d_->DrawSkinnedMesh(*playerskinned, playerskinned->bone_matrices());
	for (int i = 0; i < 20; i++)
	{
		renderer_3d_->DrawMesh(militaryRoad_Mesh_Instance[i]);
		renderer_3d_->DrawMesh(samurai_Road_Mesh_Instance[i]);
		renderer_3d_->DrawMesh(apoc_Road_Mesh_Instance[i]);
	}

	for (int i = 0; i < 12; i++)
	{
		renderer_3d_->DrawMesh(military_Building_Mesh_Instance[i]);
		renderer_3d_->DrawMesh(samurai_Building_Mesh_Instance[i]);
		renderer_3d_->DrawMesh(apoc_Building_Mesh_Instance[i]);
	}

	for (int i = 0; i < 20; i++)
	{
		renderer_3d_->DrawMesh(military_Obstacle_Mesh_Instance[i]);
		renderer_3d_->DrawMesh(samurai_Obstacle_Mesh_Instance[i]);
		renderer_3d_->DrawMesh(apoc_Obstacle_Mesh_Instance[i]);
	}

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
void SceneApp::GameRelease()
{
}



void SceneApp::EndUpdate(float frame_time)
{

	if (input_manager_->keyboard()->IsKeyPressed(gef::Keyboard::KC_RETURN))
	{
		switch (game_music_state)
		{
		case BREEZY:
			audio_manager_->LoadMusic("audio/music/breezy.wav", platform_);
			score = 0;
			game_state_ = GAME;
			break;
		case RUNBOY:
			audio_manager_->LoadMusic("audio/music/runboy.wav", platform_);
			score = 0;
			game_state_ = GAME;
			break;
		case MIAMI:
			audio_manager_->LoadMusic("audio/music/miami.wav", platform_);
			score = 0;
			game_state_ = GAME;
			break;

		}
		
	}

	if (input_manager_->keyboard()->IsKeyPressed(gef::Keyboard::KC_ESCAPE))
	{
		audio_manager_->LoadMusic("audio/music/start.wav", platform_);
		audio_manager_->PlayMusic();
		game_state_ = INIT;
	}


	UpdateSimulation(frame_time);

	if (playerskinned)
	{
		// update the pose in the anim player from the animation
		anim_player_.Update(frame_time, playerskinned->bind_pose());

		// update the bone matrices that are used for rendering the character
		// from the newly updated pose in the anim player
		playerskinned->UpdateBoneMatrices(anim_player_.pose());
	}

}
void SceneApp::EndRender()
{
	gef::Vector4 playerPosition(0.0, 0.0, -200.0f);
	endplayerpos.SetIdentity();
	endplayerpos.SetTranslation(playerPosition);
	playerskinned->set_transform(endplayerpos);

	camera_eye_V4 = gef::Vector4(0.0f, 0.0f, 300.0f);
	player_body_->SetTransform(b2Vec2(0.0, 0.0), 0);
	camera_lookat_V4 = gef::Vector4(0, 0, 0.0f);
	camera_up_V4 = gef::Vector4(0.0f, 1.0f, 0.0f, 0.0f);

	gef::Matrix44 projection_matrix;
	gef::Matrix44 view_matrix;
	projection_matrix = platform_.PerspectiveProjectionFov(camera_fov_, (float)platform_.width() / (float)platform_.height(), near_plane_, far_plane_);
	view_matrix.LookAt(camera_eye_V4, camera_lookat_V4, camera_up_V4);
	renderer_3d_->set_projection_matrix(projection_matrix);
	renderer_3d_->set_view_matrix(view_matrix);

	renderer_3d_->Begin();

	anim_player_.set_clip(dance_anim_);
	renderer_3d_->DrawSkinnedMesh(*playerskinned, playerskinned->bone_matrices());


	renderer_3d_->End();

	sprite_renderer_->Begin(false);
	font_->RenderText(
		sprite_renderer_,
		gef::Vector4(platform_.width() * 0.5f, platform_.height() * 0.6f - 56.0f, -0.99f),
		2.0f,
		0xff20a5da,
		gef::TJ_CENTRE,
		"Your final score: %i", score);

	font_->RenderText(
		sprite_renderer_,
		gef::Vector4(platform_.width() * 0.5f, platform_.height() * 0.70f - 56.0f, -0.99f),
		1.0f,
		0xff20a5da,
		gef::TJ_CENTRE,
		"Press ENTER to go to a new game", score);

	font_->RenderText(
		sprite_renderer_,
		gef::Vector4(platform_.width() * 0.5f, platform_.height() * 0.75f - 56.0f, -0.99f),
		1.0f,
		0xff20a5da,
		gef::TJ_CENTRE,
		"Press ESCAPE to go to the start menu", score);

	sprite_renderer_->End();



}
void SceneApp::EndRelease()
{
}

void SceneApp::InitOptions()
{
}



void SceneApp::DrawHUD()
{
	if (font_)
	{
		// display frame rate
		font_->RenderText(sprite_renderer_, gef::Vector4(850.0f, 510.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "FPS: %.1f", fps_);
	}
	font_->RenderText(sprite_renderer_, gef::Vector4(830.0f, 10.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "Score: %i", score);

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
	camera_eye_V4 = gef::Vector4(0.0f, 10.0f, 10.0f);
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

	for (int contact_num = 0; contact_num < contact_count; ++contact_num)
	{
		if (contact->IsTouching())
		{
			// get the colliding bodies
			b2Body* bodyA = contact->GetFixtureA()->GetBody();
			b2Body* bodyB = contact->GetFixtureB()->GetBody();

			// DO COLLISION RESPONSE HERE
			Player* player = NULL;

			GameObject* gameObjectA = NULL;
			GameObject* gameObjectB = NULL;

			gameObjectA = (GameObject*)bodyA->GetUserData();
			gameObjectB = (GameObject*)bodyB->GetUserData();

			if (gameObjectA)
			{
				if (gameObjectA->type() == PLAYER)
				{
					player = (Player*)bodyA->GetUserData();
				}
			}

			if (gameObjectB)
			{
				if (gameObjectB->type() == PLAYER)
				{
					player = (Player*)bodyB->GetUserData();
				}
			}

			if (player)
			{
				player->DecrementHealth();
			}
		}

		// Get next contact point
		contact = contact->GetNext();
	}
}
void SceneApp::CameraRotateAroundObject(float rotate)
{
	gef::Vector4 offset = camera_lookat_V4 - camera_eye_V4;
	camera_eye_transformM44.RotationY(rotate);
	offset = offset.Transform(camera_eye_transformM44);
	//camera_eye_rotateV4.Transform(camera_eye_transformM44);
	camera_eye_V4 = camera_lookat_V4 - offset;
}



gef::Skeleton* SceneApp::GetFirstSkeleton(gef::Scene* scene)
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
gef::Mesh* SceneApp::GetFirstMesh(gef::Scene* scene)
{
	gef::Mesh* mesh = NULL;

	if (scene)
	{
		// now check to see if there is any mesh data in the file, if so lets create a mesh from it
		if (scene->mesh_data.size() > 0)
			mesh = ninja->CreateMesh(platform_, scene->mesh_data.front());
	}

	return mesh;
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
gef::Animation* SceneApp::LoadAnimation(const char* anim_scene_filename, const char* anim_name)
{
	gef::Animation* anim = NULL;

	gef::Scene anim_scene;
	if (anim_scene.ReadSceneFromFile(platform_, anim_scene_filename))
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




	for (int i = 0; i < 5; i++)
	{
		delete samurai_Roads[i];
		delete military_Roads[i];
		delete apoc_Roads[i];
		samurai_Roads[i] = NULL;
		military_Roads[i] = NULL;
		apoc_Roads[i] = NULL;
	}

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
}
void SceneApp::CleanUpFont()
{
	delete font_;
	font_ = NULL;
}