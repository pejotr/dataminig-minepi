#include <iostream>
#include <map>
#include <functional>
#include <algorithm>
#include <sstream>

#include "minepi.hpp"
#include "med.hpp"

static Episode *C0;
static EpisodesCollection __table;
static EpisodesCollection __singletons;

EpisodesCollection generate_candidates(const EpisodesCollection& collection, 
        const EpisodesCollection::iterator& start, const EpisodesCollection::iterator& stop);

//! Checks if provided candidates are frequent enough
const EpisodesCollection check_candidates(const EpisodesCollection& candidates, const int freqTrsh, 
        const int window);

//! Compute set of minimal occurences of an episode
static void compute_mo(Episode *episode, const int window);

//! Performs one-time event sequence scan
/*static*/ EpisodesCollection scan_event_sequence(const EventSequence& seq, const int freqTrsh);

const std::string predicates_to_string(const std::list<PredicateType>& predicates)
{
    std::list<PredicateType>::const_iterator iter, end = predicates.end();
    std::stringstream stream;

    for( iter = predicates.begin(); iter != end; iter++) {
        stream << *iter;
    }

    return stream.str();
}

const std::string occurences_to_string(const std::list<Occurence>& occurences)
{
    std::list<Occurence>::const_iterator iter, end = occurences.end();
    std::stringstream stream;

    for(iter = occurences.begin(); iter != end; iter++)
    {
        stream << iter->start << "-" << iter->stop << " ";
    }

    return stream.str();
}

void print_table()
{
    EpisodesCollection::iterator tableIter, tableEnd = __table.end();    
    
    for(tableIter = __table.begin(); tableIter != tableEnd; tableIter++)
    {
        std::string predicates = predicates_to_string((*tableIter)->predicates);
        std::string occurences = occurences_to_string((*tableIter)->occurences);

        std::cout << predicates << " : " << occurences << std::endl;
    } 
}

void minepi(const EventSequence& seq, const PredicatesSet& set, 
        const int freqTrsh, const int wnd)
{
    EpisodesCollection::iterator iter = __table.end();        

    C0 = new Episode;
    C0->parent = NULL;

    __singletons = scan_event_sequence(seq, freqTrsh);
    __table.insert(iter, __singletons.begin(), __singletons.end());

    EpisodesCollection newEpisodes = __singletons;

    while(newEpisodes.empty() == false )
    {
        EpisodesCollection candidates = generate_candidates(newEpisodes, newEpisodes.begin(), newEpisodes.end());
        newEpisodes = check_candidates(candidates, freqTrsh, wnd);

        __table.insert(iter, newEpisodes.begin(), newEpisodes.end());
    }

    print_table();
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
                    Episode *candidate = new Episode();
                    candidate->parent = episode1;
                    candidate->predicates = episode1->predicates;
                    candidate->predicates.push_back(p2);
                    result.push_back(candidate);

                    std::string predicatesStr = predicates_to_string(candidate->predicates);
                    APPLOG("Generating new candidate %s", predicatesStr.c_str());
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
        if( e->predicates.front() == p )
            return true;

        return false;
    }
};

/*static*/ const EpisodesCollection check_candidates(const EpisodesCollection& candidates , 
                const int freqTrsh, const int window)
{
    EpisodesCollection::const_iterator iter, end = candidates.end();
    EpisodesCollection newEpisodes;
     
    for(iter = candidates.begin(); iter != end; iter++)
    {
        Episode *candidate = *iter;
        compute_mo(candidate, window);

        if(candidate->occurences.size() >= freqTrsh) 
        {
            APPLOG("Found new episode");
            newEpisodes.push_back(candidate); 
        }
        else
        {
            std::string episodePredicates = predicates_to_string(candidate->predicates);
            APPLOG("Candidate %s is not frequent enough", episodePredicates.c_str());
        }

    }

    return newEpisodes;
}

static void compute_mo(Episode *episode, const int window)
{
    std::list<PredicateType> predicates = episode->predicates; 
    std::list<Occurence> parentOccurences = episode->parent->occurences;
    std::list<Occurence>::iterator iter, end = parentOccurences.end();
    PredicateType lastPredicate = predicates.back();

    predicates.pop_back();
    std::string basePredicate = predicates_to_string(predicates);
    APPLOG("Episode predicates base(%s) tail(%d)", basePredicate.c_str(), lastPredicate);

    EpisodesCollection::iterator result = std::find_if(__singletons.begin(), __singletons.end(), 
        std::bind2nd(find_predicate(), lastPredicate));

    Episode *e = *result;
    std::list<Occurence> lastPredicateOccurences = e->occurences;
    std::list<Occurence>::iterator predOIter, predOEnd = lastPredicateOccurences.end();

    for(iter = parentOccurences.begin(); iter != end; iter++)
    {
        for(predOIter = lastPredicateOccurences.begin(); predOIter != predOEnd; predOIter++) 
        {

            if(iter->stop < predOIter->start && (predOIter->start - iter->start) <= window )
            {
                APPLOG("New occurence located %d-%d", iter->start, predOIter->stop);
                Occurence o(iter->start, predOIter->stop);
                episode->occurences.push_back(o);
                break;
            }            

        }
    }         
}
