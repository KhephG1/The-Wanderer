#include "stdafx.h"
#include "TileMap.h"
void TileMap::clear()
{
	
		for (int x = 0; x < tMap.size(); x++) {
			for (int y{}; y < tMap.at(x).size(); y++) {
				for (int z{}; z < tMap.at(x).at(y).size(); z++) {
					for (int k = 0; k < (int)tMap.at(x).at(y).at(z).size(); k++) {
						delete tMap.at(x).at(y).at(z).at(k);
						tMap.at(x).at(y).at(z).at(k) = nullptr;
					}
					tMap.at(x).at(y).at(z).clear();
				}
				tMap.at(x).at(y).clear();
			}
			tMap.at(x).clear();
		}
		tMap.clear();
	
}
TileMap::TileMap(float gridSize, int width, int height, std::string texture_file) {
	gridSizeI = (int)gridSize;
	maxSizeGrid.x = width;
	maxSizeGrid.y = height;
	maxWorldSize.x = static_cast<float>(width) * gridSize;
	maxWorldSize.y = static_cast<float>(height) * gridSize;
	layers = 1;
	gridSizeF = gridSize;
	gridSizeI = (unsigned)gridSizeF;
	textureFile = texture_file;
	tMap.reserve(maxSizeGrid.x);
	for (int x = 0; x < maxSizeGrid.x; x++) {
		tMap.push_back(std::vector<std::vector<std::vector<Tile*>>> {});
		for (int y{}; y < maxSizeGrid.y; y++) {
			tMap.reserve(maxSizeGrid.y);
			tMap.at(x).push_back(std::vector<std::vector<Tile*>>{});
			for (int z{}; z < layers; z++) {
				tMap.at(x).at(y).reserve(layers);
				tMap.at(x).at(y).push_back(std::vector<Tile*>{});
			}
		}
	}
	if (!tileSheet.loadFromFile(texture_file)) {
		throw std::runtime_error{ "ERROR" };
	}

	collision_box.setSize(sf::Vector2f{ gridSizeF, gridSizeF });
	collision_box.setFillColor(sf::Color(sf::Color::Transparent));
	collision_box.setOutlineThickness(1.f);
	collision_box.setOutlineColor(sf::Color::Red);

	from_x = 0;
	from_y = 0;
	to_x = 0;
	to_y = 0;
}

TileMap::TileMap(const std::string file_name)
{

	from_x = 0;
	from_y = 0;
	to_x = 0;
	to_y = 0;
	layers = 1;
	loadFromFile(file_name);
	collision_box.setSize(sf::Vector2f{ gridSizeF, gridSizeF });
	collision_box.setFillColor(sf::Color(sf::Color::Transparent));
	collision_box.setOutlineThickness(1.f);
	collision_box.setOutlineColor(sf::Color::Red);
}

TileMap::~TileMap()
{
	clear();
}


void TileMap::render(sf::RenderTarget& target, const sf::Vector2i gridPosition, const bool show_collision,sf::Vector2f playerPos, sf::Shader* shader)
{
	int layer = 0;
	from_x = gridPosition.x - 12;
	if (from_x < 0) {
		from_x = 0;
	}
	to_x = gridPosition.x + 13;
	if (to_x > maxSizeGrid.x) {
		to_x = maxSizeGrid.x;
	}

	from_y = gridPosition.y - 9;
	if (from_y < 0) {
		from_y = 0;
	}
	to_y = gridPosition.y + 11;
	if (to_y > maxSizeGrid.y) {
		to_y = maxSizeGrid.y;
	}

	for (int x = from_x; x < to_x; x++) {

		for (int y = from_y; y < to_y; y++) {
			for (int k{}; k < (int)tMap.at(x).at(y).at(layer).size(); k++){
				if (tMap.at(x).at(y).at(layer).at(k)->getType() == ABOVE) {
					deferredRenderTiles.push(tMap.at(x).at(y).at(layer).at(k));
				}
				else {
					if (shader) {
						tMap.at(x).at(y).at(layer).at(k)->render(target, playerPos, shader);
					}
					else {
						tMap.at(x).at(y).at(layer).at(k)->render(target);
					}
				}
				if(show_collision){
					if (tMap.at(x).at(y).at(layer).at(k)->getCollision()) {
						collision_box.setPosition(tMap.at(x).at(y).at(layer).at(k)->getPosition());
						target.draw(collision_box);
					}
				}
				if (tMap.at(x).at(y).at(layer).at(k)->getType() == TileTypes::ENEMYSPAWNER) {
					collision_box.setPosition(tMap.at(x).at(y).at(layer).at(k)->getPosition());
					target.draw(collision_box);
				}
			}
		}
	}
	
}

void TileMap::DeferredRender(sf::RenderTarget& target,sf::Vector2f playerPos, sf::Shader* shader)
{
	while (!deferredRenderTiles.empty()) {
		if (shader) {
			deferredRenderTiles.top()->render(target, playerPos, shader);
		}
		else {
			deferredRenderTiles.top()->render(target);
		}
		deferredRenderTiles.pop();
	}
}

void TileMap::addTile(const int x, const int y, const int z, const sf::IntRect texture_rect, bool collision, const short type)
{
	//Takes 2 indices from the mouse position in the grid and adds a tile to that position
	if (x < maxSizeGrid.x && y < maxSizeGrid.y && z<layers && x >=0 && y>=0 && z>=0) {
		
		if (type == TileTypes::DEFAULT) {
			tMap.at(x).at(y).at(z).push_back(new RegularTile(type, x, y, gridSizeF, tileSheet, texture_rect, collision));
		}
		else if (type == TileTypes::ENEMYSPAWNER) {
			tMap.at(x).at(y).at(z).push_back(new EnemySpawner(x, y, gridSizeF, tileSheet, texture_rect,0,0,0,0));
		}
		
	}
}

void TileMap::removeTile(const int x, const int y, const int z, const int type)
{
	if (x < maxSizeGrid.x && y < maxSizeGrid.y && z < layers && x >= 0 && y >= 0 && z >= 0) {
		if (!tMap.at(x).at(y).at(z).empty()) { // if the tile at this index is empty, we can remove a tile
			if (type > 0) {
				if (tMap.at(x).at(y).at(z).back()->getType() == type) {
					delete tMap.at(x).at(y).at(z).at(tMap.at(x).at(y).at(z).size() - 1);
					tMap.at(x).at(y).at(z).pop_back();
				}
			}
			else {
				delete tMap.at(x).at(y).at(z).at(tMap.at(x).at(y).at(z).size() - 1);
				tMap.at(x).at(y).at(z).pop_back();
			}
		}
	}
}
//saves the current game map being designed to a text file
void TileMap::saveToFile(const std::string file_name)
{
	/*Saves the entire tilemap to a text-file.
	Format:
	Basic:
	Size x y
	gridSize
	layers
	texture file

	All tiles:
	type
	gridPos x y layer
	Texture rect x y
	collision
	tile_specific...
	*/

	std::ofstream out_file;

	out_file.open(file_name);

	if (out_file.is_open())
	{
		out_file<< this->maxSizeGrid.x << " " << this->maxSizeGrid.y << "\n"
			<<gridSizeI << "\n"
			<< layers << "\n"
			<< textureFile << "\n";

		for (int x = 0; x < maxSizeGrid.x; x++)
		{
			for (int y = 0; y < maxSizeGrid.y; y++)
			{
				for (int z = 0; z < this->layers; z++)
				{
					if (!this->tMap[x][y][z].empty())
					{
						for (size_t k = 0; k < this->tMap[x][y][z].size(); k++)
						{
							out_file << x << " " << y << " " << z << " " <<
								this->tMap[x][y][z][k]->getAsString()
								<< " ";
						}
					}
				}
			}
		}
	}
	else
	{
		std::cout << "ERROR::TILEMAP::COULD NOT SAVE TO FILE::FILENAME: " << file_name << "\n";
	}

	out_file.close();
}
//loads a premade game map from a text file
void TileMap::loadFromFile(const std::string file_name)
{

	std::ifstream in_file;

	in_file.open(file_name);

	if (in_file.is_open())
	{
		sf::Vector2i size;
		int gridSize = 0;
		int layers = 0;
		std::string texture_file = "";
		int x = 0;
		int y = 0;
		int z = 0;
		int trX = 0;
		int trY = 0;
		bool collision = false;
		short type = 0;

		//Basics
		in_file >> size.x >> size.y >> gridSize >> layers >> texture_file;

		//Tiles
		gridSizeF = static_cast<float>(gridSize);
		gridSizeI = gridSize;
		maxSizeGrid.x = size.x;
		maxSizeGrid.y = size.y;
		maxWorldSize.x = static_cast<float>(size.x * gridSize);
		maxWorldSize.y = static_cast<float>(size.y * gridSize);
		this->layers = layers;
		textureFile = texture_file;

		clear();

		tMap.resize(maxSizeGrid.x, std::vector< std::vector< std::vector<Tile*> > >());
		for (int x = 0; x < maxSizeGrid.x; x++)
		{
			for (int y = 0; y < maxSizeGrid.y; y++)
			{
				this->tMap[x].resize(maxSizeGrid.y, std::vector< std::vector<Tile*> >());

				for (int z = 0; z < layers; z++)
				{
					this->tMap[x][y].resize(layers, std::vector<Tile*>());
				}
			}
		}

		if (!tileSheet.loadFromFile(texture_file))
			std::cout << "ERROR::TILEMAP::FAILED TO LOAD TILETEXTURESHEET::FILENAME: " << texture_file << "\n";

		//Load all tiles
		while (in_file >> x >> y >> z >> type)
		{
			std::cout << type << "\n";
			if (type == TileTypes::ENEMYSPAWNER)
			{
				//amount, time, max dist
				int enemy_type = 0;
				int	enemy_am = 0;
				int	enemy_tts = 0;
				int	enemy_md = 0;

				in_file >> trX >> trY >> enemy_type >> enemy_am >> enemy_tts >> enemy_md;

				tMap[x][y][z].push_back(
					new EnemySpawner(
						x, y,
						gridSizeF,
						tileSheet,
						sf::IntRect(trX, trY, gridSizeI, gridSizeI),
						enemy_type,
						enemy_am,
						enemy_tts,
						enemy_md
					)
				);
			}
			else
			{
				in_file >> trX >> trY >> collision;

				this->tMap[x][y][z].push_back(
					new RegularTile(
						type,
						x, y,
						gridSizeF,
						tileSheet,
						sf::IntRect(trX, trY, gridSizeI, gridSizeI),
						collision
					)
				);
			}
		}
	}
	else
	{
		std::cout << "ERROR::TILEMAP::COULD NOT LOAD FROM FILE::FILENAME: " << file_name << "\n";
	}

	in_file.close();
}


const sf::Texture* TileMap::getileSheet() const
{
	return &tileSheet;
}

void TileMap::update(Entity* entity, const float& dt)
{
	//Check world bounds collisions
	if (entity->getPosition().x < 0.f) {
		entity->setPosition(0.f, entity->getPosition().y);
		entity->stopVelocityX();
	}else if (entity->getPosition().x + entity->getGlobalBounds().width > maxWorldSize.x) {

		std::cout << maxWorldSize.x<<" "<<maxWorldSize.y << std::endl;
		entity->setPosition(maxWorldSize.x - entity->getGlobalBounds().width, entity->getPosition().y);
		entity->stopVelocityX();
	}
	if (entity->getPosition().y < 0.f) {
		entity->setPosition(entity->getPosition().x, 0.f);
		entity->stopVelocityY();
	}
	else if (entity->getPosition().y + entity->getGlobalBounds().height > maxWorldSize.y) {
		entity->setPosition(entity->getPosition().x, maxWorldSize.y - entity->getGlobalBounds().height);
		entity->stopVelocityY();
	}
	//Tile culling
	//Only checking for collisions with tiles near the player since these are the only tiles that the player could collide with at any given moment
	//if the player is at position 100,100 it will never collide with a tile at 9000,9000.
	layers = 0;
	from_x = entity->getGridPosition(gridSizeI).x -1;
	if (from_x < 0) {
		from_x = 0;
	}
	to_x = entity->getGridPosition(gridSizeI).x + 3;
	if (to_x > maxSizeGrid.x) {
		to_x = maxSizeGrid.x;
	}

	from_y = entity->getGridPosition(gridSizeI).y- 1;
	if (from_y < 0) {
		from_y = 0;
	}
	to_y = entity->getGridPosition(gridSizeI).y + 3;
	if (to_y > maxSizeGrid.y) {
		to_y = maxSizeGrid.y;
	}
	
	for (int x = from_x; x < to_x; x++) {
		
		for (int y = from_y; y < to_y; y++) {
			for(int k =0; k < (int)tMap.at(x).at(y).at(layers).size();k++){
				tMap.at(x).at(y).at(layers).at(k)->update();
				sf::FloatRect playerBounds = entity->getGlobalBounds();
				sf::FloatRect nextBounds = entity->getNextBounds(dt);
				sf::FloatRect wallBound = tMap.at(x).at(y).at(layers).at(k)->getGlobalBounds();
				//check for a collision (any collision) using the intersects function
				if (tMap.at(x).at(y).at(layers).at(k)->intersects(nextBounds) && tMap.at(x).at(y).at(layers).at(k)->getCollision()) {
					//once we have identified a collision check what type it is
					//right collision
					if (playerBounds.left < wallBound.left
						&& playerBounds.left + playerBounds.width < wallBound.left + wallBound.width
						&& playerBounds.top < wallBound.top + wallBound.height 
						&& playerBounds.top + playerBounds.height > wallBound.top
						) {
						entity->stopVelocityX();
						entity->setPosition(wallBound.left - playerBounds.width, playerBounds.top);
					
					}
					//left collision
					else if (playerBounds.left > wallBound.left
						&& playerBounds.left + playerBounds.width > wallBound.left + wallBound.width
						&& playerBounds.top < wallBound.top + wallBound.height
						&& playerBounds.top + playerBounds.height > wallBound.top
						) {
						entity->stopVelocityX();
						entity->setPosition(wallBound.left + wallBound.width, playerBounds.top);
					
					}
					//Bottom collision
					if (playerBounds.top < wallBound.top
						&& playerBounds.top + playerBounds.height < wallBound.top + wallBound.height
						&& playerBounds.left < wallBound.left + wallBound.width
						&& playerBounds.left + playerBounds.width > wallBound.left
						) {
						entity->stopVelocityY();
						entity->setPosition(playerBounds.left, wallBound.top - playerBounds.height);
					
					}
					//top collision
					else if (playerBounds.top > wallBound.top
						&& playerBounds.top + playerBounds.height > wallBound.top + wallBound.height
						&& playerBounds.left < wallBound.left + wallBound.width
						&& playerBounds.left + playerBounds.width > wallBound.left
						) {
						entity->stopVelocityY();
						entity->setPosition(playerBounds.left, wallBound.top + wallBound.height);
					}
				}
			}
			
		}
	}
}

const int TileMap::getLayer(const int x, const int y, const int layer) const
{
	if (x >= 0 && x < (int)tMap.size()) {
		if (y >= 0 && y < (int)tMap.at(x).size()) {
			if (layer >= 0 && layer < (int)tMap.at(x).at(y).size()) {
				return tMap.at(x).at(y).at(layer).size();
			}
		}
	}
	return -1;
	//throw std::invalid_argument{ "TileMap::getLayer ERROR" };

}

const sf::Vector2i& TileMap::getMaxSizeGrid() const
{
	return maxSizeGrid;
}

const sf::Vector2f& TileMap::getMaxSizeFloat() const
{
	return maxWorldSize;
}

const bool TileMap::checkType(const int type, const int x, const int y, const int z) const
{
	return tMap.at(x).at(y).at(0).back()->getType() == type;

}

const bool TileMap::hasTile(const int x, const int y, const int z){
	if (x > 0 && x < maxWorldSize.x && y >= 0 && y < maxWorldSize.y && z < layers) {
		return !tMap.at(x).at(y).at(z).empty();
	}
	return false;
}

