#pragma once

#include "Generic.h"


class CBindableGeneric : public CGeneric
{
public:
	template<typename T>
	T& Get();
	virtual UINT_PTR GetPTR() = 0;

	template<typename T>
	void Set(T Val);
	virtual void SetWithPTR(UINT_PTR Ptr) = 0;

	// Sets Var at Ptr to internal generic value
	virtual void SetPTR(UINT_PTR Ptr) = 0;

	template<typename T>
	static CBindableGeneric* New(T Val);


	void BindPTR(UINT_PTR Ptr)
	{
		if (std::find(m_BoundPtrs.begin(), m_BoundPtrs.end(), Ptr) != m_BoundPtrs.end())
			return;
		m_BoundPtrs.push_back(Ptr);
	}

	void UpdateBoundPTRs()
	{
		for (int i = 0; i < m_BoundPtrs.size(); i++)
		{
			SetPTR(m_BoundPtrs[i]);
		}
	}
	void UpdateBoundPTRs(UINT_PTR Exclusion)
	{
		for (int i = 0; i < m_BoundPtrs.size(); i++)
		{
			if (m_BoundPtrs[i] != Exclusion)
				SetPTR(m_BoundPtrs[i]);
		}
	}

private:
	std::vector<UINT_PTR> m_BoundPtrs;
};

template<typename T>
class CBindableGenericVar : public CBindableGeneric
{
public:
	friend class CBindableGeneric;

	UINT_PTR GetPTR() override { return (UINT_PTR)&m_Var; }
	void SetWithPTR(UINT_PTR Ptr) override { T *pPtr = (T*)Ptr; m_Var = *pPtr; UpdateBoundPTRs(Ptr); }
	void SetPTR(UINT_PTR Ptr) override { T *pPtr = (T*)Ptr; *pPtr = m_Var; }

private:
	CBindableGenericVar(T Var) : m_Var(Var) {}

	void Set(T Val) { m_Var = Val; UpdateBoundPTRs(); }
	T& Get() { return m_Var; }

	T m_Var;

	std::vector<UINT_PTR> m_BoundPtrs;
};

template<typename T>
inline T & CBindableGeneric::Get()
{
	CBindableGenericVar<T> *pCast = (CBindableGenericVar<T>*)this;
	return pCast->Get();
}

template<typename T>
inline void CBindableGeneric::Set(T Val)
{
	CBindableGenericVar<T> *pCast = (CBindableGenericVar<T>*)this;
	pCast->Set(Val);
}

template<typename T>
inline CBindableGeneric* CBindableGeneric::New(T Val)
{
	return new CBindableGenericVar<T>(Val);
}
