#pragma once

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
		Ref(Ref &&other) {
			ra = other.ra;
		}

		Ref &operator=(Ref &&other) noexcept {
			if(ra != nullptr) --ra->refCount;
			if(other.ra != nullptr) ++other.ra->refCount;

			ra = other.ra;

			return *this;
		}

		// copie
		Ref(Ref const &other) {
			ra = other.ra;
			++ra->refCount;
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

	template<class T>
	class RefAnchor {
	public:
		RefAnchor(): resource{nullptr}, refCount{0} {}
		RefAnchor(RefAnchor const &) = delete;
		RefAnchor(RefAnchor &&) = delete;
		~RefAnchor() {
			if(haveReferences()) std::cerr << "RefAnchor deleted with " << refCount << " refs !\n";
			if(resource != nullptr) delete resource;
		}

		void setValue(T *res) {
			if(resource != nullptr) delete resource;
			resource = res;
		}

		Ref<T> createReference() {
			return {this};
		}

		bool haveReferences() const { return refCount != 0; }

	private:
		friend class Ref<T>;

		T *resource;
		unsigned int refCount;
	};
}
