#include "Core.hpp"
#include "Util.hpp"
#include "FileSystem.hpp"
#include <iostream>

#include <string>
#include <vector>

void printHelpMessage()
{
	std::cout <<
		"usage: sit [--help] <command> [<args>]\n\n\n"
		"The most commonly used sit commands are:\n"
	    "   add        Add file contents to the index\n"
		"   checkout   Checkout a branch or paths to the working tree\n"
		"   commit     Record changes to the repository\n"
		"   diff       Show changes between commits, commit and working tree, etc\n"
		"   init       Create an empty Git repository or reinitialize an existing one\n"
		"   log        Show commit logs\n"
		"   reset      Reset current HEAD to the specified state\n"
		"   rm         Remove files from the working tree and from the index\n"
		"   status     Show the working tree status\n"
		"   help       Show this help message\n" << std::endl;
}

void printCheckoutArg()
{
	std::cout << 
		"Wrong arguments\n"
		"    sit checkout <commit>\n"
		"    sit checkout -- <file> [<file> ...]\n"
		"    sit checkout <commit> -- <file> [<file> ...]\n" << std::endl;
}

void printLogArg()
{
	std::cerr << 
		"Wrong arguments\n"
		"    sit log\n"
		"    sit log master\n"
		"    sit log <Commit ID>\n" << std::endl;
}

void printResetArg()
{
	std::cout << 
		"Wrong arguments\n"
		"    sit reset [--hard] <commit>\n"
		"    sit reset [--hard] -- <file> [<file> ...]\n"
		"    sit reset [--hard] <commit> -- <file> [<file> ...]\n" << std::endl;
}

void printDiffArg()
{
	std::cout <<
		"Wrong arguments\n"
		"    sit diff [<base>] [<target>]\n"
		"    <base> and <target> could be:\n"
		"        index\n"
		"        work\n"
		"        master\n"
		"        HEAD\n"
		"        <CommitID>\n"
		"    Default <base>   = index\n"
		"    Default <target> = work\n" << std::endl;
}

std::vector<std::string> argv;

int main(int argc, char** av)
{
	argv = std::vector<std::string>(av, av + argc);
	try {
		if (argv[1] == "init") {
			Sit::Core::Init();
		}
		Sit::Core::LoadRepo();
		if (argv[1] == "add") {
			for (auto iter = argv.begin() + 2; iter != argv.end(); ++iter) {
				Sit::Core::Add(*iter);
			}
		} else if (argv[1] == "rm") {
			for (auto iter = argv.begin() + 2; iter != argv.end(); ++iter) {
				Sit::Core::Rm(*iter);
			}
		} else if (argv[1] == "commit") {
			const int offset = argc > 2 && argv[2] == "--amend" ? 1 : 0;
			Sit::Core::Commit(offset);
		} else if (argv[1] == "status") {
			Sit::Core::Status();
		} else if (argv[1] == "checkout") {
			if (argc == 3) {
				// checkout <commit>
				Sit::Core::Checkout(argv[2], "");
			} else if (argc >= 4 && argv[2] == "--") {
				// checkout -- <file>
				for (auto iter = argv.begin() + 3; iter != argv.end(); ++iter) {
					Sit::Core::Checkout("", *iter);
				}
			} else if (argc >= 5 && argv[3] == "--") {
				// checkout <commit> -- <file>
				for (auto iter = argv.begin() + 4; iter != argv.end(); ++iter) {
					Sit::Core::Checkout(argv[2], *iter);
				}
			} else {
				printCheckoutArg();
				return 1;
			}
		} else if (argv[1] == "log") {
			if (argc == 2) {
				Sit::Core::Log("master");
			} else if (argc == 3) {
				Sit::Core::Log(argv[2]);
			} else {
				printLogArg();
				return 1;
			}
		} else if (argv[1] == "reset") {
			int offset = argc >= 3 && argv[2] == "--hard" ? 1 : 0;
			if (argc == offset + 3) {
				// reset [--hard] <commit>
				Sit::Core::Reset(argv[offset + 2], "", offset);
			} else if (argc >= offset + 4 && argv[offset + 2] == "--") {
				// reset [--hard] -- <file>
				for (auto i = 3; i < argc; ++i) {
					Sit::Core::Reset("", argv[offset + i], offset);
				}
			} else if (argc >= offset + 5 && argv[offset + 3] == "--") {
				// reset [--hard] <commit> -- <file>
				for (auto i = 4; i < argc; ++i) {
					Sit::Core::Reset(argv[offset + 2], argv[offset + i], offset);
				}
			} else {
				printResetArg();
				return 1;
			}
		} else if (argv[1] == "diff") {
			if (argc == 2) {
				Sit::Core::Diff("index", "work");
			} else if (argc == 3) {
				Sit::Core::Diff(argv[2], "work");
			} else if (argc == 4) {
				Sit::Core::Diff(argv[2], argv[3]);
			} else {
				printDiffArg();
			}
		} else if (argv[1] == "--help" || argv[1] == "help") {
			printHelpMessage();
		}
	} catch (const Sit::Util::SitException& e) {
		std::cerr << "!!! Sit Exception:" << std::endl;
		std::cerr << e.message << std::endl;
		if (!e.detail.empty()) {
			std::cerr << e.detail << std::endl;
		}
	} catch (const boost::filesystem::filesystem_error &fe) {
		std::cerr << fe.what() << std::endl;
	} catch (const std::exception &ec) {
		std::cerr << ec.what() << std::endl;
	}
}
