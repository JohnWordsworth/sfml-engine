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

		sf::Vector2i getWindowSize();

	private:
		Game() { };

		std::unique_ptr<sf::RenderWindow> m_window;
		sf::Vector2i m_windowSize = sf::Vector2i(1280, 720);

		std::shared_ptr<gbh::Scene> m_currentScene;
		std::shared_ptr<gbh::Scene> m_nextScene;

		std::unordered_map<std::string, std::shared_ptr<gbh::Scene>> m_scenes;
		bool m_changeScene = false;
	};

} // namespace 