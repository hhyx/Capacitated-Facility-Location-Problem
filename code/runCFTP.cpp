#include <iostream>
#include <string> 
#include <fstream> 
#include <sys/time.h>
using namespace std;

int main() {
	clock_t start, stop;
	start = clock();

	ofstream in;
	in.open("SAres.txt", ios_base::app);

	for(int i = 67; i <= 71; i++) {
		int loop = 5;
		in << "File: p" << to_string(i) << endl;
		for(int j = 0; j < loop; j++) {
			char s[100];
			sprintf_s(s,"%s %d","CFTPSA.exe", i);
			cout <<s << endl;
			system(s);
			
		}
		
		in << endl;
		in << endl;
	}

	stop = clock();
	in << "Finally: " << (double)(stop-start)/CLOCKS_PER_SEC << " s" << endl;
	in.close();

	// system("shutdown -s -t 30");
	return 0;
}
