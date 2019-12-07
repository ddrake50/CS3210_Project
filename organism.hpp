#ifndef ORGANISM_HPP
#define ORGANISM_HPP

#include <iostream>
#include <sstream>
#include <memory>
#include <cassert>

#include "area_map.hpp"

using namespace std;

struct organism {
    using org_id = char;

    organism( org_id id, int max_energy ) : 
    id_( id ), max_energy_( max_energy ), energy_( max_energy ) {}

    virtual ~organism() {} 

    org_id id() const { return id_; }
    virtual string kind() const { assert( false ); return ""; }
    virtual int growth_coefficient() const { assert( false ); return 0; }
    virtual bool can_move() const { assert( false ); return false; }
    virtual int max_energy() const { assert( false ); return 0; }
    virtual int energy_points() { assert( false ); return 0; }
    virtual void update_energy(int i);
    virtual point& position() { return pos_; }
    virtual vector<char> food_chain() const { assert( false ); return vector<char>('\0'); }
    virtual organism* clone() const { return new organism(*this); };

    virtual void write( ostream& out ) const;
    
    virtual unique_ptr<organism> new_from_stream( istream& in ) const
    {
        org_id id;
        if ( in >> id  )
            return this->create( id );

        return nullptr;
    }

    unique_ptr<organism> create( org_id id ) const
    {
        assert( false );
        return nullptr;
    }

    private:
      org_id id_;
      int max_energy_;
      int energy_;
      point pos_;
      
};

#endif
