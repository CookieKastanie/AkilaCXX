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
		~ResourceReference() { --ra->refCount; }

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
		~ResourceAnchor() { delete resource; }
		void setResource(T *res) { resource = res; }
		
		ResourceReference<T> createReference() {
			++refCount;
			return { this };
		}
		bool haveReferences() { return refCount != 0; }
	};
}
