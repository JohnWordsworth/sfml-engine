#include "sfml-engine/game.h"

#include <SFML/Graphics.hpp>
#include <iostream>

#if defined(_WIN32)
#include <Windows.h>
#endif

class TestScene : public gbh::Scene
{
protected:
	virtual void onInitializeScene();
	virtual void onUpdate(const sf::Time& deltaTime);
	virtual void onMouseEvent(sf::Event& event);

private:
	std::shared_ptr<gbh::Node> m_selectedNode;
	float m_moveSpeed = 100.0f;

	sf::Font m_arialFont;

	std::shared_ptr<gbh::TextNode> m_instructionsText;
	std::shared_ptr<gbh::TextNode> m_selectedNodeText;
	std::shared_ptr<gbh::ShapeNode> m_circle;
	std::shared_ptr<gbh::ShapeNode> m_rectangle;
};


void TestScene::onInitializeScene()
{
	std::string arialFontPath;
	bool arialLoaded = false;

#if defined(_WIN32)
	arialFontPath = "%WINDIR%\\Fonts\\arial.ttf";

	char fullPath[512];
	ExpandEnvironmentStrings(arialFontPath.c_str(), fullPath, 512);
	arialFontPath = std::string(fullPath);
#endif

	if (!arialFontPath.empty())
	{
		arialLoaded = m_arialFont.loadFromFile(arialFontPath);
	}

	m_rectangle = std::make_shared<gbh::ShapeNode>(sf::RectangleShape(sf::Vector2f(400.f, 400.f)));
	m_rectangle->getShape()->setFillColor(sf::Color::Red);
	m_rectangle->setPosition(10.f, 100.f);
	m_rectangle->setName("Rectangle");
	addChild(m_rectangle);

	m_circle = std::make_shared<gbh::ShapeNode>(sf::CircleShape(100.0f));
	m_circle->getShape()->setFillColor(sf::Color::Blue);
	m_circle->setPosition(10.f, 10.f);
	m_circle->setName("Circle");
	m_rectangle->addChild(m_circle);

	if (arialLoaded)
	{
		m_instructionsText = std::make_shared<gbh::TextNode>("Click on a node to select it and use W,A,S,D to move it around the screen.", m_arialFont, 30);
		m_instructionsText->setPosition(10.f, 10.f);
		addChild(m_instructionsText);
	}
}


void TestScene::onUpdate(const sf::Time& deltaTime)
{
	if (m_selectedNode)
	{
		sf::Vector2f positionDelta;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			positionDelta.x = -1.0f;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			positionDelta.x = 1.0f;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			positionDelta.y = -1.0f;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			positionDelta.y = 1.0f;
		}

		positionDelta *= (m_moveSpeed * deltaTime.asSeconds());
		m_selectedNode->move(positionDelta);
	}
}


void TestScene::onMouseEvent(sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed)
	{
		m_selectedNode = getNodeAtViewPoint((float)event.mouseButton.x, (float)event.mouseButton.y);

		if (m_selectedNode != nullptr) {
			std::cout << "Selected Node: " << m_selectedNode->getName() << "\n";
		}
		else {
			std::cout << "No Node Selected\n";
		}
	}
}


int main()
{
	gbh::Game& game = gbh::Game::getInstance();
	game.addScene("test", std::make_shared<TestScene>());
	game.run("test");

	return 0;
}
