#include "kimble.hpp"
Peg::Peg(const Player& owner):
    m_owner{owner}
{
}

unsigned int Peg::boardPos() const
{
    int startSpot = m_owner.getNumber() * (Kimble::board_size/(m_owner.getGame().m_nplayers));
    return ( m_position + startSpot) % Kimble::board_size;
}

const Player& Peg::getOwner() const
{
    return m_owner;
}

ostream& operator<<(ostream& out, const Peg& peg)
{
    if ( peg.m_position != -1 )
        out << peg.boardPos() << "(" << peg.m_position << ")";
    else 
        out << "Start";  
    return out;
}

    
// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4 fdm=syntax
