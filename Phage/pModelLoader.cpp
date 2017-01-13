#include "pModelLoader.h"
#include "pResourceFactory.h"


pModelLoader* pModelLoader::_instance = 0;

pModelLoader::pModelLoader(){}

pModelLoader::~pModelLoader(){}

pModel* pModelLoader::loadModel(std::string path, pMaterial* mat)

ModelInfo pModelLoader::loadModel(std::string path, pShader *shader)
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
	const aiScene* scene = importer.ReadFile(path,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);
	int index = 0;
	

	aiMesh* mesh = scene->mMeshes[0];
	mI.numVerts = mesh->mNumVertices;

	if (mesh->HasFaces())
	{
		for (int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];

			aiVector3D vertex;

			for (int j = 0; j < face.mNumIndices; j++)
			{
				index = face.mIndices[j];
				vertex = mesh->mVertices[index];

				mI.positions.push_back(vertex.x);
				mI.positions.push_back(vertex.y);
				mI.positions.push_back(vertex.z);

				if (mesh->HasVertexColors(0))
				{
					aiColor4D color = mesh->mColors[0][index];
					mI.colors.push_back(color.r);
					mI.colors.push_back(color.g);
					mI.colors.push_back(color.b);
					mI.colors.push_back(color.a);

				}
				else
				{
					mI.colors.push_back(1.0f);
					mI.colors.push_back(1.0f);
					mI.colors.push_back(1.0f);
					mI.colors.push_back(1.0f);
				}
				if (mesh->HasNormals())
				{
					aiVector3D norm = mesh->mNormals[index];
					mI.normals.push_back(norm.x);
					mI.normals.push_back(norm.y);
					mI.normals.push_back(norm.z);
				}

				if (mesh->HasTextureCoords(0))
				{
					aiVector3D uv = mesh->mTextureCoords[0][index];
					mI.uvs.push_back(uv.x);
					mI.uvs.push_back(uv.y);
				}
			}
		}		
	}

	/*if (scene->HasMaterials())
	{
		MaterialInfo matInfo;
		std::string matName;
		if (scene->HasMaterials())
		{
			int matIndex = mesh->mMaterialIndex;
			aiMaterial* material = scene->mMaterials[matIndex];

			aiString aiMatName;
			material->Get(AI_MATKEY_NAME, aiMatName);
			if (aiMatName.length == 0)
			{
				matName = matName + std::string("_material");
			}
			else
			{
				matName = aiMatName.C_Str();
			}

			aiColor3D ambColor, diffColor;
			material->Get(AI_MATKEY_COLOR_DIFFUSE, diffColor);
			matInfo.diffuse.x = diffColor.r;
			matInfo.diffuse.y = diffColor.g;
			matInfo.diffuse.z = diffColor.b;
			material->Get(AI_MATKEY_COLOR_AMBIENT, ambColor);
			matInfo.ambient.x = ambColor.r;
			matInfo.ambient.y = ambColor.g;
			matInfo.ambient.z = ambColor.b;
			material->Get(AI_MATKEY_SHININESS, matInfo.shininess);
			material->Get(AI_MATKEY_BLEND_FUNC, matInfo.blendMode);
			matInfo.alpha = 1.0f;
			std::string texDir;
			if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
			{
				aiString aiPath;
				if (material->GetTexture(aiTextureType_DIFFUSE, 0, &aiPath, nullptr, nullptr, nullptr, nullptr, nullptr) == AI_SUCCESS)
				{
					std::string rawPath = aiPath.C_Str();
					pImage *img = new pImage(rawPath, rawPath, GL_LINEAR, GL_LINEAR, GL_REPEAT);
					matInfo.texture0 = img;
					if (matInfo.texture0 != NULL)
					{
						matInfo.bHasTexture = true;
					}
					
				}

			}
			std::string name = "model material";
			std::cout << "matInfo Alpha: " << matInfo.alpha << std::endl;
			std::cout << "matInfo ambient x: " << matInfo.ambient.x << std::endl;
			std::cout << "matInfo ambient y: " << matInfo.ambient.y << std::endl;
			std::cout << "matInfo ambient z: " << matInfo.ambient.z << std::endl;
			std::cout << "matInfo has texture: " << matInfo.bHasTexture << std::endl;
			std::cout << "matInfo diffuse r: " << matInfo.diffuse.r << std::endl;
			std::cout << "matInfo diffuse g: " << matInfo.diffuse.g << std::endl;
			std::cout << "matInfo diffuse b: " << matInfo.diffuse.b << std::endl;
			std::cout << "matInfo shininess: " << matInfo.shininess << std::endl;
			std::cout << "matInfo texture name: " << matInfo.texture0->getName() << std::endl;
			std::cout << "matInfo blend mode: " << matInfo.blendMode << std::endl;
			mI.mat = new pMaterial((char*)name.c_str(), shader, matInfo);
		}

		meshName = std::string(mesh->mName.C_Str());
	}*/

	if (meshName == "") { meshName = "LoadedMesh"; }

	if (mat == nullptr) {
		mat = pResourceFactory::instance()->createDebugMaterial();
	}

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

