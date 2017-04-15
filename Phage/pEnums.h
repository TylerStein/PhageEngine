#pragma once

enum pType
{
	MODEL, MATERIAL, IMAGE, SHADER, AUDIO
};

enum keyName
{
	KEY_A = 65, KEY_B = 66, KEY_C = 67, KEY_D = 68, KEY_E = 69, KEY_F = 70, KEY_G = 71, KEY_H = 72, KEY_I = 73, KEY_J = 74, KEY_K = 75, KEY_L = 76, KEY_M = 77, KEY_N = 78,
	KEY_O = 79, KEY_P = 80, KEY_Q = 81, KEY_R = 82, KEY_S = 83, KEY_T = 84, KEY_U = 85, KEY_V = 86, KEY_W = 87, KEY_X = 88, KEY_Y = 89, KEY_Z = 90, KEY_TAB = 258,
	KEY_CAPS = 280, KEY_LSHIFT = 340, KEY_LCTRL = 341, KEY_LALT = 342, KEY_TILL = 96, KEY_COMMA = 44, KEY_PERIOD = 46, KEY_FORWARDSLASH = 47,
	KEY_BACKSLASH = 91, KEY_RSHIFT = 344, KEY_ENTER = 257, KEY_RCTRL = 345, KEY_RALT = 346, KEY_UP = 265, KEY_LEFT = 263, KEY_RIGHT = 262, KEY_DOWN = 264,
	KEY_EQUALS = 61, KEY_DASH = 45, KEY_0 = 48, KEY_9 = 57, KEY_8 = 56, KEY_7 = 55, KEY_6 = 54, KEY_5 = 53, KEY_4 = 52,
	KEY_3 = 51, KEY_2 = 50, KEY_1 = 49, KEY_SPACE = 32, KEY_SEMICOLON = 59, KEY_APOSTROPHE = 39, KEY_LBRACKET, KEY_RSQUARE_BRACKET = 93,
	KEY_LSQUARE_BRACKET = 91, KEY_ESCAPE = 256, KEY_BACKSPACE = 259, KEY_INSERT = 260, KEY_DELETE = 261, KEY_PAGE_UP = 266, KEY_PAGE_DOWN = 267, KEY_HOME = 268, KEY_END = 269,
	SCROLL_LOCK = 281, KEY_NUM_LOCK = 282, KEY_PRINT_SCREEN = 283, KEY_PAUSE = 284, KEY_F1 = 290, KEY_F2 = 291, KEY_F3 = 292, KEY_F4 = 293, KEY_F5 = 294, KEY_F6 = 295, KEY_F7 = 296,
	KEY_F8 = 297, KEY_F9 = 298, KEY_F10 = 299, KEY_F11 = 300, KEY_F12 = 301, KEY_F13 = 302, KEY_F14 = 303, KEY_F15 = 304, KEY_F16 = 305, KEY_F17 = 306, KEY_F18 = 307, KEY_F19 = 308,
	KEY_F20 = 309, KEY_F21 = 310, KEY_F22 = 311, KEY_F23 = 312, KEY_F24 = 313, KEY_F25 = 314, KEY_KP_0 = 320, KEY_KP_1 = 321, KEY_KP_2 = 322, KEY_KP_3 = 323, KEY_KP_4 = 324, KEY_KP_5 = 325,
	KEY_KP_6 = 326, KEY_KP_7 = 327, KEY_KP_8 = 328, KEY_KP_9 = 329, KEY_KP_DECIMAL = 330, KEY_KP_DIVIDE = 331, KEY_KP_MULTIPLY = 332, KEY_KP_SUBTRACT = 333, KEY_KP_ADD = 334, KEY_KP_ENTER = 335,
	KEY_KP_EQUAL = 336, KEY_MENU = 348, KEY_RIGHT_SUPER = 347, KEY_LEFT_SUPER = 343, KEY_WORLD_1 = 161, KEY_WORLD_2 = 162, KEY_UNKNOWN = -1
};

enum KeyStatus
{
	keyChecked = 0, keyDown = 1, keyUp = 2, timePressed = 3
};


enum Attributes {
	//Vertex Data
	VertexPosition = 1 << 1,
	VertexNormal = 1 << 2,
	VertexCoordinate = 1 << 3,
	VertexColor = 1 << 4,
	VertexTangent = 1 << 5,
	VertexBiTangent = 1 << 6,
	VertexIndeces = 1 << 7,
	BoneIDs = 1 << 8,
	BoneWeights = 1 << 9,

	NONE = 1 << 16
};

//Represents attributes for functions/flags
enum Uniforms {
	//Material Info
	DiffuseColor = 1 << 1, //Diffuse color multiplier
	AmbientColor = 1 << 2, //Ambient color multiplier
	SpecularColor = 1 << 3, //Specular color multiplier
	Shininess = 1 << 4, //Specular shine multiplier
	Emission = 1 << 5,

	//Textures
	DiffuseTexture = 1 << 6, //Diffuse texture
	SpecularTexture = 1 << 7, //Specular texture
	BumpTexture = 1 << 8, //Bump texture

	//View Matrices
	Camera_View = 1 << 9, //The camera view matrix
	Projection_View = 1 << 10, //The projection matrix
	Model_View = 1 << 11, //The model movement matrix
	Normal_View = 1 << 12, //The model normal matrix
	ModelViewProjection = 1 << 13, //The MVP matrix

	//Light Properties
	Light_Position = 1 << 14,
	Light_Color = 1 << 15,
	Light_Power = 1 << 16,
	Light_Ambient = 1 << 17,
	Light_Range = 1 << 18,
	Light_Count = 1 << 19,
	Light_Angle = 1 << 20,
	Light_Cone = 1 << 21,
	Light_Type = 1 << 22,

	//Misc
	Camera_Position = 1 << 23,
	Time_Elapsed = 1 << 24,
	Time_Delta = 1 << 25,
	Cube_Map_Texture = 1 << 26,

	//Animation
	Bones = 1 << 27
};
