#include <iostream>
#include "../include/cfg.h"

using namespace std;

int main() {
	cout << "Hello, World!" << endl;
	STRUCT_FILE_CFG fCfg;
	readConfig(fCfg);
	return 0;
}