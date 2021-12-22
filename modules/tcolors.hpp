#include <iostream>
#include <string>

class Tcolors {
    public:
    inline static const std::string BLACK = "\033[0;30m";
    inline static const std::string RED = "\033[0;31m";
    inline static const std::string GREEN = "\033[0;32m";
    inline static const std::string YELLOW = "\033[0;33m";
    inline static const std::string BLUE = "\033[0;34m";
    inline static const std::string PURPLE = "\033[0;35m";
    inline static const std::string CYAN = "\033[0;36m";
    inline static const std::string WHITE = "\033[0;37m";
    
    inline static const std::string BOLDBLACK = "\033[1;30m";
    inline static const std::string BOLDRED = "\033[1;31m";
    inline static const std::string BOLDGREEN = "\033[1;32m";
    inline static const std::string BOLDYELLOW = "\033[1;33m";
    inline static const std::string BOLDBLUE = "\033[1;34m";
    inline static const std::string BOLDPURPLE = "\033[1;35m";
    inline static const std::string BOLDCYAN = "\033[1;36m";
    inline static const std::string BOLDWHITE = "\033[1;37m";

    inline static const std::string DIMBLACK = "\033[2;30m";
    inline static const std::string DIMRED = "\033[2;31m";
    inline static const std::string DIMGREEN = "\033[2;32m";
    inline static const std::string DIMYELLOW = "\033[2;33m";
    inline static const std::string DIMBLUE = "\033[2;34m";
    inline static const std::string DIMPURPLE = "\033[2;35m";
    inline static const std::string DIMCYAN = "\033[2;36m";
    inline static const std::string DIMWHITE = "\033[2;37m";

    inline static const std::string UNDERLINEBLACK = "\033[4;30m";
    inline static const std::string UNDERLINERED = "\033[4;31m";
    inline static const std::string UNDERLINEGREEN = "\033[4;32m";
    inline static const std::string UNDERLINEYELLOW = "\033[4;33m";
    inline static const std::string UNDERLINEBLUE = "\033[4;34m";
    inline static const std::string UNDERLINEPURPLE = "\033[4;35m";
    inline static const std::string UNDERLINECYAN = "\033[4;36m";
    inline static const std::string UNDERLINEWHITE = "\033[4;37m";

    inline static const std::string BLINKBLACK = "\033[5;30m";
    inline static const std::string BLINKRED = "\033[5;31m";
    inline static const std::string BLINKGREEN = "\033[5;32m";
    inline static const std::string BLINKYELLOW = "\033[5;33m";
    inline static const std::string BLINKBLUE = "\033[5;34m";
    inline static const std::string BLINKPURPLE = "\033[5;35m";
    inline static const std::string BLINKCYAN = "\033[5;36m";

    inline static const std::string RESET = "\033[0m";

};