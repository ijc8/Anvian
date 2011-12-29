char *readFile(const char *filename);
GLuint loadShader(GLenum shaderType, const char *filename);
GLuint createProgram(GLuint vertexShader, GLuint fragShader);
GLuint loadTexture(const char *filename);
float degToRad(float degree);
