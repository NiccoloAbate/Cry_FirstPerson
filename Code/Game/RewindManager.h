#pragma once




class CRewindManager
{
public:

	string m_Name = "Test";
	float m_TimeScale = 1;

	void Initialize() {};

	void Test();
	void Serialize(Serialization::IArchive &ar);

	static void Rewind();
};
