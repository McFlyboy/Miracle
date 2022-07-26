#pragma once

#include <cstdint>

namespace Miracle::Application {
	struct EventTypesBase {
		using type = uint64_t;

		EventTypesBase() = delete;

		static constexpr EventTypesBase::type bit(int index) {
			return static_cast<EventTypesBase::type>(1) << static_cast<EventTypesBase::type>(index);
		}

		static constexpr EventTypesBase::type allBits() {
			EventTypesBase::type result = 0;

			for (size_t i = 0; i < sizeof(EventTypesBase::type) * static_cast<size_t>(8); i++) {
				result |= bit(static_cast<int>(i));
			}

			return result;
		}
	};

	enum class EventTypes : EventTypesBase::type {
		none = 0,

		keyInput = EventTypesBase::bit(0),

		input = keyInput,

		all = EventTypesBase::allBits()
	};

	inline EventTypes operator|(const EventTypes& lhs, const EventTypes& rhs) {
		return static_cast<EventTypes>(
			static_cast<EventTypesBase::type>(lhs) | static_cast<EventTypesBase::type>(rhs)
		);
	}

	inline EventTypes& operator|=(EventTypes& lhs, const EventTypes& rhs) {
		auto& lhsAsBase = *reinterpret_cast<EventTypesBase::type*>(&lhs);

		return *reinterpret_cast<EventTypes*>(
			&(lhsAsBase |= static_cast<EventTypesBase::type>(rhs))
		);
	}

	inline EventTypes operator&(const EventTypes& lhs, const EventTypes& rhs) {
		return static_cast<EventTypes>(
			static_cast<EventTypesBase::type>(lhs) & static_cast<EventTypesBase::type>(rhs)
		);
	}

	inline EventTypes& operator&=(EventTypes& lhs, const EventTypes& rhs) {
		auto& lhsAsBase = *reinterpret_cast<EventTypesBase::type*>(&lhs);

		return *reinterpret_cast<EventTypes*>(
			&(lhsAsBase &= static_cast<EventTypesBase::type>(rhs))
		);
	}

	class EventTypesUtilities {
	public:
		EventTypesUtilities() = delete;

		static inline bool hasAnyOfSameTypes(const EventTypes& lhs, const EventTypes& rhs) {
			return (lhs & rhs) != EventTypes::none;
		}
	};

	class Event {
	private:
		const EventTypes m_types;

	protected:
		Event(EventTypes types) :
			m_types(types)
		{}

	public:
		virtual ~Event() = default;

		inline const EventTypes& getTypes() const { return m_types; }
	};
}
