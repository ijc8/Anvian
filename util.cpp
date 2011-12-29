#include <GL/glew.h>
#include <glm/glm.hpp>
#include <SDL/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

float degToRad(float degree) {
    const float fdegToRad = 3.14159265 * 2 / 360;
    return degree * fdegToRad;
}

glm::vec3 sphericalToEuclidean(glm::vec3 sphereCoords) {
    	float phi = degToRad(sphereCoords.x);
	float theta = degToRad(sphereCoords.y + 90.0f);

	float sinTheta = sinf(theta);
	float cosTheta = cosf(theta);
	float cosPhi = cosf(phi);
	float sinPhi = sinf(phi);

        glm::vec3 dir(sinTheta * cosPhi, cosTheta, sinTheta * sinPhi);
        return (dir * sphereCoords.z);
}

char *readFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    long size;
    char *buffer;

    fseek(file, 0, SEEK_END);
    size = ftell(file);
    rewind(file);

    buffer = (char *)malloc(size + 1);
    fread(buffer, size, 1, file);
    fclose(file);
    buffer[size] = '\0';

    return buffer;
}

GLuint loadShader(GLenum shaderType, const char *filename) {
    GLuint shader = glCreateShader(shaderType);
    char *data = readFile(filename);

    glShaderSource(shader, 1, (const GLchar**)&data, NULL);
    glCompileShader(shader);

    free(data);

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        GLint infoLogLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

        GLchar *strInfoLog = (GLchar *)malloc(infoLogLength + 1);
        glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

        fprintf(stderr, "Compile failure in shader file \"%s\". Error:\n%s\n",
                filename, strInfoLog);

        free(strInfoLog);
    }

    return shader;
}

GLuint createProgram(GLuint vertexShader, GLuint fragShader) {
    GLuint program = glCreateProgram();

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);

    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        GLint infoLogLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

        GLchar *strInfoLog = (char *)malloc(infoLogLength + 1);
        glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
        fprintf(stderr, "Linker failure: %s\n", strInfoLog);
        free(strInfoLog);
        return -1;
    }

    return program;
}

GLuint loadTexture(const char *filename) {
    GLuint texture;
    GLenum format = GL_RGB;
    SDL_Surface *image = IMG_Load(filename);

    if (!image) {
        fprintf(stderr, "Failed to load image \"%s\": %s\n", filename, IMG_GetError());
        return -1;
    }

    if (image->format->Amask)
        format = GL_RGBA;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, format, image->w, image->h,
                 0, format, GL_UNSIGNED_BYTE, image->pixels);

    SDL_FreeSurface(image);

    return texture;
}
