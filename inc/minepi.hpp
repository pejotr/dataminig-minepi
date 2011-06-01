#include <map>
#include <list>

#ifndef _MINEPI_HPP_
#define _MINEPI_HPP_

struct Episode;
struct Event;

//! EventType data structure. MINEPI set E consists of this elements
typedef int PredicateType;
typedef std::list<PredicateType> PredicatesSet;
typedef std::list<Event*> EventSequence;
typedef std::list<int> Windows;
typedef std::list<Episode*> EpisodesCollection;
typedef std::map<PredicateType, Episode*> EpisodesMap;

struct Occurence
{
    Occurence() : start(0), stop(0) {}
    Occurence(int start, int stop) : start(start), stop(stop) {}
    int start;
    int stop;
};

struct Episode
{
    Episode *parent;
    std::list<PredicateType> predicates;
    std::list<Episode*> superepisodes;
    std::list<Occurence> occurences;
};

struct Event
{
    PredicateType predicate;
    Occurence occurence;
};

//! MINEPI algortihm
void minepi(const EventSequence& seq, const PredicatesSet& set, 
                    const int freqTrsh, const Windows& wnds);

#endif /* _MINEPI_HPP_ */
