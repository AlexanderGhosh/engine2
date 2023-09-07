#include "TextRendering.h"
#include <GL/glew.h>
#include <iostream>
#include <glfw3.h>
#include "../../Utils/ResourceLoader.h"
#include "../../Rendering/Shading/Manager.h"
#include "Font.h"

unsigned GUI::TextRendering::shaderId = 0;
unsigned GUI::TextRendering::VAO = 0;
unsigned GUI::TextRendering::VBO = 0;
unsigned GUI::TextRendering::EBO = 0;
std::unordered_map<std::string, GUI::Font*> GUI::TextRendering::loadedFonts = { };
bool GUI::TextRendering::isInitalized = 0;
bool GUI::TextRendering::isCleandUp = 0;

void GUI::TextRendering::init()
{
    if (isInitalized)
        return;
    isCleandUp = isInitalized = 1;

    unsigned int indices[] = {
        0
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

    /*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    shaderId = ResourceLoader::getShader("TextShader", 1);
}

glm::mat4 GUI::TextRendering::getCharacterModel(Float x, Float y, Float scale)
{
    glm::mat4 res(1);
    res = glm::translate(res, glm::vec3(x, y, 0));
    res = glm::scale(res, glm::vec3(scale, scale, 0));
    return res;
}

void GUI::TextRendering::addFont(Font* font)
{
    loadedFonts[font->getName()] = font;
}

void GUI::TextRendering::drawText(String text, float x, Float y, Float scale, Materials::MatItemBase<glm::vec4>* colour, String fontName, glm::mat4 t)
{
    try {
        const Font& font = *loadedFonts.at(fontName);
        drawText(text, x, y, scale, colour, font, t);
    }
    catch(std::exception e)
    {

    }
}

void GUI::TextRendering::drawText(String text, float x, Float y, Float scale, Materials::MatItemBase<glm::vec4>* colour, const Font& font, glm::mat4 t)
{
    init();
    Render::Shading::Manager::setActive(GUI::TextRendering::shaderId);
    int unit = 0;
    colour->tryBindTexture(unit);
    Render::Shading::Manager::setValue("colour", *colour, unit);
    Render::Shading::Manager::setValue("text", 1);
    glBindVertexArray(VAO);

    float size = 100;

    // iterate through all characters
    for (auto c = text.begin(); c != text.end(); c++)
    {
        Character ch = font.getCharacter(*c);

        float xpos = x + ch.bearing.x * scale;
        float ypos = y - (ch.size.y - ch.bearing.y) * scale;

        float w = ch.size.x * scale;
        float h = ch.size.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,  0.0f, 0.0f },
            { xpos,     ypos,      0.0f, 1.0f },
            { xpos + w, ypos,      1.0f, 1.0f },

            { xpos,     ypos + h,  0.0f, 0.0f },
            { xpos + w, ypos,      1.0f, 1.0f },
            { xpos + w, ypos + h,  1.0f, 0.0f }
        };
        // render glyph texture over quad
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, ch.texId);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.advance) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Vector2 GUI::TextRendering::getStringDimentions(String text, Unsigned scale, const Font& font)
{
    glm::vec2 res(0, -INFINITY);
    for (auto c = text.begin(); c != text.end(); c++) {
        const char& c_ = *c;
        const GUI::Character& character = font.getCharacter(c_);
        // res.x += (character.advance >> 6) * scale;

        const int& y = character.size.y;
        if (y > res.y) {
            res.y = y;
        }
    }
    return res;
}

void GUI::TextRendering::setShader(Unsigned shaderId)
{
    GUI::TextRendering::shaderId = shaderId;
}

GUI::Font* GUI::TextRendering::getFont(String name)
{
    unsigned s = loadedFonts.size();
    Font* r = loadedFonts[name];
    if (loadedFonts.size() > s) {
        loadedFonts.erase(name);
        return nullptr;
    }
    return r;
}

void GUI::TextRendering::cleanUp()
{
    if (isCleandUp)
        return;
    for (auto itt = loadedFonts.begin(); itt != loadedFonts.end();) {
        (*itt).second->cleanUp();
        itt = loadedFonts.erase(itt);
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    isCleandUp = 1;
}
