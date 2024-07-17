#ifndef OBJLOADER_H
#define OBJLOADER_H
#include "Structs.h"
bool loadOBJ(
	const char* path,
	std::vector<glm::vec4>& out_vertices,
	std::vector<glm::vec2>& out_uvs,
	std::vector<glm::vec4>& out_normals
);

std::vector<CustomObject> loadManyObj(std::string Folder, std::vector<std::string> Names);



#endif