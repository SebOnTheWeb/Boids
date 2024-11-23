#pragma once

using namespace std;

template <typename T>
void saveToFile(T* data, int nrOfDataElements, string fileName);

void saveMeasurements(int* fpsData, double* updateTimeData, int nrOfDataElements) {
	saveToFile(fpsData, nrOfDataElements, "fps_single_size64_run1.csv");
	saveToFile(updateTimeData, nrOfDataElements, "update_single_size64_run1.csv");
}

template <typename T>
void saveToFile(T* data, int nrOfDataElements, string fileName) {
	ofstream file;
	file.open(fileName);

	for (int i = 0; i < nrOfDataElements; i++) {
		file << data[i] << "\n";
	}

	file.close();
}