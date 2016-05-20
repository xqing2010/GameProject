#ifndef _FIXLENBUFFPOOL_H_
#define _FIXLENBUFFPOOL_H_

#include <vector>
#include "BaseTypeDef.h"
#include "ComDef.h"
#include "Singleton.h"

using std::vector;

template <INT blockSize = 8>
class FixLenBuffPool : public Singleton<FixLenBuffPool<blockSize> >
{
	friend class Singleton<FixLenBuffPool<blockSize> >;
protected:
	FixLenBuffPool()
	{
		ReAlloc();
	}
public:
	virtual ~FixLenBuffPool()
	{
		Release();
	}

public:
	char *Alloc()
	{
		if (nullptr == m_pUnUsed)
		{
			ReAlloc();
		}

		if (nullptr == m_pUnUsed)
		{
			//TODO LOG  MEMORY USED
			return nullptr;
		}

		char *pTemp = m_pUnUsed;

		m_pUnUsed = (char *)(*(int *)m_pUnUsed);
		return pTemp;
	}

	void Recycle(char *pData)
	{
		*(int *)pData = (int)m_pUnUsed;
		m_pUnUsed = pData;
	}
protected:
	void ReAlloc()
	{
		char *pData = (char *)malloc(blockSize * BLOCK_NUM);
		int num = BLOCK_NUM;

		m_oAllocs.push_back(pData);
		char *pTmp = pData;

		for (int index = 0; index < num - 1; ++index)
		{
			*(int *)(pTmp) = (int)(pTmp + blockSize);
			pTmp += blockSize;
		}

		(char *)(pTmp) = m_pUnUsed;
		m_pUnUsed = pData;
	}

	void Release()
	{
		for (int index = 0; index < m_oAllocs.size(); ++index)
		{
			free(m_oAllocs[index]);
		}

		m_oAllocs.clear();
	}

	 
private:
	char *m_pUnUsed;
	vector<char *> m_oAllocs;
};

typedef FixLenBuffPool<1024> StreamBuff;

#endif // FIXLENBUFFPOOL_H_
