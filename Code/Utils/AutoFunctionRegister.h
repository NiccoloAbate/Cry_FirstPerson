#pragma once


template<typename TYPE> class CStaticInstanceList
{
private:

	struct SInstanceList
	{
		CStaticInstanceList* pFirstInstance = nullptr;
	};

public:

	inline CStaticInstanceList()
	{
		SInstanceList& instanceList = GetInstanceList();
		m_pNextInstance = instanceList.pFirstInstance;
		instanceList.pFirstInstance = this;
	}

	virtual ~CStaticInstanceList() {}

	static inline TYPE* GetFirstInstance()
	{
		return static_cast<TYPE*>(GetInstanceList().pFirstInstance);
	}

	inline TYPE* GetNextInstance() const
	{
		return static_cast<TYPE*>(m_pNextInstance);
	}

private:

	static inline SInstanceList& GetInstanceList()
	{
		static SInstanceList instanceList;
		return instanceList;
	}

private:

	CStaticInstanceList* m_pNextInstance;
};
