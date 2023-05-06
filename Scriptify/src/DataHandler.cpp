#include "DataHandler.h"

#include <unordered_set>

Scriptify::Data::Data()
{
	FeatureVector = new std::vector<uint8_t*>();
}

Scriptify::Data::~Data()
{
	if (FeatureVector)
		delete FeatureVector;
}

Scriptify::DataHandler::DataHandler()
{
	m_RetData = new OutData();
}

Scriptify::DataHandler::~DataHandler()
{
	if (m_RetData) {
		delete m_RetData;
	}
}

void Scriptify::DataHandler::ReadFeatureVector(const char* path)
{
	uint32_t header[4]{}; // HEADER VALUES: Magic, Number of Images, Row Size, Column Size
	unsigned char bytes[4];

	printf("Getting input file headers from: %s\n", path);
	FILE* f = fopen(path, "rb");
	if (!f) {
		fprintf(stderr, "Failed to open input file to get header\n");
		exit(1);
	}

	// Get each value in the header
	for (int i = 0; i < 4; i++) {
		if (!fread(bytes, sizeof(bytes), 1, f)) {
			fprintf(stderr, "Failed read header from file\n");
			fclose(f);
			exit(1);
		}

		header[i] = ConvertToLittleEndian(bytes);
	}

	// Height * Width = Size
	int imageSize = header[2] * header[3];
		
	// Loop through each image, image count is defined in the header
	for (int i = 0; i < header[1]; i++) {
		Data* d = new Data();
		for (int j = 0; j < imageSize; j++) {
			uint8_t element[1];
			if (!fread(element, sizeof(element), 1, f)) {
				fprintf(stderr, "Failed read features from image: %d, %d\n", i, j);
				fclose(f);
				exit(1);
			}

			d->FeatureVector->push_back(element);
		}
		m_RawData.push_back(d);
	}
	fclose(f);

	printf("Read %d values\n", header[1]);
}

void Scriptify::DataHandler::ReadFeatureLabels(const char* path)
{
	uint32_t header[2]{}; // HEADER VALUES: Magic, Number of Images
	unsigned char bytes[4];

	printf("Getting label file headers from: %s\n", path);
	FILE* f = fopen(path, "rb");
	if (!f) {
		fprintf(stderr, "Failed to open file to get headers\n");
		exit(1);
	}

	// Get each value in the header
	for (int i = 0; i < 2; i++) {
		if (!fread(bytes, sizeof(bytes), 1, f)) {
			fprintf(stderr, "Failed read headers from files\n");
			fclose(f);
			exit(1);
		}

		header[i] = ConvertToLittleEndian(bytes);
	}

	// Loop through each image, image count is defined in the header
	for (int i = 0; i < header[1]; i++) {
		uint8_t element[1];
		if (!fread(element, sizeof(element), 1, f)) {
			fprintf(stderr, "Failed read features from image: %d\n", i);
			fclose(f);
			exit(1);
		}

		m_RawData[i]->Label = *element;
	}
	fclose(f);
}

void Scriptify::DataHandler::SplitData()
{
	printf("Splitting data...\n");
	std::unordered_set<int> usedIndexes;

	std::vector<Data*> tempData = m_RawData;

	int dataSize = tempData.size();
	int trainSize = dataSize * c_TrainSetPercent;
	int testSize = dataSize * c_TestSetPercent;
	int testOffset = trainSize + testSize;
	
	for (int i = 0; i < dataSize; i++) {
		int randIndex = rand() % tempData.size();

		if (i < trainSize) {
			m_RetData->TrainingData.push_back(tempData[randIndex]);
		}
		else if (i < testOffset)
		{
			m_RetData->TestData.push_back(tempData[randIndex]);
		}
		else {
			m_RetData->ValidationData.push_back(tempData[randIndex]);
		}

		tempData.erase(tempData.begin() + randIndex);
	}

	printf("Data Split:\n");
	printf("\t| Training Data (%.2f): %d\n", c_TrainSetPercent, (int)m_RetData->TrainingData.size());
	printf("\t| Test Data (%.2f): %d\n", c_TestSetPercent, (int)m_RetData->TestData.size());
	printf("\t| Validation Data (%.2f): %d\n", c_ValidationPercent, (int)m_RetData->ValidationData.size());
}

void Scriptify::DataHandler::CountClasses()
{
	printf("Counting Classes\n");
	int count = 0;
	for (uint32_t i = 0; i < m_RawData.size(); i++) {
		if (m_ClassMap.find(m_RawData[i]->Label) == m_ClassMap.end()) {
			m_ClassMap[m_RawData[i]->Label] = count;
			m_RawData[i]->EnumLabel = count;
			count++;
		}
	}
	printf("Counted classes: %d\n", count);
}

uint32_t Scriptify::DataHandler::ConvertToLittleEndian(const unsigned char* bytes)
{
	return (uint32_t)((bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | (bytes[3]));
}

Scriptify::OutData* Scriptify::DataHandler::GetData()
{
	return m_RetData;
}

