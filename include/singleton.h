#ifndef _SINGLETON_H_
#define _SINGLETON_H_

#include <exception.h>

namespace ssuge
{
	/// Any class that wished to be a Singleton should derive itself
	/// from this class.  Warning: the derived class must provide a 
	/// template specialization of msSingleton (to set it to nullptr) -- 
	/// usually in their derived_class.cpp file.
	template <class T>
	class Singleton
	{
	// @@@@@ ATTRIBUTES @@@@@ //
	protected:
		/// The pointer to the singleton
		static T * msSingleton;

	// @@@@@ CONSTRUCTOR / DESTRUCTOR @@@@@ //
	public:
		/// The default constructor
		explicit Singleton()
		{
			if (msSingleton != nullptr)
				THROW_EXCEPTION("Singleton already exists!");
			msSingleton = static_cast<T*>(this);
		}

		/// The destructor.  
		virtual ~Singleton()
		{
			msSingleton = nullptr;
		}

	// @@@@@ METHODS @@@@@ //
	public:
		/// Returns a pointer to the one-and-only singleton (throws a ssurge::Exception if no singleton).
		static T * getSingletonPtr()
		{
			if (msSingleton == nullptr)
				THROW_EXCEPTION("Singleton does not exist!");
			return msSingleton;
		}

	};

}

#endif