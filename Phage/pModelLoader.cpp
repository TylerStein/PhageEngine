#include "pModelLoader.h"
#include "pResourceFactory.h"

pModelLoader* pModelLoader::_instance = 0;

pModelLoader::pModelLoader(){}

pModelLoader::~pModelLoader(){}

pModel* pModelLoader::loadModel(std::string path)
{
	pModel* resModel = nullptr;

	//Get the incoming model's info as an AssetImporter scene
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_CalcTangentSpace
		| aiProcess_Triangulate
		| aiProcess_JoinIdenticalVertices
		| aiProcess_SortByPType
		| aiProcess_GenSmoothNormals);
	
	//Catch errors loading model
	if (!scene) {
#ifdef _DEBUG
		std::cout << "Failed to import model from " << path << std::endl;
		return nullptr;
#endif
		LogManager::instance()->error(("Failed to import model from " + path + "\n"));
	}

	GLint vertCount = 0;

	bool hasPositions = false;
	bool hasCoordinates = false;
	bool hasNormals = false;
	bool hasColors = false;
	bool hasTangents = false;
	bool hasIndeces = false;

	std::string meshName = "";

	std::vector<GLfloat> vPositions = std::vector<GLfloat>();
	std::vector<GLfloat> vCoordinates = std::vector<GLfloat>();
	std::vector<GLfloat> vNormals = std::vector<GLfloat>();
	std::vector<GLfloat> vColors = std::vector<GLfloat>();
	std::vector<GLfloat> vTangents = std::vector<GLfloat>();
	std::vector<GLfloat> vBiTangents = std::vector<GLfloat>();
	std::vector<GLuint> vIndeces = std::vector<GLuint>();

	//Iterate through each mesh and merge them in to one
	for(int i = 0; i < scene->mNumMeshes; ++i) {
		//Does not currently support multi-mesh models
		aiMesh* mesh = scene->mMeshes[0];

		hasPositions = mesh->HasPositions();
		hasCoordinates = mesh->HasTextureCoords(0);
		hasNormals = mesh->HasNormals();
		hasColors = mesh->HasVertexColors(0);
		hasTangents = mesh->HasTangentsAndBitangents();

		vertCount += mesh->mNumVertices;

		//Iterate through each face on the mesh
		for (int o = 0; o < mesh->mNumFaces; ++o) {
			//Store the current face
			const aiFace& face = mesh->mFaces[o];
			//Make sure the face has 3 vertices
			if (face.mNumIndices != 3) {
				std::cout << "Model loaded failed to triangulate (not 3 verts per face)" << std::endl;
				LogManager::instance()->error("Model loaded failed to triangulate!\n");
			}
			
			//Add the indeces to the array
			vIndeces.push_back(face.mIndices[0]);
			vIndeces.push_back(face.mIndices[1]);
			vIndeces.push_back(face.mIndices[2]);
		}

		//Iterate through each vertex on the mesh
		for (int o = 0; o < mesh->mNumVertices; ++o) {

			if (hasPositions) {
				vec3_addToArray(&mesh->mVertices[o], &vPositions);
			}

			if (hasCoordinates) {
				vec3_2_addToArray(&mesh->mTextureCoords[0][o], &vCoordinates);
			}

			if (hasNormals) {
				vec3_addToArray(&mesh->mNormals[o], &vNormals);
			}

			if (hasColors) {
				vec4_3_addToArray(&mesh->mColors[o][0], &vColors);
			}

			if (hasTangents) {
				vec3_addToArray(&mesh->mTangents[o], &vTangents);
				vec3_addToArray(&mesh->mBitangents[o], &vBiTangents);
			}
		}

		meshName = std::string(mesh->mName.C_Str());
	}

	if (meshName == "") { meshName = "LoadedMesh"; }

	pMaterial* mat = nullptr;

	mat = pResourceFactory::instance()->createDebugMaterial();

	resModel = new pModel(meshName, mat, GL_TRIANGLES, vPositions, vIndeces, vCoordinates, vNormals, vTangents, vBiTangents, vColors);

	return resModel;
}


std::vector<GLfloat> * pModelLoader::vec3_addToArray(aiVector3D * src, std::vector<GLfloat>* dst)
{
	dst->push_back(src->x);
	dst->push_back(src->y);
	dst->push_back(src->z);
	return dst;
}

std::vector<GLfloat> * pModelLoader::vec3_2_addToArray(aiVector3D * src, std::vector<GLfloat>* dst)
{
	dst->push_back(src->x);
	dst->push_back(src->y);
	return dst;
}

std::vector<GLfloat> * pModelLoader::vec4_3_addToArray(aiColor4D * src, std::vector<GLfloat>* dst)
{
	dst->push_back(src->r);
	dst->push_back(src->g);
	dst->push_back(src->b);
	return dst;
}

