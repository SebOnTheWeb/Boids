#pragma once

using namespace std;

#include <fstream>

class Measurements {
private:
	int* fpsData;
	double* updateTimeDataInMs;
	int maxDataElements;
	int nrOfDataElements;

	template <typename T>
	void SaveToFile(T* data, string fileName);

public:
	Measurements(int maxDataElements);
	~Measurements();

	void Update(int fps, double updateTimeInMs);
	void SaveCurrentDataToFile();
};

template <typename T>
void Measurements::SaveToFile(T* data, string fileName) {
	ofstream file;
	file.open(fileName);

	for (int i = 0; i < this->nrOfDataElements; i++) {
		file << data[i] << "\n";
	}

	file.close();
}