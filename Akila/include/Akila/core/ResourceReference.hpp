#pragma once

namespace Akila {
	template<class T>
	class ResourceAnchor;

	template<class T>
	class ResourceReference {
	public:
		union u_RA_void
		{
			ResourceAnchor<T> *ra;
			void *ptr;
		};

	private:
		friend class ResourceAnchor<T>;

		u_RA_void u_ra;

	public:
		ResourceReference(u_RA_void u_ra): u_ra{u_ra} {}
		ResourceReference(): u_ra{nullptr} {}

		//deplacement
		ResourceReference(ResourceReference &&other) {
			u_ra = std::move(other.u_ra);
			++u_ra.ra->refCount;
		}
		ResourceReference &operator=(ResourceReference &&other) {
			u_ra = std::move(other.u_ra);
			++u_ra.ra->refCount;
			return *this;
		}

		// copie
		ResourceReference(ResourceReference const &other) {
			u_ra = other.u_ra;
			++u_ra.ra->refCount;
		}
		ResourceReference operator=(ResourceReference const &other) {
			u_ra = other.u_ra;
			++u_ra.ra->refCount;
			return *this;
		}

		~ResourceReference() {
			if(u_ra.ra != nullptr) --u_ra.ra->refCount;
		}

		T &operator*() const { return *u_ra.ra->resource; }
		T *operator->() const { return u_ra.ra->resource; }

		T *raw() const { return u_ra.ra->resource; }
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

			ResourceReference<T>::u_RA_void u_ra;
			u_ra.ptr = this;

			return {u_ra};
		}

		template<class SubT>
		ResourceReference<SubT> createReference() {
			++refCount;

			ResourceReference<SubT>::u_RA_void u_ra;
			u_ra.ptr = this;

			return {u_ra};
		}

		bool haveReferences() { return refCount != 0; }
	};
}
