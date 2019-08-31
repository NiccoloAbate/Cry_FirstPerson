#pragma once

#include "Types/MinMaxVar.h"

#include <unordered_map>

template<typename T>
class CStat
{
public:

	T Min;
	T Max;
	T Current;

	CStat() = default;
	CStat(MinMaxVar<T> &Var) : Min(Var.Min), Max(Var.Max), Current(Var.Current) {}
	CStat(T Min, T Max, T Cur) : Min(Min), Max(Max), Current(Cur) {}

	void Clamp()
	{
		if (Current > Max)
			Current = Max;
		else if (Current < Min)
			Current = Min;
	}

	void ResetToMax()
	{
		Current = Max;
	}
	void ResetToMin()
	{
		Current = Min;
	}

	CStat& operator=(const T Cur)
	{
		T Old = Current;
		Current = Cur;
		Clamp();
		if (Current < Old)
		{
			OnDamaged();
			if (Current == 0)
				OnDeath();
		}
		return *this;
	}
	CStat& operator+=(const T Add)
	{
		(*this) = Current + Add;
		return *this;
	}
	CStat& operator-=(const T Sub)
	{
		(*this) = Current - Sub;
		return *this;
	}

	bool operator==(const CStat<T>& Other) const { return (this->Current == Other.Current); }
	bool operator<(const CStat<T>& Other) const { return (this->Current < Other.Current); }
	bool operator<=(const CStat<T>& Other) const { return (this->Current <= Other.Current); }
	bool operator>(const CStat<T>& Other) const { return (this->Current > Other.Current); }
	bool operator>=(const CStat<T>& Other) const { return (this->Current >= Other.Current); }
	operator T() { return Current; }
	operator MinMaxVar<T>() { return MinMaxVar<T>(Min, Max, Current); }

	static void ReflectType(Schematyc::CTypeDesc<CStat<T>>& desc)
	{
		desc.AddMember(&CStat<T>::Current, 'cur', "current", "Current", "Current Value", T());
		desc.AddMember(&CStat<T>::Min, 'min', "min", "Min", "Min", T());
		desc.AddMember(&CStat<T>::Max, 'max', "max", "Max", "Max", T());
	}
	
	using TOnDeathCallback = std::function<void()>;
	using TOnDamagedCallback = std::function<void()>;

	void AddOnDeathCallback(string Key, TOnDeathCallback Callback)
	{
		m_OnDeathCallbacks[Key] = Callback;
	}
	void RemoveOnDeathCallback(string Key)
	{
		if (m_OnDeathCallbacks.find(Key) != m_OnDeathCallbacks.end())
			m_OnDeathCallbacks.erase(Key);
	}
	void AddOnDamagedCallback(string Key, TOnDamagedCallback Callback)
	{
		m_OnDamagedCallbacks[Key] = Callback;
	}
	void RemoveOnDamagedCallback(string Key)
	{
		if (m_OnDamagedCallbacks.find(Key) != m_OnDamagedCallbacks.end())
			m_OnDamagedCallbacks.erase(Key);
	}

	struct StringHasher
	{
		std::size_t operator()(const string& k) const
		{
			return std::hash<std::string>()(k.c_str());
		}
	};

private:

	std::unordered_map<string, TOnDeathCallback, StringHasher> m_OnDeathCallbacks;
	std::unordered_map<string, TOnDamagedCallback, StringHasher> m_OnDamagedCallbacks;

	void OnDeath()
	{
		auto It = m_OnDeathCallbacks.begin();
		while (It != m_OnDeathCallbacks.end())
		{
			It->second();
			It++;
		}
	}
	void OnDamaged()
	{
		auto It = m_OnDamagedCallbacks.begin();
		while (It != m_OnDamagedCallbacks.end())
		{
			It->second();
			It++;
		}
	}

};

