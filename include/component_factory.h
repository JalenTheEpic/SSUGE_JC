#ifndef _COMPONENT_FACTORY_H_
#define _COMPONENT_FACTORY_H_

#include <stdafx.h>
#include <singleton.h>
#include <exception.h>
#include <game_object.h>

namespace ssuge
{

	/// This is the base class for (mainly component-based) factories.
	template <class T> class ComponentFactory
	{
	// @@@@@ ATTRIBUTES @@@@@ //
	protected:
		/// The array of components that is pre-allocated when the Factory is created.
		/// "Slots" within this are doled out as requested by the components themselves.
		/// All used slots are at the beginning of the pool.
		T ** mComponentPool;

		/// How many elements in the pool have we used so far?
		unsigned int mUsedSize;

		/// The total size of the pool.
		unsigned int mMaxPoolSize;

		/// A convenience mapping to quickly find a position based on a pointer to
		/// a component.
		std::map<T*, int> mSlotMappings;

	// @@@@@ CONSTRUCTORS / DESTRUCTORS @@@@@ //
	public:
		/// The constructor.  max_size is a hard limit.  Once that many components have
		/// been doled out, an exception is raised.
		explicit ComponentFactory(unsigned int max_size) : mComponentPool(nullptr), mUsedSize(0), mMaxPoolSize(0)
		{
			mComponentPool = new T*[max_size];
			if (mComponentPool)
				mMaxPoolSize = max_size;
		}

		/// Destroys the array of components (The components are destroyed by the parent game object) 
		virtual ~ComponentFactory()
		{
			if (mComponentPool)
				delete[] mComponentPool;
			mComponentPool = nullptr;
			mUsedSize = mMaxPoolSize = 0;
		}


	// @@@@@ METHODS @@@@@ //
	public:
		/// Reserves one spot in the master pool and returns it.  This method should be specialized
		/// if allocate needs additional parameters.
		T * allocate(GameObject * gobj)
		{
			T * c = new T(gobj);
			gobj->addComponent(c);
			storePtr(c);
			return c;
		}


		/// Releases a pointer from the pool.  Components should call this in their destructors.
		virtual void releasePtr(T * ptr)
		{
			// Make sure we actually have this pointer -- should we raise an exception if not?
			std::map<T*, int>::iterator iter = mSlotMappings.find(ptr);
			if (iter != mSlotMappings.end())
			{
				int slot = iter->second;
				mSlotMappings.erase(iter);
				for (unsigned int i = slot + 1; i < mUsedSize; i++)
				{
					mComponentPool[i - 1] = mComponentPool[i];
					mSlotMappings[mComponentPool[i - 1]] = i - 1;
				}
			}
#if _DEBUG
			else
			{
				THROW_EXCEPTION("We don't actually store the pointer you just tried to release!");
			}
#endif
		}


	protected:
		/// Takes an existing pointer and stores it in the pool.
		virtual void storePtr(T * ptr)
		{
			if (mUsedSize == mMaxPoolSize)
				THROW_EXCEPTION("Component Factory has reached capacity!");
			
			// I want a better way to do this...right now, we're storing all the
			// component pointers, but they're still all over the place.  Cache-coherency
			// will probably stink...
			mComponentPool[mUsedSize] = ptr;
			mSlotMappings[ptr] = mUsedSize;
			mUsedSize++;
		}	
	};

}

#endif
