#include "sfml-engine/game.h"

#include <iostream>


gbh::Game& gbh::Game::getInstance()
{
	static Game instance;
	return instance;
}


void gbh::Game::run(const std::string& firstScene)
{
	changeScene(firstScene);
	m_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(m_windowSize.x, m_windowSize.y), "SFML Engine Game");
	m_window->setVerticalSyncEnabled(true);

    sf::Clock clock;
    sf::Event event;

	while (m_window->isOpen())
	{
		while (m_window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				m_window->close();
			}

			if (m_currentScene)
			{
				if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
				{
                    updateKeyboardState(event);
					m_currentScene->onKeyboardEvent(event);
				}

				if (event.type == sf::Event::JoystickMoved || event.type == sf::Event::JoystickButtonPressed || event.type == sf::Event::JoystickButtonReleased)
				{
					m_currentScene->onJoystickEvent(event);
				}

				if (event.type == sf::Event::MouseMoved || event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased)
				{
					m_currentScene->onMouseEvent(event);
				}
			}
		}

		if (m_changeScene)
		{
			m_changeScene = false;

			if (m_currentScene != nullptr) {
				m_currentScene->hide();
			}

			m_currentScene = m_nextScene;
			m_nextScene = nullptr;

			if (m_currentScene != nullptr) {
				m_currentScene->show();
			}
		}

		m_window->clear();

		if (m_currentScene == nullptr)
		{
			std::cout << "[gbh::Game] No scene is set. Ending game.\n";
			return;
		}

		sf::Time elapsedTime = clock.restart();
        double deltaTime = elapsedTime.asSeconds();
        
        // Limit delta time to 0.25s - incase the application stalls for a while
        deltaTime = (deltaTime > 0.25f) ? 0.25f : deltaTime;

        m_currentScene->update(elapsedTime.asSeconds());
		m_currentScene->draw(*m_window);

		m_window->display();
	}
}


void gbh::Game::addScene(const std::string& name, std::shared_ptr<gbh::Scene> scene)
{
	m_scenes[name] = scene;
}


void gbh::Game::changeScene(const std::string& name)
{
	if (m_scenes.find(name) == m_scenes.end())
	{
		std::cout << "Scene with name " << name << " not found.\n";
		return;
	}

	m_nextScene = m_scenes[name];
	m_changeScene = true;
}


const std::shared_ptr<gbh::Scene>& gbh::Game::getCurrentScene() const
{
    return m_currentScene;
}


sf::Vector2i gbh::Game::getWindowSize() 
{
	return m_windowSize;
}


bool gbh::Game::isKeyPressed(sf::Keyboard::Key key)
{
    if (m_keysPressed.find(key) == m_keysPressed.end())
    {
        return false;
    }
    
    return m_keysPressed[key];
}


void gbh::Game::updateKeyboardState(sf::Event &event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        m_keysPressed[event.key.code] = true;
    }
    else
    {
        m_keysPressed[event.key.code] = false;
    }
}
