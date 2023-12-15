#ifndef __FLYENEM_H__
#define __FLYENEM_H__

#include "Entity.h"
#include "SDL/include/SDL.h"
#include "Anim.h"


struct SDL_Texture;

class Flyenem : public Entity
{
public:

	Flyenem();
	virtual ~Flyenem();

	void InitAnims();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);


public:
	pugi::xml_node config;
	const char* texturePath;
	SDL_Texture* texture = NULL;
	PhysBody* pbody;
	int width = 32, height = 18;

	iPoint spawn;
	bool isDead = false;

private:
	Animation* currentAnim = nullptr;
	SDL_Texture* mouseTileTex;

	Animation flyrigthmov;

	Animation flyleftmov;
};
#endif