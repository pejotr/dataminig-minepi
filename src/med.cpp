#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <string.h>

#include "med.hpp"

const EventSequence parse_data_file(const std::string& fileName)
{
    EventSequence seq;
    EventSequence result;
    std::ifstream dataFile(fileName.c_str());

    if(dataFile.is_open()) 
    {
        std::string line;
        while(dataFile.good())
        {
            getline(dataFile, line);
            seq = parse_data_line(line);
            result.splice(result.end(), seq);
        }
    }

    return result;
}

const EventSequence parse_data_line(const std::string& line)
{
    EventSequence result;
    char eventsSet[512] = { 0x00, };
    char *token;
    int sec, session, eventId;

    sscanf(line.c_str(), "%d[%*s %*s %*d, %*d.%*d],%d:%99[^\n]", &sec, &session, eventsSet);
    token = strtok(eventsSet," ,");

    while(token != NULL)
    {
        Event *event = new Event();
        Occurence occurence(sec, sec);

        event->predicate = atoi(token);
        event->occurence = occurence;
        result.push_back(event);

        token = strtok(NULL, " ,");
    }

    return result; 
}

bool same_predicate(const Event* first, const Event* second)
{
    return first->predicate == second->predicate;
}

const PredicatesSet generate_predicate_set(const EventSequence& seq)
{
    PredicatesSet predicateSet;
    EventSequence::iterator end, iter;
    EventSequence uniqueSeq = seq;

    uniqueSeq.unique(same_predicate);
    end = uniqueSeq.end();

    for(iter = uniqueSeq.begin(); iter != end; iter++)
    {
        predicateSet.push_back((*iter)->predicate);
    } 

    return predicateSet; 
}

void log(int lineno, const char *file, const char *format, ...) 
{
    char msg[256]; 
    char temp[512], loc[128];

    va_list args;
    va_start (args, format);
    vsprintf(msg, format, args);
  
    snprintf(loc, 128, "%s:%d", file, lineno);
    snprintf(temp, 512, "%-25s # %s", loc, msg); 
    printf("%s\n", temp);
}

