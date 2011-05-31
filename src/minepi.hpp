#include "datastructures.hpp"

#ifndef _MINEPI_HPP_
#define _MENEPI_HPP_

void generate_candidates(PSubEpisode *node);
void check_candidates();

/**
 * Compute minimal occurence of given episode
 * @param node Episode
 */
void mo(PSubEpisode *node);

#endif
