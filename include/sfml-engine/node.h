#pragma once


#include "sfml-engine/physics/physicsbody.h"
#include "sfml-engine/physics/physicsworld.h"

#include <SFML/Graphics.hpp>
#include <functional>
#include <memory>
#include <string>
#include <vector>


namespace gbh 
{
	// Forward declare some classes we need to know exist, but don't need the details of
	class Scene;
	class Game;
	
	/// <summary>
	/// A scene is made up of a collection of nodes in a hierarchy. Each node can have it's own transformation in the parent space.
	/// Use specialized versions of the Node to add visual effects (SpriteNode for an image, TextNode for text, ShapeNode for an sf::Shape).
	/// </summary>
	class Node : public sf::Transformable, public std::enable_shared_from_this<Node>
	{
		friend class gbh::Game;
		friend class gbh::Scene;
	
	public:
		virtual ~Node();
        
		void addChild(std::shared_ptr<Node> node);
        int getChildCount() const;
        std::shared_ptr<Node> getChildAtIndex(int index);
        std::shared_ptr<Node> getFirstChildWithName(const std::string& name, bool recursive);
        void removeChild(int index, bool immediate);
        void removeChildrenWithName(const std::string& name, bool immediate);
        void removeFromParent(bool immediate);
        
		void setName(const std::string& name);
		const std::string& getName() const;
        
        void setPosition(const sf::Vector2f& position);
        void setPosition(float x, float y);
        void setRotation(float angle);
        
        void move(const sf::Vector2f& offset);
        void move(float x, float y);
        void rotate(float angle);
        
        void setScale(const sf::Vector2f& factors);
        void setScale(float x, float y);
        void scale(const sf::Vector2f& factors);
        void scale(float x, float y);
        
        void setOrigin(float x, float y);
        void setOrigin(const sf::Vector2f& origin);
        const sf::Vector2f& getOrigin() const;
        	
		Scene* getScene();

		void setPhysicsBody(const std::shared_ptr<PhysicsBody>& body);
		const std::shared_ptr<PhysicsBody>& getPhysicsBody();
        
        void beginContact(const PhysicsContact& contact);
        void endContact(const PhysicsContact& contact);
        
        void setBeginContactCallback(const std::function<void(const PhysicsContact& contact)>& func);
        void setEndContactCallback(const std::function<void(const PhysicsContact& contact)>& func);

		virtual sf::FloatRect getLocalBounds() const;
		sf::FloatRect getGlobalBounds() const;
		sf::Transform getGlobalTransform() const;

		// Returns the deepest descendant that intersects a point (does not check against this node).
		std::shared_ptr<Node> getNodeAtPoint(const sf::Vector2f& point);
		std::shared_ptr<Node> getNodeAtPoint(float x, float y);

        // Get vectors in relation to the facing of this sprite
        sf::Vector2f forwardVector() const;     // 'down' the screen or 'up' in the y-axis
        sf::Vector2f backwardVector() const;    // 'up' the screen or 'down' in the y-axis
        sf::Vector2f leftVector() const;
        sf::Vector2f rightVector() const;

        // Run an function on this node (and all of it's children if recursive is set to true).
        void runAction(bool recursive, std::function<void (Node&)> action);

	protected:
        void updatePhysicsTransform();
        void updateTransformFromPhysics();
        void updateAbsoluteOrigin();

		void setScene(Scene* scene);

		void update(double deltaTime);
		void draw(sf::RenderTarget& target, const sf::Transform& parentTransform) const;

		virtual void onUpdate(double deltaTime);
		virtual void onDraw(sf::RenderTarget& target, const sf::Transform& transform) const;
        virtual void onBeginContact(const PhysicsContact& contact);
        virtual void onEndContact(const PhysicsContact& contact);
        
	private:
		std::string m_name;
		Scene* m_scene = nullptr;
		Node* m_parent = nullptr;
        sf::Vector2f m_relativeOrigin = sf::Vector2f(0.5f, 0.5f);
		std::vector<std::shared_ptr<Node>> m_children;
        
        std::shared_ptr<PhysicsBody> m_physicsBody;
        std::function<void(const PhysicsContact& contact)> m_beginContactCallback = nullptr;
        std::function<void(const PhysicsContact& contact)> m_endContactCallback = nullptr;
        
        bool m_removeInNextUpdate = false;
        bool m_physicsTransformDirty = false;
	};

} // namespace
