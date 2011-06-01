#include <map>
#include "minepi.hpp"
#include "med.hpp"

static Episode *C0;
static EpisodesCollection table;

static void generate_candidates(Episode *e);
static void check_candidates();

//! Performs one-time event sequence scan
static EpisodesCollection scan_event_sequence(const EventSequence& seq, const int freqTrsh);

void minepi(const EventSequence& seq, const PredicatesSet& set, 
        const int freqTrsh, const Windows& wnds)
{
    C0 = new Episode;
    C0->parent = NULL;

    EpisodesCollection collection = scan_event_sequence(seq, freqTrsh);

}

EpisodesCollection generate_candidates(const EpisodesCollection& collection, 
        const EpisodesCollection::iterator& start, const EpisodesCollection::iterator& stop)
{
}

EpisodesCollection scan_event_sequence(const EventSequence& seq, const int freqTrsh)
{
    EpisodesMap singletons;
    EpisodesCollection result;
    EventSequence::const_iterator iter = seq.begin(), 
                end = seq.end();
    EpisodesMap::iterator mapiter, mapend;
   
    for(; iter != end; iter++)
    {
        Event event = *iter;
        EpisodesMap::iterator tmp = singletons.find(event.predicate);

        if(tmp == singletons.end())
        {
            Episode *episode = new Episode();
            episode->parent = C0;
            episode->predicates.front() = event.predicate;
            episode->occurences.push_back(event.occurence);

            singletons[event.predicate] = episode;
        } 
        else
        {
            Episode *episode = tmp->second;
            episode->occurences.push_back(event.occurence);
        }
        
    }

    mapend = singletons.end();
    for(mapiter = singletons.begin(); mapiter != mapend; )
    {
        EpisodesMap::iterator temp = mapiter++;
        Episode *episode = temp->second;

        if( episode->occurences.size() < freqTrsh )
        {
            APPLOG("Episode %d with freq = %d appeared to be not frequent enough", 
                        episode->predicates.front(), episode->occurences.size());
            singletons.erase(temp);
            continue;
        }

        result.push_back(temp->second);
    }

    return result;
}
