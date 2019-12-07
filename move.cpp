#include "move.hpp"

using namespace std;

extern map<point,int> plant_regen;

void move_by( point const& offset, area_map& input_map, pair<vector<char>, vector<char>> ids, int org_num, map< int, organism*>& orgs ) 
{
  int energy = orgs[org_num]->energy_points();
  int max_energy = orgs[org_num]->max_energy();
  char id = orgs[org_num]->id();
  point pos = orgs[org_num]->position();
  vector<char> food = orgs[org_num]->food_chain();
  point pt = pos+offset;

  if ( orgs[org_num]->can_move() == true ) {
    if ( pt.x < input_map.extent().y && pt.x >= 0 && pt.y < input_map.extent().x && pt.y >= 0 ) {
      if ( input_map.at( pt.y, pt.x ) != '#' && input_map.at( pt.y, pt.x ) != '~' ) {
        int switch_case;
        char comp_org;
        if ( input_map.at( pt.y, pt.x ) == ' ' ) { switch_case = 0; }
        for(auto it = ids.first.begin(); it != ids.first.end(); ++it) {
          if ( input_map.at( pt.y, pt.x ) == *it ) {
            switch_case = 1;
            comp_org = *it;
            break;
          }
        }
        for(auto it = ids.second.begin(); it != ids.second.end(); ++it) {
          if ( input_map.at( pt.y, pt.x ) == *it ) {
            switch_case = 2;
            comp_org = *it;
            break;
          }
        }
        switch(switch_case)
{
    case 0:
        if ( energy > 0 ) {
            orgs[org_num]->update_energy(-1);
            input_map.at(pos.y,pos.x) = ' ';
            orgs[org_num]->position() += offset;
            input_map.at(pt.y,pt.x) = id;
        } else { 
          input_map.at( pos.y, pos.x ) = ' ';
          orgs.erase(org_num);
        }
        break;
    case 1: 
        for( char ch : food ) {
          if ( ch == comp_org ) {
            for(auto it2 = orgs.begin(); it2 != orgs.end(); ++it2) {
              if ( it2->second->position() == pt ) {
                if ( max_energy - energy >= ( it2->second->energy_points() + 1 ) ) {
                  orgs[org_num]->update_energy(it2->second->energy_points()-1);
                  input_map.at(pos.y,pos.x) = ' ';
                  orgs[org_num]->position() += offset;
                  plant_regen[pos] = 1;
                  input_map.at(pt.y,pt.x) = id;
                  break;
                } else if ( energy < max_energy ) {
                    orgs[org_num]->update_energy(max_energy - energy);
                    input_map.at(pos.y,pos.x) = ' ';
                    orgs[org_num]->position() += offset;
                    plant_regen[pos] = 1;
                    input_map.at(pt.y,pt.x) = id;
                    break;
                } else { break; }
              } 
            } break; 
          }
        }
        break;
    case 2:
        for( char ch : food ) {
          if ( ch == comp_org ) {
            for(auto it2 = orgs.begin(); it2 != orgs.end(); ++it2) {
              if ( it2->second->position() == pt ) {
                if ( max_energy - energy >= ( it2->second->energy_points() + 1 ) ) {
                  orgs[org_num]->update_energy(it2->second->energy_points()-1);
                  input_map.at(pos.y,pos.x) = ' ';
                  orgs[org_num]->position() += offset;
                  orgs.erase(it2->first);
                  input_map.at(pt.y,pt.x) = id;
                  break;
                } else if ( energy < max_energy ) {
                    orgs[org_num]->update_energy(max_energy - energy);
                    input_map.at(pos.y,pos.x) = ' ';
                    orgs[org_num]->position() += offset;
                    orgs.erase(it2->first);
                    input_map.at(pt.y,pt.x) = id;
                    break;
                } else { break; }
              }
            } break;
          }
        }
        break;
    default:
        { cout << "Error" << endl; }
}
      }
    }
  }
}