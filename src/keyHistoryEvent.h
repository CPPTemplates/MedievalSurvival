#pragma once
struct keyHistoryEvent {
	vk key;
	bool down;//false = released
	keyHistoryEvent(cvk key = sf::Keyboard::Key::Unknown, cbool& down = false) : key(key), down(down) {}
};