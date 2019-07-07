#include "model.h"


model::model()
{
	albedo = false;
	kamb = glm::vec3(0.10);
	kdif = glm::vec3(0.55);
	kspec = glm::vec3(0.70);
	shinniness = 55.0f;
	roughness = 0.1f;
	indexMaterial = 1.309;
	indexAmbient = 1.309;
	intensityParalax = 1.309;
	percentAmbient = 1.309;
	position = glm::vec3(0.0f);
	shaderType = 'b';
}

model::model(glm::vec3 pos)
{
	albedo = false;
	kamb = glm::vec3(0.10);
	kdif = glm::vec3(0.55);
	kspec = glm::vec3(0.70);
	shinniness = 55.0f;
	roughness = 0.1f;
	indexMaterial = 1.309;
	indexAmbient = 1.309;
	intensityParalax = 1.309;
	percentAmbient = 1.309;
	position = pos;
	shaderType = 'b';

}

model::~model()
{

}

bool model::loadObj(std::string path, std::vector<glm::vec3>& vert, std::vector<glm::vec3>& norm, std::vector<glm::vec2>& uvText)
{
	std::ifstream file = std::ifstream(path);
	if (!file.is_open()) {
		std::cout << "No se encuentra: " << path << std::endl;
		return false;
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

	//model* a = new model();
	//Creando el arreglo final
	/*std::vector< glm::vec3 > Vert;
	std::vector< glm::vec3 > Normal;
	std::vector< glm::vec2 > UV;*/
	for (int i = 0; i < vertInd.size(); i++)
	{
		vert.push_back(glm::vec3(allVert[vertInd[i]]));
		uvText.push_back(allUV[uvInd[i]]);
		norm.push_back(glm::vec3(allNormal[normInd[i]]));
	}
	numVertex = vert.size();

	return true;

}

model* model::loadObj(std::string path, glm::vec3 position)
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

	model *a = new model(position);
	//Creando el arreglo final
	for (int i = 0; i < vertInd.size(); i++)
	{
		a->vertex.push_back(glm::vec3(allVert[vertInd[i]]));
		a->uv.push_back(allUV[uvInd[i]]);
		a->normal.push_back(glm::vec3(allNormal[normInd[i]]));
	}
	a->numVertex = a->vertex.size();

	return a;

}

void model::loadMTL(std::string path)
{
	std::ifstream file = std::ifstream(path);
	if (!file.is_open()) {
		std::cout << "No se ecuentra: " << path << std::endl;
	}
	
	while (file)
	{
		std::string token, first, trash;
		float Kx, Ky, Kz;
		getline(file, token);
		std::istringstream str(token);
		str >> first;
		
		if (first == "Ka")
		{
			str >> Kx >> Ky >> Kz;
			kamb = glm::vec3(Kx, Ky, Kz);
		}
		else if (first == "Kd")
		{
			str >> Kx >> Ky >> Kz;
			kdif = glm::vec3(Kx, Ky, Kz);
		}
		else if (first == "Ks")
		{
			str >> Kx >> Ky >> Kz;
			kspec = glm::vec3(Kx, Ky, Kz);
		}
		else if (first == "Ke") {

		}
		else if (first == "Ni") {

		}
		else if (first == "d") {

		}
		else if (first == "illum") {

		}
		else if (first == "Ns") {

		}
		else if (first == "newmtl") {

		}
		else if (first == "map_Kd") {

		}
	}
}

model::Texture model::getTexture()
{
	return texture;
}

void model::setTexture(unsigned int diffID)
{
	texture.diffuse = diffID;
	texture.specular = -1; //NO tiene text especular
}

void model::setTexture(unsigned int diffID, unsigned int specID)
{
	texture.diffuse = diffID;
	texture.specular = specID;

}

glm::vec3 model::getPosition()
{
	return position;
}

void model::setPosition(glm::vec3 pos)
{
	this->position = pos;
}

glm::vec3 model::getKAmbient()
{
	return kamb;
}

void model::setKAmbient(glm::vec3 ambC)
{
	kamb = ambC;
}

glm::vec3 model::getKDiffuse()
{
	return kdif;
}

void model::setKDiffuse(glm::vec3 difC)
{
	kdif = difC;
}

glm::vec3 model::getKSpecular()
{
	return kspec;
}

void model::setKSpecular(glm::vec3 speC)
{
	kspec = speC;
}

float model::getShinniness()
{
	return shinniness;
}

void model::setShinniness(float n)
{
	shinniness = n;
}

float model::getRoughness()
{
	return roughness;
}

void model::setRoghness(float n)
{
	roughness = n;
}

float model::getindexMaterial()
{
	return indexMaterial;
}

void model::setindexMaterial(float n)
{
	indexMaterial = n;
}

float model::getindexAmbient()
{
	return indexAmbient;
}

void model::setindexAmbient(float n)
{
	indexAmbient = n;
}

float model::getintensityParalax()
{
	return intensityParalax;
}

void model::setintensityParalax(float n)
{
	intensityParalax = n;
}

float model::getpercentAmbient()
{
	return percentAmbient;
}

void model::setpercentAmbient(float n)
{
	percentAmbient = n;
}

bool model::getAlbedo()
{
	return albedo;
}

void model::setAlbedo(bool n)
{
	albedo = n;
}

char model::getShader()
{
	return shaderType;
}

void model::setShader(char n)
{
	shaderType = n;
}
