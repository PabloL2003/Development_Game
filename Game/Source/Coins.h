#ifndef __COIN_H__
#define __COIN_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class COIN : public Entity
{
public:

	COIN();
	virtual ~COIN();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void TeleportTo(iPoint pos);

	void IsPicked();

	void OnCollision(PhysBody* physA, PhysBody* physB);

public:

	bool isPicked = false;
	iPoint despawn;
	bool isKilled = false;
private:

	SDL_Texture* texture;
	const char* texturePath;
	PhysBody* pbody;
};

#endif // __ITEM_H__