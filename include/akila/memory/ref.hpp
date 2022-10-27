#pragma once

#include <iostream>

namespace akila {
	template<class T>
	class RefAnchor;

	template<class T>
	class Ref {
	public:
		Ref(RefAnchor<T> *ra): ra{ra} { ++ra->refCount; }
		Ref(void *ra): ra{static_cast<RefAnchor<T>*>(ra)} { ++this->ra->refCount; }
		Ref(): ra{nullptr} {}

		//deplacement
		Ref(Ref &&other) noexcept: ra{other.ra} {
			if(ra != nullptr) ++ra->refCount;
		}

		Ref &operator=(Ref &&other) noexcept {
			if(ra != nullptr) --ra->refCount;
			if(other.ra != nullptr) ++other.ra->refCount;

			ra = other.ra;

			return *this;
		}

		// copie
		Ref(Ref const &other): ra{other.ra} {
			if(ra != nullptr) ++ra->refCount;
		}

		Ref &operator=(Ref const &other) {
			if(ra != nullptr) --ra->refCount;
			if(other.ra != nullptr) ++other.ra->refCount;

			ra = other.ra;

			return *this;
		}

		~Ref() {
			if(ra != nullptr) --ra->refCount;
		}

		constexpr T &operator*() const { return *ra->resource; }
		constexpr T *operator->() const { return ra->resource; }

		constexpr T *const raw() const { return ra->resource; }
		constexpr operator T *const () const { return ra->resource; }

		template<class C>
		constexpr operator Ref<C> const () const { return {ra}; }

		bool isValid() { return ra != nullptr; }

	private:
		friend class RefAnchor<T>;

		RefAnchor<T> *ra;
	};


	class IRefAnchor {
	public:
		IRefAnchor(): refCount{0} {}
		IRefAnchor(IRefAnchor const &) = delete;
		IRefAnchor &operator=(IRefAnchor const &) = delete;
		IRefAnchor(IRefAnchor &&) = delete;
		IRefAnchor &operator=(IRefAnchor &&) = delete;

		virtual ~IRefAnchor() = default;

		unsigned int getRefCount() const { return refCount; }
		bool haveReferences() const { return refCount != 0; }

	protected:
		unsigned int refCount;
	};

	template<class T>
	class RefAnchor: public IRefAnchor {
	public:
		RefAnchor(): IRefAnchor{}, resource{nullptr} {}
		RefAnchor(RefAnchor const &) = delete;
		RefAnchor &operator=(RefAnchor const &) = delete;
		RefAnchor(RefAnchor &&) = delete;
		RefAnchor &operator=(RefAnchor &&) = delete;

		~RefAnchor() {
			if(haveReferences()) std::cerr << "RefAnchor deleted with " << refCount << " refs !\n";
			if(resource != nullptr) delete resource;
		};

		void setValue(T *res) {
			if(resource != nullptr) delete resource;
			resource = res;
		}

		Ref<T> createReference() {
			return {this};
		}

	private:
		friend class Ref<T>;

		T *resource;
	};
}
