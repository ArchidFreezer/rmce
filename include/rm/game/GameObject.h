#pragma once

#include <boost/uuid.hpp>

#include <string>
#include <Persistent.h>

/**
 * @namespace rm::game
 *
 * @brief Contains all the game objects that store in-game elements.
 * 
 * These objects are things like characters or locations that are a direct representation of specifics in the game.
 *
 */
namespace rm::game {

	/**
	 * @class GameObject
	 *
	 * @brief Polymorphic class that is the base for objects that store data on the rules of the game
	 *
	 * This class is used as the base for all classes that define objects of specific in-game entities, either persistent or transient.
	 *
	 * Creation of these objects is through a factory class that has members to create both persistent objects that will be serialised and transient objects that will not be serialised.
	 * @see GameObjectFactory
	 */
	class GameObject : public rm::Persistent {
		friend class GameObjectFactory; // Allow the factory to access the private members of the class to set the unique identifier when creating objects

	public:

		/**
		 * @brief Default destructor
		 *
		 * This is made virtual to define the class as polymorphic as a standard best practice.
		 */
		virtual ~GameObject() = default;


		/**
		 * @brief Get the unique id of the game data object
		 *
		 * Each object containing game rule data requires a unique identifier to allow it to be referenced. This method allows for the unique identifier to be retrieved as a string reference, which is necessary for serialisation where the unique
		 * identifier needs to be stored as a string in the data file.
		 *
		 * @return Identifier as a string reference.
		 */
		const std::string& id() const { return id_; }

		/**
		 * @brief Override the less than operator
		 *
		 * The creation of this overload allows all GameObject objects to be used as keys in sorted containers
		 *
		 * @param other GameObject object to compoare against
		 * @return `true` if this object is considered to be < \a other
		 * @return `false` if this object is not considered to be < \a other
		 */
		bool operator<(const GameObject& other) const { return (id_ < other.id_); }

		/**
		 * @brief Override the equality operator
		 *
		 * The creation of this overload allows all GameObject objects to be compared for equality based on their unique identifier
		 *
		 * @param other GameObject object to compoare against
		 * @return `true` if this object is considered to be == \a other
		 * @return `false` if this object is not considered to be == \a other
		 */
		bool operator==(const GameObject& other) const { return (id_ == other.id_); }

	protected:
		/**
		 * @brief Default constructor
		 *
		 * This is private to ensure that the factory class is used to create objects
		 *
		 * Sets the unique identifier for the object to a randomly generated UUID. This ensures that each object has a unique identifier without requiring the caller to provide one.
		 */
		GameObject() : id_{ to_string(boost::uuids::random_generator()()) } {};

	private:
		std::string id_; // Unique tag to ensure that each object has a unique identifier

		/**
		 * @brief Set the unique id of the game data object
		 *
		 * Each object containing game rule data requires a unique identifier to allow it to be referenced.
		 * This is necessary for deserialization where the unique identifier is stored as a string in the data file and needs to be converted back to a UUID when the object is created.
		 *
		 * @param id Unique identifier for the object as a string reference.
		 */
		void setId(const std::string& id) { id_ = id; }


		/*
		 * Delete all the copy and move constructors and assignment operators to ensure that the unique identifier is not accidentally copied or moved, which would lead to multiple
		 * objects having the same unique identifier, which would break the uniqueness requirement of the identifier and cause issues with serialisation and deserialisation.
		 */
		GameObject(const GameObject& other) = delete; // Default copy constructor
		GameObject& operator=(const GameObject& other) = delete; // Default copy assignment operator
		GameObject(GameObject&& other) = delete; // Default move constructor
		GameObject& operator=(GameObject&& other) = delete; // Default move assignment operator

	};

	/**
	 * @brief Concept that ensures that a templated type is derived from the GameObject class
	 *
	 * It is used when templating a class or method where it may be used as follows
	 *
	 * @code
	 * template <game_object T>
	 * void someFunction(T obj) {
	 *   // do something with obj
	 * }
	 * @endcode
	 */
	template<class T>
	concept game_object = std::is_base_of<GameObject, T>::value;

} // namespace rm::game