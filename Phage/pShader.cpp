#include "pShader.h"
#include "pFileReader.h"
#include "GLError.h"

#ifdef _DEBUG
#include <iostream>
#endif



pShader::pShader(std::string name, GLint flags, std::string vertexShaderPath, std::string fragmentShaderPath)
{
	if (fragmentShaderPath == "" || vertexShaderPath == "") {
		#ifdef _DEBUG
		std::cout << "Attempted to create shader without providing filepaths!" << std::endl;
		#endif
		LogManager::instance()->warning("Attempted to create shader without providing filepaths!");
		return;
	}

	this->flags = flags;

	//Set the name
	this->name = name;
	//Compile the shader
	compileShader(vertexShaderPath, fragmentShaderPath);

	//Use the shader
	glUseProgram(shaderID);
	//Find the locationID of the flags value
	GLuint flagsLocation = glGetUniformLocation(shaderID, "vFlags");
	//Pass the flags to the shader
	glUniform1i(flagsLocation, flags);

	attributeMap = new SimpleKeyValue<Attributes, GLuint>();
	GLuint tmpID;

	//Initialize the view matrices
	const glm::mat4 idmat = glm::mat4(1);
	setPropertyMat4(getPropertyID(Model_View), idmat);
	setPropertyMat4(getPropertyID(Camera_View), idmat);
	setPropertyMat4(getPropertyID(Projection_View), idmat);

	//Has material info
	if ((flags & MaterialInfo) == MaterialInfo) {
		//Get the needed attribute IDs and initialize the shader variables
		setPropertyVec3(getPropertyID(DiffuseColor), glm::vec3(1, 1, 1));

		setPropertyVec3(getPropertyID(AmbientColor), glm::vec3(1, 1, 1));

		setPropertyVec3(getPropertyID(SpecularColor), glm::vec3(1, 1, 1));

		setPropertyFloat(getPropertyID(Shininess), 1);

		LogManager::instance()->info("Material Info Set (Shader)");
	}

	

	//Has diffuse texture info
	if ((flags & Texture_Diffuse) == Texture_Diffuse) {
		setPropertyTextureID(getPropertyID(DiffuseTexture), -1);
		LogManager::instance()->info("Dffuse Texture Info Set (Shader)");
	}

	//Has bumpmap texture info
	if ((flags & Texture_Bump) == Texture_Bump) {
		setPropertyTextureID(getPropertyID(BumpTexture), -1);

		LogManager::instance()->info("Bump Texture Info Set (Shader)");
	}

	//Has specular texture info
	if ((flags & Texture_Specular) == Texture_Specular) {
		//Create a temporary pair to hold on the attribute map's return type
		setPropertyTextureID(getPropertyID(SpecularTexture), -1);

		LogManager::instance()->info("Specular Texture Info Set (Shader)");
	}

	//Has light receiving info
	if ((flags & Light_Affected) == Light_Affected) {
		//Get the needed attribute IDs and initialize the shader variables
		setPropertyVec3(getPropertyID(Light_Position), glm::vec3(0, 0, 0));

		setPropertyVec3(getPropertyID(Light_Color), glm::vec3(1, 1, 1));

		setPropertyVec3(getPropertyID(Light_Power), glm::vec3(1, 1, 1));

		setPropertyFloat(getPropertyID(Light_Attenuation), 1.0);

		setPropertyFloat(getPropertyID(Light_Ambient), 1.0);

		LogManager::instance()->info("Light Info Set (Shader)");
	}

}

pShader::~pShader()
{
}

void pShader::setPropertyMat4(GLuint attributeID, glm::mat4 data)
{
	//Make sure the shader is being used
	glUseProgram(shaderID);

	glUniformMatrix4fv(attributeID, 1, GL_FALSE, &data[0][0]);
}

void pShader::setPropertyVec3(GLuint attributeID, glm::vec3 data)
{
	//Make sure the shader is being used
	glUseProgram(shaderID);

	glUniform3fv(attributeID, 1, &data[0]);
}

void pShader::setPropertyVec2(GLuint attributeID, glm::vec2 data)
{
	//Make sure the shader is being used
	glUseProgram(shaderID);

	glUniform2fv(attributeID, 1, &data[0]);
}

void pShader::setPropertyFloat(GLuint attributeID, GLfloat data)
{
	//Make sure the shader is being used
	glUseProgram(shaderID);

	glUniform1f(attributeID, data);
}

void pShader::setPropertyTextureID(GLuint attributeID, GLuint textureID)
{	//Make sure the shader is being used
	glUseProgram(shaderID);

	glUniform1i(attributeID, textureID);
}

GLuint pShader::getPropertyID(Attributes propertyAttribute)
{
	//Make sure the shader is being used
	glUseProgram(shaderID);

	//Temporary string to track desired attribute retreived
	std::string attr = "";

	GLint res = -1;
	//Check if property attribute already exists
	res = attributeMap->findValue(propertyAttribute);
	if (res!=-1) {
		//Attribute already exists, just return it
#ifdef _DEBUG
		//std::cout << "Retreived property ID (" << res << ") from attribute (" << propertyAttribute << ")" << std::endl;
#endif
		return res;
	}
	else {
		//Generate the propertyID
		switch (propertyAttribute) {
		case DiffuseColor:
			res = glGetUniformLocationARB(shaderID, "fMaterial.diffuse");
			attr = "fMaterial.diffuse";
			break;
		case AmbientColor:
			res = glGetUniformLocationARB(shaderID, "fMaterial.ambient");
			attr = "fMaterial.ambient";
			break;
		case SpecularColor:
			res = glGetUniformLocationARB(shaderID, "fMaterial.specular");
			attr = "fMaterial.specular";
			break;
		case Shininess:
			res = glGetUniformLocationARB(shaderID, "fMaterial.shininess");
			attr = "fMaterial.shininess";
			break;
		case DiffuseTexture:
			res = glGetUniformLocation(shaderID, "fTextures.diffuse");
			attr = "fTextures.diffuse";
			break;
		case SpecularTexture:
			res = glGetUniformLocation(shaderID, "fTextures.specular");
			attr = "fTextures.specular";
			break;
		case BumpTexture:
			res = glGetUniformLocation(shaderID, "fTextures.bump");
			attr = "fTextures.bump";
			break;
		case Camera_View:
			res = glGetUniformLocationARB(shaderID, "vView.camera");
			attr = "vView.camera";
			break;
		case Projection_View:
			res = glGetUniformLocationARB(shaderID, "vView.projection");
			attr = "vView.projection";
			break;
		case Model_View:
			res = glGetUniformLocationARB(shaderID, "vView.model");
			attr = "vView.model";
			break;
		case Light_Position:
			res = glGetUniformLocationARB(shaderID, "fLight.position");
			attr = "fLight.position";
			break;
		case Light_Color:
			res = glGetUniformLocationARB(shaderID, "fLight.color");
			attr = "fLight.color";
			break;
		case Light_Power:
			res = glGetUniformLocationARB(shaderID, "fLight.power");
			attr = "fLight.power";
			break;
		case Light_Attenuation:
			res = glGetUniformLocationARB(shaderID, "fLight.attenuation");
			attr = "fLight.attenuation";
			break;
		case Light_Ambient:
			res = glGetUniformLocationARB(shaderID, "fLight.ambient");
			attr = "fLight.ambient";
			break;
		default:
			res = -1;
			break;
		}

#ifdef _DEBUG
		if (res == -1)
		{
			//std::cout << "PropertyID could not be reached for attirubute (" << attr << ")" << std::endl;
		} else {
			std::cout << "Created property ID (" << res << ") from attribute (" << attr << ")" << std::endl;

		}
#endif

	}

	//Add the found attribute ID to the map under the associated attribute enum
	attributeMap->insert(propertyAttribute, res);
	//Return the resulting ID
	return res;
}

void pShader::compileShader(std::string vertexShaderPath, std::string fragmentShaderPath)
{
	pFileReader reader;

	//Read the shader files
	std::string vs = reader.readFile((char*)vertexShaderPath.c_str());
	std::string fs = reader.readFile((char*)fragmentShaderPath.c_str());

	//Create char* of the shaders
	const char* vertex_shader = vs.c_str();
	const char* fragment_shader = fs.c_str();

	//Compile the vertex shader
	GLuint vertShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertShaderID, 1, &vertex_shader, NULL);
	glCompileShader(vertShaderID);
	GLError::checkShaderError(vertShaderID, "VertexShader");

	//Compile the fragment shader
	GLuint fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShaderID, 1, &fragment_shader, NULL);
	glCompileShader(fragShaderID);
	GLError::checkShaderError(fragShaderID, "FragmentShader");

	//Create the shader program by attaching the vertex and fragment shaders
	shaderID = glCreateProgram();
	glAttachShader(shaderID, fragShaderID);
	glAttachShader(shaderID, vertShaderID);

	//Link the shaderID as the currently used shader
	glLinkProgram(shaderID);

	GLError::printActiveAttributes(shaderID);

	//Check for linking errors
	GLError::checkLinkError(shaderID);

	glDetachShader(shaderID, fragShaderID);
	glDetachShader(shaderID, vertShaderID);
}

GLuint pShader::getShaderID()
{
	return shaderID;
}

std::string pShader::getName()
{
	return name;
}

GLint pShader::getFlags()
{
	return flags;
}
