#ifndef __WALKENEM_H__
#define __WALKENEM_H__

#include "Entity.h"
#include "SDL/include/SDL.h"
#include "Anim.h"


struct SDL_Texture;

class Wenem : public Entity
{
public:
	Wenem();
	virtual ~Wenem();

	void InitAnims();

	bool Awake();

	bool Start();

	void TeleportTo(iPoint pos);

	void SetSpawnPoint(iPoint pos);

	void MovementLogic(float dt);

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	void IsDead();

	void KilledPlayer();

public:

	const char* texturePath;
	SDL_Texture* texture = NULL;
	PhysBody* pbody;
	int width = 34, height = 16;

	iPoint spawn;
	iPoint despawn;
	bool isKilled = false;
	bool killedPlayer = false;

private:

	float movementForce = 2.0f;
	float movementDampen = 3.0;
	float maxVel = 3;


	SDL_Texture* mouseTileTex;

	Animation* currentAnim = nullptr;

	Animation wr_idle, wrigthmov;

	Animation wl_idle, wleftmov;
};
#endif