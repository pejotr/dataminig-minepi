#include <iostream>
#include <map>
#include <functional>
#include <algorithm>
#include "minepi.hpp"
#include "med.hpp"

static Episode *C0;
static EpisodesCollection __table;
static EpisodesCollection __singletons;

EpisodesCollection generate_candidates(const EpisodesCollection& collection, 
        const EpisodesCollection::iterator& start, const EpisodesCollection::iterator& stop);

const EpisodesCollection check_candidates(const EpisodesCollection& candidates);

//! Compute set of minimal occurences of an episode
static void compute_mo(Episode *episode/*, const Windows& windows*/);

//! Performs one-time event sequence scan
/*static*/ EpisodesCollection scan_event_sequence(const EventSequence& seq, const int freqTrsh);

void minepi(const EventSequence& seq, const PredicatesSet& set, 
        const int freqTrsh, const Windows& wnds)
{
    EpisodesCollection::iterator iter;        

    C0 = new Episode;
    C0->parent = NULL;

    __singletons = scan_event_sequence(seq, freqTrsh);
    //__table.insert(iter, __singletons.begin(), __singletons.end());

    EpisodesCollection candidates = generate_candidates(__singletons, __singletons.begin(), __singletons.end());
    check_candidates(candidates);
    
}

EpisodesCollection generate_candidates(const EpisodesCollection& collection, 
        const EpisodesCollection::iterator& start, const EpisodesCollection::iterator& stop)
{
    EpisodesCollection result;
    EpisodesCollection::iterator iter1, iter2;

    for(iter1 = start; iter1 != stop; iter1++)
    {
        Episode *episode1 = *iter1;
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
    EpisodesCollection singletons;
    EpisodesMap singletonsIndexer;
    EpisodesCollection result;
    EventSequence::const_iterator iter = seq.begin(), 
                end = seq.end();
    EpisodesCollection::iterator colliter, collend;
   
    for(; iter != end; iter++)
    {
        Event *event = *iter;
        EpisodesMap::iterator tmp = singletonsIndexer.find(event->predicate);

        if(tmp == singletonsIndexer.end())
        {
            APPLOG("Episode with predicate %d has not been found", event->predicate);
            Episode *episode = new Episode();
            episode->parent = C0;
            episode->predicates.push_back(event->predicate);
            episode->occurences.push_back(event->occurence);

            singletonsIndexer[event->predicate] = episode;
            singletons.push_back(episode);
        } 
        else
        {
            Episode *episode = tmp->second;
            episode->occurences.push_back(event->occurence);
        }
        
    }

    collend = singletons.end();
    for(colliter = singletons.begin(); colliter != collend; )
    {
        EpisodesCollection::iterator temp = colliter++;
        Episode *episode = *temp;

        if( episode->occurences.size() < freqTrsh )
        {
            APPLOG("Episode %d with freq = %d appeared to be not frequent enough", 
                        episode->predicates.front(), episode->occurences.size());
            singletons.erase(temp);
            continue;
        }
    }

    return singletons;
}

struct find_predicate : public std::binary_function<Episode*, PredicateType&, bool>
{
    bool operator()(const Episode* e, const PredicateType& p) const
    {
        APPLOG("Porownanie %d i %d", e->predicates.front(), p);
        if( e->predicates.front() == p )
            return true;

        return false;
    }
};

/*static*/ const EpisodesCollection check_candidates(const EpisodesCollection& candidates 
                /*const Windows& windows*/)
{
    EpisodesCollection::const_iterator iter, end = candidates.end();
     
    for(iter = candidates.begin(); iter != end; iter++)
    {
        Episode *e = *iter;
        compute_mo(e);
    }

    return EpisodesCollection();

}

static void compute_mo(Episode *episode/*, const Windows& windows*/)
{
    std::list<PredicateType> predicates = episode->predicates; 
    std::list<Occurence> parentOccurences = episode->parent->occurences;
    std::list<Occurence>::iterator iter, end = parentOccurences.end();
    PredicateType lastPredicate = predicates.back();

    APPLOG("Episode predicates %d %d", (int)predicates.front(), lastPredicate);

    EpisodesCollection::iterator result = std::find_if(__singletons.begin(), __singletons.end(), 
        std::bind2nd(find_predicate(), lastPredicate));

    Episode *e = *result;
    std::list<Occurence> lastPredicateOccurences = e->occurences;
    std::list<Occurence>::iterator predOIter, predOEnd = lastPredicateOccurences.end();

    for(iter = parentOccurences.begin(); iter != end; iter++)
    {
        for(predOIter = lastPredicateOccurences.begin(); predOIter != predOEnd; predOIter++) 
        {

            if(iter->stop < predOIter->start /* && inWindow */)
            {
                APPLOG("New occurence located %d %d", iter->start, predOIter->stop);
                Occurence o(iter->start, predOIter->stop);
                episode->occurences.push_back(o);
                break;
            }            

        }
    }         
}
