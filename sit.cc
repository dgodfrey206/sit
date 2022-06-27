#include <iostream>
#include <cstring>

#include "sit-operations.h"

extern const boost::filesystem::path repo_dir(".sit");

int main(int argc, char *const argv[])
{
	if (argc == 1) {
		/*����汾��Ϣ������*/
		return 0;
	}
	if (strcmp(argv[1], "init") == 0) {
		MainFrame::InitRepo();
	}
	if (strcmp(argv[1], "add") == 0) {
		if (argc == 2) {
			std::cerr << "Need a file name" << std::endl;
			return 0;
		}
		MainFrame::AddFile(argv[2]);
	}
	return 0;
}