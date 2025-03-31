#pragma once
#include "network/client/clientInput.h"
#include "nbt/nbtSerializer.h"


template <typename eventType>
constexpr void serializeEventData(sf::Event& event, nbtSerializer& s) {
	//copy event data to content
	eventType content = s.write ? *event.getIf<eventType>() : eventType();
	if constexpr (std::is_same_v<eventType, sf::Event::TouchBegan>) {
		s.serializeValue(L"x", content.position.x);
		s.serializeValue(L"y", content.position.y);
		s.serializeValue(L"finger", content.finger);
	}
	else if constexpr (std::is_same_v<eventType, sf::Event::MouseButtonPressed> || std::is_same_v<eventType, sf::Event::MouseButtonReleased>) {
		s.serializeValue(L"x", content.position.x);
		s.serializeValue(L"y", content.position.y);
		s.serializeValue(L"button", content.button);
	}
	else if constexpr (std::is_same_v<eventType, sf::Event::MouseMoved>) {
		s.serializeValue(L"x", content.position.x);
		s.serializeValue(L"y", content.position.y);
	}

	else if constexpr (std::is_same_v<eventType, sf::Event::KeyPressed> || std::is_same_v<eventType, sf::Event::KeyReleased>) {
		//we don'T need to serialize the scan code, that is the local key code.
		//when reading this data on another device, the result would be completely different.
		s.serializeValue(L"alt", content.alt);
		s.serializeValue(L"control", content.control);
		s.serializeValue(L"shift", content.shift);
		s.serializeValue(L"code", content.code);
	}

	else if constexpr (std::is_same_v<eventType, sf::Event::SensorChanged>) {
		s.serializeValue(L"x", content.value.x);
		s.serializeValue(L"y", content.value.y);
		s.serializeValue(L"z", content.value.z);
	}
	else if constexpr (std::is_same_v<eventType, sf::Event::MouseWheelScrolled>) {
		s.serializeValue(L"x", content.position.x);
		s.serializeValue(L"y", content.position.y);
		s.serializeValue(L"delta", content.delta);
		s.serializeValue(L"wheel", content.wheel);
	}
	else if constexpr (std::is_same_v<eventType, sf::Event::TextEntered>) {

		s.serializeValue(L"unicode", content.unicode);
	}
	else if constexpr (std::is_same_v<eventType, sf::Event::Resized>) {
		s.serializeValue(L"width", content.size.x);
		s.serializeValue(L"height", content.size.y);
	}
	if (!s.write) {
		//copy content to event data
		event = sf::Event(content);
	}
}

inline bool serializeClientInput(clientInput& input, nbtSerializer& s)
{
	if (s.push(L"events")) {
		size_t size = input.eventHistory.size();
		s.serializeValue(L"size", size);
		if (!s.write)
		{
			//don'T resize to target size, because we might not be able to serialize all events
			input.eventHistory.clear();
		}
		for (size_t i = 0; i < size; i++)
		{
			sf::Event currentEvent = s.write ? input.eventHistory[i] : sf::Event(sf::Event::Closed());
			std::wstring typeName = s.write ? std::wstring(
				currentEvent.is<sf::Event::TouchMoved>() ? L"TouchMoved" :
				currentEvent.is<sf::Event::TouchBegan>() ? L"TouchBegan" :
				currentEvent.is<sf::Event::TouchEnded>() ? L"TouchEnded" :
				currentEvent.is<sf::Event::MouseButtonPressed>() ? L"MouseButtonPressed" :
				currentEvent.is<sf::Event::MouseButtonReleased>() ? L"MouseButtonReleased" :
				currentEvent.is<sf::Event::MouseMoved>() ? L"MouseMoved" :
				currentEvent.is<sf::Event::KeyPressed>() ? L"KeyPressed" :
				currentEvent.is<sf::Event::KeyReleased>() ? L"KeyReleased" :
				currentEvent.is<sf::Event::SensorChanged>() ? L"SensorChanged" :
				currentEvent.is<sf::Event::MouseWheelScrolled>() ? L"MouseWheelScrolled" :
				currentEvent.is<sf::Event::TextEntered>() ? L"TextEntered" :
				currentEvent.is<sf::Event::Resized>() ? L"Resized" :
				currentEvent.is<sf::Event::MouseEntered>() ? L"MouseEntered" :
				currentEvent.is<sf::Event::MouseLeft>() ? L"MouseLeft" :
				currentEvent.is<sf::Event::Resized>() ? L"Resized" :
				currentEvent.is<sf::Event::Closed>() ? L"Closed" :
				currentEvent.is<sf::Event::FocusLost>() ? L"FocusLost" :
				currentEvent.is<sf::Event::FocusGained>() ? L"FocusGained" :
				L"Unknown") : L"";
			if (typeName != L"Unknown" && s.push(std::to_wstring(i)))
			{

				s.serializeValue(std::wstring(L"type"), typeName);

				if (typeName == L"TouchMoved")				serializeEventData<sf::Event::TouchMoved>(currentEvent, s);
				else if (typeName == L"TouchBegan")			serializeEventData<sf::Event::TouchBegan>(currentEvent, s);
				else if (typeName == L"TouchEnded")			serializeEventData<sf::Event::TouchEnded>(currentEvent, s);
				else if (typeName == L"MouseButtonPressed") serializeEventData<sf::Event::MouseButtonPressed>(currentEvent, s);
				else if (typeName == L"MouseButtonReleased")serializeEventData<sf::Event::MouseButtonReleased>(currentEvent, s);
				else if (typeName == L"MouseMoved")			serializeEventData<sf::Event::MouseMoved>(currentEvent, s);
				else if (typeName == L"KeyPressed")			serializeEventData<sf::Event::KeyPressed>(currentEvent, s);
				else if (typeName == L"KeyReleased")		serializeEventData<sf::Event::KeyReleased>(currentEvent, s);
				else if (typeName == L"SensorChanged")		serializeEventData<sf::Event::SensorChanged>(currentEvent, s);
				else if (typeName == L"MouseWheelScrolled") serializeEventData<sf::Event::MouseWheelScrolled>(currentEvent, s);
				else if (typeName == L"TextEntered")		serializeEventData<sf::Event::TextEntered>(currentEvent, s);
				else if (typeName == L"Resized")			serializeEventData<sf::Event::Resized>(currentEvent, s);
				else if (typeName == L"MouseEntered")		serializeEventData<sf::Event::MouseEntered>(currentEvent, s);
				else if (typeName == L"MouseLeft")			serializeEventData<sf::Event::MouseLeft>(currentEvent, s);
				else if (typeName == L"Resized")			serializeEventData<sf::Event::Resized>(currentEvent, s);
				else if (typeName == L"Closed")				serializeEventData<sf::Event::Closed>(currentEvent, s);
				else if (typeName == L"FocusLost")			serializeEventData<sf::Event::FocusLost>(currentEvent, s);
				else if (typeName == L"FocusGained")		serializeEventData<sf::Event::FocusGained>(currentEvent, s);
				if (!s.write)input.eventHistory.push_back(currentEvent);
				s.pop();
			}
		}
		s.pop();
	}
	serializeNBTValue(s, L"mouse position", input.mousePositionPixels);
	if (s.push(L"mouse buttons"))
	{
		s.serializeArray(L"clicked", (sbyte*)input.clicked, (size_t)sf::Mouse::ButtonCount);
		s.serializeArray(L"released", (sbyte*)input.clickReleased, (size_t)sf::Mouse::ButtonCount);
		s.serializeArray(L"holding", (sbyte*)input.holding, (size_t)sf::Mouse::ButtonCount);
		s.serializeValue(L"scrolldelta", input.scrollDelta);
		s.pop();
	}
	// s.serialize(keyDownHistory);
	if (s.push(L"keys"))
	{
		serializeNBTValue(s, L"down", input.keysDown);
		serializeNBTValue(s, L"up", input.keysUp);
		serializeNBTValue(s, L"holding", input.keysHolding);
		s.pop();
	}
	s.serializeValue(L"text entered", input.textEntered);

	return true;
}