#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

#include "cameranode.h"
#include "shapenode.h"
#include "spritenode.h"
#include "textnode.h"

#include "physics/debugdraw.h"
#include "physics/physicsworld.h"
#include "physics/physicsbody.h"


namespace gbh {

	class CameraNode;

    /**
     Represents a single Scene, 'State' or 'GameMode' that the Game can be in.
     For instance, these might represent the TitleScreen, GameOverScreen or MainGameScreen states of your game.
     */
	class Scene : public PhysicsContactListener
	{
		friend class gbh::Game;

	public:
		Scene();
		virtual ~Scene();

        
        // Internal Methods
        
		void initialize();
		void show();
		void hide();
		void destroy();
        void update(double deltaTime);
        void draw(sf::RenderTarget& target) const;

        
        // Camera Methods

        /** Set the provided CameraNode (or subclass) to be the current active camera in the scene. Can be set to nullptr to default to showing the area from (0,0) -> (Width,Height). */
		void setCamera(std::shared_ptr<CameraNode> cameraNode);
        
        /** Get the currently active camera. Might be a nullptr. */
		std::shared_ptr<CameraNode> getCamera();
        
        /** Get the transform used to position the current camera in world space (the inverse is used to render the scene). */
		sf::Transform getCameraTransform() const;
        
        
        // SceneGraph / Overlay
        
        /** Returns a special 'overlay' node that can be used to insert objects into the scene that do not move around with the camera. They are rendered after everything else. */
        Node& getOverlay() { return m_overlayNode; }

        /** Add the given child to the scene. The node will be added to the end of the list, which means it is rendered last (on top of) other nodes. */
        void addChild(const std::shared_ptr<gbh::Node>& node);
        
        /** Returns the number of nodes that are a direct child of this node (ignoring the fact that they themselves might have children). */
        int getChildCount() const;
        
        /** Returns the child node at the given index (or null if an invalid index was provided). */
        std::shared_ptr<Node> getChildAtIndex(int index);
        
        /** Returns the first child node in the scene graph with the given name. If recursive is true, then we also seach through children of children etc. */
        std::shared_ptr<Node> getFirstChildWithName(const std::string& name, bool recursive);
        
        /** Remove the child node at the given index. Unless immediate is manually set to true, this will actually occur as part of the next frame update instead of  immediately. */
        void removeChild(int index, bool immediate);
        
        /** Remove all children of this node with the given name. This is not recursive and does not search child nodes. */
        void removeChildrenWithName(const std::string& name, bool immediate);

        /** Returns the deepest descendant that intersects a point in screen space (as defined by the current camera). */
		std::shared_ptr<Node> getNodeAtViewPoint(const sf::Vector2f& point);
		std::shared_ptr<Node> getNodeAtViewPoint(float x, float y);
        
        
        // Physics
        
        /** Creates a new physicsworld attached to this scene with the provided gravity. */
        void createPhysicsWorld(const sf::Vector2f& gravity);
        
        /** Gets the current PhysicsWorld attached to this scene. Useful for creating new bodies in the world. Can be nullptr if called before createPhysicsWorld. */
        PhysicsWorld* getPhysicsWorld() const { return m_physicsWorld.get(); }
        
        /** Called by the Game class to drive the physics simulation. */
        void simulatePhysics(double deltaTime);
        
        /** Turn on/off debug rendering for the physics objects in the game world. */
        void setDrawPhysicsDebug(bool value) { m_drawPhysicsDebug = value; }
        
        /** Returns whether physics debug rendering is currently enabled or not. */
        bool getDrawPhysicsDebug() const { return m_drawPhysicsDebug; }
        
        /** Turn on/off debug messages for physics contact events. */
        void setDebugPhysicsEvents(bool value) { m_debugPhysicsEvents = value; }
        
        /** Returns whether physics debug messages are currently being printed for contact events. */
        bool getDebugPhysicsEvents() const { return m_debugPhysicsEvents; }
        
        /** Internal methods called by PhysicsWorld */
        void beginContact(const PhysicsContact& contact) override;
        void endContact(const PhysicsContact& contact) override;
        
        /** Returns the current list of PhysicsContacts that are still taking place. */
        const std::vector<PhysicsContact>& getContactList() const { return m_physicsContacts; };
		
	protected:
		virtual void onUpdate(double deltaTime);
		virtual void onDraw(sf::RenderTarget& target, const sf::Transform& transform) const;

		virtual void onInitializeScene();
		virtual void onShowScene();
		virtual void onHideScene();
		virtual void onDestroyScene();

		virtual void onKeyboardEvent(sf::Event& event);
		virtual void onMouseEvent(sf::Event& event);
		virtual void onJoystickEvent(sf::Event& event);
        virtual void onBeginPhysicsContact(const PhysicsContact& contact);
        virtual void onEndPhysicsContact(const PhysicsContact& contact);

	private:
		Node m_rootNode;
        Node m_overlayNode;
        
        std::shared_ptr<CameraNode> m_sceneCamera;
        std::unique_ptr<PhysicsWorld> m_physicsWorld;
        std::unique_ptr<SfmlBoxDebugDraw> m_physicsDebug;
        
        std::vector<PhysicsContact> m_physicsContacts;
    
        bool m_initialized = false;
        bool m_drawPhysicsDebug = false;
        bool m_debugPhysicsEvents = false;
	};

} // namespace
