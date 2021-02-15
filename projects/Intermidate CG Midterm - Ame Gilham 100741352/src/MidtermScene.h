//Intermdiate CG Midterm by Ame Gilham #10071352 for INFR 2350
//MidtermScene.h - The header file for the class that represents the scene being played
#pragma once

//include required features from titan
#include "Titan/Application.h"

using namespace Titan;

class MidtermScene : public TTN_Scene {
public:
	//default constructor
	MidtermScene();

	//default destructor
	~MidtermScene() = default;

	//sets up the scene
	void InitScene();

	//updates the scene
	void Update(float deltaTime);

	//keyboard input
	void KeyChecks();

	//mouse input
	void MouseButtonChecks();

private:
	//function for the imgui controls, called from update
	void ImGui();

private:
	//for camera movement 
	glm::vec2 mousePos;//position of the mouse in screenspace
	glm::vec2 rotAmmount;//the ammount the main player should be rotated
	glm::vec3 facing;//the direction the player is currently facing
	float m_deltaTime; //the change in time this frame

	//vector of all the materials in the scene, used for lighting controls 
	std::vector<TTN_Material::smatptr> m_materials;

	//the bloom effect
	TTN_BloomEffect::sbloomptr m_bloom;

	//bools for lighting controls and imgui
	bool m_noLighting = false;
	bool m_ambientOnly = false;
	bool m_specularOnly = false;
	bool m_ambientAndSpecular = true;
	bool m_ambientSpecularAndBloom = false;
	//bool for texture control and imgui
	bool m_useTextures = true;

	//entities
	entt::entity m_camera;
	entt::entity skybox;
	entt::entity ship;
	entt::entity landingPad;
	entt::entity lightHouse;
	entt::entity ground;
};