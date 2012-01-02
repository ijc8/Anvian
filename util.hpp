#include <glm/glm.hpp>

float degToRad(float degree);
glm::vec3 sphericalToEuclidean(glm::vec3 sphereCoords);

char *readFile(const char *filename);
GLuint loadShader(GLenum shaderType, const char *filename);
GLuint createProgram(GLuint vertexShader, GLuint fragShader);
GLuint loadTexture(const char *filename);
