#pragma once

#include <random>
#include <chrono>
#include <type_traits>

namespace Miracle {
	template<typename T>
	concept RandomInt = std::is_same_v<T, short>
		|| std::is_same_v<T, int>
		|| std::is_same_v<T, long>
		|| std::is_same_v<T, long long>
		|| std::is_same_v<T, unsigned short>
		|| std::is_same_v<T, unsigned int>
		|| std::is_same_v<T, unsigned long>
		|| std::is_same_v<T, unsigned long long>;

	template<typename T>
	concept RandomFloat = std::is_same_v<T, float>
		|| std::is_same_v<T, double>
		|| std::is_same_v<T, long double>;

	template<typename T>
	concept RandomBool = std::is_same_v<T, bool>;

	class Random {
	private:
		std::mt19937 m_generator;

	public:
		Random() :
			m_generator(std::chrono::high_resolution_clock::now().time_since_epoch().count())
		{}

		Random(decltype(m_generator)::result_type seed) :
			m_generator(seed)
		{}

		Random(const Random&) = delete;

		Random& operator=(const Random&) = delete;

		void setSeed(decltype(m_generator)::result_type seed) {
			m_generator.seed(seed);
		}

		template<RandomInt TInt>
		TInt next() {
			return std::uniform_int_distribution<TInt>()(m_generator);
		}

		auto next(RandomInt auto max) {
			return std::uniform_int_distribution({}, max)(m_generator);
		}

		auto next(RandomInt auto min, decltype(min) max) {
			return std::uniform_int_distribution(min, max)(m_generator);
		}

		template<RandomFloat TFloat>
		TFloat next() {
			return std::uniform_real_distribution<TFloat>()(m_generator);
		}

		auto next(RandomFloat auto max) {
			return std::uniform_real_distribution({}, max)(m_generator);
		}

		auto next(RandomFloat auto min, decltype(min) max) {
			return std::uniform_real_distribution(min, max)(m_generator);
		}

		template<RandomBool>
		auto next() {
			return std::bernoulli_distribution()(m_generator);
		}

		template<RandomBool>
		auto next(double probabilityForTrue) {
			return std::bernoulli_distribution(probabilityForTrue)(m_generator);
		}
	};
}
