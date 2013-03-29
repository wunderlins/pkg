// $Id: popen.cpp 129 2011-04-25 20:48:59Z wus $

#include <iostream>
#include <stdio.h>

using namespace std;

/**
 * run debians apt-get and check output
 */
int main(int argc, char **argv, char **envp) { 

	FILE *fp;
	char buffer[10];
	char command[255] = "apt-get install python-wxtools";
	cout << command << endl;

	// Execute command, open /dev/stdout for reading
	fp = popen(command, "r");

	// read output character by character
	while (fread(buffer, 1, 1, fp) != EOF) {
		// printf("%s", buffer);
		cout << buffer << flush;
	}
	
	// close
	pclose(fp);
}
