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

		//deplacement
		/*/
		ResourceReference &operator=(ResourceReference &&other) {
			ra = std::move(other.ra);
			++ra->refCount;
			return *this;
		}
		//*/
		ResourceReference(ResourceReference &&other) {
			ra = std::move(other.ra);
			++ra->refCount;
		}
		ResourceReference &operator=(ResourceReference &&other) {
			//return { other };
			ra = std::move(other.ra);
			++ra->refCount;
			return *this;
		}

		// copie
		/*/
		ResourceReference &operator=(ResourceReference const &other) {
			ra = other.ra;
			++ra->refCount;
			return *this;
		}
		//*/
		ResourceReference(ResourceReference const &other) {
			ra = other.ra;
			++ra->refCount;
		}
		ResourceReference operator=(ResourceReference const &other) {
			//return { other };
			ra = other.ra;
			++ra->refCount;
			return *this;
		}

		~ResourceReference() {
			if(ra != nullptr) --ra->refCount;
		}

		//T &operator*() { return *ra->resource; }
		//T *operator->() { return ra->resource; }

		T &operator*() const { return *ra->resource; }
		T *operator->() const { return ra->resource; }

		//T *raw() { return ra->resource; }
		T *raw() const { return ra->resource; }
	};

	template<class T>
	class ResourceAnchor {
	private:
		friend class ResourceReference<T>;

		T *resource;
		unsigned int refCount;

	public:
		ResourceAnchor(): resource{ nullptr }, refCount { 0 } {}
		~ResourceAnchor() {
			if(haveReferences()) std::cout << "Anchor deleted with " << refCount << " refs !\n";
			if(resource != nullptr) delete resource;
		}

		void setResource(T *res) {
			if(resource != nullptr) delete resource;
			resource = res;
		}
		
		ResourceReference<T> createReference() {
			++refCount;
			return { this };
		}

		template<class SubT>
		ResourceReference<SubT> createReference() {
			++refCount;
			return {this};
		}

		bool haveReferences() { return refCount != 0; }
	};
}
