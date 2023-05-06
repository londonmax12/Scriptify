#include "DataHandler.h"

int main() {
	Scriptify::DataHandler* dataHandler = new Scriptify::DataHandler();
	dataHandler->ReadFeatureVector("../data/train-images-idx3-ubyte");
	dataHandler->ReadFeatureLabels("../data/train-labels-idx1-ubyte");
	dataHandler->SplitData();
	dataHandler->CountClasses();
}