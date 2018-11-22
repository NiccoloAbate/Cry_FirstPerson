#pragma once


class CGeneric
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
	static CGeneric* New(T Val);
};


template <typename T>
class CGenericVar : public CGeneric
{
	friend class CGeneric;

	UINT_PTR GetPTR() override { return (UINT_PTR)&m_Var; }
	void SetWithPTR(UINT_PTR Ptr) override { T *pPtr = (T*)Ptr; m_Var = *pPtr; }
	void SetPTR(UINT_PTR Ptr) override { T *pPtr = (T*)Ptr; *pPtr = m_Var; }

private:
	CGenericVar(T Var) : m_Var(Var) {}

	void Set(T Val) { m_Var = Val; }
	T& Get() { return m_Var; }

	T m_Var;
};

template<typename T>
inline T& CGeneric::Get()
{
	CGenericVar<T> *pCast = (CGenericVar<T>*)this;
	return pCast->Get();
}

template<typename T>
inline void CGeneric::Set(T Val)
{
	CGenericVar<T> *pCast = (CGenericVar<T>*)this;
	pCast->Set(Val);
}

template<typename T>
inline CGeneric * CGeneric::New(T Val)
{
	return new CGenericVar<T>(Val);
}


