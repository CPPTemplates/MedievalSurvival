#pragma once
#include <SFML/Window.hpp>
#include "constants.h"

static constexpr sf::Keyboard::Key hotbarSlotKeys[StandardInventoryColumnCount]
{
	sf::Keyboard::Key::Num1,
	sf::Keyboard::Key::Num2,
	sf::Keyboard::Key::Num3,
	sf::Keyboard::Key::Num4,
	sf::Keyboard::Key::Num5,
	sf::Keyboard::Key::Num6,
	sf::Keyboard::Key::Num7,
	sf::Keyboard::Key::C,
	sf::Keyboard::Key::F,
};

enum class keyID
{
	hotbarSlot0 = hotbarSlotKeys[0],
	hotbarSlot1 = hotbarSlotKeys[1],
	hotbarSlot2 = hotbarSlotKeys[2],
	hotbarSlot3 = hotbarSlotKeys[3],
	hotbarSlot4 = hotbarSlotKeys[4],
	hotbarSlot5 = hotbarSlotKeys[5],
	hotbarSlot6 = hotbarSlotKeys[6],
	hotbarSlot7 = hotbarSlotKeys[7],
	hotbarSlot8 = hotbarSlotKeys[8],
	left = sf::Keyboard::Key::A,
	right = sf::Keyboard::Key::D,
	up = sf::Keyboard::Key::W,
	down = sf::Keyboard::Key::S,
	inventory = sf::Keyboard::Key::E,
	drop = sf::Keyboard::Key::Q,
	camera = sf::Keyboard::Key::F,
	hit = sf::Mouse::Button::Left,
	use = sf::Mouse::Button::Right,
	jump = sf::Keyboard::Key::Space,
	sprint = sf::Keyboard::Key::LControl,
	sneak = sf::Keyboard::Key::LShift,
	exit = sneak,
	commandLine = sf::Keyboard::Key::Slash,//backslash or question mark
	text = sf::Keyboard::Key::T,
	headUpDisplay = sf::Keyboard::Key::F1,
	screenshot = sf::Keyboard::Key::F2,
	debug = sf::Keyboard::Key::F3,
	escape = sf::Keyboard::Key::Escape,
	renderHitboxes = sf::Keyboard::Key::B
};