//==============================================================================================
//	FILENAME	:	ScopeGuard.h
//	AUTHOR		:	Juan Pablo Muraira
//	CREATION	:	9/26/01
//	Copyright (C) MicroStrategy Incorporated 2001
//	All Rights Reserved
//==============================================================================================
#ifndef MBase_ScopeGuard_h
#define MBase_ScopeGuard_h

/**
 * Scope guards are used to automatically rollback opertions.
 *
 * Declare a local scope guard with your undo operation. If everything goes ok, then Dismiss()
 * the ScopeGuard and it will have no effect.
 * If an error occurs, or an exception is thrown, the scope guard will execute the
 * undo operation as it goes out of scope. A rollback operation must never throw.
 *
 * ScopeGuard usage:
 *
 * Create a ScopeGuard with code like:
 *
 * 1. For member rollback functions
 *
 * ScopeGuard lGuard = MakeObjectGuard(<object>,&<member_function>);					 // no parameters
 * ScopeGuard lGuard = MakeObjectGuard(<object>,&<member_function>,<param1>);			 // one parameter
 * ScopeGuard lGuard = MakeObjectGuard(<object>,&<member_function>,<param1>,<param2>);	 // two parameters
 *
 * 2. For non-member rollback functions
 *
 * ScopeGuard lGuard = MakeMethodGuard(&<function>);									 // no parameters
 * ScopeGuard lGuard = MakeMethodGuard(&<function>,<param1>);							 // one parameter
 * ScopeGuard lGuard = MakeMethodGuard(&<function>,<param1>,<param2>);					 // two parameters
 *
 * Note: 
 * The pass-in parameter in MakeObjectGuard was defined as pass-in-by-value for historical reason.
 * (The one in MakeMethodGuard was defined as pass-in-by-reference, which is fine)
 * So please use reference or pointer for the pass-in parameter in MakeObjectGuard
 * Refer to the VectorGuard3 in ScopeGuardTest.cpp for detail
 */

namespace MBase
{
	/**
	 * ScopeGuardImplBase is the base class for all scope guards.
	 * We use polymorphism in order to isolate the user from knowing
	 * which particular scope guard he's creating
	 * (for one parameter, for member funtion, etc)
	 */
	class ScopeGuardImplBase
	{
	public:
		/**
		 * Do not call the rollback method
		 * This cannot throw because you can call several in a row
		 */
		void Dismiss() const throw()
		{
			mDismissed = true;
		}

		/**
		 * Do call the rollback method
		 * This cannot throw because you can call several in a row
		 */
		void UnDismiss() const throw()
		{
			mDismissed = false;
		}

	protected:
		ScopeGuardImplBase() throw():
			 mDismissed(false)
		{
		}

		/**
		 * copy constructor aqcuires ownership (destructive copy)
		 * we allow for copy contructor in order to return the correct
		 * scope guard polymorphically with MakeGuard
		 */
		ScopeGuardImplBase(const ScopeGuardImplBase& iScopeGuardImplBase) throw():
			mDismissed(iScopeGuardImplBase.mDismissed)
		{
			iScopeGuardImplBase.Dismiss();
		}

		~ScopeGuardImplBase() throw() // nonvirtual (see below why)
		{
		}

		mutable bool mDismissed;

	private:
		// Disable assignment
		ScopeGuardImplBase& operator=(const ScopeGuardImplBase&);
	};

	/**
	 * ScopeGuardImpl0 implements non-member rollback
	 * function with zero parameters
	 */
	template <class Function>
	class ScopeGuardImpl0 : public ScopeGuardImplBase
	{
	public:
		ScopeGuardImpl0(Function iFunction) throw():
			mFunction(iFunction)
		{
		}
		~ScopeGuardImpl0() throw()
		{
			if (!mDismissed)
			{
				(*mFunction)();
			}
		}
	private:
		Function mFunction;
	};

	/**
	 * Function to create a ScopeGuardImpl0.
	 */
	template <class Function>
	ScopeGuardImpl0<Function> MakeMethodGuard(Function iFunction) throw()
	{
		return ScopeGuardImpl0<Function>(iFunction);
	}

	/**
	 * ScopeGuardImpl1 implements non-member rollback
	 * function with one parameter
	 */
	template <class Function, class Parameter1>
	class ScopeGuardImpl1 : public ScopeGuardImplBase
	{
	public:
		ScopeGuardImpl1(Function iFunction,const Parameter1& iParameter1) throw():
			mFunction(iFunction),
			mParameter1(iParameter1)
		{
		}
		~ScopeGuardImpl1() throw()
		{
			if (!mDismissed)
			{
				(*mFunction)(mParameter1);
			}
		}
	private:
		Function mFunction;
		// TQMS 278351, xfan, 1/18/2011
		const Parameter1& mParameter1;
	};

	/**
	 * Function to create a ScopeGuardImpl1.
	 */
	template <class Function, class Parameter1>
	ScopeGuardImpl1<Function, Parameter1>
		MakeMethodGuard(Function iFunction, const Parameter1& iParameter1) throw()
	{
		return ScopeGuardImpl1<Function, Parameter1>(iFunction, iParameter1);
	}

	/**
	 * ScopeGuardImpl2 implements non-member rollback
	 * function with two parameters
	 */
	template <class Function, class Parameter1, class Parameter2>
	class ScopeGuardImpl2 : public ScopeGuardImplBase
	{
	public:
		ScopeGuardImpl2(Function iFunction,
						const Parameter1& iParameter1,
						const Parameter2& iParameter2) throw():
			mFunction(iFunction),
			mParameter1(iParameter1),
			mParameter2(iParameter2)
		{
		}
		~ScopeGuardImpl2() throw()
		{
			if (!mDismissed)
			{
				(*mFunction)(mParameter1,mParameter2);
			}
		}
	private:
		Function mFunction;
		const Parameter1& mParameter1;
		const Parameter2& mParameter2;
	};

	/**
	 * Function to create a ScopeGuardImpl2.
	 */
	template <class Function, class Parameter1, class Parameter2>
	ScopeGuardImpl2<Function, Parameter1, Parameter2>
		MakeMethodGuard(Function iFunction, const Parameter1& iParameter1,
						const Parameter2& iParameter2) throw()
	{
		return ScopeGuardImpl2<Function, Parameter1, Parameter2>(
			iFunction, iParameter1, iParameter2);
	}

	/**
	 * ScopeGuardImpl3 implements non-member rollback
	 * function with three parameters
	 */
	template <class Function, class Parameter1, class Parameter2, class Parameter3>
	class ScopeGuardImpl3 : public ScopeGuardImplBase
	{
	public:
		ScopeGuardImpl3(Function iFunction,
						const Parameter1& iParameter1,
						const Parameter2& iParameter2,
						const Parameter3& iParameter3) throw():
			mFunction(iFunction),
			mParameter1(iParameter1),
			mParameter2(iParameter2),
			mParameter3(iParameter3)
		{
		}
		~ScopeGuardImpl3() throw()
		{
			if (!mDismissed)
			{
				(*mFunction)(mParameter1,mParameter2,mParameter3);
			}
		}
	private:
		Function mFunction;
		const Parameter1& mParameter1;
		const Parameter2& mParameter2;
		const Parameter3& mParameter3;
	};

	/**
	 * Function to create a ScopeGuardImpl3.
	 */
	template <class Function, class Parameter1, class Parameter2, class Parameter3>
	ScopeGuardImpl3<Function, Parameter1, Parameter2, Parameter3>
		MakeMethodGuard(Function iFunction, const Parameter1& iParameter1,
						const Parameter2& iParameter2,const Parameter3& iParameter3) throw()
	{
		return ScopeGuardImpl3<Function, Parameter1, Parameter2, Parameter3>(
			iFunction, iParameter1, iParameter2, iParameter3);
	}

	/**
	 * ScopeGuardImpl4 implements non-member rollback
	 * function with four parameters
	 */
	template <class Function, class Parameter1, class Parameter2, class Parameter3, class Parameter4>
	class ScopeGuardImpl4 : public ScopeGuardImplBase
	{
	public:
		ScopeGuardImpl4(Function iFunction,
						const Parameter1& iParameter1,
						const Parameter2& iParameter2,
						const Parameter3& iParameter3,
						const Parameter4& iParameter4) throw():
			mFunction(iFunction),
			mParameter1(iParameter1),
			mParameter2(iParameter2),
			mParameter3(iParameter3),
			mParameter4(iParameter4)
		{
		}
		~ScopeGuardImpl4() throw()
		{
			if (!mDismissed)
			{
				(*mFunction)(mParameter1,mParameter2,mParameter3,mParameter4);
			}
		}
	private:
		Function mFunction;
		const Parameter1& mParameter1;
		const Parameter2& mParameter2;
		const Parameter3& mParameter3;
		const Parameter4& mParameter4;
	};

	/**
	 * Function to create a ScopeGuardImpl3.
	 */
	template <class Function, class Parameter1, class Parameter2, class Parameter3, class Parameter4>
	ScopeGuardImpl4<Function, Parameter1, Parameter2, Parameter3, Parameter4>
		MakeMethodGuard(Function iFunction, const Parameter1& iParameter1,
						const Parameter2& iParameter2,const Parameter3& iParameter3,const Parameter4& iParameter4) throw()
	{
		return ScopeGuardImpl4<Function, Parameter1, Parameter2, Parameter3, Parameter4>(
			iFunction, iParameter1, iParameter2, iParameter3, iParameter4);
	}



	/**
	 * ObjectScopeGuardImpl0 implements rollback member function
	 * with zero parameters
	 */
	template <class Object, class MemberFunction>
	class ObjectScopeGuardImpl0 : public ScopeGuardImplBase
	{
	public:
		ObjectScopeGuardImpl0(Object& iObject, MemberFunction iMemberFunction) throw():
			mObject(iObject),
			mMemberFunction(iMemberFunction)
		{
		}
		~ObjectScopeGuardImpl0() throw()
		{
			if (!mDismissed)
			{
				(mObject.*mMemberFunction)();
			}
		}
	private:
		Object& mObject;
		MemberFunction mMemberFunction;
	};

	/**
	 * Function to create a ObjectScopeGuardImpl0.
	 */
	template <class Object, class MemberFunction>
	ObjectScopeGuardImpl0<Object, MemberFunction>
		MakeObjectGuard(Object& iObject, MemberFunction iMemberFunction) throw()
	{
		return ObjectScopeGuardImpl0<Object, MemberFunction>(iObject, iMemberFunction);
	}

	/**
	 * ObjectScopeGuardImpl1 implements rollback member function
	 * with one parameter
	 */
	template <class Object, class MemberFunction, class Parameter1>
	class ObjectScopeGuardImpl1 : public ScopeGuardImplBase
	{
	public:
		ObjectScopeGuardImpl1(	Object& iObject,
								MemberFunction iMemberFunction,
								Parameter1 iParameter1) throw():
			mObject(iObject),
			mMemberFunction(iMemberFunction),
			mParameter1(iParameter1)
		{
		}
		~ObjectScopeGuardImpl1() throw()
		{
			if (!mDismissed)
			{
				(mObject.*mMemberFunction)(mParameter1);
			}
		}
	private:
		Object& mObject;
		MemberFunction mMemberFunction;
		Parameter1 mParameter1;
	};

	/**
	 * Function to create a ObjectScopeGuardImpl1.
	 */
	template <class Object, class MemberFunction, class Parameter1>
	ObjectScopeGuardImpl1<Object, MemberFunction, Parameter1>
		MakeObjectGuard(Object& iObject,
						MemberFunction iMemberFunction,
						Parameter1 iParameter1) throw()
	{
		return ObjectScopeGuardImpl1<Object, MemberFunction, Parameter1>
			(iObject, iMemberFunction, iParameter1);
	}

	/**
	 * ObjectScopeGuardImpl2 implements rollback member function
	 * with two parameters
	 */
	template <class Object, class MemberFunction, class Parameter1, class Parameter2>
	class ObjectScopeGuardImpl2 : public ScopeGuardImplBase
	{
	public:
		ObjectScopeGuardImpl2(	Object& iObject,
								MemberFunction iMemberFunction,
								Parameter1 iParameter1,
								Parameter2 iParameter2) throw():
			mObject(iObject),
			mMemberFunction(iMemberFunction),
			mParameter1(iParameter1),
			mParameter2(iParameter2)
		{
		}
		~ObjectScopeGuardImpl2() throw()
		{
			if (!mDismissed)
			{
				(mObject.*mMemberFunction)(mParameter1, mParameter2);
			}
		}
	private:
		Object& mObject;
		MemberFunction mMemberFunction;
		Parameter1 mParameter1;
		Parameter2 mParameter2;
	};

	/**
	 * Function to create a ObjectScopeGuardImpl2.
	 */
	template <class Object, class MemberFunction, class Parameter1, class Parameter2>
	ObjectScopeGuardImpl2<Object, MemberFunction, Parameter1, Parameter2>
		MakeObjectGuard(Object& iObject,MemberFunction iMemberFunction,
						Parameter1 iParameter1, Parameter2 iParameter2) throw()
	{
		return ObjectScopeGuardImpl2<Object, MemberFunction, Parameter1, Parameter2>
			(iObject, iMemberFunction, iParameter1, iParameter2);
	}

	// typedef ScopeGuard for ease of use (see top of the file)
	typedef const ScopeGuardImplBase& ScopeGuard;
}

#endif // MBase_ScopeGuard_h
