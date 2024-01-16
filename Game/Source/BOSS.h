#ifndef __BOSS_H__
#define __BOSS_H__

#include "Entity.h"
#include "SDL/include/SDL.h"
#include "Anim.h"


struct SDL_Texture;

class BOSS : public Entity
{
public:

	BOSS();
	virtual ~BOSS();

	void InitAnims();

	bool Awake();

	bool Start();

	void TeleportTo(iPoint pos);

	void SetSpawnPoint(iPoint pos);

	bool Update(float dt);

	bool CleanUp();

	void MovementLogic(float dt);


	void OnCollision(PhysBody* physA, PhysBody* physB);

	void IsDead();

	void KilledPlayer();


public:


	const char* texturePath;
	SDL_Texture* texture = NULL;
	PhysBody* pbody;
	int width = 32, height = 18;
	int isDeadFx;
	iPoint spawn;
	iPoint despawn;
	bool isKilled = false;
	bool killedPlayer = false;

private:


	float movementForce = 1.5f;
	float movementDampen = 3.0;
	float maxVel = 3;

	SDL_Texture* mouseTileTex;

	Animation* currentAnim = nullptr;

	Animation BOSSrigthmov;

	Animation BOSSleftmov;
};
#endif