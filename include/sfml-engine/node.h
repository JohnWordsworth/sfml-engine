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
	/** Forward declare some classes we need to know exist, but don't need the details of. */
	class Scene;
	class Game;
	
	/**
     A scene is made up of a collection of nodes in a hierarchy. Each node can have it's own transformation in the parent space.
     Use specialized versions of the Node to add visual effects (SpriteNode for an image, TextNode for text, ShapeNode for an sf::Shape).
	*/
	class Node : public sf::Transformable, public std::enable_shared_from_this<Node>
	{
		friend class gbh::Game;
		friend class gbh::Scene;
	
	public:
		virtual ~Node();
        
        
        // Scene Graph Methods
        
        /** Adds another node as a child of this node in the scene graph. The new node will be added as the last child, which will render on top of all other children. */
		void addChild(std::shared_ptr<Node> node);
        
        /** Gets the number of children '1-level deep' this node has (ignoring whether those nodes themselves have children). */
        int getChildCount() const;
        
        /** Returns the child node at the given index (or null if an invalid index was provided). */
        std::shared_ptr<Node> getChildAtIndex(int index);
        
        /** Returns the first child node in the scene graph with the given name. If recursive is true, then we also seach through children of children etc. */
        std::shared_ptr<Node> getFirstChildWithName(const std::string& name, bool recursive);
        
        /** Remove the child node at the given index. Unless immediate is manually set to true, this will actually occur as part of the next frame update instead of  immediately. */
        void removeChild(int index, bool immediate);
        
        /** Remove all children of this node with the given name. This is not recursive and does not search child nodes. */
        void removeChildrenWithName(const std::string& name, bool immediate);
        
        /** Removes this node from the parent node. Useful if you have a reference to this node and want to remove it from the scene. */
        void removeFromParent(bool immediate);
        
        
        // Node Properties
        
        /** Set the name of this node so that you can easily refer to it again in the future. */
		void setName(const std::string& name);
        
        /** Get the name of this node. */
		const std::string& getName() const;
        
        /** Set the position of this node in the parent node's local space. Remember that increasing 'y' goes 'down' the screen in SFML. */
        void setPosition(const sf::Vector2f& position);
        void setPosition(float x, float y);
        
        /** Set the rotation of this node where 'angle' is stored in degrees. */
        void setRotation(float angle);
        
        /** Move the node relative to it's current position by the provided offset. */
        void move(const sf::Vector2f& offset);
        void move(float x, float y);
        
        /** Rotate the node relative to it's current rotation. 'angle' is in degrees. */
        void rotate(float angle);
        
        /** Scale the node by the provided scale factors. Does not change the PhysicsBody, just the visual representation of this node. */
        void setScale(const sf::Vector2f& factors);
        void setScale(float x, float y);
        
        /** Scale the node relative to it's current scale. Does not alter the PhysicsBody attached to this node in any way. */
        void scale(const sf::Vector2f& factors);
        void scale(float x, float y);
        
        /** Set the relative origin point to use as the anchor on this node (0,0) -> (1,1).*/
        void setOrigin(float x, float y);
        void setOrigin(const sf::Vector2f& origin);
        
        /** Get the relative origin point used as the anchor on this node. */
        const sf::Vector2f& getOrigin() const;
        
        /** Get a pointer to the scene that this node is a part of (if it has been added to a scene). */
		Scene* getScene();

        
        // Physics
        
        /** Attach a PhysicsBody to this node. After this point, this Node's position will be kept in sync with the PhysicsBody. */
		void setPhysicsBody(const std::shared_ptr<PhysicsBody>& body);
        
        /** Get the PhysicsBody currently attached to this node (nullptr by default). */
		const std::shared_ptr<PhysicsBody>& getPhysicsBody();
        
        /** Called by the PhysicsWorld when a contact is made (should not be called manually). */
        void beginContact(const PhysicsContact& contact);
        
        /** Called by the PhysicsWorld when a contact is made (should not be called manually). */
        void endContact(const PhysicsContact& contact);
        
        /** Sets a function pointer or lambda that should be called whenever this node begins contact with another body in the PhysicsWorld. */
        void setBeginContactCallback(const std::function<void(const PhysicsContact& contact)>& func);
        
        /** Sets a function pointer or lambda that should be called whenever this node ends contact / separates from another body in the PhysicsWorld. */
        void setEndContactCallback(const std::function<void(const PhysicsContact& contact)>& func);

        
        // Advanced Scene Graph Methods
        
        /** Returns the bounding box for this node, in the local space (of the parent node). */
		virtual sf::FloatRect getLocalBounds() const;
        
        /** Returns the global bounding box for this node, in the Scene's world space. */
		sf::FloatRect getGlobalBounds() const;
        
        /** Returns the global transform operation for this node, in the Scene's world space. */
		sf::Transform getGlobalTransform() const;

		/** Returns the deepest descendant that intersects a point (does not check against this node). */
		std::shared_ptr<Node> getNodeAtPoint(const sf::Vector2f& point);
		std::shared_ptr<Node> getNodeAtPoint(float x, float y);

        /** Get vectors in relation to the facing of this sprite */
        sf::Vector2f forwardVector() const;     // 'down' the screen or 'up' in the y-axis
        sf::Vector2f backwardVector() const;    // 'up' the screen or 'down' in the y-axis
        sf::Vector2f leftVector() const;
        sf::Vector2f rightVector() const;

        
        // Other Methods
        
        /** Run an function on this node (and all of it's children if recursive is set to true). */
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
