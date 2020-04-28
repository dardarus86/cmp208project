																		///////////////////////////////////////////
																		//       CODE PROVIDED FOR CMP208        //
																		///////////////////////////////////////////


#ifndef _GAME_OBJECT_H
#define _GAME_OBJECT_H

#include <graphics/mesh_instance.h>
#include <box2d/Box2D.h>

class GameObject : public gef::MeshInstance
{
public:
	void UpdateFromSimulation(const b2Body* body);
	void MyCollisionResponse();
};


//this class was not used in this game however i left it in for potential future use
//class Player : public GameObject
//{
//public:
//	Player();
//	void DecrementHealth();
//};

#endif // _GAME_OBJECT_H