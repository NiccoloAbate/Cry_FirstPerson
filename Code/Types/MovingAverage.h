#pragma once

#include <array>
#include <numeric>

template<typename T, size_t SAMPLES_COUNT>
class MovingAverage
{
	static_assert(SAMPLES_COUNT > 0, "SAMPLES_COUNT shall be larger than zero!");

public:

	MovingAverage()
		: m_values()
		, m_cursor(SAMPLES_COUNT)
		, m_accumulator()
	{
	}

	MovingAverage& Push(const T& value)
	{
		if (m_cursor == SAMPLES_COUNT)
		{
			m_values.fill(value);
			m_cursor = 0;
			m_accumulator = std::accumulate(m_values.begin(), m_values.end(), T(0));
		}
		else
		{
			m_accumulator -= m_values[m_cursor];
			m_values[m_cursor] = value;
			m_accumulator += m_values[m_cursor];
			m_cursor = (m_cursor + 1) % SAMPLES_COUNT;
		}

		return *this;
	}

	T Get() const
	{
		return m_accumulator / T(SAMPLES_COUNT);
	}

	void Reset()
	{
		m_cursor = SAMPLES_COUNT;
	}

private:

	std::array<T, SAMPLES_COUNT> m_values;
	size_t m_cursor;

	T m_accumulator;
};
