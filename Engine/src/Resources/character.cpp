#include "character.hpp"

#include <glad/glad.h>

#include "debug.hpp"
#include "resources_manager.hpp"

namespace Resources
{
	Font::Font(const std::string& path)
	{
		FT_Library ft;
		if (FT_Init_FreeType(&ft))
		{
			Core::Debug::Log::error("Unable to init freetype lib" + path);
			return;
		}

		FT_Face face;
		if (FT_New_Face(ft, "resources/fonts/test.ttf", 0, &face))
		{
			Core::Debug::Log::error("Unable to load the font " + path);
			return;
		}

		FT_Set_Pixel_Sizes(face, 0, 48);

		if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
		{
			Core::Debug::Log::error("Unable to load the font glyph " + path);
			return;
		}

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

        for (unsigned char c = 0; c < 128; c++)
        {
            // load character glyph 
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }
            // generate texture
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );
            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // now store character for later use
            Character character = {
                texture,
                Core::Maths::vec2((float)face->glyph->bitmap.width, (float)face->glyph->bitmap.rows),
                Core::Maths::vec2((float)face->glyph->bitmap_left, (float)face->glyph->bitmap_top),
				(unsigned int)face->glyph->advance.x
            };

            charMap.insert(std::pair<char, Character>(c, character));
        }

        FT_Done_Face(face);
        FT_Done_FreeType(ft);
	}

	Text::Text(Engine::GameObject& gameObject, std::shared_ptr<Text> ptr)
		: Component(gameObject, ptr)
	{
		m_shaderProgram = Resources::ResourcesManager::loadShaderProgram("skyBox");
	}

	Text::Text(Engine::GameObject& gameObject, const std::string& path)
		: Text(gameObject, std::shared_ptr<Text>(this))
	{
		m_font = Resources::ResourcesManager::loadFont(path);
	}

	Text::~Text()
	{
		//Core::Debug::Log::info("Unload model " + cubeMap;
	}

	void Text::draw() const
	{
		
	}

	std::string Text::toString() const
	{
		return "";
	}

	void Text::parseComponent(Engine::GameObject& gameObject, std::istringstream& iss)
	{
		std::string path;
		iss >> path;

		gameObject.addComponent<Text>(path);
	}
}