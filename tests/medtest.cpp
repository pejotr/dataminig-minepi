#include <assert.h>
#include <iostream>
#include <string>

#include "datastructures.hpp"
#include "med.hpp"



int main()
{

    parse_data_line("53[141,seq 3991],1: 5, 6");
    parse_data_line("54[141,seq 3991],1: 5, 4");
    parse_data_line("55[141,seq 3991],1: 4");
    parse_data_line("56[141,seq 3991],1: 3, 4");
    parse_data_line("57[141,seq 3991],1: 5");
    parse_data_line("58[141,seq 3991],1: 3, 4, 5");
    parse_data_line("59[141,seq 3991],1: 3");

    assert(__root_events_list.size() == 4);

    SubEpisode *se = __root_events_list[5];
    assert(se != NULL);
    assert(se->occurences.size() == 4);


    SubEpisode *se2 = __root_events_list[3];
    assert(se2 != NULL);
    assert(se2->occurences.size() == 3);

    std::cout << "ALL TESTS PASSED!" << std::endl;

}
