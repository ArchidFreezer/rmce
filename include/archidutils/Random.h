#pragma once

#include <chrono>
#include <random>

/**
 * @brief Random number generator
 *
 * This header-only Random namespace implements a self-seeding Mersenne Twister.
 * This engine is predictable so should not be used for crypto or statistical analysis, but is Ok for generic usage
 *
 * It can be included into as many code files as needed (The inline keyword avoids ODR violations)
 * Freely redistributable, courtesy of learncpp.com (https://www.learncpp.com/cpp-tutorial/global-random-numbers-random-h/)
 *
 * A better performing engine, but is not part of the standard library, is PCG: https://www.pcg-random.org/
 *
 */
namespace Random {
	/**
	 * Seeded Mersenne Twister.
	 *
	 * There is no need to call this function explicitly
	 *
	 * Note: we'd prefer to return a std::seed_seq (to initialize a std::mt19937), but std::seed can't be copied, so it can't be returned by value.
	 * Instead, we'll create a std::mt19937, seed it, and then return the std::mt19937 (which can be copied).
	 *
	 * @return seeded Mersenne Twister
	 */
	inline std::mt19937 generate() {
		std::random_device rd{};

		// Create seed_seq with clock and 7 random numbers from std::random_device
		std::seed_seq ss{
			static_cast<std::seed_seq::result_type>(std::chrono::steady_clock::now().time_since_epoch().count()),
				rd(), rd(), rd(), rd(), rd(), rd(), rd() };

		return std::mt19937{ ss };
	}

	/**
	 * Global Mersenne Twister object.
	 * The inline keyword means we have external linkage, so one global instance for our whole program.
	 */
	inline std::mt19937 mt{ generate() }; // generates a seeded std::mt19937 and copies it into our global object

	/**
	 * @brief Generate a random int
	 *
	 * Handles cases where the two arguments have different types but can be converted to int
	 *
	 * @param min minimum returned value (inclusive)
	 * @param max maximum returned value (inclusive)
	 * @return random value between min and max
	 */
	inline int get(int min, int max) {
		return std::uniform_int_distribution{ min, max }(mt);
	}


	/**
	 * @brief Generate a random value between [min, max] (inclusive)
	 *
	 * The min and max values may be of different type
	 *
	 * @tparam T type of the parameters and return value
	 * @param min minimum returned value (inclusive)
	 * @param max maximum returned value (inclusive)
	 * @return random value between min and max
	 *
	 * Example:
	 * @code{.cpp}
	 * Random::get(1L, 6L);             // returns long
	 * Random::get(1u, 6u);             // returns unsigned int
	 * @endcode
	 */
	template <typename T>
	T get(T min, T max) {
		return std::uniform_int_distribution<T>{min, max}(mt);
	}

	/**
	 * @brief Generate a random value between [min, max] (inclusive)
	 *
	 * The min and max values may be of different type
	 * The return type must be specified as a template argument and min and max
	 *   will be converted to this type.
	 *
	 * @tparam R return type
	 * @tparam S min type, must be capable of implicit conversion to R
	 * @tparam T max type, must be capable of implicit conversion to R
	 * @param min minimum returned value (inclusive)
	 * @param max maximum returned value (inclusive)
	 * @return random value between min and max
	 *
	 * Example:
	 * @code{.cpp}
	 * Random::get<std::size_t>(0, 6);  // returns std::size_t
	 * Random::get<std::size_t>(0, 6u); // returns std::size_t
	 * Random::get<std::int>(0, 6u);    // returns int
	 * @endcode
	 */
	template <typename R, typename S, typename T>
	R get(S min, T max) {
		return get<R>(static_cast<R>(min), static_cast<R>(max));
	}
}
