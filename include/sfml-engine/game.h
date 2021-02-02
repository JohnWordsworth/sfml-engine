#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <unordered_map>

#include "sfml-engine/scene.h"


namespace gbh 
{
    /**
     Game Singleton that can be accessed from anywhere in your code using Game::getInstance()
     Handles the running of an sfml-engine game in a single SFML Window.
     You should add Scene objects to the game and then change scenes using Game::getInstance().changeScene("name");
     */
	class Game
	{
	public:
        
        /** Get the Game Singleton instance from anywhere in your project. */
		static Game& getInstance();
        
        /** Called from your main() method to run the game. The Game class will then handle inputs and execution until it exists. */
		void run(const std::string& firstScene);

        /** Add a Scene to the game with the given name. Note, that this will not run the scene - merely register it so that you can use it later. */
		void addScene(const std::string& name, std::shared_ptr<gbh::Scene> scene);
        
        /** Change which scene is currently active in the game. At the end of the current frame, this will hide the current scene and show the named scene. */
		void changeScene(const std::string& name);
        
        /** Get a shared pointer to the current scene that is active. */
        const std::shared_ptr<gbh::Scene>& getCurrentScene() const;

        /** Get a raw pointer to the underlying SFML RenderWindow object, in case you need to do manual operations with it. */
        sf::RenderWindow* getRenderWindow() { return m_window.get(); }
        
        /** Get a raw pointer to the underlying SFML RenderTarget that represents the buffer attached to the game window. */
        sf::RenderTarget* getRenderTarget() { return m_window.get(); }
        
        /** Get the size of the content area in the Window (1280x720 by default). */
		sf::Vector2i getWindowSize();
        
        /** A replacement for sf::Keyboard::isKeyPressed for macOS users who are having problems accessing keyboard commands. */
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
