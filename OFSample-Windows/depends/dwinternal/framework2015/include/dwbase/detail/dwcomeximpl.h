template <typename T>
bool IDWComponent::queryXSignal(const char *name, T **ppS)
{
	return queryXSignal(name, (void**)ppS);
}

template <typename T>
bool IDWQComponent::callMethod(const char *method, T arg)
{
	void * _a[] = {0, reinterpret_cast<void*>(&arg)};

	return _callMethod(method, _a, 2);
}

template<typename T1, typename T2>
bool IDWQComponent::callMethod(const char *method, T1 _1, T2 _2)
{
	void * _a[] = {0, reinterpret_cast<void*>(&_1), reinterpret_cast<void*>(&_2)};

	return _callMethod(method, _a, 3);
}

template<typename T1, typename T2, typename T3>
bool IDWQComponent::callMethod(const char *method, T1 _1, T2 _2, T3 _3)
{
	void * _a[] = {0, reinterpret_cast<void*>(&_1), reinterpret_cast<void*>(&_2),
					reinterpret_cast<void*>(&_3)};

	return _callMethod(method, _a, 4);
}

template<typename T1, typename T2, typename T3, typename T4>
bool IDWQComponent::callMethod(const char *method, T1 _1, T2 _2, T3 _3, T4 _4)
{
	void * _a[] = {0, reinterpret_cast<void*>(&_1), reinterpret_cast<void*>(&_2),
					reinterpret_cast<void*>(&_3), reinterpret_cast<void*>(&_4)};

	return _callMethod(method, _a, 5);
}

template<typename T1, typename T2, typename T3, typename T4, typename T5>
bool IDWQComponent::callMethod(const char *method, T1 _1, T2 _2, T3 _3, T4 _4, T5 _5)
{
	void * _a[] = {0, reinterpret_cast<void*>(&_1), reinterpret_cast<void*>(&_2),
					reinterpret_cast<void*>(&_3), reinterpret_cast<void*>(&_4),
					reinterpret_cast<void*>(&_5)};

	return _callMethod(method, _a, 6);
}

template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
bool IDWQComponent::callMethod(const char *method, T1 _1, T2 _2, T3 _3, T4 _4, T5 _5, T6 _6)
{
	void * _a[] = {0, reinterpret_cast<void*>(&_1), reinterpret_cast<void*>(&_2),
					reinterpret_cast<void*>(&_3), reinterpret_cast<void*>(&_4),
					reinterpret_cast<void*>(&_5), reinterpret_cast<void*>(&_6)};

	return _callMethod(method, _a, 7);
}

template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
bool IDWQComponent::callMethod(const char *method, T1 _1, T2 _2, T3 _3, T4 _4, T5 _5, T6 _6, T7 _7)
{
	void * _a[] = {0, reinterpret_cast<void*>(&_1), reinterpret_cast<void*>(&_2),
					reinterpret_cast<void*>(&_3), reinterpret_cast<void*>(&_4),
					reinterpret_cast<void*>(&_5), reinterpret_cast<void*>(&_6),
					reinterpret_cast<void*>(&_7)};

	return _callMethod(method, _a, 8);
}

template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
bool IDWQComponent::callMethod(const char *method, T1 _1, T2 _2, T3 _3, T4 _4, T5 _5, T6 _6, T7 _7, T8 _8)
{
	void * _a[] = {0, reinterpret_cast<void*>(&_1), reinterpret_cast<void*>(&_2),
					reinterpret_cast<void*>(&_3), reinterpret_cast<void*>(&_4),
					reinterpret_cast<void*>(&_5), reinterpret_cast<void*>(&_6),
					reinterpret_cast<void*>(&_7), reinterpret_cast<void*>(&_8)};

	return _callMethod(method, _a, 9);
}