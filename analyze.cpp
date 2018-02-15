#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

struct appType  //struct to save each ApplicationType with all his Data
{
	std::string name;
	int valueCount = 0;
	std::vector<float> runTimeData, cpuTimeData, creditData;
	float oRunTime, oCPUTime;
};

bool appRunTimeCompare(appType a, appType b)
{
	return (a.oRunTime > b.oRunTime);
}

int main(int argc, char* argv[])
{
	//Check for arguement
	if(argv[1] == NULL)
	{
		std::cout << "Missing arguments!\n";
		return 0;
	}
	
	std::ifstream fileToAnalyze(argv[1]);

	//check if File exists and is open
	if(!fileToAnalyze.is_open())
	{
		std::cout << "File does not exist!" << "\n";
		return 0;
	}

	std::string line, tmp;

	//skip header line in File
	if(fileToAnalyze.good())
	{
		getline(fileToAnalyze, line);
	}

	float runTime, cpuTime, credit;
	int maxNameLength = 0;

	std::vector<appType> appList;

	//replace with
	while(fileToAnalyze.good())
	//for(int i = 0; i < 5 && fileToAnalyze.good(); i++)
	{
		//next line into linestream
		getline(fileToAnalyze, line);
		std::stringstream lineStream(line);

		//skip time and dates each line
		for(int l = 0; l < 4; l++)
		{
			getline(lineStream, tmp, ';');
		}

		getline(lineStream, tmp, ';');
		if(tmp == "Completedandvalidated")
		{
			getline(lineStream, tmp, ';');
			runTime = stod(tmp);
			getline(lineStream, tmp, ';');
			cpuTime = stod(tmp);
			getline(lineStream, tmp, ';');
			credit = stod(tmp);
			getline(lineStream, tmp, ';'); //tmp now holds the application name
			
			//std::cout << "Application: " << application << "; Runtime: " << runTime << "; cpuTime: " << cpuTime << "; Credit: " << credit << "\n";

			for(int j = 0; j <= appList.size(); j++)
			{
				//Application Type already exists
				if(j < appList.size() && appList[j].name == tmp)
				{
					appList[j].runTimeData.push_back(runTime);
					appList[j].cpuTimeData.push_back(cpuTime);
					appList[j].creditData.push_back(credit);
					appList[j].valueCount++;
					break;
				}
				//Application Type didn't exist yet. Add it.
				else if(j == appList.size())
				{
					//std::cout << "added" << "\n";
					appList.push_back(appType());
					appList[j].name = tmp;
					if(tmp.length() > maxNameLength)
					{
						maxNameLength = tmp.length();
					}
					appList[j].runTimeData.push_back(runTime);
					appList[j].cpuTimeData.push_back(cpuTime);
					appList[j].creditData.push_back(credit);
					appList[j].valueCount++;
					break;
				}
			}
		}
	}

	fileToAnalyze.close();

	//Print
	int i, j;
	float tmpRun, tmpCPU;
	for(i = 0; i < appList.size(); i ++)
	{
		tmpRun = 0;
		tmpCPU = 0;
		for(j = 0; j < appList[i].valueCount; j++)
		{
			tmpRun += appList[i].creditData[j] / appList[i].runTimeData[j];
			tmpCPU += appList[i].creditData[j] / appList[i].cpuTimeData[j];
		}
		appList[i].oRunTime = tmpRun / appList[i].valueCount;
		appList[i].oCPUTime = tmpCPU / appList[i].valueCount;
	}

	std::sort(appList.begin(), appList.end(), appRunTimeCompare); //sort by highest credits per RunTime

	std::cout << std::left << std::setw(maxNameLength) << "Application" << "\tValues\tCredits/Second\tCredits/CPUSecond\n";
	for(i = 0; i < appList.size(); i++)
	{
		std::cout << std::left << std::setw(maxNameLength) << appList[i].name;
		std::cout << "\t" << appList[i].valueCount;
		std::cout << "\t" << std::setw(8) << appList[i].oRunTime << "\t" << std::setw(8) << appList[i].oCPUTime << "\n";
	}

	return 0;
}