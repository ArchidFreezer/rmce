#pragma once

#include <boost/beast/http.hpp>
#include <PersistentObjectSerializationManager.h>

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
	 * @brief Constructor to initialize the CharacterBuilderRequestHandler with a reference to the PersistentObjectSerializationManager
	 * @param serial_manager Reference to PersistentObjectSerializationManager for data access and JSON serialization during request handling
	 */
	CharacterBuilderRequestHandler(PersistentObjectSerializationManager& serial_manager) : serial_manager_{serial_manager} {};

	/**
	 * @brief Handle an incoming HTTP request and prepare the response
	 *
	 * The following requests are supported:
	 *
	 * | Method | Endpoint | Description |
	 * |--------|----------|-------------|
	 * |POST	 | /rmce/operations/character/initial-choices | Generate initial character creation choices based on provided parameters |
	 * |POST	 | /rmce/operations/character/stat-rolls | Generate and return potential stat values from temporary rolls and the potential roll |
	 * |POST	 | /rmce/operations/character/set-stats | Set the temporary and potential values for each stat of a character being created |
	 *
	 * @param request The HTTP request to handle
	 * @param response The HTTP response to populate based on the request
	 */
	void handleRequest(const http::request<http::string_body>& request, http::response<http::string_body>& response);

private:
	PersistentObjectSerializationManager& serial_manager_;

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
	void requestInitialChoices(http::response<http::string_body>& response, const http::request<http::string_body>& request);

	/**
	 * @brief Requests the generation of potential stat values from temporary rolls and the potential roll.
	 *
	 * This operation is specific to character stat rolls and does not follow the standard CRUD pattern. Both the temporary and potential roll values are expected to be provided in the request body as JSON, and the response will contain the
	 * generated potential stat values based on the rolls. If the value of any field is -1 then it is treated as if the field was not provided and the roll is generated randomly.
	 *
	 * @param response The HTTP response object to populate with the result of the operation.
	 * @param request The HTTP request containing the data for the operation in its body.
	 */
	void requestStatRolls(http::response<http::string_body>& response, const http::request<http::string_body>& request);

	/**
	 * @brief Sets the stat temporary and potential values for a CharacterBuilder object.
	 *
	 * This operation takes the ID of a CharacterBuilder object and the temporary and potential values for each of the 10 stats from the request body as JSON, updates the corresponding CharacterBuilder object in the cache with those values,
	 * and returns a success response. This allows the client to set the stat values for a character being created after generating them with the stat rolls endpoint.
	 * 
	 * Expects a payload in the following format:
	 * @code
	 * {
	 *   "id": "character_builder_id",
	 *   "stats": [
	 *     {"stat": "Strength", "temporary": 50, "potential": 75},
	 *     {"stat": "Dexterity", "temporary": 60, "potential": 80},
	 *     ...
	 *   ]
	 * }
	 * @endcode
	 *
	 * @param response The HTTP response object to populate with the result of the operation.
	 * @param request The HTTP request containing the data for the operation in its body.
	 */
	void requestSetStats(http::response<http::string_body>& response, const http::request<http::string_body>& request);
};

} // namespace rm::rest
