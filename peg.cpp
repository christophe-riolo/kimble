#include "kimble.hpp"
Peg::Peg(Player* owner)
{
    this->owner = owner;
}

unsigned int Peg::boardPos()
{
    int startSpot = owner->number*(BOARD_SIZE/(owner->game->nplayers));
    return ( position + startSpot) % BOARD_SIZE;
}
// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4 fdm=syntax
