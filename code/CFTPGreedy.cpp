#include <iostream>
#include <vector>
#include <string> 
#include <fstream> 
#include <sstream>
#include <sys/time.h>
#include <cmath>
#include <string.h>
#include <stdlib.h> 
using namespace std;

int Facility = 0;
int Customer = 0;
vector<vector<int> >FacilityInfo;
vector<vector<int> >CustomerInfo;

// Segmentation of string 's' by the separator 'separator' and store in 'v'
void SplitString(const string& s, vector<string>& v, const string& separator) {
    string::size_type pos1, pos2;
    pos2 = s.find(separator);
    pos1 = 0;
    while(string::npos != pos2)
    {
        v.push_back(s.substr(pos1, pos2-pos1));
         
        pos1 = pos2 + separator.size();
        pos2 = s.find(separator, pos1);
    }
    if(pos1 != s.length())
        v.push_back(s.substr(pos1));
}

// string to int
int String2Int(string str) {
	stringstream ss;
	ss << str;
	int res;
	ss >> res;
	return res;
}

// read File
void readFile(string path) {
	ifstream file(path.c_str());
	vector<string> split;
	string line;
	if (!file.is_open()) { 
        cout << "open file filed" << endl; 
    } else {
    	getline(file, line);
    	SplitString(line, split, " ");
    	for(vector<string>::iterator it = split.begin(); it != split.end(); ) {
    		if(*it == "") it = split.erase(it);
	    	else it ++;
    	}
    	Facility = String2Int(split[0]);
    	Customer = String2Int(split[split.size()-1]);
	}

	int FacilityCapacityNum = 0;
	int CustomerDemandNum = 0;
	int FacilityNum = 0;
	int CustomerAssignmentNum = 0;
	while(getline(file,line)) {
		vector<string> split;
		if(FacilityCapacityNum < Facility) {
			FacilityCapacityNum ++;
			SplitString(line, split," ");
			for(vector<string>::iterator it = split.begin(); it != split.end(); ) {
	    		if(*it == "") it = split.erase(it);
	    		else it ++;
	    	}
			std::vector<int> v;
    		v.push_back(String2Int(split[0]));
    		v.push_back(String2Int(split[split.size()-1]));
			FacilityInfo.push_back(v);
		} else if(CustomerDemandNum < Customer) {
			// CustomerDemandNum ++;
			SplitString(line, split,".");
			for(vector<string>::iterator it = split.begin(); it != split.end(); ) {
	    		if(*it == "") it = split.erase(it);
	    		else it ++;
	    	}
	    	if (split.size() == 1) {
	    		split.clear();
	    		SplitString(line, split," ");
	    		for(vector<string>::iterator it = split.begin(); it != split.end(); ) {
		    		if(*it == "") it = split.erase(it);
		    		else it ++;
		    	}
	    	}
			for(int i = 0; i < split.size(); i++) {
				std::vector<int> v;
    			v.push_back(String2Int(split[i]));
    			CustomerInfo.push_back(v);
    		}
    		CustomerDemandNum += split.size();
		} else if(FacilityNum < Facility) {
			SplitString(line, split,".");
			for(vector<string>::iterator it = split.begin(); it != split.end(); ) {
	    		if(*it == "") it = split.erase(it);
	    		else it ++;
	    	}
	    	if (split.size() == 1) {
	    		split.clear();
	    		SplitString(line, split," ");
	    		for(vector<string>::iterator it = split.begin(); it != split.end(); ) {
		    		if(*it == "") it = split.erase(it);
		    		else it ++;
		    	}
	    	}
	    	for(int i = 0; i < split.size(); i++) {
				CustomerInfo[CustomerAssignmentNum].push_back(String2Int(split[i]));
				CustomerAssignmentNum ++;
    		}
    		if(CustomerAssignmentNum == Customer) {
    			CustomerAssignmentNum = 0;
    			FacilityNum ++;
    		}

		}
	}
}

vector<int> FacilityRes;
vector<int> CustomerRes;
vector<int> FacilitySum;
int best = 1000000;
vector<int> FacilityBest;
vector<int> CustomerBest;

// Calculate the consumption of current results
int calulate() {
	int consume = 0;
	for(int i = 0; i < Facility; i++) {
		if(FacilityRes[i] == 1)	consume += FacilityInfo[i][1];
	}
	for(int i = 0; i < Customer; i++) {
		consume += CustomerInfo[i][CustomerRes[i]+1];
	}

	return consume;
}

void Greedy() {
	for(int ii = 0; ii < Facility; ii++) {
		FacilityRes.push_back(0);
		FacilitySum.push_back(0);
	}
	for(int ii = 0; ii < Customer; ii++) {
		CustomerRes.push_back(-1);
	}

	for(int i = 0; i < Customer; i++) {
		for(int ii = 0; ii < Facility; ii++) {
			FacilityRes[ii] = 0;
			FacilitySum[ii] = 0;
		}
		for(int ii = 0; ii < Customer; ii++) {
			CustomerRes[ii] = -1;
		}

		for(int j = i; j < Customer; j++) {
			int mincost = 100000;
			int minFacility = -1;
			for(int k = 0; k < Facility; k++) {
				if(FacilitySum[k]+CustomerInfo[j][0] < FacilityInfo[k][0] && mincost > CustomerInfo[j][k+1]) {
					mincost = CustomerInfo[j][k+1];
					minFacility = k;
				}
			}
			FacilityRes[minFacility] = 1;
			CustomerRes[j] = minFacility;
			FacilitySum[minFacility] += CustomerInfo[j][0];
		}
		for(int j = 0; j < i; j++) {
			int mincost = 100000;
			int minFacility = -1;
			for(int k = 0; k < Facility; k++) {
				if(FacilitySum[k]+CustomerInfo[j][0] < FacilityInfo[k][0] && mincost > CustomerInfo[j][k+1]) {
					mincost = CustomerInfo[j][k+1];
					minFacility = k;
				}
			}
			FacilityRes[minFacility] = 1;
			CustomerRes[j] = minFacility;
			FacilitySum[minFacility] += CustomerInfo[j][0];
		}
		if(best > calulate()) {
			best = calulate();
			FacilityBest.assign(FacilityRes.begin(), FacilityRes.end());
			CustomerBest.assign(CustomerRes.begin(), CustomerRes.end());
		}
	}
}

int main(int argc, char *argv[]) {
	string p(argv[1]);
	string File = "p" + p;
	clock_t start, stop;
	start = clock();

	readFile("Instances\\\\" + File);
	Greedy();
	
	stop = clock();

	ofstream in;
	in.open("Greedyres.txt", ios_base::app);
	
	in << "Use Time: " << (double)(stop-start)/CLOCKS_PER_SEC << " s" << endl;
	in << "result:  " << endl;
	in << best << endl;
	
	for(int i = 0; i < Facility; i++) {
		in << FacilityBest[i] << " ";
	}
	in << endl;
	for(int i = 0; i < Customer; i++) {
		in << CustomerBest[i] << " ";
	}
	in << endl;
	in << endl;
	in.close();
	cout << best << endl;

	return 0;
}
