#include "reproduce.hpp"

using namespace std;

extern int org_nums;

void reproduce( area_map& input_map, int org_num, map< int, organism*>& orgs, map<char, shared_ptr<organism> > species ) 
{
  int energy = orgs[org_num]->energy_points();
  int max_energy = orgs[org_num]->max_energy();
  char id = orgs[org_num]->id();
  point pos = orgs[org_num]->position();

  point mate_pos;
  int mate_num;

  set<point> surrounding_pts = {point(-1,-1), point(-1,0), point(-1,1), point(0,-1),point(0,1), point(1,-1), point(1,0), point(1,1)};

  if ( energy > (max_energy / 2) ) {
    for ( auto pts : surrounding_pts ) {
      point pt = pos+pts;

      if ( pt.x < input_map.extent().y && pt.x >= 0 && pt.y < input_map.extent().x && pt.y >= 0 ) {
        if ( input_map.at( pt.y, pt.x ) == id ) {
          for ( auto it = orgs.begin(); it != orgs.end(); ++it ) {
            if ( it->second->position() == point(pt.x, pt.y) ) {
              mate_pos = it->second->position();
              mate_num = it->first;
              break;
            }
          }
        if ( energy > (max_energy / 2) ) {
          for ( auto pts2 : surrounding_pts ) {
            point pt2 = pos+pts2;
            point mate_pt = mate_pos+pts2;

            if ( mate_pt.x < input_map.extent().y && mate_pt.x >= 0 && mate_pt.y < input_map.extent().x && mate_pt.y >= 0 ) {
              if ( input_map.at( mate_pt.y, mate_pt.x ) == ' ' ) {
                input_map.at( mate_pt.y, mate_pt.x ) = id;
                make_offspring( id, orgs, mate_pt, species );
                break;
              } else if ( pt2.x < input_map.extent().y && pt2.x >= 0 && pt2.y < input_map.extent().x && pt2.y >= 0 ) {
              if ( input_map.at( pt2.y, pt2.x ) == ' ' ) {
                input_map.at( pt2.y, pt2.x ) = id;
                make_offspring( id, orgs, pt2, species );
                break;
              } 
            }
            } 
          }
        } break;
        } 
      }
    }
  } 
}

void make_offspring( char id, map< int, organism*>& orgs, point pt, map<char, shared_ptr<organism> > species ) 
{
  for( auto org : species ) {
    if ( id == org.first ) {
      organism* new_org = org.second->clone();
      orgs[org_nums] = new_org ;
      new_org->position() = pt;
      org_nums++;
    }
  }
}