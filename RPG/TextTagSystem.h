#ifndef TEXTTAGSYSTEM_H
#define TEXTTAGSYSTEM_H
enum TagTypes {DEFAULT_TYPE, NEGATIVE, POSITIVE,EXPERIENCE, ENVIRONMENT};
class TextTagSystem
{
private:
	class TextTag {
	private:

		float dirY;
		float dirX;
		
		float speed;

		
	public:
		sf::Text txt;
		float lifeTime;
		TextTag(TextTag* tag,float pos_x,float pos_y, std::string str) {
			
			txt = tag->txt;
			txt.setPosition(pos_x, pos_y);
			txt.setString(str);
			dirX = tag->dirX;
			dirY = tag->dirY;
			lifeTime = tag->lifeTime;
			speed = tag->speed;
		}
		TextTag(sf::Font& font, std::string text, float pos_x, float pos_y,
			sf::Color color, unsigned char_size, float life_t, float spd, float dir_x, float dir_y) {
			txt.setFont(font);
			txt.setPosition(pos_x, pos_y);
			txt.setString(text);
			dirX = dir_x;
			dirY = dir_y;
			lifeTime = life_t;
			speed = spd;
			txt.setFillColor(color);
			txt.setCharacterSize(char_size);
		}
		~TextTag() {
			
		}
		const bool isactive() const{
			return lifeTime <= 0;
		}
		//Functions

		void update(const float& dt) {
			if (lifeTime > 0.f) {
				lifeTime -= 100 * dt;
				txt.move(dirX * speed * dt, dirY * speed * dt);

			}
			
		}
		void render(sf::RenderTarget* target) {
			target->draw(txt);
		}

	};
	std::vector<TextTag*> Tags;
	sf::Font font;
	std::map<unsigned, TextTag*> tagTemplates;
	//private functions
	void initVariables();
	void initTagTemplates();
	void initFonts(std::string font_file);
public:
	TextTagSystem(std::string font_file);
	virtual ~TextTagSystem();
	void createTextTagString(unsigned tagType, const float pos_x, const float pos_y, const std::string str, const std::string prefix, const std::string postifix);
	void createTextTagString(unsigned tagType, const float pos_x, const float pos_y, const int i, const std::string prefix, const std::string postifix);
	void createTextTagString(unsigned tagType, const float pos_x,const float pos_y, const double d, const std::string prefix, const std::string postifix);
	void removetextTag();
	void update(const float& dt);
	void render(sf::RenderTarget* target);
};

#endif