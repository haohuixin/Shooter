#include "TileLayer.h"
#include "Game.h"
#include <cmath>

TileLayer::TileLayer(int tileSize, const std::vector<Tileset> &tilesets) :
	m_tileSize(tileSize), m_tilesets(tilesets), m_position(0, 0), m_velocity(0, 0)
{
	m_numColumns = (TheGame::Instance()->getGameWidth() / m_tileSize);
	m_numRows = (TheGame::Instance()->getGameHeight() / m_tileSize);
}

void TileLayer::update()
{
	m_position += m_velocity;
	m_velocity.setX(5);
}

void TileLayer::setTileIDs(const std::vector<std::vector<int>>& data)
{
	m_tileIDs = data;
}

void TileLayer::setTileSize(int tileSize)
{
	m_tileSize = tileSize;
}

Tileset TileLayer::getTilesetByID(int tileID)
{
	for (int i = 0; i < m_tilesets.size(); i++)
	{
		if (i + 1 <= m_tilesets.size() - 1)
		{
			if (tileID >= m_tilesets[i].firstGridID&&tileID < m_tilesets[i + 1].firstGridID)
			{
				return m_tilesets[i];
			}
		}
		else
		{
			return m_tilesets[i];
		}
	}
	std::cout << "Did not find tileset. Returning empty tileset.\n";
	Tileset t;
	return t;
}

void TileLayer::render()
{
	int x, y, x2, y2 = 0;

	x = (int)(m_position.getX() / m_tileSize);
	y = (int)(m_position.getY() / m_tileSize);

	x2 = int(m_position.getX()) % m_tileSize;
	y2 = int(m_position.getY()) % m_tileSize;

	for (int i = 0; i < m_numRows+1; ++i)
	{
		for (int j = 0; j < m_numColumns+1; ++j)
		{
			int id = m_tileIDs[i][(j + x)%58];
			if (id == 0)
			{
				continue;
			}

			Tileset tileset = getTilesetByID(id);
			
			id--;
			TheTextureManager::Instance()->drawTile(tileset.name, 2, 2,
				(j * m_tileSize) - x2, (i * m_tileSize) - y2, m_tileSize,
				m_tileSize, (id - (tileset.firstGridID - 1)) /
				tileset.numColumns, (id - (tileset.firstGridID - 1)) %
				tileset.numColumns, TheGame::Instance()->getRenderer());
		}
	}
}

