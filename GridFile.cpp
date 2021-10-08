/*
Filename : GridFile.cpp
Author:  Vishal Chaudhary (2020aim1017@iitrpr.ac.in)

 // Program to implement Grid files and Range based query.
*/



//------------------------------------------- [HEADER FILES] -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#include <bits/stdc++.h>
using namespace std;

//------------------------------------------- [MACRO AND GLOBAL VARIABLES] ----------------------------------------------------------------------------------------------------------------------------------------------------




#define  GRID_LowLimit  0                  // corresponds to grid's lowest limit              
#define  GRID_HighLimit  400               // corresponds to grid's upper limit
#define  ZERO_Value 0                      // corresponds to '0'
#define  ONE_Value 1                       // corresponds to '1'
#define  TWO_Value 2                       // corresponds to '2'
#define  RECORD_Size 30000                    // corresponds to number of records to be generated in GridFilesInput.txt file.
#define  NUM_AXES 2


unsigned int physicalBucketSize = ZERO_Value;                         // corresponds to Bucket Size given by user.
const string& gridInputFilePath = "GridFilesInput.txt";               // corresponds to Filename of randomly generated datapoint database.
const string& gridFileBucketPathString = "GridFileBucket";            // corresponds to starting Filename of BucketFiles.
const string& extension = ".txt";                                     // file extension.

unsigned int bucketCounter = ZERO_Value;                              // keeps track of number of buckets generated.
class GridFilesMapClass;                                              // Maps Grid to bucket file.
vector<GridFilesMapClass> gridCollection;                             // refers to collection of all sub-grids present.
vector<unsigned int> XCoordinateSplitPoints;                          // refers to points where X axis was split.
vector<unsigned int> YCoordinateSplitPoints;                          // refers to points where Y axis was split.




/*
	Corresponds to datapoint having identifier, xCoordinate and yCoordinate
*/

struct DataPoint
{
	int xCoordinate;
        int yCoordinate;
	int identifier;
};

/*
	It stores Bucketname in bucketName, 
	number of datapoints in that bucket dataPointsCounter
	and vector of those points in bucketDataPoints
*/

struct PhysicalBucket
{
	string bucketName;
        int dataPointsCounter;
        vector<DataPoint> bucketDataPoints;
};


//---------------------------------------------[FUNCTION PROTOTYPES]---------------------------------------------------------------------------------------------------------------------------------------------------------
void clearGridFiles();
string generateInputRecord(int recordIdentifier);
void generateInputDatasetFile(int datasetSize);






//------------------------------------------- [FUNCTIONS] ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void initializeBucket(string fname, int ctr, PhysicalBucket& bucketInstance)
{
  bucketInstance.bucketName = fname;
  bucketInstance.dataPointsCounter = ctr;
}

void includeDataPointInBucket( DataPoint datapoint, PhysicalBucket& bucketInstance)
{
  string bucketFilename = bucketInstance.bucketName;
  bucketInstance.bucketDataPoints.push_back(datapoint);
  
  ofstream bucketFileInstance(bucketFilename, std::ios_base::app);
  bucketFileInstance << datapoint.identifier << " " << datapoint.xCoordinate << " " << datapoint.yCoordinate << "\n";
  bucketFileInstance.close();
  bucketInstance.dataPointsCounter = bucketInstance.dataPointsCounter +  ONE_Value;
  
  ifstream x(bucketFilename, ios::in);
  string data_string = "";
  while(getline(x, data_string))
  {
             DataPoint dp;
             int spaceIndex = data_string.find(" ");
             dp.identifier = stoi(data_string.substr(ZERO_Value, spaceIndex));
             int coordinateSpaceIndex = data_string.find(" ", spaceIndex + 1);
             dp.xCoordinate = stoi(data_string.substr(spaceIndex, coordinateSpaceIndex - spaceIndex));
	     dp.yCoordinate = stoi(data_string.substr(coordinateSpaceIndex+1, data_string.size()));
            
             data_string.clear();
}
  x.close();
}

void reassignBucketDataPoints(vector<DataPoint> datapoints, PhysicalBucket& bucketInstance)
{
  if( bucketInstance.bucketDataPoints.size() > 1 )
	bucketInstance.bucketDataPoints.clear();
  bucketInstance.dataPointsCounter = datapoints.size();
 
  string bucketFilename = bucketInstance.bucketName;
  ofstream bucketFileInstance(bucketFilename, ios::out | std::ofstream::trunc);

  for(auto currentdatapt : datapoints)
  {
     bucketFileInstance <<  currentdatapt.identifier << " " << currentdatapt.xCoordinate << " " << currentdatapt.yCoordinate << "\n";
     bucketInstance.bucketDataPoints.push_back(currentdatapt);
  }
  bucketFileInstance.close();
  
}

/*
	Coorsponds to a grid,
	XcoordinateList stores x mean and x max of grid
	YcoordinateList stores y mean and y max of grid
	numPoints stores number of points
	gridBucket stores the bucket instance for this grid.
		
*/ 

class GridFilesMapClass
{
       public:
       vector<int> XcoordinateList;
       vector<int> YcoordinateList;
       int numPoints;
       PhysicalBucket gridBucket;

	GridFilesMapClass(vector <int> _XcoordinateList,vector <int> _YcoordinateList, int datapointsCount, struct PhysicalBucket bucketInstance)
	{
        	this-> XcoordinateList = _XcoordinateList;
        	this-> YcoordinateList = _YcoordinateList;
		this-> numPoints = datapointsCount;
        	this-> gridBucket.bucketName = bucketInstance.bucketName;
        	this-> gridBucket.dataPointsCounter = bucketInstance.dataPointsCounter;
        	for(auto cur_datapoint : bucketInstance.bucketDataPoints)
       	 	{
            		this-> gridBucket.bucketDataPoints.push_back(cur_datapoint);
         	}
	}

	void updateGrid(vector <int> _XcoordinateList,vector <int> _YcoordinateList, int datapointsCount, struct PhysicalBucket bucketInstance)
        {
		this-> XcoordinateList = _XcoordinateList;
        	this-> YcoordinateList = _YcoordinateList;
		this-> numPoints = datapointsCount;

        	this-> gridBucket.bucketName = bucketInstance.bucketName;
                this-> gridBucket.dataPointsCounter = bucketInstance.dataPointsCounter;
                for(auto cur_datapoint : bucketInstance.bucketDataPoints)
                {
                     this-> gridBucket.bucketDataPoints.push_back(cur_datapoint);
                }
        }
};


void splitXAxis(int value)
{
   XCoordinateSplitPoints.push_back(value);
}

void splitYAxis(int value)
{
   YCoordinateSplitPoints.push_back(value);
}

void initializeXYSplit()
{
    splitXAxis(ZERO_Value);
    splitYAxis(ZERO_Value);
}

vector<DataPoint> getDataPointsFromFile(string filenamePath)
{
   vector<DataPoint> totalDataPointsCollection;
   string data_string = "";
   ifstream InputFileInstance(filenamePath, std::ios_base::in);
   if (InputFileInstance.is_open())
   {
       while(getline(InputFileInstance, data_string))
       {
             DataPoint dp;
             int spaceIndex = data_string.find(" ");
             dp.identifier = stoi(data_string.substr(ZERO_Value, spaceIndex));
             int coordinateSpaceIndex = data_string.find(" ", spaceIndex + 1);
             dp.xCoordinate = stoi(data_string.substr(spaceIndex, coordinateSpaceIndex - spaceIndex));
	     dp.yCoordinate = stoi(data_string.substr(coordinateSpaceIndex+1, data_string.size()));
             totalDataPointsCollection.push_back(dp);
             data_string.clear();
	}
          InputFileInstance.close();
   }
   return totalDataPointsCollection;
}


vector<int> getCoordinateValueListFromFile(string filenamePath, bool isXCoordinate)
{
   vector<int> coordinateCollection;
   string data_string = "";
   ifstream InputFileInstance(filenamePath, std::ios_base::in);
   if (InputFileInstance.is_open())
   {
       while(getline(InputFileInstance, data_string))
       {
            
             int spaceIndex = data_string.find(" ");
             int coordinateSpaceIndex = data_string.find(" ", spaceIndex + 1);
             if(isXCoordinate)
             {
               coordinateCollection.push_back(stoi(data_string.substr(spaceIndex, coordinateSpaceIndex - spaceIndex)));
             }
             else
               coordinateCollection.push_back(stoi(data_string.substr(coordinateSpaceIndex+1, data_string.size())));
             data_string.clear();
	}
          InputFileInstance.close();
   }
   return coordinateCollection;
}




void initializeTotalGridCollection(PhysicalBucket& currentBucket)
{
   vector<int> initialCoordinateList;
   initialCoordinateList.push_back(GRID_LowLimit);
   initialCoordinateList.push_back(GRID_HighLimit);

   GridFilesMapClass startingGrid(initialCoordinateList, initialCoordinateList, bucketCounter, currentBucket);
   gridCollection.push_back(startingGrid);
}


string getCurrentBucketName()
{
   string currentBucketString = gridFileBucketPathString + to_string(bucketCounter) + extension;
   return currentBucketString;
}

bool isPointOnGrid(GridFilesMapClass currentGrid, DataPoint dp)
{
    int x0Index = ZERO_Value;
    int x1Index = ONE_Value;
    int y0Index = ZERO_Value;
    int y1Index = ONE_Value;

    if ((currentGrid.YcoordinateList[y0Index] <= dp.yCoordinate) && (dp.yCoordinate <= currentGrid.YcoordinateList[y1Index]))
    {
       if((currentGrid.XcoordinateList[x0Index] <= dp.xCoordinate) && (dp.xCoordinate <= currentGrid.XcoordinateList[x1Index]))
       {
            return true;
       }
       return false;
    }
    return false; 
}

int getMedianFrom(int data[], int numEle)
{
   sort(data,data+numEle);
   if (numEle % TWO_Value != 0)
      return (int) data[(numEle / TWO_Value)];
   return (int)(data[((numEle-1) / TWO_Value)] + data[(numEle / TWO_Value)])/TWO_Value;
}

bool compareByXAxis(const DataPoint &dp1, const DataPoint &dp2)
{
    return dp1.xCoordinate < dp2.xCoordinate;
}

bool compareByYAxis(const DataPoint &dp1, const DataPoint &dp2)
{
    return dp1.yCoordinate < dp2.yCoordinate;
}

/*

	This Function Creates subgrids from the original grids
	and allocates corresponding buckets to the sub grids.
	
	Also, when number of data points is greater than bucket size,
	it spilts the grids, and allocates proper buckets to subgrids
	
	Splitting is done alternatively, starting with x-axis, followed 
	by y-axis and so on...

*/


void placePointsOnGrid(int physicalBucketSize)
{
  initializeXYSplit();
  vector<DataPoint> totalDataPointsCollection;
  char splitGridAxis;         
  totalDataPointsCollection = getDataPointsFromFile(gridInputFilePath);
  PhysicalBucket currentBucket;
  initializeBucket(getCurrentBucketName(), ZERO_Value, currentBucket);
  initializeTotalGridCollection(currentBucket);
  // SPLIT with X axis initially
  splitGridAxis = 'X';
  for (auto currentDataPoint : totalDataPointsCollection)
  {
      
     for(int gridIndex = ZERO_Value; gridIndex < gridCollection.size(); gridIndex++)
     {
	if(isPointOnGrid(gridCollection[gridIndex], currentDataPoint) == true)                       // check if point is lying on the grid.
        {     
            
             if(gridCollection[gridIndex].gridBucket.dataPointsCounter < physicalBucketSize)         // No splitting required.
             {
                gridCollection[gridIndex].numPoints++;                                               // increment points in current grid.
                includeDataPointInBucket(currentDataPoint, gridCollection[gridIndex].gridBucket);    // include this point to bucket. 
		break;		
             }
           
             else                                                                                   // splitting case: count(grid.points) > physicalBucketSize
             {
                
                gridCollection[gridIndex].numPoints++;
                includeDataPointInBucket(currentDataPoint, gridCollection[gridIndex].gridBucket);               
                vector<DataPoint> bucketDataPointsCollection;
                bucketDataPointsCollection = getDataPointsFromFile(gridCollection[gridIndex].gridBucket.bucketName); // collect all points in  bucketDataPointsCollection.                     
                
                bucketCounter++;                                                                   // splitting will be done so increment bucket count.


                if(splitGridAxis == 'Y')                                                           // check if splitting is to be done wrt Y-Axis.
                {
                   splitGridAxis = 'X';                						  // next time split by X axis.
                   vector<int> YcoordinatesDataPoints;
                   for (auto bucketPoint : bucketDataPointsCollection)
		   { 
		       YcoordinatesDataPoints.push_back(bucketPoint.yCoordinate);                // extract all Y-coordinates.
                   }
                   int YcoordinatesArray[YcoordinatesDataPoints.size()];
                   std::copy(YcoordinatesDataPoints.begin(), YcoordinatesDataPoints.end(), YcoordinatesArray); 

                   int medianYAxis = getMedianFrom(YcoordinatesArray, YcoordinatesDataPoints.size()); // compute median for all Y-coordinates.
                  
                   splitYAxis(medianYAxis);                                                    // add this Meadian Y axis split to YCoordinateSplitPoints.         

                   GridFilesMapClass parentGrid =  gridCollection[gridIndex];

                   PhysicalBucket newBucketInstance;
                   initializeBucket(getCurrentBucketName(), ZERO_Value, newBucketInstance);
                   sort(bucketDataPointsCollection.begin(), bucketDataPointsCollection.end(), compareByYAxis);
                                    	


                   vector<DataPoint> oldbucketDataPoints;
                   vector<DataPoint> newbucketDataPoints;
                   for (auto bucketPoint : bucketDataPointsCollection)
		   { 
                       if( bucketPoint.yCoordinate < medianYAxis)
		       		oldbucketDataPoints.push_back(bucketPoint);             // all points < medianYAxis goes to old bucket.
                       else
                                newbucketDataPoints.push_back(bucketPoint);             // else goesto new bucket.
                   }
                 
                   reassignBucketDataPoints(oldbucketDataPoints, parentGrid.gridBucket);
                   reassignBucketDataPoints(newbucketDataPoints, newBucketInstance);
                   
                   // split the grid
                   vector<GridFilesMapClass> newGridCollection;
                   vector<int> YcoordinateList;
                   YcoordinateList.push_back(medianYAxis);
                   YcoordinateList.push_back(parentGrid.YcoordinateList[ONE_Value]);
                   GridFilesMapClass newGrid (parentGrid.XcoordinateList, YcoordinateList, newBucketInstance.dataPointsCounter,newBucketInstance);
                   newGridCollection.push_back(newGrid);
                   YcoordinateList.clear();
  
                   YcoordinateList.push_back(parentGrid.YcoordinateList[ZERO_Value]);
                   YcoordinateList.push_back(medianYAxis);
                   gridCollection[gridIndex].updateGrid(parentGrid.XcoordinateList,YcoordinateList,parentGrid.gridBucket.dataPointsCounter,parentGrid.gridBucket);
                   
		   // update all existing grids.
                   for (auto currGrid : gridCollection)
                   {
                        if(currGrid.YcoordinateList[ZERO_Value] < medianYAxis &&  medianYAxis < currGrid.YcoordinateList[1])
                        {
                          int currGridPoints = ZERO_Value;
                          int newGridPoints = ZERO_Value;
                          auto currCopyGrid = currGrid;
                          vector<int> Yvalues = getCoordinateValueListFromFile(currGrid.gridBucket.bucketName, false);
                          for (auto value : Yvalues)
                          {
                               if(value < medianYAxis)
                               	   currGridPoints++;
                               else
                                   newGridPoints++; 
                          }
                         
                          vector<int> YcoordinateList;
                          YcoordinateList.push_back(medianYAxis);
                          YcoordinateList.push_back(currCopyGrid.YcoordinateList[ONE_Value]);
                          GridFilesMapClass newGrid1 (currCopyGrid.XcoordinateList, YcoordinateList,newGridPoints ,currCopyGrid.gridBucket);
                          newGridCollection.push_back(newGrid1);
                          
                          YcoordinateList.clear();
                          YcoordinateList.push_back(currCopyGrid.YcoordinateList[ZERO_Value]);
                          YcoordinateList.push_back(medianYAxis);
                          
                          currGrid.updateGrid(currCopyGrid.XcoordinateList,YcoordinateList,currGridPoints, currCopyGrid.gridBucket);
                        }
                   }
                   for(auto newgridToAdd : newGridCollection)
                   {
		       gridCollection.push_back(newgridToAdd);                   // update with new subgrids.
                   }
                   
                   break;
                }
		
                else
                {
                   splitGridAxis = 'Y';                			      // next time split by Y axis.
                   vector<int> XcoordinatesDataPoints;
                   for (auto bucketPoint : bucketDataPointsCollection)
		   { 
		       XcoordinatesDataPoints.push_back(bucketPoint.xCoordinate);                        // extract all X-coordinates.
                   }
                   int XcoordinatesArray[XcoordinatesDataPoints.size()];
                   std::copy(XcoordinatesDataPoints.begin(), XcoordinatesDataPoints.end(), XcoordinatesArray);

                   int medianXAxis = getMedianFrom(XcoordinatesArray, XcoordinatesDataPoints.size());   
                   // compute median for all X-coordinates.
                   //cout << "\nmedianXAxis is: " << medianXAxis << "\n";
                   splitXAxis(medianXAxis);                                                            // add this Meadian X axis split to XCoordinateSplitPoints.
                   GridFilesMapClass parentGrid =  gridCollection[gridIndex];
                   PhysicalBucket newBucketInstance;
                   initializeBucket(getCurrentBucketName(), ZERO_Value, newBucketInstance);
                   sort(bucketDataPointsCollection.begin(), bucketDataPointsCollection.end(), compareByXAxis);
 
                   vector<DataPoint> oldbucketDataPoints;
                   vector<DataPoint> newbucketDataPoints;
                   for (auto bucketPoint : bucketDataPointsCollection)
		   { 
                       if( bucketPoint.xCoordinate < medianXAxis)
		       		oldbucketDataPoints.push_back(bucketPoint);                         // all points < medianxAxis goes to old bucket.
                       else
                                newbucketDataPoints.push_back(bucketPoint);                         // else goesto new bucket.
                   }
                   
                   reassignBucketDataPoints(oldbucketDataPoints, parentGrid.gridBucket);
                   reassignBucketDataPoints(newbucketDataPoints, newBucketInstance);

                    // split the grid
                   vector<GridFilesMapClass> newGridCollection;
                   vector<int> XcoordinateList;
                   XcoordinateList.push_back(medianXAxis);
                   XcoordinateList.push_back(parentGrid.XcoordinateList[ONE_Value]);
                   GridFilesMapClass newGrid (XcoordinateList, parentGrid.YcoordinateList, newBucketInstance.dataPointsCounter,newBucketInstance);
                   newGridCollection.push_back(newGrid);
                   XcoordinateList.clear();
  
                   XcoordinateList.push_back(parentGrid.XcoordinateList[ZERO_Value]);
                   XcoordinateList.push_back(medianXAxis);
                   gridCollection[gridIndex].updateGrid(XcoordinateList, parentGrid.YcoordinateList,parentGrid.gridBucket.dataPointsCounter,parentGrid.gridBucket);
                   
		   // update all grids
                   for (auto currGrid : gridCollection)
                   {
                        if(currGrid.XcoordinateList[ZERO_Value] < medianXAxis &&  medianXAxis < currGrid.XcoordinateList[1])
                        {
                          int currGridPoints = ZERO_Value;
                          int newGridPoints = ZERO_Value;
                          auto currCopyGrid = currGrid;
                          vector<int> Xvalues = getCoordinateValueListFromFile(currGrid.gridBucket.bucketName, true);
                          for (auto value : Xvalues)
                          {
                               if(value < medianXAxis)
                               	   currGridPoints++;
                               else
                                   newGridPoints++; 
                          }
                         
                          vector<int> XcoordinateList;
                          XcoordinateList.push_back(medianXAxis);
                          XcoordinateList.push_back(currCopyGrid.XcoordinateList[ONE_Value]);
                          GridFilesMapClass newGrid1 (XcoordinateList, currCopyGrid.YcoordinateList, newGridPoints ,currCopyGrid.gridBucket);
                          newGridCollection.push_back(newGrid1);
                          
                          XcoordinateList.clear();
                          XcoordinateList.push_back(currCopyGrid.XcoordinateList[ZERO_Value]);
                          XcoordinateList.push_back(medianXAxis);
                          
                          currGrid.updateGrid(XcoordinateList, currCopyGrid.YcoordinateList,currGridPoints, currCopyGrid.gridBucket);
                        }
                   }
                   for(auto newgridToAdd : newGridCollection)
                   {
		       gridCollection.push_back(newgridToAdd);     // update with new subgrids.
                   }
                   break;
                   
                }   
             }
        }
     }
  }
}



//---------------------------------------------[UTILITY FUNCTIONS]-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
	Deletes existing GridFilesInput.txt
*/

void clearGridInputFiles()
{
    if (remove(gridInputFilePath.c_str()) == ZERO_Value )
        cout << "\n Old GridFilesInput.txt is deleted\n";
}


/*
	Utility Function to generate a record in dataset randomly.
*/

string generateInputRecord(int recordIdentifier)
{    
    string recordString  = ""; 
    recordString = to_string(recordIdentifier) + " ";
   
    for (int index = ZERO_Value; index < NUM_AXES ; index++)
    {
	 int random_coordinate =  GRID_LowLimit + rand() % (( GRID_HighLimit + 1 ) - GRID_LowLimit);
         recordString = recordString + to_string(random_coordinate) + " ";
    }          
    return recordString;
}

/*
	This Function Generates Random Dataset in the 
	format: ID X-coordinate Y-coordinate

*/

void generateInputDatasetFile(int datasetSize)
{
    ofstream GridInputDBInstance(gridInputFilePath, ofstream::out);
    int currRecCount = ONE_Value;
    while(datasetSize --)
    {
        GridInputDBInstance << generateInputRecord(currRecCount);
        currRecCount+= ONE_Value;
        if (datasetSize != ZERO_Value)
            GridInputDBInstance << "\n";
    }
    cout << "\n Random Coordinates generated in GridFilesInput.txt file.\n";
    GridInputDBInstance.close();

}

/*
	This Function Checks if coordinates provided by user 
	for query rectangle are within the limits of the grid.

*/

void validateRangeQueryCoordinates(int lowX, int lowY, int highX, int highY)
{
	if(lowX < GRID_LowLimit || lowY < GRID_LowLimit)
        {
           cout <<" Error!!...  lower coordinate(s) are below threshold '0' .... Aborting\n";
           exit(0);
        }
        if(highX > GRID_HighLimit || highY > GRID_HighLimit)
        {
           cout <<" Error!!...  higher coordinate(s) are above threshold '400' .... Aborting\n";
           exit(0);
        }
        
}


bool compareXCoordinateGrids(const GridFilesMapClass &grid1, const GridFilesMapClass &grid2)
{
    return ((grid1.XcoordinateList[ONE_Value] < grid2.XcoordinateList[ONE_Value]) || (grid1.XcoordinateList[ZERO_Value] < grid2.XcoordinateList[ZERO_Value]));
}



bool compareDataPoints(const DataPoint &dp1, const DataPoint& dp2)
{
   return (dp1.xCoordinate < dp2.xCoordinate);
}


bool similarityResolve(const DataPoint &dp1, const DataPoint& dp2)
{
   if (dp1.identifier == dp2.identifier)
         return true;
   return false;
}

/* 
	This Function displays the points belonging to query rectangle in the grid
*/
void displayPointsInsideQueryRectangle(int lowX, int lowY, int highX, int highY,vector<string> BucketFileNames)
{
        cout << "\n\n ===================== [Points lying inside query rectangle using Range query are ] ======================:\n";
        
	for(auto curBFileName : BucketFileNames)
   	{
    		ifstream InputFileInstance(curBFileName, std::ios_base::in);
    		if (InputFileInstance.is_open())
    		{
       			string data_string = " ";
       			while(getline(InputFileInstance, data_string))
       			{
          		   DataPoint dp;
          		   int spaceIndex = data_string.find(" ");
          		   dp.identifier = stoi(data_string.substr(ZERO_Value, spaceIndex));
          		   int coordinateSpaceIndex = data_string.find(" ", spaceIndex + 1);
          		   dp.xCoordinate = stoi(data_string.substr(spaceIndex, coordinateSpaceIndex - spaceIndex));
	  		   dp.yCoordinate = stoi(data_string.substr(coordinateSpaceIndex+1, data_string.size()));
           		   if(dp.xCoordinate >= lowX && dp.xCoordinate <= highX)
              		   {
            		   	  if(dp.yCoordinate >= lowY && dp.yCoordinate <= highY)
                 	     	  {
			 		cout << "\n \t DataPoint id: " << dp.identifier <<"\t";
                         		cout <<" Point(X,Y) is:  (" <<  dp.xCoordinate << " ," << dp.yCoordinate << " )";  
                                        
                	           }	
      		           }
          		   data_string.clear();
       			}
       			InputFileInstance.close(); 
    		}
	}
      cout << "\n\n ===================== [Range query ends ] ======================:\n";
}

/* 
	This function prints Data Points Generated inside Query Rectangle.
	We are soritng the Grid Collection by X coordinate, and selectiong only those
	points which overlps with given Query Rectangle.
	Hence, Range Query is highly Optimized.
*/

void executeRangeQuery(int range_lowXcoordinate, int range_lowYcoordinate, int range_highXcoordinate,int range_highYcoordinate)
{
        validateRangeQueryCoordinates(range_lowXcoordinate, range_lowYcoordinate, range_highXcoordinate,range_highYcoordinate);
	sort(gridCollection.begin(),gridCollection.end(),compareXCoordinateGrids);
     
        int startingIndex = ZERO_Value;
        int endIndex = ZERO_Value;
 
	auto it = gridCollection.begin();
        int counter = ZERO_Value;
        while(it!= gridCollection.end())
        {
            if ( ((*it).XcoordinateList[ONE_Value]) < range_lowXcoordinate)
               startingIndex++;
            if(((*it).XcoordinateList[ZERO_Value]) > range_highXcoordinate)
            {
		endIndex = counter;
                break;
            }
            it++;
            counter++;
        }
     
        vector<DataPoint> dataPointVector;
        if(startingIndex > 1)
        {
               startingIndex-= 1;
        }

        vector<string> BucketFileNames;
        
        for(int index = startingIndex; index <= endIndex; index ++)
        {
                if (std::find(BucketFileNames.begin(), BucketFileNames.end(), gridCollection[index].gridBucket.bucketName) == BucketFileNames.end())
                {
                 	BucketFileNames.push_back(gridCollection[index].gridBucket.bucketName);
                }
        }

	displayPointsInsideQueryRectangle(range_lowXcoordinate, range_lowYcoordinate, range_highXcoordinate,range_highYcoordinate, BucketFileNames);

 
}

/*
	This function prints Data Points Generated by Naive Approach.
	In this approach, we are traversing through all points and printing those which
	lies inside provided query rectangle.
*/

void displayDataPointsByNaiive(int range_lowXcoordinate, int range_lowYcoordinate, int range_highXcoordinate, int range_highYcoordinate)
{
   ifstream InputFileInstance(gridInputFilePath, std::ios_base::in);
   
   if (InputFileInstance.is_open())
   {
       vector<DataPoint> dataPointVector;
       string data_string = " ";
       while(getline(InputFileInstance, data_string))
       {
          DataPoint dp;
          int spaceIndex = data_string.find(" ");
          dp.identifier = stoi(data_string.substr(ZERO_Value, spaceIndex));
          int coordinateSpaceIndex = data_string.find(" ", spaceIndex + 1);
          dp.xCoordinate = stoi(data_string.substr(spaceIndex, coordinateSpaceIndex - spaceIndex));
	  dp.yCoordinate = stoi(data_string.substr(coordinateSpaceIndex+1, data_string.size()));
          dataPointVector.push_back(dp);
          data_string.clear();
       }
       
       cout << "\n\n ==================== [Printing Points using Naive Approach ] ================= \n";
      
       for (auto currentDataPoint: dataPointVector)
       {
	 if(currentDataPoint.xCoordinate >= range_lowXcoordinate && currentDataPoint.xCoordinate <= range_highXcoordinate)
         {
                 if(currentDataPoint.yCoordinate >= range_lowYcoordinate && currentDataPoint.yCoordinate <= range_highYcoordinate)
                 {
         	    
                     cout << "\n \t DataPoint id: " << currentDataPoint.identifier <<"\t";
                     cout <<" Point(X,Y) is:  (" <<  currentDataPoint.xCoordinate << " ," << currentDataPoint.yCoordinate << " )";  
                 }
          }
       }

        cout << "\n\n ===================== [Naiive Approach ends ] ======================:\n";
     }
     else
     {
	cout <<"\n Error!!... GridFilesInput.txt not found ...Aborting \n";
        exit(0);
     }
}

//This function prints contents of all grids and corresponding contents of buckets.

void printAllGridAndBuckets()
{
     int counter = 1;
     for(auto currentGrid : gridCollection)
     {   cout<< "\n";
         cout<< "\n ============================ [Grid " << counter <<" details:  ] ==========================================:\n";
         cout << "\n\t Grid left corner coordinates (X,Y): " << "( " << currentGrid.XcoordinateList[ZERO_Value] <<", " <<  currentGrid.YcoordinateList[ZERO_Value] <<" )";
         cout << "\n\t Grid right corner coordinates (X,Y): " << "( " << currentGrid.XcoordinateList[ONE_Value] <<", " <<  currentGrid.YcoordinateList[ONE_Value] <<" )";
         cout << "\n\t Name of Bucket File corresponding to this grid: " << currentGrid.gridBucket.bucketName;
         
         cout << "\n\t Contents of bucket file: " << currentGrid.gridBucket.bucketName <<" are =>\n";
         
         ifstream inputStream(currentGrid.gridBucket.bucketName, ios::in);
  	 string data_string = "";
  
  	 while(getline(inputStream, data_string))
  	 {
             DataPoint dp;
             int spaceIndex = data_string.find(" ");
             dp.identifier = stoi(data_string.substr(ZERO_Value, spaceIndex));
             int coordinateSpaceIndex = data_string.find(" ", spaceIndex + 1);
             dp.xCoordinate = stoi(data_string.substr(spaceIndex, coordinateSpaceIndex - spaceIndex));
	     dp.yCoordinate = stoi(data_string.substr(coordinateSpaceIndex+1, data_string.size()));
             cout <<"\n\t Data Point Identifier: " << dp.identifier ;
             cout<<"\t Data point coordinates (X,Y): "<< "( " << dp.xCoordinate <<", " <<  dp.yCoordinate <<" )";
             data_string.clear();
	  }
          inputStream.close();
         counter++;
     }
     cout <<"\n ============================ [Print Grid details over] ==========================================:\n";
}


// ---------------------------------------[INT MAIN] ----------------------------------------------------------------------------
int main()
{
	clearGridInputFiles();
        srand(time(NULL));
	generateInputDatasetFile(RECORD_Size);
	 
      	cout << "\n Enter the bucket size (Physical Bucket size): ";
	cin >> physicalBucketSize;
        srand(time(NULL));
	placePointsOnGrid(physicalBucketSize);
        cout << "\n\t Generating Bucket Files.....\n";
        splitXAxis(GRID_HighLimit);
        splitYAxis(GRID_HighLimit);
        
        cout << "\n\t Printing Grid contents.....\n";
        printAllGridAndBuckets();

        int range_lowXcoordinate; int range_lowYcoordinate;
	int range_highXcoordinate;int range_highYcoordinate;

        cout << "\nEnter the coordinates for Range Query rectangle\n";
        cout << " Enter LowerLeft X,Y coordinates: ";
        cin >> range_lowXcoordinate >> range_lowYcoordinate;
        cout << "\n Enter HigherRight X,Y coordinates: ";
        cin >> range_highXcoordinate >> range_highYcoordinate;

        auto NaiiveTimeStart = chrono::steady_clock::now();
        displayDataPointsByNaiive(range_lowXcoordinate, range_lowYcoordinate, range_highXcoordinate,range_highYcoordinate);
        auto NaiiveTimeEnd = chrono::steady_clock::now();
        
        auto RangeQTimeStart = chrono::steady_clock::now();
        executeRangeQuery(range_lowXcoordinate, range_lowYcoordinate, range_highXcoordinate,range_highYcoordinate);
        auto RangeQTimeEnd = chrono::steady_clock::now();

        cout << "\n================================================[ Time taken by Naiive Approach: "<< chrono::duration <double, milli> (NaiiveTimeEnd - NaiiveTimeStart ).count() <<" ms ]===================\n"; 

	cout << "\n================================================[ Time taken by Range Query Approach: "<< chrono::duration <double, milli> (RangeQTimeEnd - RangeQTimeStart ).count() <<" ms ]===================\n"; 

	return 0;
}
// ---------------------------------------[INT MAIN ENDS] --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



