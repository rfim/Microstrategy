//==============================================================================================
//	FILENAME	:	Timer.h
//	AUTHOR		:	vovechkin
//	CREATION	:	2004-06-14
//	Copyright (C) MicroStrategy Incorporated 2004
//	All Rights Reserved
//==============================================================================================
#ifndef MBase_Timer_h
#define MBase_Timer_h

#include "Base.h"
#include "ReturnString.h"

#include "../Defines/BasicTypes.h"

namespace MBase
{
	/**
	 * The Timer class can be used to measure time intervals with
	 * microsecond precision. The start time is always the moment
	 * of creation of Timer instance, and the end time is whenever 
	 * GetAgeInMicroseconds is called.
	 * 
	 * Example:
	 *
	 *	MBase::Timer lTimer;
	 *
	 *	// do something here
	 *
	 *	::printf("it took %ld usec.", lTimer.GetAgeInMicroseconds());
	 */
	class DLL_BASE_EXIM Timer
	{
	public:

		Timer();

		//akini 08-17-2006
		void ResetTimer();

		unsigned Int64 GetAgeInMicroseconds() const;

		inline unsigned Int64 GetAgeInMilliseconds() const
		{
			return GetAgeInMicroseconds() / 1000;
		}

		inline unsigned Int64 GetAgeInSeconds() const
		{
			return GetAgeInMicroseconds() / 1000000;
		}

	private:

#ifdef WIN32
		unsigned Int64 mStartTimeInPerformanceCounterUnits;
#else
		unsigned Int64 mStartTimeInMicroseconds;
#endif
	};

	/**
	 * The TotalTimer class can be used to measure time intervals with
	 * microsecond precision. TotalTimer accumulates single measurements 
	 * taken by the TotalTimer::Timer class.
	 *
	 * Using TotalTimer for multiple measurements may result in better
	 * precision than summing multiple microsecond values.
	 * 
	 * Example:
	 *
	 *	MBase::TotalTimer lTotalTimer;
	 *
	 *	// measure action 1
	 *  {
	 *      MBase::TotalTimer::Timer lTimer(lTotalTimer);
	 *	    Action1();
	 *  }
	 *
	 *  // do not measure here
	 *  ActionZ();
	 *
	 *  // measure action 2
	 *  {
	 *      MBase::TotalTimer::Timer lTimer(lTotalTimer);
	 *	    Action2();
	 *  }
	 *
	 *	::printf("Two actions took %ld usec.", lTimer.GetTotalInMicroseconds());
	 */
	class DLL_BASE_EXIM TotalTimer
	{
	public:

		/**
		 * A single-interval measurement for one TotalTimer instance.
		 */
		class DLL_BASE_EXIM Timer
		{
		public:

			Timer(TotalTimer& irTotalTimer);

			~Timer() throw()
			{
				if (mpTotalTimer != NULL)
				{
					Stop();
				}
			}

			void Stop() throw();

		private:

			TotalTimer* mpTotalTimer;
#ifdef WIN32
			unsigned Int64 mStartTimeInPerformanceCounterUnits;
#else
			unsigned Int64 mStartTimeInMicroseconds;
#endif
		};

		/**
		 * A single-interval measurement for two TotalTimer instances.
		 */
		class DLL_BASE_EXIM Timer2
		{
		public:

			Timer2(TotalTimer& irTotalTimer1, TotalTimer& irTotalTimer2);
			~Timer2() throw()
			{
				if (mpTotalTimer1 != NULL)
				{
					Stop();
				}
			}

			void Stop() throw();

		private:

			TotalTimer* mpTotalTimer1;
			TotalTimer* mpTotalTimer2;
#ifdef WIN32
			unsigned Int64 mStartTimeInPerformanceCounterUnits;
#else
			unsigned Int64 mStartTimeInMicroseconds;
#endif
		};

		/**
		 * A single-interval measurement for three TotalTimer instances.
		 */
		class DLL_BASE_EXIM Timer3
		{
		public:

			Timer3(TotalTimer& irTotalTimer1, TotalTimer& irTotalTimer2, TotalTimer& irTotalTimer3);
			~Timer3() throw()
			{
				if (mpTotalTimer1 != NULL)
				{
					Stop();
				}
			}

			void Stop() throw();

		private:

			TotalTimer* mpTotalTimer1;
			TotalTimer* mpTotalTimer2;
			TotalTimer* mpTotalTimer3;
#ifdef WIN32
			unsigned Int64 mStartTimeInPerformanceCounterUnits;
#else
			unsigned Int64 mStartTimeInMicroseconds;
#endif
		};

	public:

		TotalTimer():
#ifdef WIN32
			mTotalTimeInPerformanceCounterUnits(0),
#else
			mTotalTimeInMicroseconds(0),
#endif
			mIncrementCount(0)
		{
		}

		/**
		 * @return total measured time in microseconds.
		 */
		unsigned Int64 GetTotalInMicroseconds() const;

		inline unsigned Int64 GetTotalInMilliseconds() const
		{
			return GetTotalInMicroseconds() / 1000;
		}

		inline unsigned Int64 GetTotalInSeconds() const
		{
			return GetTotalInMicroseconds() / 1000000;
		}

		/**
		 * @return average measured time in microseconds.
		 */
		inline unsigned Int64 GetAverageInMicroseconds() const
		{
			return (mIncrementCount > 0) ? GetTotalInMicroseconds() / mIncrementCount : 0;
		}

		inline unsigned Int64 GetAverageInMilliseconds() const
		{
			return GetAverageInMicroseconds() / 1000;
		}

		inline unsigned Int64 GetAverageInSeconds() const
		{
			return GetAverageInMicroseconds() / 1000000;
		}

		/**
		 * Get the number of times the total time has been incremented.
		 */
		inline unsigned Int64 GetIncrementCount() const
		{
			return mIncrementCount;
		}

	private:

		friend class Timer;
		friend class Timer2;
		friend class Timer3;

#ifdef WIN32
		unsigned Int64 mTotalTimeInPerformanceCounterUnits;
#else
		unsigned Int64 mTotalTimeInMicroseconds;
#endif
		unsigned Int64 mIncrementCount;
	};
}

#endif // MBase_Timer_h
