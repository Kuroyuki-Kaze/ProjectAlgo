#include <iostream>
#include <string>

#ifndef LOGGING_HPP
#define LOGGING_HPP

class Logging {
    public:
        std::string fmt;
        int level;

        inline const static int __DEBUG = 4;
        inline const static int __INFO = 3;
        inline const static int __WARN = 2;
        inline const static int __ERROR = 1;
        inline const static int __FATAL = 0;

        

        Logging() {
            fmt = "";
            level = 0;
        }

        void configure(int level, std::string fmt) {
            this->level = level;
            this->fmt = fmt;
        }

        std::string format(std::string msg, std::string fmt, int level) {
            // replace %(lvl)s with the level in all caps, and %(msg)s with the message
            std::string formatted = fmt;
            std::string lvl;

            switch (level) {
                case __DEBUG:
                    lvl = "DEBUG";
                    break;
                case __INFO:
                    lvl = "INFO";
                    break;
                case __WARN:
                    lvl = "WARN";
                    break;
                case __ERROR:
                    lvl = "ERROR";
                    break;
                case __FATAL:
                    lvl = "FATAL";
                    break;
                default:
                    lvl = "UNKNOWN";
                    break;
            }

            std::string part1;
            std::string part2;

            while (formatted.find("%(lvl)s") != std::string::npos) {
                part1 = formatted.substr(0, formatted.find("%(lvl)s"));
                part2 = formatted.substr(formatted.find("%(lvl)s") + 7);
                formatted = part1 + lvl + part2;
            }

            while (formatted.find("%(msg)s") != std::string::npos) {
                part1 = formatted.substr(0, formatted.find("%(msg)s"));
                part2 = formatted.substr(formatted.find("%(msg)s") + 7);
                formatted = part1 + msg + part2;
            }

            return formatted;
        }

        void debug(std::string msg) {
            if (level >= __DEBUG) {
                std::cout << format(msg, fmt, __DEBUG) << std::endl;
            }
        }

        void debug(std::string msg, std::string fmt) {
            if (level >= __DEBUG) {
                std::cout << format(msg, fmt, __DEBUG) << std::endl;
            }
        }

        void info(std::string msg) {
            if (level >= __INFO) {
                std::cout << format(msg, fmt, __INFO) << std::endl;
            }
        }

        void info(std::string msg, std::string fmt) {
            if (level >= __INFO) {
                std::cout << format(msg, fmt, __INFO) << std::endl;
            }
        }

        void warn(std::string msg) {
            if (level >= __WARN) {
                std::cout << format(msg, fmt, __WARN) << std::endl;
            }
        }

        void warn(std::string msg, std::string fmt) {
            if (level >= __WARN) {
                std::cout << format(msg, fmt, __WARN) << std::endl;
            }
        }

        void error(std::string msg) {
            if (level >= __ERROR) {
                std::cout << format(msg, fmt, __ERROR) << std::endl;
            }
        }

        void error(std::string msg, std::string fmt) {
            if (level >= __ERROR) {
                std::cout << format(msg, fmt, __ERROR) << std::endl;
            }
        }

        void fatal(std::string msg) {
            if (level >= __FATAL) {
                std::cout << format(msg, fmt, __FATAL) << std::endl;
            }
        }

        void fatal(std::string msg, std::string fmt) {
            if (level >= __FATAL) {
                std::cout << format(msg, fmt, __FATAL) << std::endl;
            }
        }

        void log(int level, std::string msg) {
            switch (level) {
                case __DEBUG:
                    debug(msg);
                    break;
                case __INFO:
                    info(msg);
                    break;
                case __WARN:
                    warn(msg);
                    break;
                case __ERROR:
                    error(msg);
                    break;
                case __FATAL:
                    fatal(msg);
                    break;
                default:
                    break;
            }
        }

        void log(int level, std::string msg, std::string fmt) {
            switch (level) {
                case __DEBUG:
                    debug(msg, fmt);
                    break;
                case __INFO:
                    info(msg, fmt);
                    break;
                case __WARN:
                    warn(msg, fmt);
                    break;
                case __ERROR:
                    error(msg, fmt);
                    break;
                case __FATAL:
                    fatal(msg, fmt);
                    break;
                default:
                    break;
            }
        }
};

#endif