#include <list>

#ifndef _DATASTRUCTURES_HPP_
#define _DATASTRUCTURES_HPP_

struct Occurence
{
    int start;
    int stop;
};

struct SubEpisode
{
    int *path;
    int pathSize;
    SubEpisode *me;
    SubEpisode *parent;
    std::list<SubEpisode*> subelements;
    std::list<Occurence*> occurences;

    SubEpisode(SubEpisode *m, SubEpisode *p) : me(me), parent(p) {}
};

typedef SubEpisode* PSubEpisode;

#endif /* _DATASTRUCTURES_HPP_ */
