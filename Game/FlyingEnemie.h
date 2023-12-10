#ifndef __FLYENEM_H__
#define __FLYENEM_H__

#include "../Entity.h"
#include "SDL/include/SDL.h"
#include "../Anim.h"


struct SDL_Texture;

class Flyenem : public Entity
{
public:

	Flyenem();
	virtual ~Flyenem();

	void InitAnims();

	bool Awake();

	bool Start();

	void AnimationLogic(float dt);

	void IsDead();

	//bool Update(float dt);

	//bool CleanUp();




public:

	const char* texturePath;
	SDL_Texture* texture = NULL;
	PhysBody* pbody;


private:
	Animation* currentAnim = nullptr;

	Animation flyrigthmov;

	Animation flyleftmov;
};
#endif