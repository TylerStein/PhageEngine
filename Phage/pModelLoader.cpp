#include "pModelLoader.h"


pModelLoader* pModelLoader::_instance = 0;

pModelLoader::pModelLoader(){}

pModelLoader::~pModelLoader(){}

ModelInfo pModelLoader::loadModel(std::string path, pShader *shader)
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
		mI.numVerts = mesh->mNumVertices;
		int iMeshFaces = mesh->mNumFaces;
		
		for (int j = 0; j < iMeshFaces; j++)
		{
			const aiFace& face = mesh->mFaces[j];
			for (int k = 0; k < 4; k++)
			{
				
				aiVector3D pos = mesh->mVertices[face.mIndices[k]];
				aiVector3D normal = mesh->HasNormals() ? mesh->mNormals[face.mIndices[k]] : aiVector3D(1.0f, 1.0f, 1.0f);
				/*
				glm::vec3 posConversion;
				glm::vec3 normConversion;

				posConversion.x = pos.x;
				posConversion.y = pos.y;
				posConversion.z = pos.z;
				normConversion.x = normal.x;
				normConversion.y = normal.y;
				normConversion.z = normal.z;

				mI.colors.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
				mI.positions.push_back(posConversion);
				mI.normals.push_back(normConversion);
				*/
				mI.colors.push_back(1.0f);
				mI.colors.push_back(1.0f);
				mI.colors.push_back(1.0f);

				mI.positions.push_back(pos.x);
				mI.positions.push_back(pos.y);
				mI.positions.push_back(pos.z);

				mI.normals.push_back(normal.x);
				mI.normals.push_back(normal.y);
				mI.normals.push_back(normal.z);

				if (scene->HasMaterials())
				{
					aiVector3D uv = mesh->mTextureCoords[0][face.mIndices[k]];
					//glm::vec3 uvConversion;
					//uvConversion.x = uv.x;
					//uvConversion.y = uv.y;
					//mI.uvs.push_back(uvConversion);
					mI.uvs.push_back(uv.x);
					mI.uvs.push_back(uv.y);

				}
				
			}
			
		}
		if (scene->HasMaterials())
		{
			for (int l = 0; l < scene->mNumMaterials; l++)
			{
				aiMaterial* material = scene->mMaterials[l];
				aiColor3D colorConvert;
				float shineAndAlpha;
				int bMode;
				std::string texDir;
				MaterialInfo matInfo;
				if (AI_SUCCESS != material->Get(AI_MATKEY_COLOR_DIFFUSE, colorConvert))
				{
					matInfo.diffuse.x = colorConvert.r;
					matInfo.diffuse.y = colorConvert.g;
					matInfo.diffuse.z = colorConvert.b;
				}
				if (AI_SUCCESS != material->Get(AI_MATKEY_COLOR_AMBIENT, colorConvert))
				{
					matInfo.diffuse.x = colorConvert.r;
					matInfo.diffuse.y = colorConvert.g;
					matInfo.diffuse.z = colorConvert.b;
				}
				if (AI_SUCCESS != material->Get(AI_MATKEY_SHININESS, shineAndAlpha))
				{
					matInfo.shininess = shineAndAlpha;
				}
				if (AI_SUCCESS != material->Get(AI_MATKEY_BLEND_FUNC, bMode))
				{
					matInfo.blendMode = bMode;
				}
				if (AI_SUCCESS != material->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, l), texDir))
				{
					std::cout << texDir << std::endl;
					std::string name;
					material->Get(AI_MATKEY_NAME, name);
					pImage *img = new pImage(name, texDir, GL_LINEAR, GL_LINEAR, GL_REPEAT);
					matInfo.texture0 = img;
				}
				if (matInfo.texture0 != NULL)
				{
					matInfo.bHasTexture = true;
				}
				matInfo.alpha = 1.0f;
				std::string name = "model material" + l;
				std::cout << matInfo.alpha << std::endl;
				std::cout << matInfo.ambient.x << std::endl;
				std::cout << matInfo.ambient.y << std::endl;
				std::cout << matInfo.ambient.z << std::endl;
				std::cout << matInfo.bHasTexture << std::endl;
				std::cout << matInfo.diffuse.r << std::endl;
				std::cout << matInfo.diffuse.g << std::endl;
				std::cout << matInfo.diffuse.b << std::endl;
				std::cout << matInfo.shininess << std::endl;
				std::cout << matInfo.texture0->getName() << std::endl;
				std::cout << matInfo.blendMode << std::endl;
				mI.mat = new pMaterial((char*)name.c_str(), shader, matInfo);
				
			}
		}
	}
	
	return mI;
}




