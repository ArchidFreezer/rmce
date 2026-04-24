#pragma once

#include <boost/beast/http.hpp>
#include <PersistentObjectSerializationManager.h>

namespace http = boost::beast::http;

namespace rm::rest {

/**
 * @class DataRequestHandler
 * @brief Handles HTTP requests related to game objects, including CRUD operations and listing/counting objects of specific types.
 *
 * This class processes HTTP requests that involve retrieving, creating, updating, or deleting game objects. It uses the PersistentObjectSerializationManager to access and manipulate the underlying data as needed for these operations. The
 * handler supports various endpoints for listing objects, counting objects, and retrieving specific objects by ID.
 */
class DataRequestHandler {
public:
	/**
	 * @brief Constructor to initialize the DataRequestHandler with a reference to the PersistentObjectSerializationManager
	 * @param serial_manager Reference to PersistentObjectSerializationManager for data access and JSON serialization during request handling
	 */
	DataRequestHandler(PersistentObjectSerializationManager& serial_manager) : serial_manager_{serial_manager} {};

	/**
	 * @brief Handle an incoming HTTP request related to game objects and prepare the response
	 * 
	 * The following requests are supported:
	 * 
	 * | Method | Endpoint | Description |
	 * |--------|----------|-------------|
	 * |GET    | /rmce/objects/count?types={types} | Get the count of objects of specific types (e.g. /rmce/objects/count?types=skill,race) |
	 * |GET    | /rmce/objects/prefixes | Get a list of all known type prefixes for objects (e.g. "skill", "race") |
	 * |GET    | /rmce/objects/{type} | Get a list of all objects of a specific type (e.g. /rmce/objects/skill) |
	 * |GET    | /rmce/objects/{type}?count | Get the count of objects of a specific type (e.g. /rmce/objects/skill?count) |
	 * |GET    | /rmce/objects/{type}?ids | Get a list of all object IDs of a specific type (e.g. /rmce/objects/skill?ids) |
	 * |GET    | /rmce/objects/{type}/{id} | Get a specific object by its type and ID (e.g. /rmce/objects/skill/SKILL_ACTING) |
	 * |POST   | /rmce/objects/{type} | Create a new object of a specific type |
	 * |PUT    | /rmce/objects/{type} | Update an existing object by its type using the ID in the JSON payload |
	 * |DELETE | /rmce/objects/{type}/{id} | Delete an existing object by its type and ID |
	 * 
	 * @param request The HTTP request to handle
	 * @param response The HTTP response to populate based on the request
	 */
	void handleRequest(const http::request<http::string_body>& request, http::response<http::string_body>& response);

private:
	PersistentObjectSerializationManager& serial_manager_;

	/**
	 * @brief Requests prefixes and populates the HTTP response.
	 * @param response The HTTP response object to populate with prefix data.
	 */
	void requestPrefixes(http::response<http::string_body>& response);

	/**
	 * @brief Requests a list of objects of a specified type and populates the HTTP response.
	 * @param response The HTTP response object to populate with the list of objects.
	 * @param type The type of objects to list.
	 */
	void requestListObjects(http::response<http::string_body>& response, std::string_view type);

	/**
	 * @brief Requests a list of object IDs of a specified type and populates the HTTP response.
	 * @param response The HTTP response object to populate with the list of object IDs.
	 * @param type The type of objects for which to list IDs.
	 */
	void requestListObjectIds(http::response<http::string_body>& response, std::string_view type);

	/**
	 * @brief Requests the count of objects of a specified type and populates the HTTP response.
	 * @param response The HTTP response object to populate with the count of objects.
	 * @param type The type of objects for which to count.
	 */
	void requestCountTypeObjects(http::response<http::string_body>& response, std::string_view type);

	/**
	 * @brief Requests the count of objects of multiple specified types and populates the HTTP response.
	 * @param response The HTTP response object to populate with the count of objects.
	 * @param types The types of objects for which to count as a CSV list (e.g., "skill,book,skillcategory").
	 */
	void requestCountMultiTypeObjects(http::response<http::string_body>& response, std::string_view types);

	/**
	 * @brief Requests a specific object by its type and ID, and populates the HTTP response.
	 * @param response The HTTP response object to populate with the requested object data.
	 * @param type The type of the object to retrieve.
	 * @param id The ID of the object to retrieve.
	 */
	void requestObjectById(http::response<http::string_body>& response, std::string_view type, std::string_view id);

	/**
	 * @brief Requests the creation of a new object of a specified type, using data from the HTTP request body, and populates the HTTP response.
	 * @param response The HTTP response object to populate with the result of the create operation.
	 * @param type The type of the object to create.
	 * @param request The HTTP request containing the data for the new object in its body.
	 */
	void requestCreateObject(http::response<http::string_body>& response, std::string_view type, const http::request<http::string_body>& request);

	/**
	 * @brief Requests the update of an existing object of a specified type, using data from the HTTP request body, and populates the HTTP response.
	 * @param response The HTTP response object to populate with the result of the update operation.
	 * @param type The type of the object to update.
	 * @param request The HTTP request containing the data for the object to update in its body.
	 */
	void requestUpdateObject(http::response<http::string_body>& response, std::string_view type, const http::request<http::string_body>& request);

	/**
	 * @brief Requests the deletion of an object by its type and ID, and populates the HTTP response.
	 * @param response The HTTP response object to populate with the result of the delete operation.
	 * @param type The type of the object to delete.
	 * @param id The ID of the object to delete.
	 */
	void requestDeleteObject(http::response<http::string_body>& response, std::string_view type, std::string_view id);
};

} // namespace rm::rest