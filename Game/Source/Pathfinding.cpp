#include "App.h"
#include "Map.h"
#include "Pathfinding.h"

#include "Defs.h"
#include "Log.h"

PathFinding::PathFinding()
{
	map = NULL;
	width = 0;
	height = 0;
}

PathFinding::~PathFinding()
{
	RELEASE_ARRAY(map);
}

bool PathFinding::CleanUp()
{
	LOG("Freeing Pathfinding library");

	lastPath.Clear();
	RELEASE_ARRAY(map);

	return true;
}

void PathFinding::SetNavigationMap(uint w, uint h, uchar* data)
{
	//width = w;
	//height = h;

	//RELEASE_ARRAY(map);
	//map = new uchar[width * height];
	//memcpy(map, data, width * height);
}

bool PathFinding::CheckBoundaries(const iPoint& pos) const
{
	return (pos.x >= 0 && pos.x <= (int)width &&
		    pos.y >= 0 && pos.y <= (int)height);
}

bool PathFinding::IsWalkable(const iPoint& pos) const
{
	uchar walkId = GetTileAt(pos);
	bool isWalkable = walkId != INVALID_WALK_CODE && walkId > 0;
	return isWalkable;
}

uchar PathFinding::GetTileAt(const iPoint& pos) const
{
	if (CheckBoundaries(pos))
		return map[(pos.y * width) + pos.x];

	return INVALID_WALK_CODE;
}

const DynArray<iPoint>* PathFinding::GetLastPath() const
{
	return &lastPath;
}

void PathFinding::ClearLastPath()
{
	lastPath.Clear();
}

int PathFinding::CreatePath(const iPoint& origin, const iPoint& destination)
{
	int ret = -1;
	int iterations = 0;

	if (IsWalkable(origin) && IsWalkable(destination))
	{
		PathList frontier;
		PathList visited;

		frontier.list.Add(PathNode(0, 0, origin, nullptr));

		while (frontier.list.Count() > 0)
		{
			ListItem<PathNode>* lowest = frontier.GetNodeLowestScore();
			ListItem<PathNode>* node = new ListItem<PathNode>(*lowest);
			frontier.list.Del(lowest);

			if (node->data.pos == destination)
			{
				lastPath.Clear();

				const PathNode* pathNode = &node->data;

				while (pathNode)
				{
					lastPath.PushBack(pathNode->pos);
					pathNode = pathNode->parent;
				}

				lastPath.Flip();
				ret = lastPath.Count();
				LOG("Created path of %d steps in %d iterations", ret, iterations);
				break;
			}

			PathList adjacent;
			node->data.FindWalkableAdjacents(adjacent);

			ListItem<PathNode>* neighbourg = adjacent.list.start;
			while (neighbourg != NULL)
			{
				if (visited.Find(neighbourg->data.pos) == NULL)
				{
					visited.list.Add(neighbourg->data);

					ListItem<PathNode>* neighbourgInFrontier = frontier.Find(neighbourg->data.pos);
					if (neighbourgInFrontier == NULL)
					{
						neighbourg->data.CalculateF(destination);
						frontier.list.Add(neighbourg->data);
					}
					else
					{
						if (neighbourgInFrontier->data.g > neighbourg->data.g + 1)
						{
							neighbourgInFrontier->data.parent = neighbourg->data.parent;
							neighbourgInFrontier->data.CalculateF(destination);
						}
					}
				}
				neighbourg = neighbourg->next;
			}

			++iterations;
			
		}
	}

	return ret;
}

ListItem<PathNode>* PathList::Find(const iPoint& point) const
{
	ListItem<PathNode>* item = list.start;
	while (item)
	{
		if (item->data.pos == point)
			return item;
		item = item->next;
	}

	return item;
}

ListItem<PathNode>* PathList::GetNodeLowestScore() const
{
	ListItem<PathNode>* ret = NULL;
	int min = 65535;

	ListItem<PathNode>* item = list.end;
	while (item)
	{
		if (item->data.Score() < min)
		{
			min = item->data.Score();
			ret = item;
		}
		item = item->prev;
	}

	return ret;
}

PathNode::PathNode() : g(-1), h(-1), pos(-1, -1), parent(NULL)
{}

PathNode::PathNode(int g, int h, const iPoint& pos, const PathNode* parent) : g(g), h(h), pos(pos), parent(parent)
{}

PathNode::PathNode(const PathNode& node) : g(node.g), h(node.h), pos(node.pos), parent(node.parent)
{}

uint PathNode::FindWalkableAdjacents(PathList& listToFill) const
{
	iPoint tile;
	uint before = listToFill.list.Count();

	// top
	tile.Create(pos.x, pos.y + 1);
	if (app->map->pathfinding->IsWalkable(tile)) listToFill.list.Add(PathNode(-1, -1, tile, this));

	//// bottom
	tile.Create(pos.x, pos.y - 1);
	if (app->map->pathfinding->IsWalkable(tile)) listToFill.list.Add(PathNode(-1, -1, tile, this));

	//// left
	tile.Create(pos.x + 1, pos.y);
	if (app->map->pathfinding->IsWalkable(tile)) listToFill.list.Add(PathNode(-1, -1, tile, this));

	//// right
	tile.Create(pos.x - 1, pos.y);
	if (app->map->pathfinding->IsWalkable(tile)) listToFill.list.Add(PathNode(-1, -1, tile, this));

	return listToFill.list.Count();
}

int PathNode::Score() const
{
	return g + h;
}

int PathNode::CalculateF(const iPoint& destination)
{
	g = parent->g + 1;
	h = pos.DistanceTo(destination);

	return g + h;
}