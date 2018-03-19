//==============================================================================================
//	FILENAME	:	SynchObject.h
//	AUTHOR		:	Jing Ning
//	CREATION	:	9/26/01
//	Copyright (C) MicroStrategy Incorporated 2001
//==============================================================================================
#ifndef MSynch_SynchObject_h
#define MSynch_SynchObject_h

///////////////////////////////////////////////////////////////////////////////////////
// SynchObject is just a placeholder. Another synch class derives from it to indicate
// the lock stack it is a "synch object" and therefore may be included in the locking order
///////////////////////////////////////////////////////////////////////////////////////
namespace MSynch 
{
	class SynchObject 
	{
	public:
		/// IsLocked returns if a lock is being currently held.
		/// IsLocked is for debugging purposes only (contention count)
		/// It is not thread safe.
		virtual bool IsLocked() const = 0;
	};
}

#endif // MSynch_SynchObject_h
