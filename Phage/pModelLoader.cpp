#include "pModelLoader.h"
#include "pResourceFactory.h"
#include "pAnimator.h"
#include "pSkeletonManager.h"
#include <filesystem>

pModelLoader* pModelLoader::_instance = 0;

pModelLoader::pModelLoader() { uMatCount = 0, uMeshCount = 0; }

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
		| aiProcess_CalcTangentSpace
		);
	
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
#ifdef _DEBUG
	std::cout << "Beginning import of model from path: " << path << std::endl;
	double startTime = glfwGetTime();
#endif



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
#endif
		LogManager::instance()->error(("Failed to import model from " + path + "\n"));
		return nullptr;
	}

	//Get a name from the model root node
	std::string rootName = scene->mRootNode->mName.C_Str();
	if (rootName.empty()) { rootName = "imported_model_root"; }


	//Load embedded textures
	if (scene->HasTextures()) {
		for (int i = 0; i < scene->mNumTextures; ++i) {
			
		}
	}

	//Load materials
	std::vector<pMaterial*> indexedMaterials = std::vector<pMaterial*>();
	if (scene->HasMaterials()) {
		pShader* shaderRef = pResourceFactory::instance()->createPhongShader();

		for (int i = 0; i < scene->mNumMaterials; ++i) {
			const aiMaterial* mater = scene->mMaterials[i];
			pMaterial* newMat = processMaterial(*mater, shaderRef, path, rootName);
			indexedMaterials.push_back(newMat);
		}
	}


	//Node name and animation
	std::map<std::string, pAnimation*> animationMap = std::map<std::string, pAnimation*>();

	bool first = true;

	//Indexed mesh map
	std::vector<pModel*> indexedMeshes = std::vector<pModel*>();

	//Skeleton and the associated meshe's index
	std::map<unsigned int, Skeleton> skeletonMap = std::map<unsigned int, Skeleton>();

	//Load meshes & skeletons
	if (scene->HasMeshes())
 {
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

			if (first) {
				newMesh->directory = path;
				first = false;
			}
			indexedMeshes.push_back(newMesh);

			//Extract skeleton
			if (mesh->HasBones()) {
				Skeleton* skelly = processSkeleton(*mesh);
				unsigned int idx = indexedMeshes.size() - 1;
				skeletonMap.emplace(idx, skelly);
			}
		}
	}

	std::map<std::string, pAnimation*> animationMap = std::map <std::string, pAnimation*>();

	//Load scene animations
	if(scene->HasAnimations()) {
		animationMap = processAnimations(*scene);
	}


	//Build node tree
	pSceneNode* rootNode = processNodes(*scene, indexedMeshes, skeletonMap, animationMap);


#ifdef _DEBUG
	float loadTime = (float)(glfwGetTime() - startTime);
	printf("Model load completed in %.2f seconds\n", loadTime);
#endif

	return rootNode;
}

void pModelLoader::loadModelToResources(std::string path, pMaterial * mat)
{

}

std::map<std::string, pAnimation*> pModelLoader::processAnimations(const aiScene& scene)
{
	std::map<std::string, pAnimation*> resMap = std::map<std::string, pAnimation*>();

	//Iterate through each aiAnimation
	for (int i = 0; i < scene.mNumAnimations; ++i) {
		const aiAnimation anim = *scene.mAnimations[i];



	}

	return resMap;
}

Skeleton* pModelLoader::processSkeleton(const aiMesh& mesh) {
	Skeleton* res = nullptr;

	std::string name = std::string(mesh.mName .C_Str()) + std::string("_skeleton");
	unsigned int numJoints = 0;
	std::map<std::string, unsigned int> jointMap = std::map<std::string, unsigned int>();
	std::vector<Joint*> joints = std::vector<Joint*>();

	//Iterate through each joint/bone
	for (int i = 0; i < mesh.mNumBones; i++) {
		unsigned int jointIndex = 0;
		std::string jointName = std::string(mesh.mBones[i]->mName.data);

		//Search for a joint with this name
		if (jointMap.find(jointName) == jointMap.end()) {
			//No joint found, create and add one
			jointIndex = jointMap.size();
			numJoints++;
			Joint* joint;
			joints.push_back(joint);
		}else{
			//Joint already exists, grab th
			numJoints = jointMap[jointName];
		}

		//Set this joint's index
		jointMap[jointName] = jointIndex;

		//Set the joint transform
		joints[numJoints]->_inverseTransform = aiMat4_to_glmMat4(mesh.mBones[i]->mOffsetMatrix);

		//Get bone-vertex weight data from this joint
		for (unsigned int j = 0; j < mesh.mBones[i]->mNumWeights; i++) {
			unsigned int vertexID = mesh.mBones[i]->mWeights[j].mVertexId;
			float weight = mesh.mBones[i]->mWeights[j].mWeight;

		    joints[i]->addBoneData(jointIndex, weight);
		}

		res->_joints.emplace(jointIndex, joints[jointIndex]);
	}

	pSkeletonManager::instance()->addSkeleton(res);

	return res;
}

pMaterial * pModelLoader::processMaterial(const aiMaterial & material, pShader* shader, const std::string& modelPath, std::string backupName)
{
	MaterialInfo matInfo;
	matInfo.reset();

	aiString matName;
	material.Get(AI_MATKEY_NAME, matName);
	if (matName.length <= 0) {
		if (backupName.empty()) {
			matName = "unknown_material" + std::to_string(uMatCount);
			uMatCount++;
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
			matInfo.diffuseTexture = processTextureImage(texPath, modelPath, texName);
		}
	}
	


	texPath = "";
	if (material.GetTexture(aiTextureType_NORMALS, 0, &texPath) == aiReturn_SUCCESS) {
		std::string path = texPath.C_Str();
		if (!path.empty()) {
			std::string texName = matName.C_Str() + std::string("_normalTexture");
			matInfo.bumpTexture = processTextureImage(texPath, modelPath, texName);
		}
	}
	


	texPath = "";
	if (material.GetTexture(aiTextureType_SPECULAR, 0, &texPath) == aiReturn_SUCCESS) {
		std::string path = texPath.C_Str();
		if (!path.empty()) {
			std::string texName = matName.C_Str() + std::string("_specularTexture");
			matInfo.specularTexture = processTextureImage(texPath, modelPath, texName);
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

pImage * pModelLoader::processTextureImage(const aiString & texPath, const std::string& modelPath, const std::string& name)
{
	using namespace std::experimental::filesystem::v1;

	try {

		//Create a filesystem path object with the proposed paths
		path tPath(texPath.C_Str());
		path mPath(modelPath.c_str());

		//Check that the path is valid (points to some file)
		if (!tPath.has_extension()) {
#ifdef _DEBUG
			printf("Supplied texture path has no extention, texture not loaded.\n");
#endif
			LogManager::instance()->warning("Supplied texture path has no extention, texture not loaded.\n");
			return nullptr;
		}

		//Store the proposed model and texture directories
		path modelDir = mPath.parent_path();
		path texDir = tPath.parent_path();

		//Check if the originally proposed directory exists
		if (exists(tPath)) {
			//Check if the originally proposed file exists
			if (is_regular_file(tPath)) {
				//All's well, create the image
				return pResourceFactory::instance()->loadImage(name, texPath.C_Str());
			}
		}

		//The image was not in the originally given directory, start manually searching
		//First check for <image name> in <model directory>
		//So it's <model dir>/<image>

		//Get image name+type (image.type)
		std::string imageName = tPath.filename().generic_string();

		//Check model directory for image
		path testImagePath = (modelDir.generic_string() + "/" + imageName);
		if (exists(testImagePath)) {
			if (is_regular_file(testImagePath)) {
				//Image found, add to manager and return
				return pResourceFactory::instance()->loadImage(name, testImagePath.generic_string());
			}
		}

		//The image was not in the model directory
		//Check for a parent directory of the image, then check for <parent dir> within <model dir>
		//So it's <model dir>/<image parent dir>/<image>

		//First get the parent folder string
		path::iterator itr = tPath.end(); //Itr points to "image.type"
		if (itr != tPath.begin()) {
			//Itr becomes the parent folder name
			itr++;
			std::string parentDir = itr->generic_string();

			//Construct test directory
			testImagePath = (modelDir.generic_string() + "/" + parentDir + "/" + imageName);

			if (exists(testImagePath)) {
				if (is_regular_file(testImagePath)) {
					//Image found, add to manager and return
					return pResourceFactory::instance()->loadImage(name, testImagePath.generic_string());
				}
			}
		}
	}
	catch (const filesystem_error& ex) {
#ifdef _DEBUG
		std::cout << "Filesystem Error:\n" << ex.what() << "\n";
#endif
		std::string what = std::string(ex.what());
		LogManager::instance()->warning("Filesystem Error:\n" + what + "\n");
	}

#ifdef _DEBUG
	std::cout << "Unable to load image from path: " << texPath.C_Str() << std::endl;
#endif
	LogManager::instance()->info("Unable to load image from path: " + std::string(texPath.C_Str()) + "\n");

	return nullptr;
}

pModel * pModelLoader::processMesh(const aiMesh & mesh, pMaterial* mat, std::string backupName)
{
	bool hasPositions = false;
	bool hasCoordinates = false;
	bool hasNormals = false;
	bool hasColors = false;
	bool hasTangents = false;
	bool hasIndeces = false;

	bool hasBones = false;

	std::string meshName = "";
	meshName = std::string(mesh.mName.C_Str());

	if (meshName.empty()) {
		if (backupName.empty()) {
			meshName = "unnamed_mesh" + std::to_string(uMeshCount);
			uMeshCount++;
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
	std::vector<VertexBoneData> vBoneData = std::vector<VertexBoneData>();

	hasPositions = mesh.HasPositions();
	hasCoordinates = mesh.HasTextureCoords(0);
	hasNormals = mesh.HasNormals();
	hasColors = mesh.HasVertexColors(0);
	hasTangents = mesh.HasTangentsAndBitangents();
	hasBones = mesh.HasBones();

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

	if (hasBones) {
		//For each bone contained in this mesh
		for (int i = 0; i < mesh.mNumBones; ++i) {
			//Store a reference to this bone
			const aiBone thisBone = *mesh.mBones[i];

			//Store this bone's name
			const std::string thisBoneName = std::string(thisBone.mName.C_Str());

			//Store this bone's offset (from mesh space to this bone pos)
			const glm::mat4 thisBoneOffset = aiMat4_to_glmMat4(thisBone.mOffsetMatrix);

			//For each vertex weight data in this bone
			for (int o = 0; o < thisBone.mNumWeights; ++o) {
				//Store this bone's vertex ID and weight
				const unsigned int thisVertID = thisBone.mWeights[o].mVertexId;
				const float thisVertWeight = thisBone.mWeights[o].mWeight;


				 
			}
		}
	}

	pMaterial* matRef = mat;

	if (matRef == nullptr) {
		mat = pResourceFactory::instance()->createDebugMaterial();
	}

	pModel* resModel = pResourceFactory::instance()->createModel(meshName, matRef, GL_TRIANGLES, vPositions, vIndeces, vCoordinates, vNormals, vTangents, vBiTangents, vColors);

	return resModel;
}

pSceneNode * pModelLoader::processNodes(const aiScene& scene, const std::vector<pModel*> indexedMeshes, const std::map<unsigned int, Skeleton> indexedSkeletons, const std::map <std::string, pAnimation*> animationMap, aiNode* root)
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

	if (indexedSkeletons.size() > 0) {
		//const Skeleton* nodeSkeleton = &skeletonMap.at(thisNode->getName());
		const unsigned int meshIndex = root->mMeshes[0];
		const Skeleton* skeleton = &indexedSkeletons.at(meshIndex);

		if (skeleton != nullptr) {
			pSceneObject* obj = thisNode->getAttachedSceneObject();
			//TODO: Add skeleton to scene object here
		}
	}

	if (animationMap.size() > 0) {
		const pAnimation* anim = animationMap.at(thisNode->getName());
		if (anim != nullptr) {
			pSceneObject* obj = thisNode->getAttachedSceneObject();
			//TODO: Add animator with animation to scene object here
		}
	}

	//Process this node's children
	for (int i = 0; i < root->mNumChildren; ++i) {
		aiNode* child = root->mChildren[i];
		if (child != NULL) {
			thisNode->appendChild(processNodes(scene, indexedMeshes, indexedSkeletons, animationMap, child));
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

glm::mat4 pModelLoader::aiMat4_to_glmMat4(aiMatrix4x4 src)
{
	glm::mat4 res = glm::mat4();

	glm::vec4 rowA = glm::vec4(src.a1, src.a2, src.a3, src.a4);
	glm::vec4 rowB = glm::vec4(src.b1, src.b2, src.b3, src.b4);
	glm::vec4 rowC = glm::vec4(src.c1, src.c2, src.c3, src.c4);
	glm::vec4 rowD = glm::vec4(src.d1, src.d2, src.d3, src.d4);

	res[0] = rowA;
	res[1] = rowB;
	res[2] = rowC;
	res[3] = rowD;

	return res;
}

