#include "eventTranslator.h"

void eventTranslator::processEvent(const sf::Event &event)
{
    auto processMouseDown = [this](cveci2 &mousePos, cmb &button)
    {
        receiver.mouseDown(
            mousePos,
            button);
        dragStartPositions[button] = mousePos;
    };
    auto processMouseMove = [this](cveci2 &mousePos, cmb &button)
    {
        receiver.mouseMove(
            mousePos,
            button);
        receiver.drag(dragStartPositions[button], mousePos, button);
    };
    auto processMouseUp = [this](cveci2 &mousePos, cmb &button)
    {
        receiver.mouseUp(
            mousePos,
            button);
        receiver.drop(dragStartPositions[button], mousePos, button);
        dragStartPositions.erase(button);
    };
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
    {
        receiver.keyDown(keyPressed->code);
    }
    else if (const auto* keyReleased = event.getIf<sf::Event::KeyReleased>())
    {
        receiver.keyUp(keyReleased->code);
    }
    else if (const auto* textEntered = event.getIf<sf::Event::TextEntered>())
    {
        receiver.enterText(textEntered->unicode);
    }
    else if (const auto* touchBegan = event.getIf<sf::Event::TouchBegan>())
    {
        processMouseDown(veci2(touchBegan->position.x, touchBegan->position.y), (mb)touchBegan->finger);
    }
    else if (const auto* mouseButtonPressed = event.getIf<sf::Event::MouseButtonPressed>())
    {
        processMouseDown(veci2(mouseButtonPressed->position.x, mouseButtonPressed->position.y), (mb)mouseButtonPressed->button);
    }
    else if (const auto* touchMoved = event.getIf<sf::Event::TouchMoved>())
    {
        processMouseMove(veci2(touchMoved->position.x, touchMoved->position.y), (mb)touchMoved->finger);
    }
    else if (const auto* MouseMoved = event.getIf<sf::Event::MouseMoved>())
    {
        processMouseMove(cveci2(MouseMoved->position.x, MouseMoved->position.y), noButton);
    }
    else if (const auto* touchEnded = event.getIf<sf::Event::TouchEnded>())
    {
        processMouseUp(
            cveci2(touchEnded->position.x, touchEnded->position.y),
            (mb)touchEnded->finger);
    }

    else if (const auto* mouseButtonReleased = event.getIf<sf::Event::MouseButtonReleased>())
    {
        processMouseUp(
            cveci2(mouseButtonReleased->position.x, mouseButtonReleased->position.y),
            (mb)mouseButtonReleased->button);
    }
    else if (const auto* MouseWheelScrolled = event.getIf<sf::Event::MouseWheelScrolled>())
    {
        receiver.scroll(cveci2(MouseWheelScrolled->position.x, MouseWheelScrolled->position.y),
                        (int)MouseWheelScrolled->delta);
    }
    else if (const auto* Resized = event.getIf<sf::Event::Resized>())
    {
        receiver.layout(rectanglei2(veci2(), veci2(Resized->size.x, Resized->size.y)));
    }
    else if (event.is<sf::Event::FocusLost>())
    {
        receiver.lostFocus();
    }
    else if (event.is<sf::Event::FocusGained>())
    {
        receiver.focus();
    }
}