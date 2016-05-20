#ifndef _SINGLETON_H_
#define _SINGLETON_H_

template <typename T> 
class Singleton {
public:
	static T *GetInstance() {
		if (NULL == s_pInst) {
			s_pInst = new T();
		}
		return s_pInst;
	}
protected:
	Singleton(){}
	Singleton(const Singleton<T> &rhs) {};
	T *operator=(const Singleton<T> &rhs);
private:
	static T *s_pInst;
};
template<typename T> T *Singleton<T>::s_pInst = NULL;
#endif