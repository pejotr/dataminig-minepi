#include <iostream>
#include <fstream>
#include <utility>

#include <string>

#include <cstdlib>
#include <string.h>

#include "med.hpp"


int parse_data_file(std::string fileName)
{
    std::ifstream dataFile(fileName.c_str());

    if(dataFile.is_open()) 
    {
        std::string line;
        while(dataFile.good())
        {
            getline(dataFile, line);

            if(!parse_data_line(line))
            {
                // report error
            }

        }
    }
}

int parse_data_line(const std::string& line)
{
    char params[512] = { 0x00, };
    char *pch;
    int sec, session;
    PSubEpisode se;

    sscanf(line.c_str(), "%d[%*s %*d],%d:%99[^\n]", &sec, &session, params);
    pch = strtok(params," ,");

    while(pch != NULL)
    {
        int eventid = atoi(pch);
        Occurence *o = new Occurence();
        o->start = o->stop = sec;

        se = __root_events_list[eventid];

        if( se == NULL) 
        {
            se = new SubEpisode(NULL, NULL);
            se->path = new int[1];
            se->path[1] = eventid;

           __root_events_list[eventid] =  se;
        }

        se->occurences.push_back(o);
        pch = strtok(NULL, " ,");
    }

    return 1;
}

