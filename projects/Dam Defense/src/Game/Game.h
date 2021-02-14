//Dam Defense, by Atlas X Games
//Game.h, the header file for the class that represents the main gameworld scene
#pragma once

//include required features from titan
#include "Titan/Application.h"
#include "Titan/Systems/Sound.h"
#include "EnemyComponent.h"

using namespace Titan;

class Game : public TTN_Scene {
public:
	//default constructor
	Game();

	//default destrcutor
	~Game() = default;

	//sets up the scene
	void InitScene();

	//update the scene
	void Update(float deltaTime);

	//late render (terrain, water, etc.)
	void PostRender();

	//keyboard input
	void KeyDownChecks();
	void KeyChecks();
	void KeyUpChecks();

	//mouse input
	void MouseButtonDownChecks();
	void MouseButtonChecks();
	void MouseButtonUpChecks();

	bool GetGameIsPaused() { return m_paused; }
	void SetGameIsPaused(bool paused) { m_paused = paused; }

	bool GetGameIsOver() { return m_gameOver; }
	void SetGameIsOver(bool over) { m_gameOver = over; }

	bool GetGameWin() { return m_gameWin; }
	void SetGameWin(bool win) { m_gameWin = win; }

	bool GetGameShouldRestart() { return m_restart; }
	void SetGameShouldRestart(bool restart) { m_restart = restart; }

	static int GetDamHealth() { return Dam_health; }

	//function to restart the game reseting all the data
	void RestartData();
	//Assets
public:
#pragma region assets
	//shader programs
	TTN_Shader::sshptr shaderProgramUnTextured;
	TTN_Shader::sshptr shaderProgramTextured;
	TTN_Shader::sshptr shaderProgramSkybox;
	TTN_Shader::sshptr shaderProgramAnimatedTextured;
	TTN_Shader::sshptr shaderProgramWater;
	TTN_Shader::sshptr shaderProgramTerrain;

	//meshes
	TTN_Mesh::smptr cannonMesh;
	TTN_Mesh::smptr boat1Mesh;
	TTN_Mesh::smptr boat2Mesh;
	TTN_Mesh::smptr boat3Mesh;
	TTN_Mesh::smptr flamethrowerMesh;
	TTN_Mesh::smptr birdMesh;
	TTN_Mesh::smptr treeMesh[3];
	TTN_Mesh::smptr rockMesh[5];
	TTN_Mesh::smptr damMesh;

	TTN_Mesh::smptr skyboxMesh;
	TTN_Mesh::smptr sphereMesh; //used for cannonballs and particles
	TTN_Mesh::smptr terrainPlain;

	//textures
	TTN_Texture2D::st2dptr cannonText;
	TTN_TextureCubeMap::stcmptr skyboxText;
	TTN_Texture2D::st2dptr terrainMap;
	TTN_Texture2D::st2dptr sandText;
	TTN_Texture2D::st2dptr rockText;
	TTN_Texture2D::st2dptr grassText;
	TTN_Texture2D::st2dptr waterText;
	TTN_Texture2D::st2dptr boat1Text;
	TTN_Texture2D::st2dptr boat2Text;
	TTN_Texture2D::st2dptr boat3Text;
	TTN_Texture2D::st2dptr flamethrowerText;
	TTN_Texture2D::st2dptr birdText;
	TTN_Texture2D::st2dptr treeText;
	TTN_Texture2D::st2dptr damText;

	//materials
	TTN_Material::smatptr boat1Mat;
	TTN_Material::smatptr boat2Mat;
	TTN_Material::smatptr boat3Mat;
	TTN_Material::smatptr flamethrowerMat;
	TTN_Material::smatptr birdMat;
	TTN_Material::smatptr treeMat;
	TTN_Material::smatptr rockMat;
	TTN_Material::smatptr damMat;
	std::vector<TTN_Material::smatptr> m_mats;

	TTN_Material::smatptr cannonMat;
	TTN_Material::smatptr skyboxMat;
	TTN_Material::smatptr smokeMat;
	TTN_Material::smatptr fireMat;

	TTN_Texture2D::st2dptr healthBar;

	//sound
	TTN_AudioEngine& engine = TTN_AudioEngine::Instance();
	
	TTN_AudioEventHolder::saehptr m_cannonFiringSounds;
	TTN_AudioEventHolder::saehptr m_splashSounds;
	TTN_AudioEventHolder::saehptr m_music;
	TTN_AudioEventHolder::saehptr m_jingle;
	TTN_AudioEventHolder::saehptr m_flamethrowerSound;

#pragma endregion

	//Entities
protected:
	entt::entity camera;
	entt::entity light;
	entt::entity skybox;
	entt::entity cannon;
	std::vector<std::pair<entt::entity, bool>> cannonBalls;
	std::vector<entt::entity> boats;
	entt::entity smokePS;
	entt::entity terrain;
	entt::entity water;
	entt::entity birds[3];
	entt::entity dam;

	std::vector<entt::entity> flamethrowers;
	std::vector<entt::entity> flames;

	/////// OTHER DATA ///////////
#pragma region Data
protected:
	/////// Player control data/////////
	float cannonBallForce = 3600.0f;;//a multiplier for the ammount of force should be applied to a cannonball when it is fired
	float playerShootCooldown = 0.7f;//the ammount of time in seconds that the player has to wait between shots

	glm::vec2 mousePos;//position of the mouse in screenspace
	glm::vec2 rotAmmount;//the ammount the main player should be rotated
	glm::vec3 playerDir;//the direction the cannon is currently facing
	float playerShootCooldownTimer;//how much time until the player can shoot again

	//////// GAMEPLAY DATA ////////////
	int lastWave = 3; //the wave the player needs to reach and beat to win
	float damage = 1.0f; //damage of boats (dam health is 100.f)

	/////// Terrain and water control data ////////
	float terrainScale = 0.15f;//the terrain scale

	float water_time; //the current time for the water, used in it's sin wave
	float water_waveSpeed;//the speed of the waves
	float water_waveBaseHeightIncrease;//the base height of the waves
	float water_waveHeightMultiplier;//how much the waves should grow
	float water_waveLenghtMultiplier;//how long the waves should be

	//////// DAM AND FLAMETHROWER CONTROL DATA ///////
	float FlameThrowerCoolDown = 10.0f; //how long the player has to wait between flamethrower uses
	float FlameActiveTime = 3.0f; //how long the flamethrower lasts
	int Dam_MaxHealth = 100; //the maximum health of the dam

	bool Flaming; //if flamethrowers are active right now
	float FlameTimer; //flamethrower cooldown
	float FlameAnim; //flamethrower duration
	inline static int Dam_health;//the current health on the dam

	//////// BIRD CONTROL DATA ///////////////
	glm::vec3 birdBase = glm::vec3(100, 15, 135); //starting position
	glm::vec3 birdTarget = glm::vec3(-100, 15, -65); //lerps to this position
	float birdTimer;//timer to track how far through the lerp they are

	///////////SCENE CONTROL DATA///////////
	bool m_paused; //wheter or not the scene is paused
	bool m_gameOver = false; //wheter or not the player has yet gameover
	bool m_gameWin = false;//wheter or not the player has won
	bool m_restart;//wheter or not the game is restarting

	/////////////ENEMY AND WAVE CONTROLS//////////////////
	float m_timeBetweenEnemyWaves = 5.0f; //rest time between waves
	float m_timeBetweenEnemySpawns = 2.0f; //cooldown between when boats spawn
	int m_enemiesPerWave = 5; //how many enemy enemies should it add to each wave, so wave number * this is the number of enemies in any given wave

	int m_currentWave = 1; //the current wave
	float m_timeTilNextWave; //the timer until the next wave starts, used after a wave has ended
	float m_timeUntilNextSpawn; //the timer until the next boat spawns
	int m_boatsRemainingThisWave; //the number of boats that need to be destoryed before the wave starts again
	int m_boatsStillNeedingToSpawnThisWave; //the number of boats that still need to be spawned before the wave can end
	bool m_rightSideSpawn = true; //wheter or not it should be using the right (true) or left (false) spawner

	/////////// SOUND CONTROL///////////////
	//control melody
	float melodyTimeTracker = 0.0f;
	int timesMelodiesPlayed = 0;
	int timesMelodyShouldPlay; //will be random between 2 and 3 every time
	bool melodyFinishedThisFrame = false;
	bool partialMelody = false;
	bool fullMelodyFinishedThisFrame = false;

	//jingle controls
	bool playJingle = false;
	float timeSinceJingleStartedPlaying = 0.0f;

#pragma endregion

	///////PARTICLE TEMPLATES//////////
	TTN_ParticleTemplate smokeParticle;//smoke burst particles
	TTN_ParticleTemplate fireParticle;//fire particles
	TTN_ParticleTemplate expolsionParticle;//expolsion particles

	//set up functions, called by InitScene()
protected:
	void SetUpAssets();
	void SetUpEntities();
	void SetUpOtherData();

	//update functions, called by Update()
protected:
	void PlayerRotate(float deltaTime);
	void StopFiring();

	//functions for spawning enemies
	void SpawnBoatLeft();
	void SpawnBoatRight();
	void WaveUpdate(float deltaTime);

	//flamethrowers, collisions, and damages
	void Flamethrower();
	void FlamethrowerUpdate(float deltaTime);
	void Collisions();
	void Damage(float deltaTime);

	//sounds
	void GameSounds(float dt);
	//misc
	void BirdUpate(float deltaTime);
	void ImGui();

	//other functions, ussually called in relation to something happening like player input or a collision
protected:
	void CreateCannonball();
	void DeleteCannonballs();

	void CreateExpolsion(glm::vec3 location);

	//CG assingment 2 stuff
protected:
	//color correction effect
	TTN_ColorCorrect::scolcorptr m_colorCorrectEffect;
	//bools for imgui controls
	bool m_applyWarmLut;
	bool m_applyCoolLut;
	bool m_applyCustomLut;

	bool m_noLighting;
	bool m_ambientOnly;
	bool m_specularOnly;
	bool m_ambientAndSpecular;
	bool m_ambientSpecularAndOutline;
	//float to control outline size
	float m_outlineSize = 0.2f;

	//variables for if the specular and diffuse ramps should be used
	bool m_useDiffuseRamp = false;
	bool m_useSpecularRamp = false;
	bool m_useTextures = true;
};

enum class Numbers {
	

};

class GameUI : public TTN_Scene {
	friend class Game;
public:
	//default constructor
	GameUI();

	//default destrcutor 
	~GameUI() = default;

	//sets up the scene
	void InitScene();

	//update the scene
	void Update(float deltaTime);

private:
	//entities
	entt::entity cam;
	//healthbar
	entt::entity healthBar;
	//health
	entt::entity healthDam;
	//score
	entt::entity score;
	

	//assets
	TTN_Texture2D::st2dptr textureHealth;
	TTN_Texture2D::st2dptr textureHealthDam;
	TTN_Texture2D::st2dptr textureScore;

	//dam health
	float DamHealth;
};

inline float SmoothStep(float t) {
	return (t * t * (3.0f - 2.0f * t));
}

inline float FastStart(float t) {
	return std::cbrt(t);
}

inline float SlowStart(float t) {
	return (t * t * t);
}

inline float ZeroOneZero(float t) {
	return (-4.0f * (t * t) + 4.0f * t);
}