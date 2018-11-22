#pragma once



class CSerializationEntension
{
public:

	typedef DynArray<char> FrameBuffer;
	typedef std::function<void(Serialization::IArchive&)> SerializationFunction;
	typedef UINT_PTR Pointer;

	struct FrameInfoBuffer
	{
		static float fFrameTime;
	};

	void Serialize(Serialization::IArchive &ar);

	void AddSerializationExtension(SerializationFunction Function);

private:
	std::vector<SerializationFunction> m_SerializationFunctions;
};

