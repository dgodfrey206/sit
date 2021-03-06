#ifndef _SRC_COLOR_HPP
#define _SRC_COLOR_HPP
#include <string>

#ifdef _WIN32
#include <windows.h>  // for WinAPI and Sleep()
#define _NO_OLDNAMES  // for MinGW compatibility
#undef min
#endif

namespace Sit {
namespace Color {

#ifdef _WIN32
const std::string RESET = "";
const std::string CLS = "";              // Clears screen
const std::string BLACK = "";        // Black
const std::string RED = "";          // Red
const std::string GREEN = "";        // Green
const std::string BROWN = "";        // Brown / dark yellow
const std::string BLUE = "";         // Blue
const std::string MAGENTA = "";      // Magenta / purple
const std::string CYAN = "";         // Cyan
const std::string GREY = "";         // Grey / dark white
const std::string DARKGREY = "";     // Dark grey / light black
const std::string LIGHTRED = "";     // Light red
const std::string LIGHTGREEN = "";   // Light green
const std::string YELLOW = "";       // Yellow (bright)
const std::string LIGHTBLUE = "";    // Light blue
const std::string LIGHTMAGENTA = ""; // Light magenta / light purple
const std::string LIGHTCYAN = "";    // Light cyan
const std::string WHITE = "";        // White (bright)
#else
const std::string RESET = "\033[0m";
const std::string CLS = "\033[2J";              // Clears screen
const std::string BLACK = "\033[22;30m";        // Black
const std::string RED = "\033[22;31m";          // Red
const std::string GREEN = "\033[22;32m";        // Green
const std::string BROWN = "\033[22;33m";        // Brown / dark yellow
const std::string BLUE = "\033[22;34m";         // Blue
const std::string MAGENTA = "\033[22;35m";      // Magenta / purple
const std::string CYAN = "\033[22;36m";         // Cyan
const std::string GREY = "\033[22;37m";         // Grey / dark white
const std::string DARKGREY = "\033[01;30m";     // Dark grey / light black
const std::string LIGHTRED = "\033[01;31m";     // Light red
const std::string LIGHTGREEN = "\033[01;32m";   // Light green
const std::string YELLOW = "\033[01;33m";       // Yellow (bright)
const std::string LIGHTBLUE = "\033[01;34m";    // Light blue
const std::string LIGHTMAGENTA = "\033[01;35m"; // Light magenta / light purple
const std::string LIGHTCYAN = "\033[01;36m";    // Light cyan
const std::string WHITE = "\033[01;37m";        // White (bright)
#endif

class ColorType {
#ifdef _WIN32
	int windows;
#else
	std::string ansi;
#endif
public:
	friend std::ostream& operator<<(std::ostream &out, const ColorType& c);
#ifdef _WIN32
	ColorType(const std::string &a, const int w): windows(w) {}
#else
	ColorType(const std::string &a, const int w): ansi(a) {}
#endif
};

inline std::ostream& operator<<(std::ostream &out, const ColorType& c) {
#ifdef _WIN32
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, (WORD)c.windows);
#else
	out << c.ansi;
#endif
	return out;
}

#ifdef _WIN32
extern const ColorType RESET       ;
extern const ColorType CLS         ; // Clears screen
extern const ColorType BLACK       ; // Black
extern const ColorType RED         ; // Red
extern const ColorType GREEN       ; // Green
extern const ColorType BROWN       ; // Brown / dark yellow
extern const ColorType BLUE        ; // Blue
extern const ColorType MAGENTA     ; // Magenta / purple
extern const ColorType CYAN        ; // Cyan
extern const ColorType GREY        ; // Grey / dark white
extern const ColorType DARKGREY    ; // Dark grey / light black
extern const ColorType LIGHTRED    ; // Light red
extern const ColorType LIGHTGREEN  ; // Light green
extern const ColorType YELLOW      ; // Yellow (bright)
extern const ColorType LIGHTBLUE   ; // Light blue
extern const ColorType LIGHTMAGENTA; // Light magenta / light purple
extern const ColorType LIGHTCYAN   ; // Light cyan
extern const ColorType WHITE       ; // White (bright)
#endif
}
}

#endif
