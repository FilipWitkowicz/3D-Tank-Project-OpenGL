/*
Niniejszy program jest wolnym oprogramowaniem; możesz go
rozprowadzać dalej i / lub modyfikować na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundację Wolnego
Oprogramowania - według wersji 2 tej Licencji lub(według twojego
wyboru) którejś z późniejszych wersji.

Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on
użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej
gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH
ZASTOSOWAŃ.W celu uzyskania bliższych informacji sięgnij do
Powszechnej Licencji Publicznej GNU.

Z pewnością wraz z niniejszym programem otrzymałeś też egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
jeśli nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
*/

#define GLM_FORCE_RADIANS
#define GLM_FORCE_SWIZZLE

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include "myCube.h"
#include "myTeapot.h"
#include "objloader.hpp"
#include "Gasienica.h"
#include <vector>
#include <chrono>
#include "Structs.h"
#include "Pocisk.h"
#include <future>


float aspectRatio=1;

ShaderProgram *sp;

std::vector<CustomObject> Objects;

bool fire = false;
bool shot = false;
bool reload = false;
float reload_shift = 0;
bool reload_back = false;
int reload_time = 0;

void draw(struct CustomObject v, GLuint tex) {

	glUniform1i(sp->u("textureMap0"), 0); //drawScene
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);


	glEnableVertexAttribArray(sp->a("vertex"));
	glEnableVertexAttribArray(sp->a("normal"));
	glEnableVertexAttribArray(sp->a("texCoord0"));


	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, static_cast<float*>(glm::value_ptr(v.vertices.front())));
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, static_cast<float*>(glm::value_ptr(v.normals.front())));
	glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, glm::value_ptr(v.uvs.front()));

	glDrawArrays(GL_TRIANGLES, 0, v.vertices.size());


	glDisableVertexAttribArray(sp->a("vertex"));
	glDisableVertexAttribArray(sp->a("normal"));
	glDisableVertexAttribArray(sp->a("texCoord0"));
}
float speed_x = 0;//[radiany/s]
float speed_y = 0;//[radiany/s]
float camera_x = 0;
float camera_y = 0;
float camera_z = 0;
float pr_OBROT_WIEZY = 0;
float pr_OBROT_LUFY = 0;
float tank_speed = 0;


float gSpeed1 = 0.0f;
float gSpeed2 = 0.0f;
float gg1 = 0.0f;
float gg2 = 0.0f;
float gg_speed = 0.04f;




Gasienica G(0.0f, 0.0f, 0.0f);
Gasienica G2(0.0f, 0.0f, 0.0f);

Pocisk pocisk;


//Odkomentuj, żeby rysować kostkę
float* vertices = myCubeVertices;
float* normals = myCubeNormals;
float* texCoords = myCubeTexCoords;
//float* colors = myCubeColors;
int vertexCount = myCubeVertexCount;

void drawCube(float* vertices, float* normal, int vertexCount, float* texCoords, GLuint tex) {

	glUniform1i(sp->u("textureMap0"), 0); //drawScene
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);

	glEnableVertexAttribArray(sp->a("texCoord0"));
	glEnableVertexAttribArray(sp->a("vertex"));  //Włącz przesyłanie danych do atrybutu vertex
	glEnableVertexAttribArray(sp->a("normal"));  //Włącz przesyłanie danych do atrybutu normal


	glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, texCoords);//odpowiednia tabli
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, vertices); //Wskaż tablicę z danymi dla atrybutu vertex
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, normals); //Wskaż tablicę z danymi dla atrybutu normal



	glDrawArrays(GL_TRIANGLES, 0, vertexCount); //Narysuj obiekt

	glDisableVertexAttribArray(sp->a("vertex"));  //Wyłącz przesyłanie danych do atrybutu vertex
	glDisableVertexAttribArray(sp->a("normal"));  //Wyłącz przesyłanie danych do atrybutu normal
	glDisableVertexAttribArray(sp->a("texCoord0"));

}



void keyCallback(GLFWwindow* window,int key,int scancode,int action,int mods) {
	if (action == GLFW_PRESS) {

		//swobodny kamery
		if (key == GLFW_KEY_1) {
			camera_x = -PI;
		}
		if (key == GLFW_KEY_2) {
			camera_x = PI;
		}
		if (key == GLFW_KEY_3) {
			camera_y = -PI;
		}
		if (key == GLFW_KEY_4) {
			camera_y = PI;
		} 
		if (key == GLFW_KEY_5) {
			camera_z = -PI;
		}
		if (key == GLFW_KEY_6) {
			camera_z = PI;
		}

		//swobodny ruch czolgu
		if (key == GLFW_KEY_J) {
			speed_y = -PI;
		}
		if (key == GLFW_KEY_L) {
			speed_y = PI;
		}
		if (key == GLFW_KEY_I) {
			speed_x = -PI;
		}
		if (key == GLFW_KEY_K) {
			speed_x = PI;
		}

		//obrot wiezy
		if (key == GLFW_KEY_A) {
			pr_OBROT_WIEZY = -PI / 4;
		}
		if (key == GLFW_KEY_D) {
			pr_OBROT_WIEZY = PI / 4;
		}

		//obrot lufy
		if (key == GLFW_KEY_W) {
			pr_OBROT_LUFY = -PI / 8;
		}
		if (key == GLFW_KEY_S) {
			pr_OBROT_LUFY = PI / 8;
		}



		//gasienice
		if (key == GLFW_KEY_UP) {
			gSpeed1 = 0.04f;
			gSpeed2 = 0.04f;
			tank_speed = -PI;
		}
		else if (key == GLFW_KEY_DOWN) {
			gSpeed1 = -0.04f;
			gSpeed2 = -0.04f;
			tank_speed = PI;
		}
		if (key == GLFW_KEY_LEFT) {
			gSpeed1 = -0.04f;
			gSpeed2 = 0.04f;
			speed_y = -PI / 4;
		}
		else if (key == GLFW_KEY_RIGHT) {
			gSpeed1 = 0.04f;
			gSpeed2 = -0.04f;
			speed_y = PI / 4;
		}

		//strzal
		//GLFW_MOUSE_BUTTON_LEFT
		if (key == GLFW_KEY_Z) {
			fire = true;
		}

	}
	if (action == GLFW_RELEASE) {

		//swobodny kamery
		if (key == GLFW_KEY_1) {
			camera_x = 0;
		}
		if (key == GLFW_KEY_2) {
			camera_x = 0;
		}
		if (key == GLFW_KEY_3) {
			camera_y = 0;
		}
		if (key == GLFW_KEY_4) {
			camera_y = 0;
		}
		if (key == GLFW_KEY_5) {
			camera_z = 0;
		}
		if (key == GLFW_KEY_6) {
			camera_z = 0;
		}

		if (key == GLFW_KEY_J || key == GLFW_KEY_L) {
			speed_y = 0;
		}
		if (key == GLFW_KEY_K || key == GLFW_KEY_I) {
			speed_x = 0;
		}
		if (key == GLFW_KEY_A || key == GLFW_KEY_D) {
			pr_OBROT_WIEZY = 0;
		}
		if (key == GLFW_KEY_S || key == GLFW_KEY_W) {
			pr_OBROT_LUFY = 0;
		}
		if (key == GLFW_KEY_UP || key == GLFW_KEY_DOWN) {
			gSpeed1 = 0.0f;
			gSpeed2 = 0.0f;
			tank_speed = 0;
		}
		if (key == GLFW_KEY_LEFT) {
			gSpeed1 = 0.0f;
			gSpeed2 = 0.0f;
			speed_y = 0;
		}
		if (key == GLFW_KEY_RIGHT) {
			gSpeed1 = 0.0f;
			gSpeed2 = 0.0f;
			speed_y = 0;
		}
		//strzal
		//GLFW_MOUSE_BUTTON_LEFT
		if (key == GLFW_KEY_Z) {
			fire = false;
		}
	}
}

//Procedura obsługi błędów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

void windowResizeCallback(GLFWwindow* window,int width,int height) {
    if (height==0) return;
    aspectRatio=(float)width/(float)height;
    glViewport(0,0,width,height);
}


GLuint tex0;
GLuint tex1;
GLuint tex2;
GLuint readTexture(const char* filename) {
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);
	//Wczytanie do pamięci komputera
	std::vector<unsigned char> image; //Alokuj wektor do wczytania obrazka
	unsigned width, height; //Zmienne do których wczytamy wymiary obrazka
	//Wczytaj obrazek
	unsigned error = lodepng::decode(image, width, height, filename);
	//Import do pamięci karty graficznej
	glGenTextures(1, &tex); //Zainicjuj jeden uchwyt
	glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
	//Wczytaj obrazek do pamięci KG skojarzonej z uchwytem
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	return tex;
}
//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
	//************Tutaj umieszczaj kod, który należy wykonać raz, na początku programu************
	glClearColor(0,0,0,1);
	glEnable(GL_DEPTH_TEST);
	glfwSetWindowSizeCallback(window,windowResizeCallback);

	std::vector<std::string> ObjectNames
	{
		"IS7_KADLUB.obj",
		"IS7_WIEZA.obj",
		"IS7_LUFA.obj",
		"kolo.obj",
		"kolo_napedowe.obj",
		"oslonka.obj",
		"lufa.obj",
		"pocisk.obj"
	};


	Objects = loadManyObj("Objects", ObjectNames);


	glfwSetKeyCallback(window,keyCallback);

	sp=new ShaderProgram("v_simplest.glsl",NULL,"f_simplest.glsl");
	tex0 = readTexture("asfalt2.png");
	tex1 = readTexture("tanktex.png");
	tex2 = readTexture("metal_spec.png");


}


//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
    //************Tutaj umieszczaj kod, który należy wykonać po zakończeniu pętli głównej************

    delete sp;
	glDeleteTextures(1, &tex0);
	glDeleteTextures(1, &tex1);
	glDeleteTextures(1, &tex2);
}



float angle_x = 0;
float angle_y = 0;
float OBROT_WIEZY = 0;
float OBROT_LUFY = 0;
float move = 0;
float StartAngle = 270.0f;
float move_camera_x = 0;
float move_camera_y = 0;
float move_camera_z = 0;
//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window, double czas) {
	//************Tutaj umieszczaj kod rysujący obraz******************l
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	angle_x = speed_x * czas; //Oblicz kąt o jaki obiekt obrócił się podczas poprzedniej klatki
	angle_y = speed_y * czas; //Oblicz kąt o jaki obiekt obrócił się podczas poprzedniej klatki
	OBROT_WIEZY += pr_OBROT_WIEZY * czas;
	OBROT_LUFY += pr_OBROT_LUFY * czas;
	move = tank_speed * czas;
	move_camera_x += camera_x * czas;
	move_camera_y += camera_y * czas;
	move_camera_z += camera_z * czas;

	//glm::mat4 V=glm::lookAt(
 //        glm::vec3(move_camera_x, 20 + move_camera_y, 20 + move_camera_z),
 //        glm::vec3(0,0,0),
 //        glm::vec3(0.0f,1.0f,0.0f)); //Wylicz macierz widoku

	glm::mat4 V = glm::lookAt(
		glm::vec3(0, 5, 20),
		glm::vec3(0, 0, 0),
		glm::vec3(0.0f, 1.0f, 0.0f)); //Wylicz macierz widoku



    glm::mat4 P=glm::perspective(50.0f*PI/180.0f, aspectRatio, 0.01f, 50.0f); //Wylicz macierz rzutowania

	static glm::mat4 M = glm::mat4(1.0f);

    sp->use();//Aktywacja programu cieniującego
    //Przeslij parametry programu cieniującego do karty graficznej
    glUniformMatrix4fv(sp->u("P"),1,false,glm::value_ptr(P));
    glUniformMatrix4fv(sp->u("V"),1,false,glm::value_ptr(V));
	glUniform4f(sp->u("color"), 0.458f, 0.46f, 0.305f, 1);

	M = glm::rotate(M, glm::radians(StartAngle), glm::vec3(0.0f, 1.0f, 0.0f)); // Obrot czolgu lufa do kamery
	M = glm::translate(M, glm::vec3(0.0f, 0.0f, move)); // Ruch przod-tyl
	M = glm::rotate(M, -angle_y, glm::vec3(0.0f, 1.0f, 0.0f));  // Rotacja czolgu jak i ruch "calej kamery"
	M = glm::rotate(M, -angle_x, glm::vec3(1.0f, 0.0f, 0.0f));  // ruch "calej kamery"
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));

	
	//KADLUB
	glm::mat4 M_KADLUB = M;
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M_KADLUB));
	draw(Objects[0], tex1);

	//WIEZA
	glm::mat4 M_WIEZA = M_KADLUB;
	M_WIEZA = glm::rotate(M_WIEZA, OBROT_WIEZY, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M_WIEZA));
	draw(Objects[1], tex1);


	//OSLONKA
	glm::mat4 M_OSLONKA = M_WIEZA;
	float wychylenie_gora = -PI / 30;
	float wychylenie_dol = PI / 60;


	if (OBROT_LUFY >= wychylenie_gora && OBROT_LUFY <= wychylenie_dol)
		M_OSLONKA = glm::rotate(M_OSLONKA, -OBROT_LUFY, glm::vec3(1.0f, 0.0f, 0.0f));
	else if (OBROT_LUFY < wychylenie_gora) {
		M_OSLONKA = glm::rotate(M_OSLONKA, -wychylenie_gora, glm::vec3(1.0f, 0.0f, 0.0f));
		OBROT_LUFY = wychylenie_gora;
	}
	else if (OBROT_LUFY > wychylenie_dol) {
		M_OSLONKA = glm::rotate(M_OSLONKA, -wychylenie_dol, glm::vec3(1.0f, 0.0f, 0.0f));
		OBROT_LUFY = wychylenie_dol;
	}


	M_OSLONKA = glm::rotate(M_OSLONKA, -PI / 2, glm::vec3(1.0f, 0.0f, 0.0f));
	M_OSLONKA = glm::translate(M_OSLONKA, glm::vec3(0.0f, 2.25f, 1.8f));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M_OSLONKA));
	//draw(Objects[2]);
	draw(Objects[5], tex1);

	//pocisk
	if (pocisk.Active) {
		if (pocisk.LifeTime <= 0) {
			pocisk.LifeTime = pocisk.LifeTimeMax;
			pocisk.Active = false;
		}
		else {
			pocisk.LifeTime -= 0.05f;
			pocisk.Matrix = glm::translate(pocisk.Matrix, glm::vec3(0.0f, pocisk.Speed, 0.0f));
			glm::mat4 DrawPociskMatrix = glm::scale(pocisk.Matrix, glm::vec3(0.8f, 0.8f, 0.8f));
			glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(DrawPociskMatrix));
			draw(Objects[7], tex2);
		}
	}

	//LUFA
	glm::mat4 M_LUFA = M_OSLONKA;

	if (fire && !reload) {
		shot = true;
		reload = true;
		reload_time = 100;

		pocisk.Active = true;
		pocisk.Matrix = M_LUFA;
		pocisk.Matrix = glm::translate(pocisk.Matrix, glm::vec3(0.0f, 5.0f, 0.0f));
		pocisk.Speed = 0.2f;
	}
	if (shot) {

		if (reload_shift < 1 && reload_back == false) {
			reload_shift += 0.2;
		}
		else if (reload_shift >= 1 || reload_back == true) {
			reload_back = true;
			reload_shift -= 0.05;
		}
		if (reload_shift <= 0) {
			reload_shift = 0.0;
			shot = false;
			reload_back = false;
		}
	}
	if (reload_time > 0) {
		reload_time--;
	}
	else {
		reload = false;
	}


	M_LUFA = glm::translate(M_LUFA, glm::vec3(0.0f, -reload_shift / 3, 0.0f));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M_LUFA));
	draw(Objects[6], tex1);




	//kola gasienic
	glUniform4f(sp->u("color"), 0.2f, 0.2f, 0.2f, 1);
	for (int j = 0; j < 2; j++) {
		bool IsRight = j == 0 ? true : false;

		glm::mat4 M_Kola = M;
		if (IsRight)
			M_Kola = glm::translate(M_Kola, glm::vec3(1.45f, 0.0f, 0.0f));
		else
			M_Kola = glm::translate(M_Kola, glm::vec3(-1.45f, 0.0f, 0.0f));


		M_Kola = glm::rotate(M_Kola, -PI / 2, glm::vec3(0.0f, 0.0f, 1.0f));
		M_Kola = glm::translate(M_Kola, glm::vec3(-0.39f, 0.0f, 0.0f));
		M_Kola = glm::translate(M_Kola, glm::vec3(0.0f, 0.0f, 2.0f));
		for (int i = 0; i < 6; i++) {
			glm::mat4 M_Kola_i = M_Kola;
			if (!IsRight)
				M_Kola_i = glm::rotate(M_Kola_i, PI, glm::vec3(0.0f, 0.0f, 1.0f));

			M_Kola_i = glm::translate(M_Kola_i, glm::vec3(0.0f, 0.0f, -i * 0.8f));
			if (IsRight)
				M_Kola_i = glm::rotate(M_Kola_i, gg1, glm::vec3(0.0f, 1.0f, 0.0f));
			else
				M_Kola_i = glm::rotate(M_Kola_i, -gg2, glm::vec3(0.0f, 1.0f, 0.0f));


			glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M_Kola_i));
			draw(Objects[3], tex2);
		}

		M_Kola = glm::translate(M_Kola, glm::vec3(0.0f, 0.0f, 1.04f));
		M_Kola = glm::translate(M_Kola, glm::vec3(-0.13f, 0.0f, 0.0f));
		M_Kola = glm::scale(M_Kola, glm::vec3(1.0f, 1.4f, 1.0f));
		if (!IsRight)
			M_Kola = glm::rotate(M_Kola, PI, glm::vec3(0.0f, 0.0f, 1.0f));

		M_Kola = glm::translate(M_Kola, glm::vec3(0.0f, -0.05f, 0.0f));
		if (IsRight)
			M_Kola = glm::rotate(M_Kola, gg1, glm::vec3(0.0f, 1.0f, 0.0f));
		else
			M_Kola = glm::rotate(M_Kola, -gg2, glm::vec3(0.0f, 1.0f, 0.0f));



		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M_Kola));
		draw(Objects[4], tex2);


		M_Kola = M;
		if (IsRight)
			M_Kola = glm::translate(M_Kola, glm::vec3(1.45f, 0.0f, 0.0f));
		else
			M_Kola = glm::translate(M_Kola, glm::vec3(-1.45f, 0.0f, 0.0f));


		M_Kola = glm::translate(M_Kola, glm::vec3(0.0f, 0.0f, -3.0f));
		M_Kola = glm::translate(M_Kola, glm::vec3(0.0f, 0.5f, 0.0f));
		if (IsRight) {
			M_Kola = glm::rotate(M_Kola, -PI / 2, glm::vec3(0.0f, 0.0f, 1.0f));
			M_Kola = glm::rotate(M_Kola, gg1, glm::vec3(0.0f, 1.0f, 0.0f));
		}
		else {
			M_Kola = glm::rotate(M_Kola, PI / 2, glm::vec3(0.0f, 0.0f, 1.0f));
			M_Kola = glm::rotate(M_Kola, -gg2, glm::vec3(0.0f, 1.0f, 0.0f));

		}


		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M_Kola));
		draw(Objects[3], tex2);

	}

	if (gSpeed1 > 0) {
		gg1 -= gg_speed;
	}
	if (gSpeed2 > 0) {
		gg2 -= gg_speed;
	}
	if (gSpeed1 < 0) {
		gg1 += gg_speed;
	}
	if (gSpeed2 < 0) {
		gg2 += gg_speed;
	}

	//gasienice 
	G.Move(gSpeed1);
	G2.Move(gSpeed2);

	M = glm::rotate(M, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	for (int i = 0; i < G.ogniwa.size(); i++) {
		glm::mat4 MOgniwa = M * G.ogniwa[i].DrawMatrix;

		if (gSpeed1 < 0) {
			MOgniwa = glm::translate(MOgniwa, glm::vec3(0.0f, 0.0f, -2.8f));
		}

		MOgniwa = glm::translate(MOgniwa, glm::vec3(0.0f, 0.0f, 1.4f));
		MOgniwa = glm::scale(MOgniwa, glm::vec3(0.1, 0.02, 0.3));
		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(MOgniwa));
		drawCube(vertices, normals, vertexCount, myCubeTexCoords, tex2);
	}
	for (int i = 0; i < G2.ogniwa.size(); i++) {
		glm::mat4 MOgniwa = M * G2.ogniwa[i].DrawMatrix;

		if (gSpeed2 < 0) {
			MOgniwa = glm::translate(MOgniwa, glm::vec3(0.0f, 0.0f, 2.8f));
		}

		MOgniwa = glm::translate(MOgniwa, glm::vec3(0.0f, 0.0f, -1.4f));
		MOgniwa = glm::scale(MOgniwa, glm::vec3(0.1, 0.02, 0.3));
		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(MOgniwa));
		drawCube(vertices, normals, vertexCount, myCubeTexCoords, tex2);
	}


	glUniform4f(sp->u("color"), 0.2f, 0.6f, 0.0f, 1);
	glm::mat4 M_pod = glm::mat4(1.0f);
	M_pod = glm::translate(M_pod, glm::vec3(0.0f, -1.2f, 0.0f));
	M_pod = glm::scale(M_pod, glm::vec3(20.0f, 1.0f, 20.0f));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M_pod));
	drawCube(vertices, normals, vertexCount, myCubeTexCoords, tex0);



	M = glm::rotate(M, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	M = glm::rotate(M, -glm::radians(StartAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    glfwSwapBuffers(window); //Przerzuć tylny bufor na przedni
}


int main(void)
{
	GLFWwindow* window; //Wskaźnik na obiekt reprezentujący okno

	glfwSetErrorCallback(error_callback);//Zarejestruj procedurę obsługi błędów

	if (!glfwInit()) { //Zainicjuj bibliotekę GLFW
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(500, 500, "OpenGL", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

	if (!window) //Jeżeli okna nie udało się utworzyć, to zamknij program
	{
		fprintf(stderr, "Nie można utworzyć okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje się aktywny i polecenia OpenGL będą dotyczyć właśnie jego.
	glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

	if (glewInit() != GLEW_OK) { //Zainicjuj bibliotekę GLEW
		fprintf(stderr, "Nie można zainicjować GLEW.\n");
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window); //Operacje inicjujące


	//Główna pętla	

	glfwSetTime(0); //Wyzeruj licznik czasu
	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{

		double czas = glfwGetTime();
		glfwSetTime(0); //Wyzeruj licznik czasu
		drawScene(window, czas); //Wykonaj procedurę rysującą

		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły

	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}
