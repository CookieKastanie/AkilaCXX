#pragma once

#include <vector>
#include <algorithm>
#include "akila/ecs/signature.hpp"
#include "akila/ecs/entity.hpp"

namespace akila {
	namespace internal {
		class SystemManager;
		class Coordinator;
	}

	class EntitySet {
	public:
		using Iterator = std::vector<Entity>::iterator;
		using ConstIterator = std::vector<Entity>::const_iterator;

		using ReverseIterator = std::vector<Entity>::reverse_iterator;
		using ConstReverseIterator = std::vector<Entity>::const_reverse_iterator;

		inline Iterator begin() { return data.begin(); }
		inline ConstIterator begin() const { return data.begin(); }
		inline ConstIterator cbegin() const { return data.cbegin(); }
		inline Iterator end() { return data.end(); }
		inline ConstIterator end() const { return data.end(); }
		inline ConstIterator cend() const { return data.cend(); }

		inline ReverseIterator rbegin() { return data.rbegin(); }
		inline ConstReverseIterator rbegin() const { return data.rbegin(); }
		inline ConstReverseIterator crbegin() const { return data.crbegin(); }
		inline ReverseIterator rend() { return data.rend(); }
		inline ConstReverseIterator rend() const { return data.rend(); }
		inline ConstReverseIterator crend() const { return data.crend(); }

		Entity front() const { return data.front(); }
		Entity back() const { return data.back(); }

		inline bool empty() { return data.empty(); }
		inline std::size_t size() { return data.size(); }

		inline Entity at(std::size_t index) { return data.at(index); }
		inline Entity operator [](std::size_t index) { return data[index]; }

	private:
		friend class System;

		EntitySet() = default;

		bool insert(Entity e) {
			auto range = std::equal_range(data.begin(), data.end(), e, predicate);

			if(range.first == range.second) {
				data.push_back(e);
				std::sort(data.begin(), data.end(), predicate);
				return true;
			}

			return false;
		}

		bool erase(Entity e) {
			auto range = std::equal_range(data.begin(), data.end(), e, predicate);

			if(range.first != range.second) {
				data.erase(range.first);
				return true;
			}

			return false;
		}

		std::vector<Entity> data;
		std::greater<Entity> predicate;
	};

	class System {
	public:
		System(Signature signature): signature{signature} {};
		virtual ~System() = default;

	protected:
		EntitySet entities;

		virtual void onAdd(Entity entity) {};
		virtual void onRemove(Entity entity) {};

	private:
		friend class internal::SystemManager;
		friend class internal::Coordinator;

		Signature signature;

		bool isCompatible(Signature const &s) {
			return (signature & s) == signature;
		}

		void eraseEntity(internal::EntityId entityId) {
			if(entities.erase(entityId))
				onRemove(entityId);
		}

		void addIfCompatible(internal::EntityId entityId, Signature const &s) {
			if(isCompatible(s)) {
				if(entities.insert(entityId))
					onAdd(entityId);
			}
		}

		void eraseIfNotCompatible(internal::EntityId entityId, Signature const &s) {
			if(!isCompatible(s)) {
				eraseEntity(entityId);
			}
		}
	};
}
