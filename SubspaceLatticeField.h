#pragma once

#include <vector>
#include <map>
//#include <Boid.h>
#include <algorithm>
#include <ScalarDef.h>

template<typename T> void erase_unordered(std::vector<T>& v, size_t index)
{
	v[index] = v.back();
	v.pop_back();
}

template <class T> class SubSpaceLatticeField
{
public:
	std::vector<T>* map;
	std::vector<T>* mapEnd;

	SCALAR x, y, width, height, maxSearchWidth;
	int iWidth, iHeight;

	SubSpaceLatticeField()
	{

	}

	SubSpaceLatticeField(SCALAR x, SCALAR y, SCALAR width, SCALAR height, SCALAR maxSearchWidth)
	{
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
		this->maxSearchWidth = maxSearchWidth;

		this->iWidth = width / maxSearchWidth;
		this->iHeight = height / maxSearchWidth;

		map = new std::vector<T>[this->iWidth * this->iHeight];

		for (int i = 0; i < this->iWidth * this->iHeight; i++)
			map[i] = std::vector<T>();

		mapEnd = &map[this->iWidth * this->iHeight];
	}

	std::pair<int, int> ConvertFromWorldToGrid(SCALAR& worldX, SCALAR& worldY)
	{
		return std::make_pair<int, int>(((width / 2.0) + worldX) / maxSearchWidth, ((height / 2) + worldY) / maxSearchWidth);
	}

	inline std::vector<T>* FetchPure(int x, int y)
	{
		std::vector<T>* pos = &map[(x * iWidth) + y];

		return pos >= map && pos < mapEnd ? pos : nullptr;
	}

	inline void Fetch(std::vector<T>* results, int x, int y)
	{
		std::vector<T>* pos = this->FetchPure(x, y);

		if (pos)
			results->insert(results->end(), pos->begin(), pos->end());
	}

	void Sample(SCALAR& worldX, SCALAR& worldY, std::vector<T>* results = nullptr)
	{
		auto gridPos = ConvertFromWorldToGrid(worldX, worldY);

		Fetch(results, gridPos.first, gridPos.second);

		Fetch(results, gridPos.first, gridPos.second + 1);
		Fetch(results, gridPos.first - 1, gridPos.second);
		Fetch(results, gridPos.first + 1, gridPos.second);
		Fetch(results, gridPos.first, gridPos.second - 1);

		Fetch(results, gridPos.first - 1, gridPos.second - 1);
		Fetch(results, gridPos.first + 1, gridPos.second - 1);
		Fetch(results, gridPos.first - 1, gridPos.second + 1);
		Fetch(results, gridPos.first + 1, gridPos.second + 1);
	}

	void Update(std::vector<Boid*>* boids)
	{
		for (Boid* boid : *boids)
		{
			auto lastGridpos = this->ConvertFromWorldToGrid(boid->lastPos[0], boid->lastPos[1]);
			auto newGridpos = this->ConvertFromWorldToGrid(boid->pos[0], boid->pos[1]);

			if (memcmp(&lastGridpos, &newGridpos, sizeof(std::pair<int, int>)) != 0)
			{
				std::vector<T>* oldGrid = FetchPure(lastGridpos.first, lastGridpos.second);
				if (oldGrid)
				{
					auto index = std::find(oldGrid->begin(), oldGrid->end(), boid);
					if (index != oldGrid->end())
					{
						erase_unordered(*oldGrid, index - oldGrid->begin());
					}
				}

				std::vector<T>* newGrid = FetchPure(newGridpos.first, newGridpos.second);
				if (newGrid)
				{
					auto index = std::find(newGrid->begin(), newGrid->end(), boid);
					if (index == newGrid->end())
					{
						newGrid->push_back(boid);
					}
				}
			}
		}
	}
};