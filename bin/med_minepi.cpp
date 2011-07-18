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
    if(argc < 3) 
    {
        std::cout << "Usage: ./med_minepi [minimal-occurances] [window-size]" << std::endl;
        return 0;
    }


    const EventSequence seq = parse_data_file("file1-all.txt");
    const PredicatesSet predicateSet = generate_predicate_set(seq);


    int window = atoi(argv[2]);
    int freqTs = atoi(argv[1]);

    minepi(seq, predicateSet, freqTs, window);

}
