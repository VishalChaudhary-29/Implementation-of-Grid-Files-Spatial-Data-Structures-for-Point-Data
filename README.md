# Implementation-of-Grid-Files-Spatial-Data-Structures-for-Point-Data
Implementation of Non-Uniform Grid files for storing and querying 2d data points in the secondary memory.

Programming Language used: C++
Operations Supported: 
  - Insertion with Buckets.
  - Handling overflow of Buckets by chaining and splitting of grid.
  - Range based query on grid along with comparison with Brute Force Search approach.


Codefile Name: GridFile.cpp

Note(s):
- To change number of records in dataset, please change Global variable "RECORD_Size".
- The randomly generated dataset is stored in "GridFilesInput.txt" file.
- As datapoints are randomly generated, outputs may vary every time the code is executed.
- Bucket files must be deleted before executing the code.
- For large dataset size, code may take a while to run, so please be patient.
- Bucket files are assigned names as "GridFileBucket0.txt", "GridFileBucket1.txt" ... and so on. 
