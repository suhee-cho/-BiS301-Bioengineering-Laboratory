Lab6: DNA sequence data analysis - Dynamic Programming
----------------------
**DP_Main.cpp**
- Main code for finding the optimal sequence alignment and its score

**DPMat.cpp**
- Defines Class: Dynamic programming matrix
- Initialized with sequences that are supposed to be aligned
- Has cells, each of which is in charge of the element of the matrix
- Has method (fill_mat()) that fills in the matrix

**DPmat.h**
- Header file for DPmat.cpp

**Cell.cpp**
- Defines Class: Cell
- Contains its alignment score and flags

**Cell.h**
- Header file for Cell.cpp

**output.exe
- Executable file of the DP_Main.cpp
