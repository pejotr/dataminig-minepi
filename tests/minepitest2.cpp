#include <assert.h>
#include <iostream>
#include <list>
#include <algorithm>
#include "med.hpp"


EpisodesCollection scan_event_sequence(const EventSequence& seq, const int freqTrsh);
EpisodesCollection generate_candidates(const EpisodesCollection& collection, 
        EpisodesCollection::iterator start, EpisodesCollection::iterator stop);


int main(int argc, char** argv)
{
    const EventSequence seq = parse_data_file("parse.dat");
    const PredicatesSet predicateSet = generate_predicate_set(seq);
    Windows windows;

    windows.push_back(10);

    minepi(seq, predicateSet, 2, 150);

}
