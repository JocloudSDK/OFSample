#ifndef __LIBS_SYSBASE_SYSBASE_DETAIL_LEAK_DETECT_H__
#define __LIBS_SYSBASE_SYSBASE_DETAIL_LEAK_DETECT_H__
#pragma once

#include <set>

#ifdef __LEAKCOUNT_DETECTION
	typedef CXMutex Mutex;
	typedef CXMutexLock MutexLock;
	template <typename Ty_>
	class LeakCounterSet {
	 public:
		static void Add(Ty_ *obj) {
			MutexLock lock(mu_);
			my_set_.insert(obj);
		}

		static void Remove(Ty_ *obj) {
			MutexLock lock(mu_);
			my_set_.erase(obj);
		}

	 private:
		static std::set<Ty_*> my_set_;
		static Mutex mu_;
	};

	template <typename Ty_>
	std::set<Ty_*> LeakCounterSet<Ty_>::my_set_;

	template <typename Ty_>
	Mutex LeakCounterSet<Ty_>::mu_;

	template <typename Ty_>
	void AddIntoLeakCounterSet(Ty_ *obj) {
		LeakCounterSet<Ty_>::Add(obj);
	}
	template <typename Ty_>
	void RemoveFromLeakCounterSet(Ty_ *obj) {
		LeakCounterSet<Ty_>::Remove(obj);
	}

#define ADD_LEAK_COUNTER_DETAIL(value) \
		AddIntoLeakCounterSet(value)

#define REMOVE_LEAK_COUNTER_DETAIL(value) \
		RemoveFromLeakCounterSet(value)
#else //__LEAKCOUNT_DETECTION

#define ADD_LEAK_COUNTER_DETAIL(value)
#define REMOVE_LEAK_COUNTER_DETAIL(value)

#endif //__LEAKCOUNT_DETECTION


#endif //__LIBS_SYSBASE_SYSBASE_DETAIL_LEAK_DETECT_H__
