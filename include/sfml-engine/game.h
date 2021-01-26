#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <unordered_map>

#include "sfml-engine/scene.h"


namespace gbh 
{
	/// <summary>
	/// Encapsulates a game that runs in a single SFML window.
	/// </summary>
	class Game 
	{
	public:
		static Game& getInstance();
		void run(const std::string& firstScene);

		void addScene(const std::string& name, std::shared_ptr<gbh::Scene> scene);
		void changeScene(const std::string& name);
        const std::shared_ptr<gbh::Scene>& getCurrentScene() const;

        sf::RenderWindow* getRenderWindow() { return m_window.get(); }
        sf::RenderTarget* getRenderTarget() { return m_window.get(); } 
		sf::Vector2i getWindowSize();
        
        // This is a hack as the keyboard doesn't seem to work on macOS Big Sur under some circumstances
        bool isKeyPressed(sf::Keyboard::Key key);

	private:
		Game() { };

        void updateKeyboardState(sf::Event& event);

        
		std::unique_ptr<sf::RenderWindow> m_window;
		sf::Vector2i m_windowSize = sf::Vector2i(1280, 720);

		std::shared_ptr<gbh::Scene> m_currentScene;
		std::shared_ptr<gbh::Scene> m_nextScene;

		std::unordered_map<std::string, std::shared_ptr<gbh::Scene>> m_scenes;
		bool m_changeScene = false;
        
        std::unordered_map<sf::Keyboard::Key, bool> m_keysPressed;
	};

} // namespace 
