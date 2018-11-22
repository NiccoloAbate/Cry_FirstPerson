#pragma once


template<typename T>
struct MinMaxVar
{
	T Min;
	T Max;
	T Current;

	MinMaxVar() = default;
	MinMaxVar(T Min, T Max, T Cur) : Min(Min), Max(Max), Current(Cur) {}

	void Clamp()
	{
		if (Current > Max)
			Current = Max;
		else if (Current < Min)
			Current = Min;
	}

	MinMaxVar& operator=(const T Cur)
	{
		Current = Cur;
		Clamp();
		return *this;
	}
	MinMaxVar& operator+=(const T Add)
	{
		Current += Add;
		Clamp();
		return *this;
	}
	MinMaxVar& operator-=(const T Sub)
	{
		Current -= Sub;
		Clamp();
		return *this;
	}

	bool operator==(const MinMaxVar<T>& Other) const { return (this->Current == Other.Current); }
	bool operator<(const MinMaxVar<T>& Other) const { return (this->Current < Other.Current); }
	bool operator<=(const MinMaxVar<T>& Other) const { return (this->Current <= Other.Current); }
	bool operator>(const MinMaxVar<T>& Other) const { return (this->Current > Other.Current); }
	bool operator>=(const MinMaxVar<T>& Other) const { return (this->Current >= Other.Current); }
	operator T() { return Current; }

	static void ReflectType(Schematyc::CTypeDesc<MinMaxVar<T>>& desc)
	{
		desc.AddMember(&MinMaxVar<T>::Current, 'cur', "current", "Current", "Current Value", T());
		desc.AddMember(&MinMaxVar<T>::Min, 'min', "min", "Min", "Min", T());
		desc.AddMember(&MinMaxVar<T>::Max, 'max', "max", "Max", "Max", T());
	}
};
