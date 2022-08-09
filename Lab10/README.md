Lab10: Microarray data analysis 2
-------------
**PreLab.cpp**
- C++ file for prelab activity
- Reads data of the samples from a specific form of files and converts them as a class
- Determines the best cutoff of expression of a gene that can separate samples into normal and cancer
- Calculates information gain for an input gene and sample list

**MainLab.cpp**
- C++ file for prelab activity
- Trains decision tree to classify samples into normal and cancer
- Predicts the class of unknown samples using the trained decision tree
- Validates the performance with k-fold cross validation

**Lab10_GSE13164_DEG.txt**
- Previously known DEG data that are used to train decision tree

**Lab10_GSE13164_train.txt**
- Data of the samples that are used in the decision tree training

**Lab10_GSE13164_test.txt**
- Data of the samples that should be tested and classified

**test.exe**
- Executable file coded from MainLab.cpp

*Note: I used MinGW g++ compiler to compile .exe files*
