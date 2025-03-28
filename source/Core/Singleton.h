#pragma once
#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define DBG_NEW new
#endif
template <class T>
class Singleton
{
public:
	Singleton() {}
	~Singleton() {}

	static T* Get()
	{
		if (s_pInstance_ == nullptr)
		{
			s_pInstance_.reset(DBG_NEW T);
		}
		return s_pInstance_.get();
	}
protected:
	inline static std::unique_ptr<T> s_pInstance_ = nullptr;

private:
	//コピー、ムーブの場合、delete
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
	Singleton(Singleton&&) = delete;
	Singleton& operator=(Singleton&&) = delete;
};
