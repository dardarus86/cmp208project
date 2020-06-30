///////////////////////////////////////////
// CODE CREATED BY JAMIE HADDOW 0705082  //
//				COMMENTED                //
///////////////////////////////////////////


#ifndef _COLLISION_H
#define _COLLISION_H

enum OBJECT_TYPE
{
	PLAYER,
	OBSTACLE,
	END

};

//simplified gameobjects classdue to not needing meshinstance inheritance
class collision
{
public:
	inline void set_type(OBJECT_TYPE type) { type_ = type; }
	inline OBJECT_TYPE type() { return type_; }
private:
	OBJECT_TYPE type_;
};


#endif // _GAME_OBJECT_H