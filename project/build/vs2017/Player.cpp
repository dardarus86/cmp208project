#include <system/platform.h>
#include <graphics/renderer_3d.h>
#include <graphics/mesh.h>
#include <maths/math_utils.h>
#include <input/keyboard.h>
#include <graphics/scene.h>
#include <animation/skeleton.h>
#include <animation/animation.h>
#include <stdlib.h>
#include <system/debug_log.h>
#include "Player.h"

void Player::InitPlayer(b2World* world, gef::Platform& platform)
{
	ninja = new gef::Scene();
	playerscalingV4 = gef::Vector4{ 0.01f,0.01f,0.01f,0 };

	player_scaleM44.Scale(playerscalingV4);

	ninja->ReadSceneFromFile(platform, "player/ninja.scn");
	ninja->CreateMaterials(platform);

	player_mesh_ = GetFirstMesh(ninja, platform);

	gef::Skeleton* skeleton = GetFirstSkeleton(ninja);

	if (skeleton)
	{
		playerskinned = new gef::SkinnedMeshInstance(*skeleton);
		anim_player_.Init(playerskinned->bind_pose());
		playerskinned->set_mesh(player_mesh_);

	}
	playerskinned->set_transform(player_scaleM44);

	walk_anim_ = LoadAnimation("player/ninjarun.scn", "", platform);
	slide_anim_ = LoadAnimation("player/ninjafly.scn", "", platform);
	up_anim_ = LoadAnimation("player/ninjaup.scn", "", platform);
	down_anim_ = LoadAnimation("player/ninjadown.scn", "",platform);
	dance_anim_ = LoadAnimation("player/ninjadance.scn", "", platform);

	if (walk_anim_)
	{
		anim_player_.set_clip(walk_anim_);
		anim_player_.set_looping(true);
		anim_player_.set_anim_time(0.0f);
	}

	// create a physics body for the player
	player_body_def.type = b2_dynamicBody;
	player_body_def.position = b2Vec2(0.0f, 0.0f);
	player_body_ = world->CreateBody(&player_body_def);

	// create the shape for the player
	player_shape.SetAsBox(0.37f, 1.8f);

	// create the fixture
	player_fixture_def.shape = &player_shape;
	player_fixture_def.density = 0.8f;
	player_fixture_def.friction = 0.2f;

	// create the fixture on the rigid body
	player_body_->CreateFixture(&player_fixture_def);

	// update visuals from simulation data
	//player_.UpdateFromSimulation(player_body_);
	//vel = player_body_->GetLinearVelocity();
	// create a connection between the rigid body and GameObject
	player_body_->SetUserData(&playerskinned);
	
}

void Player::Render(gef::Renderer3D* renderer)
{

	renderer->DrawSkinnedMesh(*playerskinned, playerskinned->bone_matrices());
}
void Player::update(float frame_time, int* score, gef::InputManager* inputmanager)
{
	counter--;

	if (player_body_->GetPosition().x > 1190)
	{
		SetGoalFinished(true);
	}

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

	if (inputmanager->keyboard()->IsKeyDown(gef::Keyboard::KC_S))
	{

		if (issliding_)
		{
			fixturelistplayer = player_body_->GetFixtureList();
			b2Shape* shape = fixturelistplayer->GetShape();
			b2PolygonShape* polygon = dynamic_cast<b2PolygonShape*>(shape);
			polygon->SetAsBox(0.3f, 0.3f);
			anim_player_.set_clip(slide_anim_);
			playerslideV4 = gef::Vector4(-1.0f, 1.0f, 0, 0) + playerPosition;
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
		player_shape.SetAsBox(0.37f, 1.0f);
		player_fixture_def.shape = &player_shape;
		player_fixture_def.density = 0.3f;
		player_fixture_def.friction = 0.2f;
		player_body_->DestroyFixture(player_body_->GetFixtureList());
		player_body_->CreateFixture(&player_fixture_def);
		issliding_ = false;
		anim_player_.set_clip(walk_anim_);
		playerslideV4 = gef::Vector4(0.5f, 5.2, 0, 0);
		player_transformslideM44.SetIdentity();
		player_transformslideM44.SetTranslation(playerslideV4);
		player_rotateslideM44.RotationZ(gef::DegToRad(0.0f));
		gef::Matrix44 combined3 = (player_scaleM44 * player_rotateM44 * player_rotateslideM44 * player_transformslideM44);
		combined3.SetTranslation(playerPosition);

		playerskinned->set_transform(combined3);
	}


	if (inputmanager->keyboard()->IsKeyDown(gef::Keyboard::KC_W) && counter <= 0)
	{
		counter = 70;
		player_body_->ApplyLinearImpulse(b2Vec2(0.0f, 5.0f), b2Vec2(player_body_->GetPosition().x, player_body_->GetPosition().y), true);
	}

	if (player_body_->GetLinearVelocity().y > 0.05)
	{
		anim_player_.set_clip(up_anim_);
		anim_player_.set_looping(false);
	}
	else if (player_body_->GetLinearVelocity().y < -0.02)
	{
		anim_player_.set_clip(down_anim_);
		anim_player_.set_looping(false);
		// we need to give it some velocity to make it a bit more realistic
		player_body_->ApplyForce(b2Vec2(2.0f, 0.2f), b2Vec2(player_body_->GetPosition().x, player_body_->GetPosition().y), true);
	}


	if (player_body_->GetLinearVelocity().y < 0.02 && player_body_->GetLinearVelocity().y > -0.02)
	{
		if (!issliding_)
		{
			playeradjusty = b2Vec2(player_body_->GetPosition().x, 0.00);
			player_body_->SetTransform(playeradjusty, 0);
			anim_player_.set_clip(walk_anim_);
			anim_player_.set_looping(true);
		}
	}

	if (playerskinned)
	{
		// update the pose in the anim player from the animation
		anim_player_.Update(frame_time, playerskinned->bind_pose());

		// update the bone matrices that are used for rendering the character
		// from the newly updated pose in the anim player
		playerskinned->UpdateBoneMatrices(anim_player_.pose());
	}
	if (counter < 0)
	{
		counter = 0;
	}

	gef::DebugOut("Player: %0.1f %0.1f %0.1f %0.01f\n", player_body_->GetPosition().x, player_body_->GetPosition().y, player_body_->GetLinearVelocity().x, player_body_->GetLinearVelocity().y);
}


void Player::SetGoalFinished(bool goalfinish)
{
	goalreached_ = goalfinish;
}
bool Player::GetGoalFinished()
{
	return goalreached_;
}
void Player::ReduceScore(int score)
{
	score = -5;
}
float Player::GetPlayerBodyX()
{
	return player_body_->GetPosition().x;
}
float Player::GetPlayerBodyY()
{
	return player_body_->GetPosition().y;
}

float Player::GetPlayerBodyXVecolity()
{
	return player_body_->GetLinearVelocity().x;
}

gef::Skeleton* Player::GetFirstSkeleton(gef::Scene* scene)
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
gef::Mesh* Player::GetFirstMesh(gef::Scene* scene, gef::Platform& platform)
{
	gef::Mesh* mesh = NULL;

	if (scene)
	{
		// now check to see if there is any mesh data in the file, if so lets create a mesh from it
		if (scene->mesh_data.size() > 0)
			mesh = ninja->CreateMesh(platform, scene->mesh_data.front());
	}

	return mesh;
}

gef::Animation* Player::LoadAnimation(const char* anim_scene_filename, const char* anim_name, gef::Platform& platform)
{
	gef::Animation* anim = NULL;

	gef::Scene anim_scene;
	if (anim_scene.ReadSceneFromFile(platform, anim_scene_filename))
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

gef::Scene* Player::LoadSceneAssets(gef::Platform& platform, const char* filename)
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

gef::Mesh* Player::GetMeshFromSceneAssets(gef::Scene* scene)
{
	gef::Mesh* mesh = NULL;

	// if the scene data contains at least one mesh
	// return the first mesh
	if (scene && scene->meshes.size() > 0)
		mesh = scene->meshes.front();

	return mesh;
}
