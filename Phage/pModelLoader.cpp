#include "pModelLoader.h"
#include "pResourceFactory.h"
#include "pAnimator.h"
#include "pSkeletonManager.h"
#include "pAnimationManager.h"
#include <filesystem>

#define IMPORTFLAGS_DEFAULT aiProcess_Triangulate| aiProcess_JoinIdenticalVertices| aiProcess_SortByPType | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace
#define IMPORTFLAGS_BASIC aiProcess_Triangulate | aiProcess_CalcTangentSpace

pModelLoader* pModelLoader::_instance = 0;

pModelLoader::pModelLoader() {/* uMatCount = 0, uMeshCount = 0;*/ }

pModelLoader::~pModelLoader(){}

pModel* pModelLoader::loadModel(std::string path, pMaterial* mat)
{
	pModel* resModel = nullptr;

	//Get the incoming model's info as an AssetImporter scene
	Assimp::Importer importer;
	importer.SetPropertyInteger(AI_CONFIG_PP_LBW_MAX_WEIGHTS, 4); //Set the maximum number of bone weights supported
	const aiScene* scene = importer.ReadFile(path, IMPORTFLAGS_DEFAULT);
	
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
	for(unsigned int i = 0U; i < scene->mNumMeshes; ++i) {
		//Does not currently support multi-mesh models
		aiMesh* mesh = scene->mMeshes[0];

		hasPositions = mesh->HasPositions();
		hasCoordinates = mesh->HasTextureCoords(0);
		hasNormals = mesh->HasNormals();
		hasColors = mesh->HasVertexColors(0);
		hasTangents = mesh->HasTangentsAndBitangents();

		vertCount += mesh->mNumVertices;

		//Iterate through each face on the mesh
		for (unsigned int o = 0U; o < mesh->mNumFaces; ++o) {
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
		for (unsigned int o = 0U; o < mesh->mNumVertices; ++o) {

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
		for (unsigned int i = 0U; i < scene->mNumTextures; ++i) {
#ifdef _DEBUG
			std::cout << "Unable to import embedded texture [" << std::to_string(i) << "]\n";
#endif
		}
	}

	//Load materials
	std::vector<pMaterial*> indexedMaterials = std::vector<pMaterial*>();
	if (scene->HasMaterials()) {
		pShader* shaderRef = pResourceFactory::instance()->createPhongShader();

		for (unsigned int i = 0U; i < scene->mNumMaterials; ++i) {
			const aiMaterial* mater = scene->mMaterials[i];
			pMaterial* newMat = processMaterial(*mater, shaderRef, path, rootName);
			indexedMaterials.push_back(newMat);
		}
	}



	bool first = true;

	//Indexed mesh map
	std::vector<pModel*> indexedMeshes = std::vector<pModel*>();

	//Load meshes & skeletons
	if (scene->HasMeshes())
 {
		for (unsigned int i = 0U; i < scene->mNumMeshes; ++i) {
			const aiMesh* mesh = scene->mMeshes[i];
	
			//Store this mesh material index
			int matIndex = mesh->mMaterialIndex;
			pMaterial* matRef = mat;
			if (matRef == nullptr) {
				//If there is not a custom material, try using the indexed material at i
				if (indexedMaterials.size() > matIndex) {
					matRef = indexedMaterials[matIndex];
				}
			}

			pModel* newMesh = processMesh(*mesh, matRef, rootName);

			//Decide if this mesh should hold its file directory
			if (first) {
				newMesh->directory = path;
				first = false;
			}

			indexedMeshes.push_back(newMesh);

			//Extract skeleton
			if (mesh->HasBones()) {
				//auto joints = extractJoints(*mesh);

				//Process the skeleton
				Skeleton* skelly = processSkeleton(*mesh);
				//Hand the skeleton over to the model/mesh
				newMesh->setSkeleton(skelly);
			}
		}
	}

	//List of animations
	std::vector<SkeletalAnimation*> skeletalAnimations = std::vector<SkeletalAnimation*>();

	//Load scene animations
	if(scene->HasAnimations()) {
		skeletalAnimations = processSkeletalAnimations(*scene);
	}


	//Build node tree
	pSceneNode* rootNode = processNodes(*scene, indexedMeshes, skeletalAnimations);


#ifdef _DEBUG
	float loadTime = (float)(glfwGetTime() - startTime);
	printf("Model load completed in %.2f seconds\n", loadTime);
#endif

	return rootNode;
}

void pModelLoader::loadModelToResources(std::string path, pMaterial * mat)
{

}

/*
std::map<std::string, Animation*> pModelLoader::processAnimations(const aiScene& scene)
{
	std::map<std::string, Animation*> resMap = std::map<std::string, Animation*>();

	//Iterate through each aiAnimation
	for (unsigned int i = 0U; i < scene.mNumAnimations; ++i) {
		const aiAnimation anim = *scene.mAnimations[i];
		
		//Initialize the new animation
		pAnimation* newAnim = new pAnimation(anim.mName.C_Str(), anim.mDuration, anim.mTicksPerSecond);

		
		pSkeletalAnimationClip* skeletalClip = new pSkeletalAnimationClip(anim.mName.C_Str());

		std::map<double, SkeletalAnimationSample> keyframes = std::map<double, SkeletalAnimationSample>();

		//Iterate through each channel (bone/joint)
		unsigned int numPoses = 0;
		for (unsigned int o = 0U; o < anim.mNumChannels; ++o) {
			const aiNodeAnim* nodeAnim = anim.mChannels[o];

			//Get the name of the bone/joint this set of keyframes is meant for
			const std::string jointName = nodeAnim->mNodeName.C_Str();

			//Iterate through each position keyframe
			for (unsigned int p = 0U; p < nodeAnim->mNumPositionKeys; ++p)  {
				glm::vec3 pos = glm::vec3(nodeAnim->mPositionKeys[p].mValue.x, nodeAnim->mPositionKeys[p].mValue.y, nodeAnim->mPositionKeys[p].mValue.z);

				//Check if a keyframe already exists at this time
				if (&keyframes[nodeAnim->mPositionKeys[p].mTime] == NULL) {
					JointPose * newPose = new JointPose(jointName, pos, glm::quat(), 1.0f);
					keyframes.emplace(nodeAnim->mPositionKeys[p].mTime, SkeletalAnimationSample(newPose, 1));
					numPoses++;
				}
				else {
					JointPose* oldPose = keyframes[nodeAnim->mPositionKeys[p].mTime].getPose(jointName);
					if (oldPose != nullptr) {
						oldPose->_translation = pos;
					}
				}
			}

			//Iterate through each rotation keyframe
			for (unsigned int p = 0U; p < nodeAnim->mNumRotationKeys; ++p) {
				glm::quat rot = glm::quat(nodeAnim->mRotationKeys[p].mValue.x, nodeAnim->mRotationKeys[p].mValue.y, nodeAnim->mRotationKeys[p].mValue.z, nodeAnim->mRotationKeys[p].mValue.w);

				//Check if a keyframe already exists at this time
				if (&keyframes[nodeAnim->mRotationKeys[p].mTime] == NULL) {
					JointPose * newPose = new JointPose(jointName, glm::vec3(0), rot, 1.0f);
					keyframes.emplace(nodeAnim->mRotationKeys[p].mTime, SkeletalAnimationSample(newPose, 1));
					numPoses++;
				}
				else {
					JointPose* oldPose = keyframes[nodeAnim->mRotationKeys[p].mTime].getPose(jointName);
					if (oldPose != nullptr) {
						oldPose->_rotation = rot;
					}
				}
			}

			//Iterate through each scaling keyframe
			for (unsigned int p = 0U; p < nodeAnim->mNumScalingKeys; ++p) {
				glm::vec3 vScale = glm::vec3(nodeAnim->mScalingKeys[p].mValue.x, nodeAnim->mScalingKeys[p].mValue.y, nodeAnim->mScalingKeys[p].mValue.z);

				//Use the scalar average of the original vec3 scale
				float sScale = (vScale.x + vScale.y + vScale.z) * 0.333f;

				//Check if a keyframe already exists at this time
				if (&keyframes[nodeAnim->mScalingKeys[p].mTime] == NULL) {
					JointPose * newPose = new JointPose(jointName, glm::vec3(0), glm::quat(), sScale);
					keyframes.emplace(nodeAnim->mScalingKeys[p].mTime, SkeletalAnimationSample(newPose, 1));
					numPoses++;
				}
				else {
					JointPose* oldPose = keyframes[nodeAnim->mScalingKeys[p].mTime].getPose(jointName);
					if (oldPose != nullptr) {
						oldPose->_scale = sScale;
					}
				}
			}


		}

		
		//Store the animation samples in the skeletal animation clip
		int x = 0;
		for (auto const& iter : keyframes) {
			skeletalClip->_animationSamples[x] = iter.second;
			x++;
		}

		//Add the skeletal animation clip to the final animation
		newAnim->setAnimationClip(skeletalClip);

		//Add to the complete animations list
		resMap.emplace(newAnim->getName(), newAnim);
	}

	return resMap;
}
*/

std::vector<SkeletalAnimation*> pModelLoader::processSkeletalAnimations(const aiScene & scene)
{
	std::vector<SkeletalAnimation*> res = std::vector<SkeletalAnimation*>();

	//Iterate through each aiAnimation
	for (unsigned int i = 0U; i < scene.mNumAnimations; ++i) {
		const aiAnimation* anim = scene.mAnimations[i];

		if (anim->mNumChannels <= 0) { continue; }

		//Initialize the new animation
		SkeletalAnimation* newAnim = new SkeletalAnimation(anim->mName.C_Str(), anim->mDuration, anim->mTicksPerSecond);

#ifdef _DEBUG
		std::printf("Importing a new Animation:\nName - %s\nDuration - %f\nTicksPerSecond - %f\n", std::string(newAnim->getName()).c_str(), newAnim->GetDuration(), newAnim->GetFrameRate());
#endif

		//Iterate through each joint channel
		for (unsigned int j = 0; j < anim->mNumChannels; ++j) {
			const aiNodeAnim* nodeAnim = anim->mChannels[j];

			//Get the name of the joint node this channel is for
			std::string name = nodeAnim->mNodeName.C_Str();

			AnimationChannel* newChannel = new AnimationChannel(name);

			//Iterate through position keys
			for (int k = 0; k < nodeAnim->mNumPositionKeys; k++) {
				//Get this keyframe's position value
				glm::vec3 pos = glm::vec3(nodeAnim->mPositionKeys[k].mValue.x, nodeAnim->mPositionKeys[k].mValue.y, nodeAnim->mPositionKeys[k].mValue.z);

				//Create a keyframe
				NodeKey<glm::vec3> keyframe = NodeKey<glm::vec3>(nodeAnim->mPositionKeys[k].mTime, name, pos);
				
				//Give the animation the new keyframe
				newChannel->AddTranslationKey(keyframe);
			}


			//Iterate through rotation keys
			for (int k = 0; k < nodeAnim->mNumRotationKeys; k++) {
				//Get this frame's rotation value
				glm::quat rot = glm::quat(nodeAnim->mRotationKeys[k].mValue.x, nodeAnim->mRotationKeys[k].mValue.y, nodeAnim->mRotationKeys[k].mValue.z, nodeAnim->mRotationKeys[k].mValue.w);
			
				//Create a keyframe
				NodeKey<glm::quat> keyframe = NodeKey<glm::quat>(nodeAnim->mRotationKeys[k].mTime, name, rot);

				//Give the animation the new keyframe
				newChannel->AddRotationKey(keyframe);
			
			}

			//Iterate through scaling keys
			for (int k = 0; k < nodeAnim->mNumScalingKeys; k++) {
				//Get this frame's scaling value
				glm::vec3 scl = glm::vec3(nodeAnim->mPositionKeys[k].mValue.x, nodeAnim->mPositionKeys[k].mValue.y, nodeAnim->mPositionKeys[k].mValue.z);

				//Create a keyframe
				NodeKey<glm::vec3> keyframe = NodeKey<glm::vec3>(nodeAnim->mRotationKeys[k].mTime, name, scl);

				//Give the animation the new keyframe
				newChannel->AddScalingKey(keyframe);
			}

			newAnim->addChannel(newChannel);

		}

		res.push_back(newAnim);
		pAnimationManager::instance()->addAnimation(newAnim);

	}


	return res;
}

Skeleton* pModelLoader::processSkeleton(const aiMesh& mesh) {
	std::string name = std::string(mesh.mName.C_Str()) + std::string("_skeleton");

	Skeleton* res = new Skeleton(name.c_str());
	
	for (unsigned int i = 0U; i < mesh.mNumBones; i++) {
		std::string jointName = mesh.mBones[i]->mName.C_Str();
		glm::mat4 jointOffset = aiMat4_to_glmMat4(mesh.mBones[i]->mOffsetMatrix);
		unsigned int jointWeightCount = mesh.mBones[i]->mNumWeights;

		Joint* newJoint = new Joint(jointName, i, jointOffset);
		res->addJoint(newJoint);

		for (unsigned int o = 0U; o < jointWeightCount; o++) {
			unsigned int vertexID = mesh.mBones[i]->mWeights[o].mVertexId;
			float vertexWeight = mesh.mBones[i]->mWeights[o].mWeight;
			
			res->addJointData(vertexID, newJoint->ID(), vertexWeight);
		}


	}

	return res;
}

/*
std::vector<Joint*> pModelLoader::extractJoints(const aiMesh& mesh) {
	std::vector<Joint*> res = std::vector<Joint*>();

	for (unsigned int i = 0U; i < mesh.mNumBones; i++) {
		std::string jointName = mesh.mBones[i]->mName.C_Str();
		glm::mat4 jointOffset = aiMat4_to_glmMat4(mesh.mBones[i]->mOffsetMatrix);
		unsigned int jointWeightCount = mesh.mBones[i]->mNumWeights;

		Joint* newJoint = new Joint(jointName, i, jointOffset);
		res.push_back(newJoint);

		for (unsigned int o = 0U; o < jointWeightCount; o++) {
			unsigned int vertexID = mesh.mBones[i]->mWeights[o].mVertexId;
			float vertexWeight = mesh.mBones[i]->mWeights[o].mWeight;

			res->addJointData(vertexID, newJoint->ID(), vertexWeight);
		}


	}

	return res;
}*/

pMaterial * pModelLoader::processMaterial(const aiMaterial & material, pShader* shader, const std::string& modelPath, std::string backupName)
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
	
	unsigned int uMatCount = 0;
	std::string sMatName = matName.C_Str();
	std::string origName = sMatName;
	while (pResourceFactory::instance()->hasModel(sMatName)) {
		sMatName = origName + std::to_string(uMatCount);
		uMatCount++;
	}

	pMaterial* newMat = pResourceFactory::instance()->createMaterial(sMatName, shader, matInfo);

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
		path::iterator itr = tPath.end(); //Itr points to end ""
		if (itr != tPath.begin()) {
			//Itr becomes file name (last element)
			itr--;
			std::string fileDir = itr->generic_string();

			//Itr becomes the parent folder name (second-last element)
			itr--;
			std::string parentDir = itr->generic_string();

			//Construct test directory
			testImagePath = (modelDir.generic_string() + "/" + parentDir + "/" + fileDir);

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
			meshName = "unnamed_mesh";
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
	//std::vector<VertexBoneData> vBoneData = std::vector<VertexBoneData>();

	hasPositions = mesh.HasPositions();
	hasCoordinates = mesh.HasTextureCoords(0);
	hasNormals = mesh.HasNormals();
	hasColors = mesh.HasVertexColors(0);
	hasTangents = mesh.HasTangentsAndBitangents();
	hasBones = mesh.HasBones();

	//Iterate through each face on the mesh
	for (unsigned int o = 0U; o < mesh.mNumFaces; ++o) {
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
	for (unsigned int o = 0U; o < mesh.mNumVertices; ++o) {

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

	unsigned int uMeshCount = 0;
	std::string origName = meshName;
	while (pResourceFactory::instance()->hasModel(meshName)) {
		meshName = origName + std::to_string(uMeshCount);
		uMeshCount++;
	}

	pModel* resModel = pResourceFactory::instance()->createModel(meshName, matRef, GL_TRIANGLES, vPositions, vIndeces, vCoordinates, vNormals, vTangents, vBiTangents, vColors);

	return resModel;
}

pSceneNode * pModelLoader::processNodes(const aiScene& scene, const std::vector<pModel*> indexedMeshes, const std::vector<SkeletalAnimation*> skeletalAnimations, aiNode* root, glm::mat4& parentTransform)
{
	if (root == nullptr) { root = scene.mRootNode; }

	std::string rootName = root->mName.C_Str();

	bool isBone = false;

	//Iterate through each mesh
	for (auto meshIter : indexedMeshes) {
		//Check for a skeleton in this mesh
		Skeleton* skelly = meshIter->getSkeleton();
		if (skelly != nullptr) {
			//Check if the skeleton has already gone through this process
			if (skelly->isOrganized() == false) {
				Joint* thisNodeJoint = skelly->getJoint(rootName);
				//Check if this node represents a skeleton joint
				if (thisNodeJoint != nullptr) {
					//This node represents this bone in the hierarchy!
					matchSkeleton(root, skelly);
					skelly->setOrganized(true);
					isBone = true;
				}
			}

			//Find an animation meant for this skeleton
			for (auto animIter : skeletalAnimations) {
				//Check for a channel that exists matching the skeleton root
				AnimationChannel* chnl = animIter->getChannel(skelly->getJoint(0)->Name());
				if (chnl != nullptr) {
					//This animation is (probably) meant for this skeleton
					animIter->SetSkeleton(skelly);
				}
			}
		}
	}

	if (!isBone) {
		pSceneNode* thisNode = new pSceneNode();
		if (thisNode->getAttachedSceneObject() == nullptr) { thisNode->setAttachedSceneObject(new pSceneObject()); }

		//Extract node name
		thisNode->setName(rootName);



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
			if (numMeshes > 1) {
				for (unsigned int i = 0U; i < numMeshes; ++i) {
					pModel* nodeMesh = indexedMeshes[root->mMeshes[i]];

					pSceneObject* newChildObject = new pSceneObject(nodeMesh, nullptr, nullptr, nullptr);
					pSceneNode* newChild = new pSceneNode(glm::vec3(0), glm::vec3(0), glm::vec3(1), std::string("unknown_child"), newChildObject, thisNode);

					//Handle tacking on an animator if appropriate
					if (skeletalAnimations.size() > 0) {
						for (auto animIter : skeletalAnimations) {
							//Check for matching animation and mesh skeletons
							if (animIter->GetSkeleton()->getName() == nodeMesh->getSkeleton()->getName()) {
								pAnimator* newAnimator = new pAnimator(animIter, newChildObject);
								newChildObject->attachAnimator(newAnimator);
								break;
							}
						}
					}


				}
			}
			else {
				unsigned int meshIndex = root->mMeshes[0];
				//Get the pModel from indexed meshes
				pModel* nodeMesh = indexedMeshes[meshIndex];
				//Give the node's sceneObject the mesh
				pSceneObject* sceneObj = thisNode->getAttachedSceneObject();
				sceneObj->attachModel(nodeMesh);

				//Handle tacking on an animator if appropriate
				if (skeletalAnimations.size() > 0) {
					for (auto animIter : skeletalAnimations) {
						//Check for matching animation and mesh skeletons
						if (animIter->GetSkeleton()->getName() == nodeMesh->getSkeleton()->getName()) {
							pAnimator* newAnimator = new pAnimator(animIter, sceneObj);
							sceneObj->attachAnimator(newAnimator);
							break;
						}
					}
				}
			}
		}




		//Process this node's children
		for (unsigned int i = 0U; i < root->mNumChildren; ++i) {
			aiNode* child = root->mChildren[i];
			if (child != NULL) {
				pSceneNode* potentialChild = (processNodes(scene, indexedMeshes, skeletalAnimations, child, parentTransform));
				if (potentialChild != nullptr) {
					thisNode->appendChild(potentialChild);
				}
			}
		}
		return thisNode;
	}
	else {
		for (unsigned int i = 0U; i < root->mNumChildren; ++i) {
			aiNode* child = root->mChildren[i];
			if (child != NULL) {
				processNodes(scene, indexedMeshes, skeletalAnimations, child, parentTransform);
			}
		}

		return nullptr;
	}


}

aiNode pModelLoader::findNode(const aiNode& rootNode, aiString name)
{
	if (rootNode.mName == name) { return rootNode; }

	for (int i = 0; i < rootNode.mNumChildren; i++) {
		aiNode child = *rootNode.mChildren[i];
		if (findNode(child, name).mName.C_Str() != "EMPTY") {
			return child;
		}
	}

	return aiNode("EMPTY");
}

Joint * pModelLoader::matchSkeleton(const aiNode* rootNode, Skeleton * skeleton)
{
	//Look for a joint in the skeleton that uses this node
	Joint* joint = skeleton->getJoint(rootNode->mName.C_Str());

	if (joint != nullptr) {
		//Check this node for children
		for (int i = 0; i < rootNode->mNumChildren; i++) {
			//Add node children as child joints if they exist
			Joint* child = matchSkeleton(rootNode->mChildren[i], skeleton);
			if (child != nullptr) {
				joint->AddChild(child);
			}
		}
	}

	return joint;
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

