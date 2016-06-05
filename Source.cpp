#include <iostream>
#include <glew.h>
#include <glfw3.h>
#include <iostream>
#include <fstream> // para trabajar con archivos
#include <string>

// ancho y alto de la ventana
int const WIDTH = 640, HEIGHT = 380;

// vertex data
GLfloat triangle_vertices[] = {
	0.0f, .5f, 0.0f,
	-.5f, -.5f, 0.0f,
	.5f, -.5f, 0.0f
};

// funcion calback para manejar entrada dpor teclado del usuario
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// helper para leer archivos
std::string readFile(const char*);

GLuint createShaderFromFile(const GLchar*, GLenum);

int main()
{
	// iniciar glfw
	glfwInit();
	// configure the glfw window
	glfwWindowHint(GLFW_OPENGL_CORE_PROFILE, GLFW_OPENGL_ANY_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	// crear la ventana
	GLFWwindow* myWindow = glfwCreateWindow(WIDTH, HEIGHT, "Hello window repaso", nullptr, nullptr);

	// si la ventana no se creo...
	if (myWindow == nullptr)
	{
		glfwTerminate();
		std::cout << "Ventana no pudo ser creada" << std::endl;
		system("pause");
		return -1;
	}
	// hacemos actual el contexto recien creado de nuestra ventana
	glfwMakeContextCurrent(myWindow);

	// permitimos q glew use metodos experimentales de opengl
	glewExperimental = GL_TRUE;
	// si glew no puede ser iniciado...
	if (glewInit() != GLEW_OK)
	{
		std::cout << "glew no pudo ser iniciado" << std::endl;
		system("pause");
 		return -1;
	}

	// obtener las dimensiones dibujables de la ventana actual
	int width, height;
	glfwGetFramebufferSize(myWindow, &width, &height);

	// especificar a opengl el espacio q queremos q use de la ventana para q dibuje
	glViewport(0, 0, width, height);

	// registramos la funcion key_callback con el callback apropiado
	glfwSetKeyCallback(myWindow, key_callback);

	// crear el VBO(vertex buffer object) con el fin de designar un espacio en la GPU para los vertices del triangulo
	GLuint VBO_ID;
	glGenBuffers(1, &VBO_ID);
	// bind the newly created buffer to the GL_ARRAY_BUFFER target
	glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);
	// copies the previously defined triangle vertices data into the buffer's memory:
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), triangle_vertices, GL_STATIC_DRAW);

	
	// creamos el vertex shader
	GLuint vertexShader_ID = createShaderFromFile("vertexShader.vert", GL_VERTEX_SHADER);

	// simple game loop
	while (!glfwWindowShouldClose(myWindow))
	{
		// check and call event s
		glfwPollEvents();

		// rendering comands here
		glClearColor(.2, .3, .3, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		// swap the buffers
		glfwSwapBuffers(myWindow);
	}

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

GLuint createShaderFromFile(const char *filepath, GLenum shader_type)
{
	GLint shader_ID = glCreateShader(shader_type); // crea un shader program
	std::string source_str = readFile(filepath); // carga el source code del shader program
	const char *source_char = source_str.c_str(); // convierte source code a c-style
	glShaderSource(shader_ID, 1, &source_char, NULL); // define el source code del shader program
	glCompileShader(shader_ID); // compila el shader program

	// comprueba estado de la compilacion
	GLint compileStatus;
	glGetShaderiv(shader_ID, GL_COMPILE_STATUS, &compileStatus);
	// si la compilacion no fue exitosa...
	if (compileStatus != GL_TRUE)
	{
		std::cout << "Shader failed to compile" << std::endl;
		// obtenemos el lenght en bytes del mensaje log
		GLint infoLoglength;
		glGetShaderiv(shader_ID, GL_INFO_LOG_LENGTH, &infoLoglength);
		// mostramos el log message
		GLchar *infoLog = new GLchar[infoLoglength + 1];
		glGetShaderInfoLog(shader_ID, infoLoglength, NULL, infoLog);
		std::cout << infoLog << std::endl;
		delete infoLog;
		return 0;
	}

	return shader_ID;
}


std::string readFile(const char *filePath) 
{
	std::ifstream myFile;
	myFile.open(filePath);
	// si el archivo no abre
	if (!myFile.is_open())
	{
		myFile.close();
		std::cout << "Shader Not Found" << std::endl;
		return "";
	}
	std::string source((std::istreambuf_iterator<GLchar>(myFile)), std::istreambuf_iterator<GLchar>());
	myFile.close();
	return source;
}
