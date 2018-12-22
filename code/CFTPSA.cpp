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

// Simulated annealing

vector<int> FacilityRes;
vector<int> CustomerRes;
int best = 0;
vector<int> FacilityBest;
vector<int> CustomerBest;
vector<int> FacilityBefore;
vector<int> CustomerBefore;

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

// Initialize a solution
void init() {
	int consume = 0;
	int FacilityNum = 0;
	for(int j = 0; j < Customer; j++) {
		if(consume+CustomerInfo[j][0] < FacilityInfo[FacilityNum][0]) {
			CustomerRes.push_back(FacilityNum);
			consume += CustomerInfo[j][0];
		} else {
			consume = 0;
			FacilityNum ++;
			j--;
		}
	}
	for(int i = 0; i < Facility; i++) {
		FacilityRes.push_back(1);
	}

	best = calulate();
	FacilityBest.assign(FacilityRes.begin(), FacilityRes.end());
	CustomerBest.assign(CustomerRes.begin(), CustomerRes.end());
}

// Determine whether two customers can exchange
bool valid(int r1, int r2) {
	int sum1 = 0;
	int sum2 = 0;
	for(int i = 0; i < Customer; i++) {
		if(i != r1 && CustomerRes[i] == CustomerRes[r1]) {
			sum1 += CustomerInfo[i][0];
		} else if(i != r2 && CustomerRes[i] == CustomerRes[r2]) {
			sum2 += CustomerInfo[i][0];
		}
	}
	if (sum1+CustomerInfo[r2][0] < FacilityInfo[CustomerRes[r1]][0] && 
		sum2+CustomerInfo[r1][0] < FacilityInfo[CustomerRes[r2]][0]) {
		return true;
	}
	return false;
}

bool valid2() {
	int FacilitySum[Facility];
	memset(FacilitySum, 0, Facility*sizeof(FacilitySum[0]));

	for(int i = 0; i < Customer; i++) {
		FacilitySum[CustomerRes[i]] += CustomerInfo[i][0];
	}

	bool flag = true;
	for(int i = 0; i < Facility; i++) {
		if(FacilitySum[i] > 0 && FacilitySum[i] < FacilityInfo[i][0])
			FacilityRes[i] = 1;
		if(FacilitySum[i] > FacilityInfo[i][0])
			flag = false;
	}
	
	return flag;
}

// various exchange methods
void method1(int r1, int r2) {
	if (valid(r1, r2)) {
		int tem = CustomerRes[r1];
		CustomerRes[r1] = CustomerRes[r2];
		CustomerRes[r2] = tem;
	}
}

void method2(int r1, int r2) {
	int OriginalFactory = CustomerRes[r1];
	int sum = 0;

	for (int i = 0; i < Customer; i++) {
		if (CustomerRes[i] == r2) {
			sum += CustomerInfo[i][0];
		}
	}
	if (sum+CustomerInfo[r1][0] < FacilityInfo[r2][0]) {
		CustomerRes[r1] = r2;
		FacilityRes[r2] = 1;
		
		for(int i = 0; i < Customer; i++) {
			if(CustomerRes[i] == OriginalFactory)
				OriginalFactory = -1;
		}
		if(OriginalFactory != -1)
			FacilityRes[OriginalFactory] = 0;
	}
}

void method3(int r1, int r2) {
	if (r1 > r2) {
		int tem = r1;
		r1 = r2;
		r2 = tem;
	}
	while(r1 < r2) {
		if (valid(r1, r2)) {
			int tem = CustomerRes[r1];
			CustomerRes[r1] = CustomerRes[r2];
			CustomerRes[r2] = tem;
		}
		r1 ++;
		r2 --;
	}
}

void method4(int r1, int r2) {
	if (r1 > r2) {
		int tem = r1;
		r1 = r2;
		r2 = tem;
	}
	if (r2 == 0)
		r2 ++;
	FacilityBefore.assign(FacilityRes.begin(), FacilityRes.end());
	CustomerBefore.assign(CustomerRes.begin(), CustomerRes.end());

	int tem = CustomerRes[r1];
	for(int i = r1; i < r2-1; i++) {
		CustomerRes[r1] = CustomerRes[r1+1];
	}
	CustomerRes[r2-1] = tem;

	if(!valid2()) {
		FacilityRes.assign(FacilityBefore.begin(),FacilityBefore.end());
		CustomerRes.assign(CustomerBefore.begin(),CustomerBefore.end());
	}
}

void method5(int r1, int r2, int r3, int r4) {
	
	FacilityBefore.assign(FacilityRes.begin(), FacilityRes.end());
	CustomerBefore.assign(CustomerRes.begin(), CustomerRes.end());

	CustomerRes[r3] = r1;
	CustomerRes[r4] = r2;

	if(!valid2()) {
		FacilityRes.assign(FacilityBefore.begin(),FacilityBefore.end());
		CustomerRes.assign(CustomerBefore.begin(),CustomerBefore.end());
	}
}

// create new solution
// local greed, compare various exchange methods, choose the best method
void newSolutions() {
	FacilityBefore.assign(FacilityRes.begin(), FacilityRes.end());
	CustomerBefore.assign(CustomerRes.begin(), CustomerRes.end());

	int r1 = rand()%Customer;
	int r2 = rand()%Customer;
	method1(r1, r2);
	int res1 = calulate();
	FacilityRes.assign(FacilityBefore.begin(),FacilityBefore.end());
	CustomerRes.assign(CustomerBefore.begin(),CustomerBefore.end());

	int r3 = rand()%Customer;
	int r4 = rand()%Facility;
	method2(r3, r4);
	int res2 = calulate();
	FacilityRes.assign(FacilityBefore.begin(),FacilityBefore.end());
	CustomerRes.assign(CustomerBefore.begin(),CustomerBefore.end());

	int r5 = rand()%Customer;
	int r6 = rand()%Customer;
	method3(r5, r6);
	int res3 = calulate();
	FacilityRes.assign(FacilityBefore.begin(),FacilityBefore.end());
	CustomerRes.assign(CustomerBefore.begin(),CustomerBefore.end());

	int r7 = rand()%Customer;
	int r8 = rand()%Customer;
	method4(r7, r8);
	int res4 = calulate();
	FacilityRes.assign(FacilityBefore.begin(),FacilityBefore.end());
	CustomerRes.assign(CustomerBefore.begin(),CustomerBefore.end());

	int r9 = rand()%Facility;
	int r10 = rand()%Facility;
	int r11 = rand()%Customer;
	int r12 = rand()%Customer;
	while(r11 == r12) {
		r12 = rand()%Customer;
	}
	method5(r9, r10, r11, r12);
	int res5 = calulate();
	FacilityRes.assign(FacilityBefore.begin(),FacilityBefore.end());
	CustomerRes.assign(CustomerBefore.begin(),CustomerBefore.end());

	if(res1 <= res2 && res1 <= res3 && res1 <= res4 && res1 <= res5) {
		method1(r1, r2);
	} else if(res2 <= res1 && res2 <= res3 && res2 <= res4 && res2 <= res5) {
		method2(r3, r4);
	} else if(res3 <= res1 && res3 <= res2 && res3 <= res4 && res3 <= res5) {
		method3(r5, r6);
	} else if(res4 <= res1 && res4 <= res2 && res4 <= res3 && res4 <= res5) {
		method4(r7, r8);
	} else {
		method5(r9, r10, r11, r12);
	}

}

void simulateAnneal() {
	double T = 100;
	double T_end = 10e-5; 
	int innerLoop = 1000;
	double cooling_rate = 0.99;
	double T_cout = T/10;

	init();
	
	while(T > T_end) {
		innerLoop *= 1.0001;
		int d ;
		for(int i = 0; i < innerLoop; i++) {
			int before = calulate();
			FacilityBefore.assign(FacilityRes.begin(), FacilityRes.end());
			CustomerBefore.assign(CustomerRes.begin(), CustomerRes.end());
			newSolutions();
			d = calulate() - before;
			double ran = rand()%1000/1000.0;
			if (d < 0 || exp(-d / T / 10) > ran) {
				;
			} else {
				FacilityRes.assign(FacilityBefore.begin(),FacilityBefore.end());
	            CustomerRes.assign(CustomerBefore.begin(),CustomerBefore.end());
			}

			if (best > calulate()) {
				best = calulate();
				FacilityBest.assign(FacilityRes.begin(), FacilityRes.end());
				CustomerBest.assign(CustomerRes.begin(), CustomerRes.end());
			}

		}
		T *= cooling_rate;
		if (T < T_cout) {
			cout << T << " " << calulate() << endl;
			T_cout /= 10;
		}
	}
}


int main(int argc, char *argv[]) {
	srand((unsigned int)(time(NULL))); 
	string p(argv[1]);
	string File = "p" + p;

	clock_t start, stop;
	start = clock();

	readFile("Instances\\\\" + File);
	simulateAnneal();
	
	stop = clock();
	
	ofstream in;
	in.open("res.txt", ios_base::app);
	
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

	// cout << "File: " << File << endl;
	// cout << "Use Time: " << (double)(stop-start)/CLOCKS_PER_SEC << " s" << endl;
	// cout << "result:  " << endl;
	// cout << best << endl;
	// for(int i = 0; i < Facility; i++) {
	// 	cout << FacilityBest[i] << " ";
	// }
	// cout << endl;
	// for(int i = 0; i < Customer; i++) {
	// 	cout << CustomerBest[i] << " ";
	// }
	// cout << endl;
	// cout << endl;

	return 0;	
} 
