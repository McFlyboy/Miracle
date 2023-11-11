#pragma once

#include <cstddef>
#include <random>
#include <chrono>
#include <concepts>

namespace Miracle {
	template<typename T>
	concept RandomInt = std::same_as<T, short>
		|| std::same_as<T, int>
		|| std::same_as<T, long>
		|| std::same_as<T, long long>
		|| std::same_as<T, unsigned short>
		|| std::same_as<T, unsigned int>
		|| std::same_as<T, unsigned long>
		|| std::same_as<T, unsigned long long>;

	template<typename T>
	concept RandomFloat = std::same_as<T, float>
		|| std::same_as<T, double>
		|| std::same_as<T, long double>;

	template<typename T>
	concept RandomBool = std::same_as<T, bool>;

	template<typename T>
	concept RandomContainerOrView = requires(T containerOrView) {
		{ containerOrView.begin() };
		{ containerOrView.size() } -> std::same_as<size_t>;
	};

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

		auto&& element(RandomContainerOrView auto&& containerOrView) {
			size_t remainingIncrementations = next<size_t>(containerOrView.size() - 1);
			auto iterator = containerOrView.begin();

			while (remainingIncrementations > 0) {
				iterator++;
				remainingIncrementations--;
			}

			return *iterator;
		}
	};
}
