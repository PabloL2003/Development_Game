#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Anim.h"


struct SDL_Texture;

class Player : public Entity
{
public:

	Player();
	
	virtual ~Player();

	void InitAnims();

	bool Awake();

	bool Start();

	void AnimationLogic();

	void MovementLogic();

	void SetSpawnPoint(iPoint pos);

	void TeleportTo(iPoint pos);

	void IsDead();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

public:
	float speed = 0.2f;
	const char* texturePath;
	SDL_Texture* texture = NULL;
	PhysBody* pbody;
	int pickCoinFxId;
	int width = 24, height = 43;

	iPoint spawn = iPoint(400, 352);
	
	int jumps = 2;
	bool jumping = false;
	bool isKilled = false;

private:
	float movementForce = 10.0f;
	float jumpForce = 300.0f;
	float movementDampen = 12.0;
	float idleDampenMultiplier = 3.0f;
	float maxVel = 7.0;


	Animation* currentAnim = nullptr;

	Animation rightIdle, rightRun;

	Animation leftIdle, leftRun;

};

#endif // __PLAYER_H__