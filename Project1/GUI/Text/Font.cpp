#include "Font.h"
#include <GL/glew.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "TextRendering.h"

GUI::Character::Character() : texId(0), size(0), bearing(0), advance(0)
{
}

GUI::Font::Font() : name("")
{
   
}

GUI::Font::Font(String fontName) : Font()
{
    this->name = fontName;
    FT_Library ft;
    // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return;
    }

    // find path to font
    // C:\Windows\Fonts
    // Resources/Fonts/
    std::string font_name = "C:\\Windows\\Fonts\\" + fontName + ".ttf";

    // load font as face
    FT_Face face;
    if (FT_New_Face(ft, font_name.c_str(), 0, &face)) {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return;
    }
    else {
        // set size to load glyphs as
        FT_Set_Pixel_Sizes(face, 0, 128);

        // disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // load first 128 characters of ASCII set
        for (unsigned char c = 0; c < 128; c++)
        {
            // Load character glyph 
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
            Character character = Character();

            character.texId = texture;

            character.size =    glm::vec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
            character.bearing = glm::vec2(face->glyph->bitmap_left , face->glyph->bitmap_top );

            character.advance = static_cast<unsigned>(face->glyph->advance.x) >> 6;

            chars.insert({ c, character });
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    // destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void GUI::Font::cleanUp()
{
    for (auto itt = chars.begin(); itt != chars.end();) {
        glDeleteTextures(1, &(*itt).second.texId);
        itt = chars.erase(itt);
    }
}

const GUI::Character& GUI::Font::getCharacter(const char& character) const
{
	return chars.at(character);
}

String GUI::Font::getName() const
{
    return name;
}
