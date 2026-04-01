#pragma once

#include <boost/beast/http.hpp>
#include <PersistentObjectSerializationManager.h>
// #include <PersistentObjectManager.h>

namespace http = boost::beast::http;

namespace rm::rest {

/**
 * @class CharacterBuilderRequestHandler
 * @brief Handles HTTP requests related to character creation.
 *
 * This class provides methods to handle specific HTTP requests for generating initial character creation choices and potential stat values based on temporary rolls. It interacts with the PersistentObjectManager to access necessary data
 * during request handling.
 */
class CharacterBuilderRequestHandler {
public:
	/**
	 * @brief Constructor to initialize the CharacterBuilderRequestHandler with a reference to the PersistentObjectManager
	 * @param object_manager Reference to PersistentObjectManager for data access during request handling
	 */
	CharacterBuilderRequestHandler(rm::PersistentObjectManager& object_manager) : object_manager_{object_manager} {};

	/**
	 * @brief Requests the generation of initial character creation choices based on provided parameters.
	 *
	 * This operation is specific to character creation and does not follow the standard CRUD pattern. The request body is expected to contain JSON with parameters that define the initial choices for character creation.
	 * The function will create a CharacterBuilder object in the cache and set it with the initial choices, returning the generated ID of the new object in the response. The client can then use this ID to retrieve the CharacterBuilder
	 * object and make further updates to it as needed during the character creation process.
	 *
	 * @param response The HTTP response object to populate with the result of the operation.
	 * @param request The HTTP request containing the data for the operation in its body.
	 */
	void requestCharacterInitialChoices(http::response<http::string_body>& response, const http::request<http::string_body>& request);

	/**
	 * @brief Requests the generation of potential stat values from temporary rolls and the potential roll.
	 *
	 * This operation is specific to character stat rolls and does not follow the standard CRUD pattern. Both the temporary and potential roll values are expected to be provided in the request body as JSON, and the response will contain the
	 * generated potential stat values based on the rolls. If the value of any field is -1 then it is treated as if the field was not provided and the roll is generated randomly.
	 *
	 * @param response The HTTP response object to populate with the result of the operation.
	 * @param request The HTTP request containing the data for the operation in its body.
	 */
	void requestCharacterStatRolls(http::response<http::string_body>& response, const http::request<http::string_body>& request);

private:
	rm::PersistentObjectManager& object_manager_;
};
} // namespace rm::rest
