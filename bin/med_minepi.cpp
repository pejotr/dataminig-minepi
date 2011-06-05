#include <assert.h>
#include <iostream>
#include <list>
#include <algorithm>
#include "med.hpp"


EpisodesCollection scan_event_sequence(const EventSequence& seq, const int freqTrsh);
EpisodesCollection generate_candidates(const EpisodesCollection& collection, 
        EpisodesCollection::iterator start, EpisodesCollection::iterator stop);


int main()
{
    const EventSequence seq = parse_data_file("file2-all.txt");
    const PredicatesSet predicateSet = generate_predicate_set(seq);

    minepi(seq, predicateSet, 100, 20);

}
