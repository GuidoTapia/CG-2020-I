
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shaderClass.h>



const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
bool flagR = 0;
bool flagG = 0;
bool flagB = 0;
float value = 0.5;
# define M_PI   3.14159265358979323846 /* pi */ 
#define PROF 8
float T_CAMBIO = 1.0;
bool flag = false;
bool flagMov = false;
float timeGiro = 0.0f;
float timePause = 0.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		flag = !flag;
	}
	if (key == GLFW_KEY_M && action == GLFW_PRESS) {
		if (flagMov == true) {
			timePause = glfwGetTime();
		}
		else {
			timeGiro += glfwGetTime() - timePause;
		}
		flagMov = !flagMov;
	}
	else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS && T_CAMBIO > 0.01) {
		T_CAMBIO /= 1.5;
	}
	else if (key == GLFW_KEY_UP  && action == GLFW_PRESS && T_CAMBIO < 10.0) {
		T_CAMBIO *= 1.5;
	}
}


void recursion(float vertices[], unsigned int** indices, unsigned int v1, unsigned int v2, unsigned int v3, int& i_ver, int i_ind[], int prof=0) {
	if (prof < PROF-1) {
		unsigned int v1_2 = i_ver;
		unsigned int v1_3 = i_ver+1;
		unsigned int v2_3 = i_ver+2;
		v1 *= 6;
		v2 *= 6;
		v3 *= 6;
		i_ver *= 6;
		for (int i = 0; i < 6; ++i) {
			vertices[i_ver + i] = (vertices[v1 + i] + vertices[v2 + i]) / 2;
			vertices[i_ver + i+6] = (vertices[v1 + i] + vertices[v3 + i]) / 2;
			vertices[i_ver + i+12] = (vertices[v3 + i] + vertices[v2 + i]) / 2;
		}
		double mayor = std::max(vertices[i_ver + 3], std::max(vertices[i_ver + 4], vertices[i_ver + 5]));
		vertices[i_ver + 3] /= mayor;
		vertices[i_ver + 4] /= mayor;
		vertices[i_ver + 5] /= mayor;

		mayor = std::max(vertices[i_ver + 9], std::max(vertices[i_ver + 10], vertices[i_ver + 11]));
		vertices[i_ver + 9] /= mayor;
		vertices[i_ver + 10] /= mayor;
		vertices[i_ver + 11] /= mayor;
		
		mayor = std::max(vertices[i_ver + 15], std::max(vertices[i_ver + 16], vertices[i_ver + 17]));
		vertices[i_ver + 15] /= mayor;
		vertices[i_ver + 16] /= mayor;
		vertices[i_ver + 17] /= mayor;

		v1 /= 6;
		v2 /= 6;
		v3 /= 6;
		i_ver /= 6;
		i_ver += 3;
		recursion(vertices, indices, v1, v1_2, v1_3, i_ver, i_ind, prof + 1);
		recursion(vertices, indices, v1_2, v2, v2_3, i_ver, i_ind, prof + 1);
		recursion(vertices, indices, v1_3, v2_3, v3, i_ver, i_ind, prof + 1);
	}
	indices[prof][i_ind[prof]] = v1;
	i_ind[prof]++;
	indices[prof][i_ind[prof]] = v2;
	i_ind[prof]++;
	indices[prof][i_ind[prof]] = v3;
	i_ind[prof]++;
}
void recursion3D(float vertices[], unsigned int** indices, unsigned int v1, unsigned int v2, unsigned int v3, unsigned int v4, int& i_ver, int i_ind[], int prof=0) {
	if (prof < PROF - 1) {
		unsigned int v1_2 = i_ver;
		unsigned int v1_3 = i_ver + 1;
		unsigned int v1_4 = i_ver + 2;
		unsigned int v2_3 = i_ver + 3;
		unsigned int v2_4 = i_ver + 4;
		unsigned int v3_4 = i_ver + 5;
		v1 *= 6;
		v2 *= 6;
		v3 *= 6;
		v4 *= 6;
		i_ver *= 6;
		for (int i = 0; i < 6; ++i) {
			vertices[i_ver + i] = (vertices[v1 + i] + vertices[v2 + i]) / 2;
			vertices[i_ver + i + 6] = (vertices[v1 + i] + vertices[v3 + i]) / 2;
			vertices[i_ver + i + 12] = (vertices[v1 + i] + vertices[v4 + i]) / 2;
			vertices[i_ver + i + 18] = (vertices[v2 + i] + vertices[v3 + i]) / 2;
			vertices[i_ver + i + 24] = (vertices[v2 + i] + vertices[v4 + i]) / 2;
			vertices[i_ver + i + 30] = (vertices[v3 + i] + vertices[v4 + i]) / 2;
		}
		for (int i = 3; i < 36; i += 6) {
			double mayor = std::max(vertices[i_ver + i], std::max(vertices[i_ver + i + 1], vertices[i_ver + i + 2]));
			vertices[i_ver + i] /= mayor;
			vertices[i_ver + i + 1] /= mayor;
			vertices[i_ver + i + 2] /= mayor;
		}

		v1 /= 6;
		v2 /= 6;
		v3 /= 6;
		v4 /= 6;
		i_ver /= 6;
		i_ver += 6;
		recursion3D(vertices, indices, v1, v1_2, v1_3, v1_4, i_ver, i_ind, prof + 1);
		recursion3D(vertices, indices, v1_2, v2, v2_3, v2_4, i_ver, i_ind, prof + 1);
		recursion3D(vertices, indices, v1_3, v2_3, v3, v3_4, i_ver, i_ind, prof + 1);
		recursion3D(vertices, indices, v1_4, v2_4, v3_4, v4, i_ver, i_ind, prof + 1);
	}
	indices[prof][i_ind[prof]] = v1;
	i_ind[prof]++;
	indices[prof][i_ind[prof]] = v2;
	i_ind[prof]++;
	indices[prof][i_ind[prof]] = v3;
	i_ind[prof]++;
	indices[prof][i_ind[prof]] = v1;
	i_ind[prof]++;
	indices[prof][i_ind[prof]] = v2;
	i_ind[prof]++;
	indices[prof][i_ind[prof]] = v4;
	i_ind[prof]++;
	indices[prof][i_ind[prof]] = v1;
	i_ind[prof]++;
	indices[prof][i_ind[prof]] = v3;
	i_ind[prof]++;
	indices[prof][i_ind[prof]] = v4;
	i_ind[prof]++;
	indices[prof][i_ind[prof]] = v2;
	i_ind[prof]++;
	indices[prof][i_ind[prof]] = v3;
	i_ind[prof]++;
	indices[prof][i_ind[prof]] = v4;
	i_ind[prof]++;
}


int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);


	Shader ourShader("shaders\\shader.vs", "shaders\\shader.fs");

	float* vertices = new float[6 * pow(3, PROF)];

	vertices[0] = -0.606f;
	vertices[1] = -0.35f;
	vertices[2] = 0.0f;
	vertices[3] = 1.0f;
	vertices[4] = 0.0f;
	vertices[5] = 0.0f;

	vertices[6] = 0.606f;
	vertices[7] = -0.35f;
	vertices[8] = 0.0f;
	vertices[9] = 0.0f;
	vertices[10] = 1.0f;
	vertices[11] = 0.0f;

	vertices[12] = 0.0f;
	vertices[13] = 0.7f;
	vertices[14] = 0.0f;
	vertices[15] = 0.0f;
	vertices[16] = 0.0f;
	vertices[17] = 1.0f;
	unsigned int* indices[PROF];
	for (int i = 0; i < PROF; ++i) {
		indices[i] = new unsigned int[3 * pow(3, i)];
		std::cout << 3 * pow(3, i) << std::endl;
	}

	int i_ver = 3;
	int i_ind[PROF] = {};
	recursion(vertices, indices, 0, 1, 2, i_ver, i_ind);


	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 4 * 6 * i_ver, vertices, GL_STATIC_DRAW);



	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	int i = 0;
	int cambio = -1;
	int prev = 0;
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float timeValue = glfwGetTime();
		//std::cout << timeValue << std::endl;

		int aux = (timeValue - (int)(timeValue / (PROF)) * PROF) * T_CAMBIO;
		int auxPi = (int)((timeValue - timeGiro) / (M_PI*2));

		ourShader.use();

		glm::mat4 model = glm::mat4(1.0f); 
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		model = glm::rotate(model, (float)flagMov*((float)glfwGetTime()-timeGiro) + (float)(!flagMov)*(timePause - timeGiro),
																glm::vec3(auxPi%7==0 || auxPi % 7 == 3|| auxPi % 7 == 4 || auxPi % 7 == 6,
																auxPi % 7 == 1 || auxPi % 7 == 3 || auxPi % 7 == 5 || auxPi % 7 == 6,
																auxPi % 7 == 2 || auxPi % 7 == 4 || auxPi % 7 == 5 || auxPi % 7 == 6));
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
		unsigned int viewLoc = glGetUniformLocation(ourShader.ID, "view");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
		ourShader.setMat4("projection", projection);

		glBindVertexArray(VAO);

		


		if (flag && aux != prev) {
			prev = aux;
			if (i == 0 || i == PROF - 1)
				cambio *= -1;
			i += cambio;
			
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * 3 * pow(3, i), indices[i], GL_STATIC_DRAW);
		glDrawElements(GL_TRIANGLES, i_ind[i], GL_UNSIGNED_INT, 0);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	delete[] vertices;
	for (int i = 0; i < PROF; ++i) {
		delete indices[i];
	}
	glfwTerminate();
	return 0;
}

