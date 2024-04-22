#ifndef ENEMYSPAWNER_H
#define ENEMYSPAWNER_H
#include "Enemy.h"
#include "Tile.h"
// a tile we can place that will spawn and manage enemies. when out of view it will ensure all enemies associated with the tile despawn
class EnemySpawner:public Tile
{
private:
	sf::Vector2i gridPosition;
	int Enemytype;
	int EnemyCount;
	int EnemySpawnTimer;
	float EnemymaxDistance;
	sf::RectangleShape shape;
public:
	EnemySpawner(int grid_x, int grid_y, float gridSizeF, const sf::Texture& texture, const sf::IntRect texture_rect, 
		int enemy_type, int enemy_count,int enemy_spawn_timer,int enemy_max_Distance );
	virtual ~EnemySpawner();
	void update() override;
	void render(sf::RenderTarget& target, sf::Vector2f player_Pos = sf::Vector2f{}, sf::Shader* shader = nullptr) const;
	virtual const std::string getAsString() const;
};


#endif