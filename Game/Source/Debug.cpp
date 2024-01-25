#include "Debug.h"
#include "App.h"
#include "Input.h"
#include "Render.h"

#include "Player.h"
#include "Textures.h"
#include "EntityManager.h"
#include "Physics.h"
#include "Map.h"
#include "Scene.h"

#include "Defs.h"

#include <string>

using namespace std;

Debug::Debug() : Module()
{
	debug = false;
	name.Create("debug");
}

Debug::~Debug()
{

}

bool Debug::Start()
{
	debug = false;

	return true;
}

bool Debug::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN) debug = !debug;

	if (debug)
	{
		//Teleport to the start of the level
		if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		{
			app->scene->player->TeleportTo(app->scene->player->spawn);
			app->scene->enemie->TeleportTo(app->scene->enemie->spawn);
			app->scene->enemie2->TeleportTo(app->scene->enemie2->spawn);
			app->scene->enemie3->TeleportTo(app->scene->enemie3->spawn);
			app->scene->enemie4->TeleportTo(app->scene->enemie4->spawn);
		}

		//Enable/Disable hitboxes
		if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		{
			hitboxes = !hitboxes;
		}

		//Enable/Disable god mode
		if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		{
			godMode = !godMode;
			
			if (godMode)
			{
				app->scene->player->pbody->body->SetGravityScale(0.0f);
				app->scene->player->pbody->body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));

			}
		}

		//Enable/Disable FPS cap
		if (app->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN) 
		{
			limitFPS = !limitFPS;
		}

		//Teleport player to the first checkpoint
		if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		{
			app->scene->player->SetSpawnPoint(app->scene->player->spawn);
			app->scene->player->TeleportTo(app->scene->player->spawn);
		}

		// Teleport player to the second checkpoint
		if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		{
			app->scene->player->SetSpawnPoint(app->scene->player->spawn2);
			app->scene->player->TeleportTo(app->scene->player->spawn2);
		}
	
	}
	else 
	{
		if (variables != false) variables = false;
		if (hitboxes != false) hitboxes = false;
		if (debugCamera != false) debugCamera = false;
		if (godMode != false) godMode = false;
		if (limitFPS != false) limitFPS = false;
	}


	return true;
}

bool Debug::PostUpdate() 
{
	if (hitboxes) DrawHitboxes();
	DebugDraw();

	return true;
}

void Debug::DebugDraw()
{
	if(limitFPS) 
	{
		app->maxFrameDuration = 32;
	}
	else
	{
		app->maxFrameDuration = 16;
	}
}

void Debug::DrawHitboxes()
{
	for (b2Body* b = app->physics->world->GetBodyList(); b; b = b->GetNext())
	{
		for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch (f->GetType())
			{
				// Draw circles ------------------------------------------------
			case b2Shape::e_circle:
			{
				b2CircleShape* shape = (b2CircleShape*)f->GetShape();
				b2Vec2 pos = f->GetBody()->GetPosition();
				app->render->DrawCircle(METERS_TO_PIXELS(pos.x + app->render->camera.x), METERS_TO_PIXELS(pos.y + app->render->camera.y), METERS_TO_PIXELS(shape->m_radius), 255, 255, 255);
			}
			break;

			// Draw polygons ------------------------------------------------
			case b2Shape::e_polygon:
			{
				b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
				int32 count = polygonShape->GetVertexCount();
				b2Vec2 prev, v;

				for (int32 i = 0; i < count; ++i)
				{
					v = b->GetWorldPoint(polygonShape->GetVertex(i));
					if (i > 0)
						app->render->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);

					prev = v;
				}

				v = b->GetWorldPoint(polygonShape->GetVertex(0));
				app->render->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
			}
			break;

			// Draw chains contour -------------------------------------------
			case b2Shape::e_chain:
			{
				b2ChainShape* shape = (b2ChainShape*)f->GetShape();
				b2Vec2 prev, v;

				for (int32 i = 0; i < shape->m_count; ++i)
				{
					v = b->GetWorldPoint(shape->m_vertices[i]);
					if (i > 0)

						app->render->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
					prev = v;
				}

				v = b->GetWorldPoint(shape->m_vertices[0]);
				app->render->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
			}
			break;

			// Draw a single segment(edge) ----------------------------------
			case b2Shape::e_edge:
			{
				b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
				b2Vec2 v1, v2;

				v1 = b->GetWorldPoint(shape->m_vertex0);
				v1 = b->GetWorldPoint(shape->m_vertex1);
				app->render->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 100, 100, 255);
			}
			break;
			}
		}
	}
}