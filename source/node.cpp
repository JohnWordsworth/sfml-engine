#include "sfml-engine/node.h"

#include "sfml-engine/mathutils.h"
#include "sfml-engine/physics/physicsworld.h"



gbh::Node::~Node() 
{

}


void gbh::Node::addChild(std::shared_ptr<Node> node)
{
	m_children.push_back(node);
	node->m_parent = this;
	node->setScene(m_scene);
    node->updateAbsoluteOrigin();
}


int gbh::Node::getChildCount() const
{
    return (int)m_children.size();
}


std::shared_ptr<gbh::Node> gbh::Node::getChildAtIndex(int index)
{
    if (index >= 0 && index < m_children.size())
    {
        return m_children[index];
    }
    
    return nullptr;
}


std::shared_ptr<gbh::Node> gbh::Node::getFirstChildWithName(const std::string& name, bool recursive)
{
    for(int i = 0; i < m_children.size(); ++i)
    {
        if (m_children[i]->getName() == name)
        {
            return m_children[i];
        }
    }
    
    if (recursive)
    {
        for(int i = 0; i < m_children.size(); ++i)
        {
            std::shared_ptr<gbh::Node> result = m_children[i]->getFirstChildWithName(name, recursive);
            
            if (result)
            {
                return result;
            }
        }
    }
    
    return nullptr;
}


void gbh::Node::removeChild(int index, bool immediate)
{
    if (immediate)
    {
        m_children.erase(m_children.begin() + index);
    }
    else
    {
        m_children[index]->m_removeInNextUpdate = true;
    }
}


void gbh::Node::removeChildrenWithName(const std::string& name, bool immediate)
{
    std::vector<int> itemsToRemove;
    
    for(int i = 0; i < m_children.size(); ++i)
    {
        if (m_children[i]->getName() == name)
        {
            if (immediate)
            {
                itemsToRemove.push_back(i);
            }
            else
            {
                m_children[i]->m_removeInNextUpdate = true;
            }
        }
    }
    
    // Remove from back, so that all of the indices are still valid
    for(int i = (int)itemsToRemove.size() - 1; i >= 0; --i)
    {
        m_children.erase(m_children.begin() + i);
    }
}


void gbh::Node::removeAllChildren(bool immediate)
{
    if (immediate)
    {
        m_children.clear();
    }
    else
    {
        for(int i = 0; i < m_children.size(); ++i)
        {
            m_children[i]->m_removeInNextUpdate = true;
        }
    }
}


void gbh::Node::removeFromParent(bool immediate)
{
    if (immediate)
    {
        if (m_parent != nullptr)
        {
            for(int i = 0; i < m_parent->m_children.size(); ++i)
            {
                if (m_parent->m_children[i].get() == this)
                {
                    m_parent->m_children.erase(m_parent->m_children.begin() + i);
                }
            }
        }
    }
    else
    {
        m_removeInNextUpdate = false;
    }
}



void gbh::Node::setName(const std::string& name)
{
	m_name = name;
}


const std::string& gbh::Node::getName() const
{
	return m_name;
}


void gbh::Node::setPosition(const sf::Vector2f& position)
{
    Transformable::setPosition(position);
    m_physicsTransformDirty = true;
}


void gbh::Node::setPosition(float x, float y)
{
    Transformable::setPosition(x, y);
    m_physicsTransformDirty = true;
}


void gbh::Node::setRotation(float angle)
{
    Transformable::setRotation(angle);
    m_physicsTransformDirty = true;
}


void gbh::Node::move(const sf::Vector2f& offset)
{
    Transformable::move(offset);
    m_physicsTransformDirty = true;
}


void gbh::Node::move(float x, float y)
{
    Transformable::move(x, y);
    m_physicsTransformDirty = true;
}


void gbh::Node::rotate(float angle)
{
    Transformable::rotate(angle);
    m_physicsTransformDirty = true;
}


void gbh::Node::setScale(const sf::Vector2f& factors)
{
    Transformable::setScale(factors);
    updateAbsoluteOrigin();
}


void gbh::Node::setScale(float x, float y)
{
    Transformable::setScale(x, y);
    updateAbsoluteOrigin();
}


void gbh::Node::scale(const sf::Vector2f& factors)
{
    Transformable::scale(factors);
    updateAbsoluteOrigin();
}


void gbh::Node::scale(float x, float y)
{
    Transformable::scale(x, y);
    updateAbsoluteOrigin();
}


void gbh::Node::setOrigin(float x, float y)
{
    m_relativeOrigin = sf::Vector2f(x, y);
    updateAbsoluteOrigin();
}


void gbh::Node::setOrigin(const sf::Vector2f& origin)
{
    m_relativeOrigin = origin;
    updateAbsoluteOrigin();
}


const sf::Vector2f& gbh::Node::getOrigin() const
{
    return m_relativeOrigin;
}


gbh::Scene* gbh::Node::getScene()
{
	return m_scene;
}


void gbh::Node::setPhysicsBody(const std::shared_ptr<PhysicsBody>& body)
{
    if (m_physicsBody != nullptr)
    {
        m_physicsBody->setNode(nullptr);
    }
    
    m_physicsBody = body;
    m_physicsBody->setNode(this);
    
    m_physicsTransformDirty = true;
}


const std::shared_ptr<gbh::PhysicsBody>& gbh::Node::getPhysicsBody()
{
    return m_physicsBody;
}


void gbh::Node::beginContact(const gbh::PhysicsContact& contact)
{
    onBeginContact(contact);
    
    if (m_beginContactCallback)
    {
        m_beginContactCallback(contact);
    }
}


void gbh::Node::endContact(const gbh::PhysicsContact& contact)
{
    onEndContact(contact);
    
    if (m_endContactCallback)
    {
        m_endContactCallback(contact);
    }
}


void gbh::Node::setBeginContactCallback(const std::function<void(const gbh::PhysicsContact& contact)>& func)
{
    m_beginContactCallback = func;
}


void gbh::Node::setEndContactCallback(const std::function<void(const gbh::PhysicsContact& contact)>& func)
{
    m_endContactCallback = func;
}


sf::FloatRect gbh::Node::getLocalBounds() const
{
	return sf::FloatRect();
}


sf::FloatRect gbh::Node::getGlobalBounds() const
{
	sf::Transform globalTransform = getGlobalTransform();
	return globalTransform.transformRect(getLocalBounds());
}


sf::Transform gbh::Node::getGlobalTransform() const
{
    // Expensive - should probably cache these
    
    std::vector<sf::Transform> transforms;
	const Node* node = this;

	while (node != nullptr) {
		transforms.push_back(node->getTransform());
		node = node->m_parent;
	}

	sf::Transform transform;

	for (int i = (int)transforms.size() - 1; i >= 0; --i) {
		transform *= transforms[i];
	}

	return transform;
}


std::shared_ptr<gbh::Node> gbh::Node::getNodeAtPoint(const sf::Vector2f& point)
{
	gbh::Node* node = this;
	sf::Transform parentTransform = sf::Transform();
	bool searching = true;

	while (searching) 
	{
		searching = false;

		// Start from the last entry, as we often throw a bunch of nodes on one layer and use 
		// the fact that they render "bottom to top" to layer our graphics. i.e. This will 
		// more likely match the expectations of the user.

		for (int i = (int)node->m_children.size() - 1; i >= 0; --i)
		{
			sf::Transform transform = node->m_children[i]->getTransform() * parentTransform;
			sf::FloatRect bounds = transform.transformRect(node->m_children[i]->getLocalBounds());

			if (bounds.contains(point))
			{
				node = node->m_children[i].get();
				parentTransform = transform;
				searching = true;
				break;
			}
		}
	}

	if (node == this) {
		return nullptr;
	}

	return node->shared_from_this();
}


std::shared_ptr<gbh::Node> gbh::Node::getNodeAtPoint(float x, float y)
{
	return getNodeAtPoint(sf::Vector2f(x, y));
}


sf::Vector2f gbh::Node::forwardVector() const
{
    float angle = gbh::math::degreesToRadians(getRotation());
    sf::Vector2f result;
    
    result.x = -sin(angle);
    result.y = cos(angle);

    return result;
}


sf::Vector2f gbh::Node::backwardVector() const
{
    float angle = gbh::math::degreesToRadians(getRotation() + 180.0f);
    sf::Vector2f result;
    
    result.x = -sin(angle);
    result.y = cos(angle);

    return result;
}


sf::Vector2f gbh::Node::leftVector() const
{
    float angle = gbh::math::degreesToRadians(getRotation() - 90.0f);
    sf::Vector2f result;
    
    result.x = -sin(angle);
    result.y = cos(angle);

    return result;
}


sf::Vector2f gbh::Node::rightVector() const
{
    float angle = gbh::math::degreesToRadians(getRotation() + 90.0f);
    sf::Vector2f result;
    
    result.x = -sin(angle);
    result.y = cos(angle);

    return result;
}


void gbh::Node::runAction(bool recursive, std::function<void (Node&)> action)
{
    action(*this);
    
    if (recursive)
    {
        for(int i = 0; i < m_children.size(); ++i)
        {
            m_children[i]->runAction(recursive, action);
        }
    }
}


void gbh::Node::update(double deltaTime)
{
    // Remove all nodes marked for deletion
    for(int i = (int)m_children.size() - 1; i >= 0; --i)
    {
        if (m_children[i]->m_removeInNextUpdate)
        {
            m_children.erase(m_children.begin() + i);
        }
    }
    
    // Call the 'onUpdate' virtual method
	onUpdate(deltaTime);
	
    // Update all children
	for(std::size_t i = 0; i < m_children.size(); ++i) {
		m_children[i]->update(deltaTime);
	}
}


void gbh::Node::draw(sf::RenderTarget &target, const sf::Transform &parentTransform) const
{
	sf::Transform combinedTransform = parentTransform * getTransform();
	onDraw(target, combinedTransform);
	
	for(std::size_t i = 0; i < m_children.size(); ++i) {
		m_children[i]->draw(target, combinedTransform);
	}
}


void gbh::Node::updatePhysicsTransform()
{
    sf::Transform transform = m_parent->getGlobalTransform();
    sf::Vector2f globalPosition = transform.transformPoint(getPosition());
    globalPosition.y = -globalPosition.y;
    
    m_physicsBody->setTransform(globalPosition, getRotation());
    m_physicsTransformDirty = false;
}


void gbh::Node::updateTransformFromPhysics()
{
    if (m_physicsBody)
    {
        sf::Transform transform = m_parent->getGlobalTransform().getInverse();
        
        sf::Vector2f globalPosition = m_physicsBody->getPosition();
        globalPosition.y = -globalPosition.y;
        sf::Vector2f localPosition = transform.transformPoint(globalPosition);
        
        Transformable::setPosition(localPosition);
        Transformable::setRotation(m_physicsBody->getAngle());
    }
}


void gbh::Node::setScene(gbh::Scene* scene)
{
	m_scene = scene;

	for (size_t i = 0; i < m_children.size(); ++i)
	{
		m_children[i]->setScene(scene);
	}
}


void gbh::Node::onUpdate(double deltaTime)
{
	// Subclasses of the base node should use this to update themselves
}


void gbh::Node::onDraw(sf::RenderTarget& target, const sf::Transform& transform) const
{
	// Subclasses of the base node use this to draw their contents
}


void gbh::Node::onBeginContact(const PhysicsContact& contact)
{
    // Subclasses of the base node can use this to detect collisions
}


void gbh::Node::onEndContact(const PhysicsContact &contact)
{
    // Subclasses of the base node can use this to detect collisions
}


void gbh::Node::updateAbsoluteOrigin()
{
    sf::FloatRect bounds = getLocalBounds();
    sf::Vector2f absoluteOrigin = sf::Vector2f(bounds.left + (m_relativeOrigin.x * bounds.width), bounds.top + (m_relativeOrigin.y * bounds.height));
    sf::Transformable::setOrigin(absoluteOrigin);
}
