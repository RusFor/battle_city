#include "brick_wall.h"

#include "../../Resources/resource_manager.h"
#include "../../Renderer/sprite.h"

using namespace std::literals;

BrickWall::BrickWall(const Type brick_wall_type,
					 const glm::vec2& position,
					 const glm::vec2& size,
					 const float rotation)
	: GameObject(position, size, rotation)
	, current_state_{State::Destroyed,
					 State::Destroyed,
					 State::Destroyed,
					 State::Destroyed}{
	sprites_[static_cast<size_t>(State::All)] = ResourceManager::GetSprite("brick_all"s);
	sprites_[static_cast<size_t>(State::Top_left)] = ResourceManager::GetSprite("brick_top_left"s);
	sprites_[static_cast<size_t>(State::Top_right)] = ResourceManager::GetSprite("brick_top_right"s);
	sprites_[static_cast<size_t>(State::Top)] = ResourceManager::GetSprite("brick_top"s);
	sprites_[static_cast<size_t>(State::Bottom_left)] = ResourceManager::GetSprite("brick_bottom_left"s);
	sprites_[static_cast<size_t>(State::Left)] = ResourceManager::GetSprite("brick_left"s);
	sprites_[static_cast<size_t>(State::Top_right_bottom_left)] = ResourceManager::GetSprite("brick_top_right_bottom_left"s);
	sprites_[static_cast<size_t>(State::Top_bottom_left)] = ResourceManager::GetSprite("brick_top_bottom_left"s);
	sprites_[static_cast<size_t>(State::Bottom_right)] = ResourceManager::GetSprite("brick_bottom_right"s);
	sprites_[static_cast<size_t>(State::Top_left_bottom_right)] = ResourceManager::GetSprite("brick_top_left_bottom_right"s);
	sprites_[static_cast<size_t>(State::Right)] = ResourceManager::GetSprite("brick_right"s);
	sprites_[static_cast<size_t>(State::Top_bottom_right)] = ResourceManager::GetSprite("brick_top_bottom_right"s);
	sprites_[static_cast<size_t>(State::Bottom)] = ResourceManager::GetSprite("brick_bottom"s);
	sprites_[static_cast<size_t>(State::Top_left_bottom)] = ResourceManager::GetSprite("brick_top_left_bottom"s);
	sprites_[static_cast<size_t>(State::Top_right_bottom)] = ResourceManager::GetSprite("brick_top_right_bottom"s);

	switch(brick_wall_type){
	case Type::All:
		current_state_.fill(State::All);
		break;
	case Type::Top:
		current_state_[static_cast<size_t>(Location::TopLeft)] = State::All;
		current_state_[static_cast<size_t>(Location::TopRight)] = State::All;
		break;
	case Type::Bottom:
		current_state_[static_cast<size_t>(Location::BottomLeft)] = State::All;
		current_state_[static_cast<size_t>(Location::BottomRight)] = State::All;
		break;
	case Type::Left:
		current_state_[static_cast<size_t>(Location::TopLeft)] = State::All;
		current_state_[static_cast<size_t>(Location::BottomLeft)] = State::All;
		break;
	case Type::Right:
		current_state_[static_cast<size_t>(Location::TopRight)] = State::All;
		current_state_[static_cast<size_t>(Location::BottomRight)] = State::All;
		break;
	case Type::TopLeft:
		current_state_[static_cast<size_t>(Location::TopLeft)] = State::All;
		break;
	case Type::TopRight:
		current_state_[static_cast<size_t>(Location::TopRight)] = State::All;
		break;
	case Type::BottomLeft:
		current_state_[static_cast<size_t>(Location::BottomLeft)] = State::All;
		break;
	case Type::BottomRight:
		current_state_[static_cast<size_t>(Location::BottomRight)] = State::All;
		break;
	}
}

void BrickWall::RenderBlock(const BrickWall::Location location) const{
	static const std::array<glm::vec2, 4> offsets = {glm::vec2(0, size_.y / 2.f),
													 glm::vec2(size_.x / 2.f, size_.y / 2.f),
													 glm::vec2(0,0),
													 glm::vec2(size_.x / 2.f, 0)};

	const State state = current_state_[static_cast<size_t>(location)];
	if(state != State::Destroyed){
		sprites_[static_cast<size_t>(state)]->Render(position_ + offsets[static_cast<size_t>(location)], size_ / 2.f, rotation_);
	}
}

void BrickWall::Render() const{
	RenderBlock(Location::TopLeft);
	RenderBlock(Location::TopRight);
	RenderBlock(Location::BottomLeft);
	RenderBlock(Location::BottomRight);
}


void BrickWall::Update(const uint64_t delta){}
