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


void gbh::Scene::update(double deltaTime)
{
    onUpdate(deltaTime);
    simulatePhysics(deltaTime);
	m_rootNode.update(deltaTime);
}


void gbh::Scene::draw(sf::RenderTarget& target) const
{
    sf::Transform rootTransform;
    
	if (m_camera != nullptr)
	{
        rootTransform = getCameraTransform().getInverse();
	}

	m_rootNode.draw(target, rootTransform);
    
    if (m_physicsWorld && m_drawPhysicsDebug)
    {
        m_physicsDebug->PreDraw();
        m_physicsWorld->getBoxWorld()->DebugDraw();
    }
}


void gbh::Scene::addChild(const std::shared_ptr<gbh::Node>& node)
{
    return m_rootNode.addChild(node);
}


int gbh::Scene::getChildCount() const
{
    return m_rootNode.getChildCount();
}


std::shared_ptr<gbh::Node> gbh::Scene::getChildAtIndex(int index)
{
    return m_rootNode.getChildAtIndex(index);
}


std::shared_ptr<gbh::Node> gbh::Scene::getFirstChildWithName(const std::string& name, bool recursive)
{
    return m_rootNode.getFirstChildWithName(name, recursive);
}


void gbh::Scene::removeChild(int index, bool immediate)
{
    return m_rootNode.removeChild(index, immediate);
}


void gbh::Scene::removeChildrenWithName(const std::string& name, bool immediate)
{
    return m_rootNode.removeChildrenWithName(name, immediate);
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


void gbh::Scene::createPhysicsWorld(const sf::Vector2f& gravity)
{
    m_physicsWorld = std::make_unique<PhysicsWorld>(gravity);
    m_physicsDebug = std::make_unique<SfmlBoxDebugDraw>(Game::getInstance().getRenderWindow(), this);
    m_physicsDebug->SetFlags(b2Draw::e_shapeBit | b2Draw::e_centerOfMassBit);
    m_physicsWorld->getBoxWorld()->SetDebugDraw(m_physicsDebug.get());
}


void gbh::Scene::simulatePhysics(double deltaTime)
{
    if (m_physicsWorld == nullptr) {
        return;
    }
    
    // For any objects that have had their position update, push those changes to the physics world
    m_rootNode.runAction(true, [](Node& node) {
        if (node.m_physicsBody == nullptr) {
            return;
        }
        
        if (node.m_physicsTransformDirty)
        {
            node.updatePhysicsTransform();
        }
    });
    
    // Run the physics simulation
    m_physicsWorld->simulate(deltaTime);
    
    // Udpate any objects that have a physics body attached
    m_rootNode.runAction(true, [](Node& node) {
        if (node.m_physicsBody == nullptr) {
            return;
        }
        
        node.updateTransformFromPhysics();
    });
}


void gbh::Scene::onUpdate(double deltaTime) { }
void gbh::Scene::onDraw(sf::RenderTarget& target, const sf::Transform& transform) const { }

void gbh::Scene::onInitializeScene() { }
void gbh::Scene::onShowScene() { }
void gbh::Scene::onHideScene() { }
void gbh::Scene::onDestroyScene() { }

void gbh::Scene::onKeyboardEvent(sf::Event& event) { }
void gbh::Scene::onMouseEvent(sf::Event& event) { }
void gbh::Scene::onJoystickEvent(sf::Event& event) { }
