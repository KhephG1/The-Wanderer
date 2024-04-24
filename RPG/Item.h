#ifndef ITEM_H
#define ITEM_H

enum ItemTypes{DEFAULT_ITEM = 0, RANGEDWEAPON, MELEEWEAPON};
enum ItemRarities { COMMON = 0, UNCOMMON,RARE,EPIC,LENGENDARY,MYTHIC,NUTSINSANEBONKERSRARITY};
class Item
{
private:
	void initVariables();
protected:
	//Variables
	unsigned value;
	short unsigned type;

public:
	Item(unsigned value);
	virtual ~Item();


	//Functions
	virtual Item* clone() = 0;
};

#endif