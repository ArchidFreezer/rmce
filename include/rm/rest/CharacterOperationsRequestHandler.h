#pragma once

#include <boost/beast/http.hpp>
#include <PersistentObjectSerializationManager.h>

namespace http = boost::beast::http;

namespace rm::rest {

/**
 * @class CharacterOperationsRequestHandler
 * @brief Handles HTTP requests related to character creation.
 *
 * This class provides methods to handle specific HTTP requests for generating initial character creation choices and potential stat values based on temporary rolls. It interacts with the PersistentObjectManager to access necessary data
 * during request handling.
 */
class CharacterOperationsRequestHandler {
public:
	/**
	 * @brief Constructor to initialize the CharacterOperationsRequestHandler with a reference to the PersistentObjectSerializationManager
	 * @param serial_manager Reference to PersistentObjectSerializationManager for data access and JSON serialization during request handling
	 */
	CharacterOperationsRequestHandler(PersistentObjectSerializationManager& serial_manager) : serial_manager_{serial_manager} {};

	/**
	 * @brief Handle an incoming HTTP request and prepare the response
	 *
	 * The following requests are supported:
	 *
	 * | Method | Endpoint | Description |
	 * |--------|----------|-------------|
	 * |GET	 | /rmce/operations/character/dump?id={id} | Dump the current state of a character being created using the specified character ID |
	 * |POST	 | /rmce/operations/character/initial-choices | Generate initial character creation choices based on provided parameters |
	 * |POST	 | /rmce/operations/character/stat-rolls | Generate and return potential stat values from temporary rolls and the potential roll |
	 * |POST	 | /rmce/operations/character/set-stats | Set the temporary and potential values for each stat of a character being created |
	 * |GET	 | /rmce/operations/character/hobby-choices?id={id} | Retrieve the available hobby choices for a character being created using the specified character ID |
	 * |POST	 | /rmce/operations/character/set-hobby-choices | Set the selected hobby choices for a character being created |
	 *
	 * @param request The HTTP request to handle
	 * @param response The HTTP response to populate based on the request
	 */
	void handleRequest(const http::request<http::string_body>& request, http::response<http::string_body>& response);

private:
	PersistentObjectSerializationManager& serial_manager_;

	/**
	 * @brief Requests the automatic generation of a character based on provided primary definitions.
	 *
	 * This operation is specific to character creation and does not follow the standard CRUD pattern. The request body is expected to contain JSON with parameters that define the initial choices for character creation.
	 * The function will take the provided primary definitions and automatically generate a completed level 0 character based on them.
	 *
	 * @param response The HTTP response object to populate with the result of the operation.
	 * @param request The HTTP request containing the data for the operation in its body.
	 */
	void requestAutoPrimary(http::response<http::string_body>& response, const http::request<http::string_body>& request);

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
	void requestPrimaryDefinition(http::response<http::string_body>& response, const http::request<http::string_body>& request);

	/**
	 * @brief Requests the generation of available choices for the primary character attributes based on the initial definition.
	 *
	 * This operation is specific to character creation and does not follow the standard CRUD pattern. The request body is expected to contain JSON with the ID of the CharacterBuilder object for which to generate primary choices.
	 * The function will return the available choices for the primary character attributes (such as everyman skill categories for the race choice, skill category choices for the culture type choice, and skill development type and spell list
	 * choices for the profession choice) based on the initial definition of the character in the specified CharacterBuilder object.
	 *
	 * @param response The HTTP response object to populate with the result of the operation.
	 * @param request The HTTP request containing the data for the operation in its body.
	 */
	void requestPrimaryChoices(http::response<http::string_body>& response, const http::request<http::string_body>& request);

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

	/**
	 * @brief Sets the choices for generating the physique of a character being created.
	 *
	 * This operation is specific to character creation and does not follow the standard CRUD pattern. The request body is expected to contain JSON with the ID of the CharacterBuilder object for which to set the choices.
	 * The function will update the specified CharacterBuilder object in the cache with the provided choices and return a success response.
	 *
	 * @param response The HTTP response object to populate with the result of the operation.
	 * @param request The HTTP request containing the data for the operation in its body.
	 */
	void requestPhysiqueChoices(http::response<http::string_body>& response, const http::request<http::string_body>& request);

	/**
	 * @brief Sets the selected hobby choices for a character being created.
	 *
	 * This operation is specific to character creation and does not follow the standard CRUD pattern. The request body is expected to contain JSON with the ID of the CharacterBuilder object and the selected hobby choices to set for that
	 * object. The function will update the specified CharacterBuilder object in the cache with the selected hobby choices and return a success response.
	 *
	 * @param response The HTTP response object to populate with the result of the operation.
	 * @param request The HTTP request containing the data for the operation in its body.
	 */
	void requestSetHobbyChoices(http::response<http::string_body>& response, const http::request<http::string_body>& request);

	/**
	 * @brief Sets the selected background choices for a character being created.
	 *
	 * This operation is specific to character creation and does not follow the standard CRUD pattern. The request body is expected to contain JSON with the ID of the CharacterBuilder object and the selected background choices to set for
	 * that object. The function will update the specified CharacterBuilder object in the cache with the selected background choices and return a success response.
	 *
	 * @param response The HTTP response object to be populated with the result of the request.
	 * @param request The HTTP request object containing the background choices to be set.
	 */
	void requestSetBackgroundChoices(http::response<http::string_body>& response, const http::request<http::string_body>& request);

	/**
	 * @brief Applies the level-up choices for a character being leveled up.
	 *
	 * This operation is specific to character levelling and does not follow the standard CRUD pattern. The request body is expected to contain JSON with the ID of the CharacterLeveller object and the selected levelling
	 * choices to apply for that object. The function will update the specified CharacterLeveller object in the cache by applying the levelling choices and return a success response.
	 *
	 * @param response The HTTP response object to be populated with the result of the request.
	 * @param request The HTTP request object containing the levelling choices to be applied.
	 */
	void requestLevelUp(http::response<http::string_body>& response, const http::request<http::string_body>& request);

	/**
	 * @brief Dumps the current state of a CharacterBuilder object for debugging purposes.
	 *
	 * This operation is specific to character creation and does not follow the standard CRUD pattern. The request is expected to contain the ID of the CharacterBuilder object to dump in the query parameters. The function will return a JSON
	 * representation of the current state of the specified CharacterBuilder object in the response, which can be used for debugging purposes to see all the choices that have been made for that character so far.
	 *
	 * @param response The HTTP response object to populate with the result of the operation.
	 * @param request The HTTP request containing the data for the operation in its body.
	 * @param id The ID of the CharacterBuilder object to dump.
	 */
	void requestDump(http::response<http::string_body>& response, const http::request<http::string_body>& request, std::string id);
};

} // namespace rm::rest
