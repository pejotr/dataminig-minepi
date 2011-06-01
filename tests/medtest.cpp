#include <assert.h>
#include <iostream>
#include <list>
#include "med.hpp"



int main()
{
    std::cout << "Testing parse_data_line 1 ";
    {
        const EventSequence seq = parse_data_line("53[141, seq 47681, 3991.3455],1: 5, 6, 11");
        assert(seq.size() == 3);

        std::list<int> cmp;
        cmp.push_back(5);
        cmp.push_back(6);
        cmp.push_back(11);
 
        std::list<int> values;
        for(EventSequence::const_iterator iter = seq.begin(); iter != seq.end(); iter++)
        {
            values.push_back(static_cast<int>((*iter)->predicate));
            assert((*iter)->occurence.start == 53);
        }

        assert(cmp == values);
    }
    std::cout << "[PASSED]" << std::endl;

    std::cout << "Testing parse_data_line 2 ";
    {
        const EventSequence seq = parse_data_line("53[141, seq 47681, 31.094000],1:5");
        assert(seq.size() == 1);
        assert(static_cast<int>(seq.front()->predicate) == 5 );
    }
    std::cout << "[PASSED]" << std::endl;


    std::cout << "Testing parse_data_file 1 ";
    {
        const EventSequence seq = parse_data_file("parse.dat");
        assert(seq.size() == 10);
        
        std::list<int> cmp;
        cmp.push_back(4);
        cmp.push_back(4);
        cmp.push_back(4);
        cmp.push_back(6);
        cmp.push_back(1);
        cmp.push_back(5);
        cmp.push_back(1);
        cmp.push_back(5);
        cmp.push_back(5);
        cmp.push_back(1);

        std::list<int> values;
        for(EventSequence::const_iterator iter = seq.begin(); iter != seq.end(); iter++)
        {
            values.push_back(static_cast<int>((*iter)->predicate));
        }
        assert(cmp == values);
    }
    std::cout << "[PASSED]" << std::endl;

    std::cout << " *** ALL TESTS PASSED! *** " << std::endl;

}
