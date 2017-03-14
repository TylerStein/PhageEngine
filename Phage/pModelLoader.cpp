#include "pModelLoader.h"
#include "pResourceFactory.h"

pModelLoader* pModelLoader::_instance = 0;

pModelLoader::pModelLoader(){}

pModelLoader::~pModelLoader(){}

pModel* pModelLoader::loadModel(std::string path, pMaterial* mat)
{
	pModel* resModel = nullptr;

	//Get the incoming model's info as an AssetImporter scene
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate
		| aiProcess_JoinIdenticalVertices
		| aiProcess_SortByPType
		| aiProcess_GenSmoothNormals
		| aiProcess_CalcTangentSpace);
	
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

	if (mat == nullptr) {
		mat = pResourceFactory::instance()->createDebugMaterial();
	}

	resModel = new pModel(meshName, mat, GL_TRIANGLES, vPositions, vIndeces, vCoordinates, vNormals, vTangents, vBiTangents, vColors);

	return resModel;
}

pSceneNode * pModelLoader::loadModelToSceneObjects(std::string path, pMaterial * mat)
{
	//Get the incoming model's info as an AssetImporter scene
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate
		| aiProcess_JoinIdenticalVertices
		| aiProcess_SortByPType
		| aiProcess_GenSmoothNormals
		| aiProcess_CalcTangentSpace
		| aiProcess_GenUVCoords);

	//Catch errors loading model
	if (!scene) {
#ifdef _DEBUG
		std::cout << "Failed to import model from " << path << std::endl;
		return nullptr;
#endif
		LogManager::instance()->error(("Failed to import model from " + path + "\n"));
	}

	//Load embedded textures
	if (scene->HasTextures()) {
		for (int i = 0; i < scene->mNumTextures; ++i) {
		
		}
	}

	//Get a name from the model root node
	std::string rootName = scene->mRootNode->mName.C_Str();
	if (rootName.empty()) { rootName = "imported_model_root"; }

	std::vector<pMaterial*> indexedMaterials = std::vector<pMaterial*>();

	//Load materials
	if (scene->HasMaterials()) {
		pShader* shaderRef = pResourceFactory::instance()->createPhongShader();

		for (int i = 0; i < scene->mNumMaterials; ++i) {
			const aiMaterial* mater = scene->mMaterials[i];
			pMaterial* newMat = processMaterial(*mater, shaderRef, rootName);
			indexedMaterials.push_back(newMat);
		}
	}

	std::vector<pModel*> indexedMeshes = std::vector<pModel*>();

	//Load meshes
	if (scene->HasMeshes()) {
		for (int i = 0; i < scene->mNumMeshes; ++i) {
			const aiMesh* mesh = scene->mMeshes[i];
			//Store this meshe's material index
			int matIndex = mesh->mMaterialIndex;
			pMaterial* matRef = mat;
			if (matRef == nullptr) {
				//If there is not a custom material, try using the indexed material at i
				if (indexedMaterials.size() > matIndex) {
					matRef = indexedMaterials[matIndex];
				}
			}
			pModel* newMesh = processMesh(*mesh, matRef, rootName);
			indexedMeshes.push_back(newMesh);
		}
	}

	//Build node tree
	pSceneNode* rootNode = processNodes(*scene, indexedMeshes);


	return rootNode;
}

void pModelLoader::loadModelToResources(std::string path, pMaterial * mat)
{

}

pMaterial * pModelLoader::processMaterial(const aiMaterial & material, pShader* shader, std::string backupName)
{
	MaterialInfo matInfo;
	matInfo.reset();

	aiString matName;
	material.Get(AI_MATKEY_NAME, matName);
	if (matName.length <= 0) {
		if (backupName.empty()) {
			matName = "unknown_material";
		}
		else {
			matName = backupName + "_material";
		}
	}

	aiColor3D diffuse = aiColor3D(0.0f);
	aiColor3D ambient = aiColor3D(0.0f);
	aiColor3D specular = aiColor3D(0.0f);
	aiColor3D emission = aiColor3D(0.0f);
	float shine = 0.0f;

	material.Get(AI_MATKEY_COLOR_AMBIENT, ambient);
	material.Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
	material.Get(AI_MATKEY_COLOR_SPECULAR, specular);
	material.Get(AI_MATKEY_COLOR_EMISSIVE, emission);
	material.Get(AI_MATKEY_SHININESS, shine);

	if (shine == 0) { shine = 1; }
	if (diffuse.r + diffuse.g + diffuse.b == 0) { diffuse = aiColor3D(1.0f); }
	if (specular.r + specular.g + specular.b == 0) { specular = aiColor3D(1.0f); }
	if (ambient.r + ambient.g + ambient.b == 0) { ambient = aiColor3D(0.1f); }

	matInfo.diffuse = glm::vec3(diffuse.r, diffuse.g, diffuse.b);
	matInfo.ambient = glm::vec3(ambient.r, ambient.g, ambient.b);
	matInfo.specular = glm::vec3(specular.r, specular.g, specular.b);
	matInfo.emission = glm::vec3(emission.r, emission.g, emission.b);
	matInfo.shininess = shine;
	matInfo.useLight = true;

	aiString texPath;

	if (material.GetTexture(aiTextureType_DIFFUSE, 0, &texPath) == aiReturn_SUCCESS) {
		std::string path = texPath.C_Str();
		if (!path.empty()) {
			std::string texName = matName.C_Str() + std::string("_diffuseTexture");
			matInfo.diffuseTexture = pResourceFactory::instance()->getImage(texName, path);
		}
	}
	


	texPath = "";
	if (material.GetTexture(aiTextureType_NORMALS, 0, &texPath) == aiReturn_SUCCESS) {
		std::string path = texPath.C_Str();
		if (!path.empty()) {
			std::string texName = matName.C_Str() + std::string("_normalTexture");
			matInfo.bumpTexture = pResourceFactory::instance()->getImage(texName, path);
		}
	}
	


	texPath = "";
	if (material.GetTexture(aiTextureType_SPECULAR, 0, &texPath) == aiReturn_SUCCESS) {
		std::string path = texPath.C_Str();
		if (!path.empty()) {
			std::string texName = matName.C_Str() + std::string("_specularTexture");
			matInfo.specularTexture = pResourceFactory::instance()->getImage(texName, path);
		}
	}

	if (matInfo.diffuseTexture == nullptr) {
		matInfo.diffuseTexture = pResourceFactory::instance()->createDebugImage();
	}
	
	if (matInfo.specularTexture == nullptr) {
		matInfo.specularTexture = pResourceFactory::instance()->createDebugImage();
	}
	

	pMaterial* newMat = pResourceFactory::instance()->createMaterial(matName.C_Str(), shader, matInfo);

	return newMat;
}

pModel * pModelLoader::processMesh(const aiMesh & mesh, pMaterial* mat, std::string backupName)
{
	bool hasPositions = false;
	bool hasCoordinates = false;
	bool hasNormals = false;
	bool hasColors = false;
	bool hasTangents = false;
	bool hasIndeces = false;

	std::string meshName = "";
	meshName = std::string(mesh.mName.C_Str());

	if (meshName.empty()) {
		if (backupName.empty() <= 0) {
			meshName = "unkown_mesh";
		}
		else {
			meshName = backupName + "_mesh";
		}
	}

	std::vector<GLfloat> vPositions = std::vector<GLfloat>();
	std::vector<GLfloat> vCoordinates = std::vector<GLfloat>();
	std::vector<GLfloat> vNormals = std::vector<GLfloat>();
	std::vector<GLfloat> vColors = std::vector<GLfloat>();
	std::vector<GLfloat> vTangents = std::vector<GLfloat>();
	std::vector<GLfloat> vBiTangents = std::vector<GLfloat>();
	std::vector<GLuint> vIndeces = std::vector<GLuint>();

	hasPositions = mesh.HasPositions();
	hasCoordinates = mesh.HasTextureCoords(0);
	hasNormals = mesh.HasNormals();
	hasColors = mesh.HasVertexColors(0);
	hasTangents = mesh.HasTangentsAndBitangents();

	//Iterate through each face on the mesh
	for (int o = 0; o < mesh.mNumFaces; ++o) {
		//Store the current face
		const aiFace& face = mesh.mFaces[o];
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
	for (int o = 0; o < mesh.mNumVertices; ++o) {

		if (hasPositions) {
			vec3_addToArray(&mesh.mVertices[o], &vPositions);
		}

		if (hasCoordinates) {
			vec3_2_addToArray(&mesh.mTextureCoords[0][o], &vCoordinates);
		}

		if (hasNormals) {
			vec3_addToArray(&mesh.mNormals[o], &vNormals);
		}

		if (hasColors) {
			vec4_3_addToArray(&mesh.mColors[o][0], &vColors);
		}

		if (hasTangents) {
			vec3_addToArray(&mesh.mTangents[o], &vTangents);
			vec3_addToArray(&mesh.mBitangents[o], &vBiTangents);
		}
	}



	pMaterial* matRef = mat;

	if (matRef == nullptr) {
		mat = pResourceFactory::instance()->createDebugMaterial();
	}

	pModel* resModel = pResourceFactory::instance()->createModel(meshName, matRef, GL_TRIANGLES, vPositions, vIndeces, vCoordinates, vNormals, vTangents, vBiTangents, vColors);

	return resModel;
}

pSceneNode * pModelLoader::processNodes(const aiScene & scene, const std::vector<pModel*> indexedMeshes, const aiNode* root)
{
	if (root == nullptr) { root = scene.mRootNode; }
	pSceneNode* thisNode = new pSceneNode();
	if (thisNode->getAttachedSceneObject() == nullptr) { thisNode->setAttachedSceneObject(new pSceneObject()); }

	//Extract node name
	thisNode->setName(root->mName.C_Str());

	//Extract node transformations
	aiVector3D scl = aiVector3D(1.0);
	aiVector3D pos = aiVector3D(0.0);
	aiQuaternion rot = aiQuaternion();
	root->mTransformation.Decompose(scl, rot, pos);
	
	//Apply node transformations
	thisNode->setPosition(glm::vec3(pos.x, pos.y, pos.z));
	thisNode->setScale(glm::vec3(scl.x, scl.y, scl.z));
	thisNode->setRotation(aiQuat_to_glmQuat(rot));

	//Extract node mesh index
	int numMeshes = root->mNumMeshes;
	if (numMeshes > 0) {
		unsigned int meshIndex = root->mMeshes[0];
		//Get the pModel from indexed meshes
		pModel* nodeMesh = indexedMeshes[meshIndex];
		//Give the node's sceneObject the mesh
		pSceneObject* sceneObj = thisNode->getAttachedSceneObject();
		sceneObj->attachModel(nodeMesh);
	}

	//Process this node's children
	for (int i = 0; i < root->mNumChildren; ++i) {
		aiNode* child = root->mChildren[i];
		if (child != NULL) {
			thisNode->appendChild(processNodes(scene, indexedMeshes, child));
		}
	}

	return thisNode;
}

glm::quat pModelLoader::aiQuat_to_glmQuat(aiQuaternion aiQuat)
{
	//Does W go first or last?
	//glm::quat(aiQuat.w, aiQuat.x, aiQuat.y, aiQuat.z);
	return glm::quat(aiQuat.x, aiQuat.y, aiQuat.z, aiQuat.w);
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

