#include <iostream>
#include <map>
#include "minepi.hpp"
#include "med.hpp"

static Episode *C0;
static EpisodesCollection table;

/*static*/ EpisodesCollection generate_candidates(const EpisodesCollection& collection, 
        const EpisodesCollection::iterator& start, const EpisodesCollection::iterator& stop);

static void check_candidates();

void print_episode(const PredicatesSet& set)
{
    PredicatesSet::const_iterator i, e = set.end();
    for(i = set.begin(); i != e; i++)
    {
        std::cout << (int)*i;
    }
    std::cout << std::endl;
}

//! Performs one-time event sequence scan
/*static*/ EpisodesCollection scan_event_sequence(const EventSequence& seq, const int freqTrsh);

void minepi(const EventSequence& seq, const PredicatesSet& set, 
        const int freqTrsh, const Windows& wnds)
{
    EpisodesCollection::iterator iter;        

    C0 = new Episode;
    C0->parent = NULL;

    EpisodesCollection collection = scan_event_sequence(seq, freqTrsh);
    table.insert(iter, collection.begin(), collection.end());

    //generate_candidates(table, table.begin(), table.end());
}

EpisodesCollection generate_candidates(const EpisodesCollection& collection, 
        EpisodesCollection::iterator start, EpisodesCollection::iterator stop)
{
    EpisodesCollection result;
    EpisodesCollection::iterator iter1, iter2;

    std::cout << collection.size() << std::endl;

    for(iter1 = start; iter1 != stop; iter1++)
    {
        Episode *episode1 = *iter1;
        std::cout << episode1->predicates.front() << std::endl;
        print_episode(episode1->predicates);
        for(iter2 = start; iter2 != stop; iter2++)
        {
            Episode *episode2 = *iter2;

            if(episode1->parent == episode2->parent && 
                        episode1->predicates != episode2->predicates)
            {
                std::list<PredicateType> ep1 = episode1->predicates,
                                         ep2 = episode2->predicates;
                PredicateType p1 = ep1.back(), p2 = ep2.back();
                            
                ep1.pop_back();
                ep2.pop_back();

                if(ep1 == ep2 && p1 != p2)
                {
                    APPLOG("Generating new candidate");
                    Episode *candidate = new Episode();
                    candidate->parent = episode1;
                    candidate->predicates = episode1->predicates;
                    candidate->predicates.push_back(p2);
                    result.push_back(candidate);
                }

            }
        } 
    } 

    return result;
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
        Event *event = *iter;
        EpisodesMap::iterator tmp = singletons.find(event->predicate);

        if(tmp == singletons.end())
        {
            APPLOG("Episode with predicate %d has not been found", event->predicate);
            Episode *episode = new Episode();
            episode->parent = C0;
            episode->predicates.push_back(event->predicate);
            episode->occurences.push_back(event->occurence);

            singletons[event->predicate] = episode;
        } 
        else
        {
            Episode *episode = tmp->second;
            episode->occurences.push_back(event->occurence);
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
