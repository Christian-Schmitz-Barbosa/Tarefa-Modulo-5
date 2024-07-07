#ifndef SPRITE_H
#define SPRITE_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include "stb_image.h"



class Sprite {
public:
    GLuint VAO, VBO, EBO, textureID;
    glm::vec3 position, scale;
    float rotation;
    GLuint shaderProgram;
    float speed = 200.0f;

    float currentTime;

    int spriteWidth, spriteHeight;
    float texX, texY, texW, texH;
    int animationFlag;

    Sprite(GLuint shaderProgram, const char* texturePath)
        : shaderProgram(shaderProgram), position(0.0f), scale(1.0f), rotation(0.0f),
            currentTime(0.0f), animationFlag(-1), texW(0.25f), texH(0.25f), texX(0.0f), texY(0.0f){
        init();
        textureID = loadTexture(texturePath);
    }

    void init() {
        float ds = 1.0f / 4.0f;
        float vertices[] = {
            // positions          // texture coords
            -0.5f, -0.5f, 0.0f,  0.0f,0.0f ,
             0.5f, -0.5f, 0.0f,  ds, 0.0f,
             0.5f,  0.5f, 0.0f,  ds, ds,
            -0.5f,  0.5f, 0.0f,  0.0f, ds
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void animation(int position, float deltaTime) {
        currentTime += deltaTime;
        if (currentTime < 0.25f && animationFlag == position) {
            return;
        }
        currentTime = 0.0f;
        switch (position) {
        case 0:
            if (animationFlag != 0 || texX >= 0.75f) {
                texX = 0.0f;
                animationFlag = 0;
            }
            else {
                texX += 0.25f;
            }
            texY = 0.75f;
            break;
        case 1:
            if (animationFlag != 1 || texX >= 0.75f) {
                texX = 0.0f;
                animationFlag = 1;
            }
            else {
                texX += 0.25f;
            }
            texY = 0.0f;
            break;
        case 2:
            if (animationFlag != 2 || texX >= 0.75f) {
                texX = 0.0f;
                animationFlag = 2;
            }
            else {
                texX += 0.25f;
            }
            texY = 0.5f;
            break;
        case 3:
            if (animationFlag != 3 || texX >= 0.75f) {
                texX = 0.0f;
                animationFlag = 3;
            }
            else {
                texX += 0.25f;
            }
            texY = 0.25f;
            break;
        }
    }

    void update() {
        float vertices[] = {
            // positions          // texture coords
            -0.5f, -0.5f, 0.0f,  texX,        texY,
             0.5f, -0.5f, 0.0f,  texX + texW, texY,
             0.5f,  0.5f, 0.0f,  texX + texW, texY + texH,
            -0.5f,  0.5f, 0.0f,  texX,        texY + texH
        };

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(scale.x, scale.y, scale.z));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    }

    void draw(glm::mat4 projection) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = glm::scale(model, scale);

        glUseProgram(shaderProgram);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        glBindTexture(GL_TEXTURE_2D, textureID);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

    }

private:
    GLuint loadTexture(const char* path) {
        GLuint textureID;
        glGenTextures(1, &textureID);

        int width, height, nrComponents;
        unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
        if (data) {
            GLenum format;
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            spriteWidth = width;
            spriteHeight = height;

            stbi_image_free(data);
        }
        else {
            std::cout << "Failed to load texture" << std::endl;
            stbi_image_free(data);
        }

        return textureID;
    }
};

#endif // SPRITE_H
