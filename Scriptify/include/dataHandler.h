#pragma once

#include <vector>
#include <unordered_map>

namespace Scriptify {
	constexpr float c_TrainSetPercent = 0.75f;
	constexpr float c_TestSetPercent = 0.20f;
	constexpr float c_ValidationPercent = 0.05f;

	struct Data
	{
		Data();
		~Data();

		std::vector<uint8_t*>* FeatureVector;
		uint8_t Label;
		int EnumLabel;
	};

	struct OutData {
		std::vector<Data*> TrainingData;
		std::vector<Data*> TestData;
		std::vector<Data*> ValidationData;
	};

	class DataHandler {
	public:
		DataHandler();
		~DataHandler();

		void ReadFeatureVector(const char* path);
		void ReadFeatureLabels(const char* path);
		void SplitData();
		void CountClasses();
		uint32_t ConvertToLittleEndian(const unsigned char* bytes);

		OutData* GetData();

	private:
		std::vector<Data*> m_RawData;
		OutData* m_RetData;
		int m_ClassesAmount;
		int m_FeatureVectorSize;

		std::unordered_map<uint8_t, int> m_ClassMap;
	};
}