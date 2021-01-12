#include "sfml-engine/scene.h"
#include "sfml-engine/game.h"

#include <iostream>



gbh::Scene::Scene()
{
	m_rootNode.setScene(this);
}


gbh::Scene::~Scene()
{
	destroy();
}


void gbh::Scene::initialize()
{
	if (!m_initialized)
	{
		m_initialized = true;
		onInitializeScene();
	}
}


void gbh::Scene::show()
{
	initialize();
	onShowScene();
}


void gbh::Scene::hide()
{
	onHideScene();
}


void gbh::Scene::destroy()
{
	if (m_initialized)
	{
		m_initialized = false;
		onDestroyScene();
	}
}


void gbh::Scene::setCamera(std::shared_ptr<CameraNode> cameraNode)
{
	m_camera = cameraNode;
}


std::shared_ptr<gbh::CameraNode> gbh::Scene::getCamera()
{
	return m_camera;
}


sf::Transform gbh::Scene::getCameraTransform() const
{
	if (m_camera == nullptr)
	{
		return sf::Transform::Identity;
	}

	sf::Vector2i windowSize = Game::getInstance().getWindowSize();
	sf::Transform screen = sf::Transform().translate(-windowSize.x * 0.5f, -windowSize.y * 0.5f);
	sf::Transform invScreen = sf::Transform().translate(windowSize.x * 0.5f, windowSize.y * 0.5f);
	sf::Transform camera = m_camera->getTransform();

	sf::Transform transform = invScreen * camera * screen;
	return transform;
}


void gbh::Scene::update(sf::Time deltaTime)
{
	m_rootNode.update(deltaTime);
}


void gbh::Scene::draw(sf::RenderTarget& target) const
{
	if (m_camera == nullptr) 
	{
		m_rootNode.draw(target, sf::Transform());
		return;
	}

	sf::Transform transform = getCameraTransform().getInverse();
	m_rootNode.draw(target, transform);
}


void gbh::Scene::addChild(const std::shared_ptr<gbh::Node>& node)
{
	m_rootNode.addChild(node);
}


std::shared_ptr<gbh::Node> gbh::Scene::getNodeAtViewPoint(const sf::Vector2f& point)
{
	sf::Transform transform = getCameraTransform();
	sf::Vector2f scenePoint = transform.transformPoint(point);
	return m_rootNode.getNodeAtPoint(scenePoint);
}


std::shared_ptr<gbh::Node> gbh::Scene::getNodeAtViewPoint(float x, float y)
{
	return getNodeAtViewPoint(sf::Vector2f(x, y));
}


void gbh::Scene::onUpdate(const sf::Time& deltaTime) { }
void gbh::Scene::onDraw(sf::RenderTarget& target, const sf::Transform& transform) const { }

void gbh::Scene::onInitializeScene() { }
void gbh::Scene::onShowScene() { }
void gbh::Scene::onHideScene() { }
void gbh::Scene::onDestroyScene() { }

void gbh::Scene::onKeyboardEvent(sf::Event& event) { }
void gbh::Scene::onMouseEvent(sf::Event& event) { }
void gbh::Scene::onJoystickEvent(sf::Event& event) { }
