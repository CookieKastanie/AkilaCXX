#pragma once

namespace Akila {
	template<class T>
	class ResourceAnchor;

	template<class T>
	class ResourceReference {
	private:
		friend class ResourceAnchor<T>;

		ResourceAnchor<T> *ra;
		ResourceReference(ResourceAnchor<T> *ra): ra{ra} {}

	public:
		ResourceReference(): ra{nullptr} {}

		ResourceReference &operator=(ResourceReference &&other) {
			ra = std::move(other.ra);
			++ra->refCount;
			return *this;
		}

		~ResourceReference() {
			if(ra != nullptr) --ra->refCount;
		}

		T &operator*() { return *ra->resource; }
		T *operator->() { return ra->resource; }
	};

	template<class T>
	class ResourceAnchor {
	private:
		friend class ResourceReference<T>;

		T *resource;
		unsigned int refCount;

	public:
		ResourceAnchor(): resource{ nullptr }, refCount { 0 } {}
		~ResourceAnchor() { if(resource != nullptr) delete resource; }
		void setResource(T *res) {
			if(resource != nullptr) delete resource;
			resource = res;
		}
		
		ResourceReference<T> createReference() {
			++refCount;
			return { this };
		}
		bool haveReferences() { return refCount != 0; }
	};
}
