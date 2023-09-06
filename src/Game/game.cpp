#include "game.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>

#include "../Renderer/shader_program.h"
#include "../Resources/resource_manager.h"
#include "../Renderer/texture_2D.h"
#include "../Renderer/sprite.h"
#include "../Renderer/animated_sprite.h"

#include <string>
#include <iostream>

Game::Game(const glm::ivec2& window_size)
	: current_state_(GameState::Active)
	, window_size_(window_size){
	keys_.fill(false);
}

Game::~Game(){}

void Game::Render(){
	ResourceManager::GetAnimatedSprite("NewAnimatedSprite")->Render();
}

void Game::Update(const uint64_t delta){
	ResourceManager::GetAnimatedSprite("NewAnimatedSprite")->Update(delta);
};

void Game::SetKey(const int key, int action){
	keys_[key] = action;
}

bool Game::Initialize(){
	auto default_shader_program = ResourceManager::LoadShaders("DefaultShader",
															   "res/shaders/vertex.vert",
															   "res/shaders/fragment.frag");
	if(!default_shader_program){
		std::cerr << "Can't create shader program: " << "DefaultShader" << std::endl;
		return false;
	}

	auto sprite_shader_program = ResourceManager::LoadShaders("SpriteShader",
															  "res/shaders/sprite_v.vert",
															  "res/shaders/sprite_f.frag");
	if(!default_shader_program){
		std::cerr << "Can't create shader program: " << "SpriteShader" << std::endl;
		return false;
	}

	auto tex = ResourceManager::LoadTexture("DefaultTexture", "res/textures/map_16x16.png");

	std::vector<std::string> sub_textures_names{
		"block",				"top_block",
		"bottom_block",			"left_block",
		"right_block",			"top_left_block",
		"top_right_block",		"bottom_left_block",
		"bottom_right_block",

		"beton",				"top_beton",
		"bottom_beton",			"left_beton",
		"right_beton",			"top_left_beton",
		"top_right_beton",		"bottom_left_beton",
		"bottom_right_beton",

		"water1",	"water2",	"water3",
		"trees",	"ice",		"wall",
		"eagle",	"dead_eagle",
		"nothing",	"respawn1", "respawn2",
		"respawn3",	"respawn4",


	};
	auto texture_atlas = ResourceManager::LoadTextureAtlas("DefaultTextureAtlas",
														   "res/textures/map_16x16.png",
														   sub_textures_names,
														   16, 16);

	auto animated_sprite = ResourceManager::LoadAnimatedSprite("NewAnimatedSprite",
															   "DefaultTextureAtlas",
															   "SpriteShader",
															   100, 100,
															   "block");
	animated_sprite->SetPosition(glm::vec2(300, 300));

	std::vector<std::pair<std::string, uint64_t>> water_state;
	water_state.emplace_back(std::make_pair<std::string, uint64_t>("water1", 1'000'000'000));
	water_state.emplace_back(std::make_pair<std::string, uint64_t>("water2", 1'000'000'000));
	water_state.emplace_back(std::make_pair<std::string, uint64_t>("water3", 1'000'000'000));

	std::vector<std::pair<std::string, uint64_t>> eagle_state;
	eagle_state.emplace_back(std::make_pair<std::string, uint64_t>("eagle", 1'000'000'000));
	eagle_state.emplace_back(std::make_pair<std::string, uint64_t>("dead_eagle", 1'000'000'000));

	animated_sprite->InsertState("water_state", water_state);
	animated_sprite->InsertState("eagle_state", eagle_state);

	animated_sprite->SetState("water_state");

	default_shader_program->Use();
	default_shader_program->SetInt("tex", 0);

	glm::mat4 model_matrix_1 = glm::mat4(1.f);
	model_matrix_1 = glm::translate(model_matrix_1, glm::vec3(50.f, 50.f, 0.f));

	glm::mat4 model_matrix_2 = glm::mat4(1.f);
	model_matrix_2 = glm::translate(model_matrix_2, glm::vec3(590.f, 50.f, 0.f));

	glm::mat4 pojection_matrix = glm::ortho(0.f, static_cast<float>(window_size_.x),
											0.f, static_cast<float>(window_size_.y),
											-100.f, 100.f);

	default_shader_program->SetMatrix4("projection_matrix", pojection_matrix);

	sprite_shader_program->Use();
	sprite_shader_program->SetInt("tex", 0);
	sprite_shader_program->SetMatrix4("projection_matrix", pojection_matrix);

	return true;
}