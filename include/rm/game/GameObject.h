#pragma once

#include <boost/uuid.hpp>

#include <string>
namespace rm::game {

	// Forward declaration of GameObject to avoid circular dependency in the concept definition
	class GameObject;

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
	class GameObject {
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
		std::string id() const { return to_string(id_); }

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
		GameObject() : id_{ boost::uuids::random_generator()() } {};

	private:
		boost::uuids::uuid id_; // Unique tag to ensure that each object has a unique identifier

		/**
		 * @brief Default copy constructor for GameObject.
		 * @param other The GameObject instance to copy from.
		 */
		GameObject(const GameObject& other) = default; // Default copy constructor

		/**
		 * @brief Set the unique id of the game data object
		 *
		 * Each object containing game rule data requires a unique identifier to allow it to be referenced.
		 * This is necessary for deserialization where the unique identifier is stored as a string in the data file and needs to be converted back to a UUID when the object is created.
		 *
		 * @param id Unique identifier for the object as a string reference.
		 */
		void setId(const std::string& id) { id_ = boost::uuids::string_generator()(id); }
	};

} // namespace rm::game