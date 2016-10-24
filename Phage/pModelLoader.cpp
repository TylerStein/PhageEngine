#include "pModelLoader.h"

pModelLoader* pModelLoader::_instance = 0;

pModelLoader::pModelLoader(){}

pModelLoader::~pModelLoader(){}

pModelLoader::ModelInfo pModelLoader::loadModel(std::string path)
{
	ModelInfo mI;

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	for (int i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[i];
		int iMeshFaces = mesh->mNumFaces;

		for (int j = 0; j < iMeshFaces; j++)
		{
			const aiFace& face = mesh->mFaces[j];
			for (int k = 0; k < 3; k++)
			{
				aiVector3D pos = mesh->mVertices[face.mIndices[k]];
				aiVector3D uv = mesh->mTextureCoords[0][face.mIndices[k]];
				aiVector3D normal = mesh->HasNormals() ? mesh->mNormals[face.mIndices[k]] : aiVector3D(1.0f, 1.0f, 1.0f);

				glm::vec3 posConversion;
				glm::vec3 uvConversion;
				glm::vec3 normConversion;

				posConversion.x = pos.x;
				posConversion.y = pos.y;
				posConversion.z = pos.z;
				uvConversion.x = uv.x;
				uvConversion.y = uv.y;
				uvConversion.z = uv.z;
				normConversion.x = normal.x;
				normConversion.y = normal.y;
				normConversion.z = normal.z;


				mI.positions.push_back(posConversion);
				mI.uvs.push_back(uvConversion);
				mI.normals.push_back(normConversion);
			}
		}
	}
	return mI;
}




