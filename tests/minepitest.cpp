#include <assert.h>
#include <iostream>
#include <list>
#include <algorithm>
#include "med.hpp"


EpisodesCollection scan_event_sequence(const EventSequence& seq, const int freqTrsh);
EpisodesCollection generate_candidates(const EpisodesCollection& collection, 
        const EpisodesCollection::iterator& start, const EpisodesCollection::iterator& stop);

const EpisodesCollection check_candidates(const EpisodesCollection& candidates);

void myfunction (PredicateType i) {
  std::cout << i;
}


int main()
{
    {
        const EventSequence seq = parse_data_file("parse.dat");
        EpisodesCollection collection = scan_event_sequence(seq, 2);

        assert( static_cast<int>((collection.front())->predicates.front()) == 4 );
        assert( static_cast<int>((collection.front())->predicates.size())  == 1 );
        assert( collection.front()->occurences.size() == 3 );

        collection.pop_front();

        assert( static_cast<int>((collection.front())->predicates.front()) == 1 );
        assert( collection.front()->occurences.size() == 3 );
    }
    std::cout << "--------------------------------------------------------------------------------" << std::endl;
    std::cout << "************************ [scan_event_sequence 1 PASSED] ************************" << std::endl;
    std::cout << "--------------------------------------------------------------------------------" << std::endl;


    {
        const EventSequence seq = parse_data_file("parse.dat");
        EpisodesCollection collection = scan_event_sequence(seq, 2);
       
        assert(collection.size() == 3);

        EpisodesCollection candidates = generate_candidates(collection, collection.begin(), collection.end());
        EpisodesCollection::iterator i, e = candidates.end();

        for(i = candidates.begin(); i != e; i++) {
            std::cout << " \t TEST: ";
            std::list<PredicateType> predicates = (*i)->predicates;
            std::for_each(predicates.begin(), predicates.end(), myfunction);
            std::cout << std::endl;
        }
    }
    std::cout << "--------------------------------------------------------------------------------" << std::endl;
    std::cout << "************************ [generate_candidates 1 PASSED] ************************" << std::endl;
    std::cout << "--------------------------------------------------------------------------------" << std::endl;
}
