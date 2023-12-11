#ifndef __WALKENEM_H__
#define __WALKENEM_H__

#include "../Entity.h"
#include "SDL/include/SDL.h"
#include "../Anim.h"


struct SDL_Texture;

class Wenem : public Entity
{
	public:
		Wenem();
		virtual ~Wenem();

		void InitAnims();

		bool Awake();

		bool Start();


		bool Update(float dt);

		bool CleanUp();

		

	public:

		const char* texturePath;
		SDL_Texture* texture = NULL;
		PhysBody* pbody;
		int width = 34, height = 16;

		iPoint spawn;

private:
	Animation* currentAnim = nullptr;

	Animation wr_idle, wrigthmov;

	Animation wl_idle, wleftmov;
};
#endif