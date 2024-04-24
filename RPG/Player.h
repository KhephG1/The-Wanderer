#ifndef PLAYER_H
#define PLAYER_H
#include "Entity.h"
#include "Items.h"
#include "Inventory.h"
class Player: public Entity
{
private:
	bool facing_right;
	bool attacking;
	bool show_hitbox;
	Sword* sword;
	Inventory* inventory;
	void initVariables();
	void initComponents();
	void initAnimations();
	void initInventory();
public:
	Player(float xpos, float ypos, sf::Texture* tex_sheet);
	virtual ~Player();
	AttributeComponent* getAttributeComponent();
	//Functions
	
	void updateAnimation(const float& dt);
	virtual void update(const float& dt, sf::Vector2f mousePosView) override;
	virtual void render(sf::RenderTarget* target, sf::Shader* shader, const bool show_hitbox = false, sf::Vector2f light_position = sf::Vector2f{}) override;
	void loseHP(const int hp);
	void gainHP(const int hp);
	void gainXP(const int xp);
	void loseXP(const int xp);
	void hideHitbox(const bool choice);
	const bool showHitbox() const;
	 Weapon* getWeapon()const;
};
#endif
