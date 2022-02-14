#pragma once

#include <memory>

namespace Akila {
	template<typename T>
	using UniquePtr = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr UniquePtr<T> createUniquePtr(Args&& ... args) {
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ptr = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ptr<T> createPtr(Args&& ... args) {
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	constexpr Ptr<T> createPtr(T* obj) {
		return std::shared_ptr<T>(obj);
	}

	///////////////////////////////////////////////////////

	template<class T>
	class RefAnchor;

	template<class T>
	class Ref {
	public:
		union u_RA_void
		{
			RefAnchor<T> *ra;
			void *ptr;
		};

	private:
		friend class RefAnchor<T>;

		u_RA_void u_ra;

	public:
		Ref(u_RA_void u_ra): u_ra{u_ra} {}
		Ref(): u_ra{nullptr} {}

		//deplacement
		Ref(Ref &&other) {
			u_ra = other.u_ra;
			other.u_ra.ptr = nullptr;
		}
		Ref &operator=(Ref &&other) noexcept {
			u_ra = other.u_ra;
			other.u_ra.ptr = nullptr;

			return *this;
		}

		// copie
		Ref(Ref const &other) {
			u_ra = other.u_ra;
			++u_ra.ra->refCount;
		}
		Ref operator=(Ref const &other) {
			u_ra = other.u_ra;
			++u_ra.ra->refCount;
			return *this;
		}

		~Ref() {
			if(u_ra.ra != nullptr) --u_ra.ra->refCount;
		}

		T &operator*() const { return *u_ra.ra->resource; }
		T *operator->() const { return u_ra.ra->resource; }

		T *raw() const { return u_ra.ra->resource; }
	};

	template<class T>
	class RefAnchor {
	private:
		friend class Ref<T>;

		T *resource;
		unsigned int refCount;

	public:
		RefAnchor(): resource{nullptr}, refCount{0} {}
		RefAnchor(RefAnchor const &) = delete;
		RefAnchor(RefAnchor &&) = delete;
		~RefAnchor() {
			if(haveReferences()) std::cout << "RefAnchor deleted with " << refCount << " refs !\n";
			if(resource != nullptr) delete resource;
		}

		void setValue(T *res) {
			if(resource != nullptr) delete resource;
			resource = res;
		}

		Ref<T> createReference() {
			++refCount;

			Ref<T>::u_RA_void u_ra;
			u_ra.ptr = this;

			return {u_ra};
		}

		template<class SubT>
		Ref<SubT> createReference() {
			++refCount;

			Ref<SubT>::u_RA_void u_ra;
			u_ra.ptr = this;

			return {u_ra};
		}

		bool haveReferences() { return refCount != 0; }
	};
}
