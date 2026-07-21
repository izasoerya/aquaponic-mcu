#if !defined(PARSER_H)
#define PARSER_H

#include "utils.h"
#include <cstring>

class Parser
{
private:
public:
    Parser() {}
    ~Parser() {}

    static AppState parseCommand(const char *incoming)
    {
        if (strcmp(incoming, "OTA") == 0)
        {
            return AppState::OTA_MODE;
        }
        else
        {
            return AppState::NORMAL_MODE;
        }
    }
};

#endif // PARSER_H
