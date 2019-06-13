#include "model.h"


model::model()
{

}

model::~model()
{

}

model model::loadObj(std::string path)
{
	std::ifstream file = std::ifstream(path);
	if (!file.is_open()) {
		std::cout << "No se ecuentra: " << path << std::endl;
	}

	std::vector< glm::vec3 > allVert;
	std::vector< glm::vec3 > allNormal;
	std::vector< glm::vec2 > allUV;
	std::vector< unsigned int > vertInd, normInd, uvInd;
	//int count = 0;
	while (file) {
		std::string token, first, trash;
		float vx, vy, vz;
		getline(file, token);
		std::istringstream str(token);
		str >> first;
		//std::cout << " " << first << std::endl;
		//Vertex position
		if (first == "v")
		{
			str >> vx >> vy >> vz;
			allVert.push_back(glm::vec3(vx, vy, vz));

		}
		//Coordenadas uv
		else if (first == "vt")
		{
			str >> vx >> vy;
			allUV.push_back(glm::vec2(vx, vy));

		}
		//Normal
		else if (first == "vn")
		{
			str >> vx >> vy >> vz;
			allNormal.push_back(glm::vec3(vx, vy, vz));

		}
		//faces
		else if (first == "f")
		{
			unsigned int vertIndex[3], uvIndex[3], normalIndex[3];
			std::replace_if(std::begin(token), std::end(token), [](const char& ch) { return ch == '/'; }, ' ');

			std::istringstream face_str(token);
			face_str.ignore(token.length(), ' ');

			face_str >> vertIndex[0] >> uvIndex[0] >> normalIndex[0]
				>> vertIndex[1] >> uvIndex[1] >> normalIndex[1]
				>> vertIndex[2] >> uvIndex[2] >> normalIndex[2];

			//Se le resta 1 porque el index de los vertices en el obj empieza en 1
			vertInd.push_back(vertIndex[0] - 1);
			vertInd.push_back(vertIndex[1] - 1);
			vertInd.push_back(vertIndex[2] - 1);

			uvInd.push_back(uvIndex[0] - 1);
			uvInd.push_back(uvIndex[1] - 1);
			uvInd.push_back(uvIndex[2] - 1);

			normInd.push_back(normalIndex[0] - 1);
			normInd.push_back(normalIndex[1] - 1);
			normInd.push_back(normalIndex[2] - 1);
		}
		else if (first == "usmtl")
		{
			//img
		}
		else if (first == "mtllib")
		{
			//mtl
		}
		else if (first == "s")
		{

		}

	}

	model a;
	//Creando el arreglo final
	std::vector< glm::vec3 > Vert;
	std::vector< glm::vec3 > Normal;
	std::vector< glm::vec2 > UV;
	for (int i = 0; i < vertInd.size(); i++)
	{
		a.vertex.push_back(glm::vec3(allVert[vertInd[i]]));
		a.uv.push_back(allUV[uvInd[i]]);
		a.normal.push_back(glm::vec3(allNormal[normInd[i]]));
	}

	return a;

}