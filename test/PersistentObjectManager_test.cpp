#include <gtest/gtest.h>

#include <PersistentCache.h>
#include <PersistentObjectManager.h>
#include <BookData.h>
#include <GameObject.h>

using namespace rm;
using namespace rm::rule;

namespace {

// -----------------------------------------------------------------------
// Minimal default_persistent_object for testing GameObject-style objects
// -----------------------------------------------------------------------

/**
 * @brief Minimal default_persistent_object for testing default_persistent_object overloads
 */
class TestGameObject : public rm::game::GameObject {
public:
	TestGameObject() = default;
	~TestGameObject() override = default;
};

// -----------------------------------------------------------------------
// Test fixture
// -----------------------------------------------------------------------

/**
 * @brief Test fixture that provides a fresh PersistentCache and PersistentObjectManager for each test
 */
class PersistentObjectManagerTest : public ::testing::Test {
protected:
	PersistentCache cache;
	PersistentObjectManager manager{cache};
};

// -----------------------------------------------------------------------
// get<id_persistent_object T>(string) — BookData is id_persistent_object
// -----------------------------------------------------------------------

/**
 * @brief get() creates a new id_persistent_object and adds it to the cache when it does not already exist
 */
TEST_F(PersistentObjectManagerTest, GetIdPersistentObject_CreatesWhenNotExists) {
	BookData& book = manager.get<BookData>("rulebook");

	EXPECT_EQ(book.id(), "BOOK_RULEBOOK");
}

/**
 * @brief get() returns the same object when called twice with the same id
 */
TEST_F(PersistentObjectManagerTest, GetIdPersistentObject_ReturnsSameObjectOnSecondCall) {
	BookData& book1 = manager.get<BookData>("rulebook");
	BookData& book2 = manager.get<BookData>("BOOK_RULEBOOK");

	EXPECT_EQ(&book1, &book2);
}

/**
 * @brief get() strips an existing prefix before creating the object to avoid double-prefixing
 */
TEST_F(PersistentObjectManagerTest, GetIdPersistentObject_HandlesAlreadyPrefixedId) {
	BookData& book = manager.get<BookData>("BOOK_RULEBOOK");

	EXPECT_EQ(book.id(), "BOOK_RULEBOOK");
}

/**
 * @brief get() with a prefixed and unprefixed id that refer to the same object returns the same reference
 */
TEST_F(PersistentObjectManagerTest, GetIdPersistentObject_PrefixedAndUnprefixedReturnSameObject) {
	BookData& book1 = manager.get<BookData>("rulebook");
	BookData& book2 = manager.get<BookData>("BOOK_RULEBOOK");

	EXPECT_EQ(&book1, &book2);
}

// -----------------------------------------------------------------------
// get<default_persistent_object T>() — TestGameObject is default_persistent_object
// -----------------------------------------------------------------------

/**
 * @brief get() with no arguments creates a new default_persistent_object with a UUID id and adds it to the cache
 */
TEST_F(PersistentObjectManagerTest, GetDefaultPersistentObject_CreatesWithUuid) {
	TestGameObject& obj = manager.get<TestGameObject>();

	EXPECT_FALSE(obj.id().empty());
}

/**
 * @brief Two successive no-argument get() calls create two distinct objects
 */
TEST_F(PersistentObjectManagerTest, GetDefaultPersistentObject_TwoCallsCreateDistinctObjects) {
	TestGameObject& obj1 = manager.get<TestGameObject>();
	TestGameObject& obj2 = manager.get<TestGameObject>();

	EXPECT_NE(obj1.id(), obj2.id());
	EXPECT_NE(&obj1, &obj2);
}

/**
 * @brief get(id) retrieves a previously created default_persistent_object by its uuid
 */
TEST_F(PersistentObjectManagerTest, GetDefaultPersistentObject_RetrievableById) {
	TestGameObject& created = manager.get<TestGameObject>();
	std::string id = created.id();

	TestGameObject& retrieved = manager.get<TestGameObject>(id);

	EXPECT_EQ(&created, &retrieved);
}

// -----------------------------------------------------------------------
// exists()
// -----------------------------------------------------------------------

/**
 * @brief exists() returns false before an object is created
 */
TEST_F(PersistentObjectManagerTest, Exists_ReturnsFalseForUnknownId) {
	EXPECT_FALSE(manager.exists<BookData>("BOOK_UNKNOWN"));
}

/**
 * @brief exists() returns true after an object has been created
 */
TEST_F(PersistentObjectManagerTest, Exists_ReturnsTrueAfterCreate) {
	manager.get<BookData>("companion");

	EXPECT_TRUE(manager.exists<BookData>("BOOK_COMPANION"));
}

// -----------------------------------------------------------------------
// deleteObject() / isDeleted() / undeleteObject()
// -----------------------------------------------------------------------

/**
 * @brief isDeleted() returns false for an object that has not been deleted
 */
TEST_F(PersistentObjectManagerTest, IsDeleted_ReturnsFalseByDefault) {
	BookData& book = manager.get<BookData>("companion");

	EXPECT_FALSE(manager.isDeleted(book.id()));
}

/**
 * @brief deleteObject() causes isDeleted() to return true
 */
TEST_F(PersistentObjectManagerTest, DeleteObject_SetsDeletedFlag) {
	BookData& book = manager.get<BookData>("companion");
	manager.deleteObject(book.id());

	EXPECT_TRUE(manager.isDeleted(book.id()));
}

/**
 * @brief get() throws out_of_range when the object has been deleted
 */
TEST_F(PersistentObjectManagerTest, Get_ThrowsWhenObjectIsDeleted) {
	BookData& book = manager.get<BookData>("companion");
	std::string id = book.id();
	manager.deleteObject(id);

	EXPECT_THROW(manager.get<BookData>(id), std::out_of_range);
}

/**
 * @brief undeleteObject() restores a deleted object so it can be retrieved again
 */
TEST_F(PersistentObjectManagerTest, UndeleteObject_AllowsGetAfterDeletion) {
	BookData& book = manager.get<BookData>("companion");
	std::string id = book.id();
	manager.deleteObject(id);
	manager.undeleteObject(id);

	EXPECT_NO_THROW(manager.get<BookData>(id));
	EXPECT_FALSE(manager.isDeleted(id));
}

// -----------------------------------------------------------------------
// flagIncomplete() / isIncomplete() / unflagIncomplete()
// -----------------------------------------------------------------------

/**
 * @brief isNonSerialized() returns false for an object that has not been flagged
 */
TEST_F(PersistentObjectManagerTest, IsNonSerialized_ReturnsFalseByDefault) {
	BookData& book = manager.get<BookData>("companion");

	EXPECT_FALSE(manager.isNonSerialized(book.id()));
}

/**
 * @brief flagNonSerialized() causes isNonSerialized() to return true
 */
TEST_F(PersistentObjectManagerTest, FlagNonSerialized_SetsFlag) {
	BookData& book = manager.get<BookData>("companion");
	manager.flagNonSerialized(book.id());

	EXPECT_TRUE(manager.isNonSerialized(book.id()));
}

/**
 * @brief unflagNonSerialized() clears the non-serialized flag
 */
TEST_F(PersistentObjectManagerTest, UnflagNonSerialized_ClearsFlag) {
	BookData& book = manager.get<BookData>("companion");
	manager.flagNonSerialized(book.id());
	manager.unflagNonSerialized(book.id());

	EXPECT_FALSE(manager.isNonSerialized(book.id()));
}

// -----------------------------------------------------------------------
// getAll()
// -----------------------------------------------------------------------

/**
 * @brief getAll() returns an empty vector when no objects of the type exist
 */
TEST_F(PersistentObjectManagerTest, GetAll_EmptyWhenNoneExist) {
	auto books = manager.getAll<BookData>();

	EXPECT_TRUE(books.empty());
}

/**
 * @brief getAll() returns all non-deleted, non-incomplete objects of the type
 */
TEST_F(PersistentObjectManagerTest, GetAll_ReturnsAllActiveObjects) {
	manager.get<BookData>("companion");
	manager.get<BookData>("core");
	manager.get<BookData>("arms");

	auto books = manager.getAll<BookData>();

	EXPECT_EQ(books.size(), 3u);
}

/**
 * @brief getAll() excludes deleted objects
 */
TEST_F(PersistentObjectManagerTest, GetAll_ExcludesDeletedObjects) {
	BookData& book1 = manager.get<BookData>("companion");
	manager.get<BookData>("core");
	manager.deleteObject(book1.id());

	auto books = manager.getAll<BookData>();

	EXPECT_EQ(books.size(), 1u);
}

/**
 * @brief getAll() excludes objects flagged as incomplete
 */
TEST_F(PersistentObjectManagerTest, GetAll_ExcludesIncompleteObjects) {
	BookData& book1 = manager.get<BookData>("companion");
	manager.get<BookData>("core");
	manager.flagNonSerialized(book1.id());

	auto books = manager.getAll<BookData>();

	EXPECT_EQ(books.size(), 1u);
}

// -----------------------------------------------------------------------
// keys()
// -----------------------------------------------------------------------

/**
 * @brief keys() populates the set with all cached ids of the type, excluding deleted ones
 */
TEST_F(PersistentObjectManagerTest, Keys_PopulatesSetWithCachedIds) {
	BookData& book1 = manager.get<BookData>("companion");
	BookData& book2 = manager.get<BookData>("core");

	std::set<std::string> keys;
	manager.keys<BookData>(keys);

	EXPECT_EQ(keys.size(), 2u);
	EXPECT_NE(keys.find(book1.id()), keys.end());
	EXPECT_NE(keys.find(book2.id()), keys.end());
}

/**
 * @brief keys() excludes deleted objects
 */
TEST_F(PersistentObjectManagerTest, Keys_ExcludesDeletedObjects) {
	BookData& book1 = manager.get<BookData>("companion");
	manager.get<BookData>("core");
	manager.deleteObject(book1.id());

	std::set<std::string> keys;
	manager.keys<BookData>(keys);

	EXPECT_EQ(keys.size(), 1u);
	EXPECT_EQ(keys.find(book1.id()), keys.end());
}

/**
 * @brief keys() clears any prior content of the set before populating it
 */
TEST_F(PersistentObjectManagerTest, Keys_ClearsPriorSetContent) {
	manager.get<BookData>("companion");

	std::set<std::string> keys{"stale_entry"};
	manager.keys<BookData>(keys);

	EXPECT_EQ(keys.find("stale_entry"), keys.end());
}

// -----------------------------------------------------------------------
// create() — transient objects, not added to cache
// -----------------------------------------------------------------------

/**
 * @brief create() returns a unique_ptr to a new default_persistent_object not stored in the cache
 */
TEST_F(PersistentObjectManagerTest, Create_DefaultObject_NotAddedToCache) {
	auto obj = manager.create<TestGameObject>();

	EXPECT_FALSE(obj->id().empty());
	EXPECT_FALSE(manager.exists<TestGameObject>(obj->id()));
}

/**
 * @brief create(id) returns a unique_ptr to a new id_persistent_object not stored in the cache
 */
TEST_F(PersistentObjectManagerTest, Create_IdObject_NotAddedToCache) {
	auto book = manager.create<BookData>("transient");

	EXPECT_EQ(book->id(), "BOOK_TRANSIENT");
	EXPECT_FALSE(manager.exists<BookData>("BOOK_TRANSIENT"));
}

} // namespace