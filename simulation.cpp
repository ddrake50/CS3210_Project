#include "simulation.hpp"

using namespace std;

map<point,int> plant_regen;
int org_nums = 0;

void start( string map_file, string species_file ) 
{
  fstream newfile;
  area_map input_map;
  map<char, shared_ptr<organism> > orgs;
  vector<char> plant_ids;
  vector<char> animal_ids;

  newfile.open( map_file, ios::in );
  if (newfile.is_open()){
    input_map = load_map( newfile );
    newfile.close();
  }
  newfile.open( species_file, ios::in );
  if (newfile.is_open()){
    string buffer;
    while ( std::getline( newfile, buffer ) ) {
      auto org = read_from_string( buffer );
      orgs[org->id()] = move(org);
      }
    newfile.close();
  }

  auto locations = load_positions( input_map, orgs);
  auto ids = extract_ids(orgs);

  start_simulation( input_map, locations, ids, orgs );
}

map< int, organism* > load_positions( area_map input_map , map<char, shared_ptr<organism> > orgs ) 
{
  map< int, organism* > org_locations;
  for ( auto r = 0; r<input_map.extent().y; r++ ) {
    for ( auto c = 0; c<input_map.extent().x; c++ ) {
      for( auto it = orgs.begin(); it != orgs.end(); ++it ) {
        if ( input_map.at( c, r ) == it->first ) {
          organism* new_org = it->second->clone();
          org_locations[org_nums] = new_org ;
          new_org->position() = point(r,c);
          org_nums++;
        }
      }
    }
  }
  return org_locations;
}

pair<vector<char>, vector<char>> extract_ids(map<char, shared_ptr<organism> > const& orgs) 
{
  vector<char> plant_ids;
  vector<char> animal_ids;

  for (auto const& org : orgs) {
    if (org.second->kind() == "plant") {
      plant_ids.push_back(org.first);
    } else { animal_ids.push_back(org.first); }
  }
  return make_pair(plant_ids, animal_ids);
}

void start_simulation( area_map input_map, map< int, organism*>& locations, pair<vector<char>, vector<char>> ids, map<char, shared_ptr<organism> > orgs ) 
{
  int num_runs = 0;
  char menu_selection;

  while( menu_selection != 's' && menu_selection != 'S' ) {
    cout << "Simulation Menu\n" << "Run 1 iteration: Press R\n" << "Run several simulations: Press M\n" << "Press S to Save\n" << "Press A to Abort\n";
    cin >> menu_selection;
    if( menu_selection == 'r' || menu_selection == 'R' ) { 
      run_simulation( input_map, locations, ids, orgs );
      num_runs++;
      save_map( cout, input_map );
      cout << "Ran 1 iteration\n";
    } else if ( menu_selection == 'm' || menu_selection == 'M' ) { 
        for( int i = 0; i<10; ++i ){
          run_simulation( input_map, locations, ids, orgs );
        }
        num_runs+=10;
        save_map(cout, input_map); 
        cout << "Ran batch of iterations\n";
    } else if ( menu_selection == 's' || menu_selection == 'S' ) { 
        save_map_to_file("simulation_state.txt", input_map);
        cout << "Simulation Saved\n"; 
    } else if ( menu_selection == 'a' || menu_selection == 'A' ) {
        if (num_runs > 0) {
          cout << "Save and Exit?\n" << "Press: Y/N\n";
          cin >> menu_selection;
          if ( menu_selection == 'y' || menu_selection == 'Y' ) { 
            menu_selection = 'S';
            cout << "Simulation saved and exited\n";
          } else if ( menu_selection == 'n' || menu_selection == 'N' ) { 
              cout << "Simulation Aborted\n"; 
              break;
          }
        } else { 
            cout << "Simulation Aborted\n"; 
            break; 
        }
    } else { cout << "Please reselect option\n"; }
  }
}

void run_simulation( area_map& input_map, map< int, organism*>& locations, pair<vector<char>, vector<char>> ids, map<char, shared_ptr<organism> > orgs ) 
{
  auto locations_copy = locations;

  for(auto it = locations_copy.begin(); it != locations_copy.end(); ++it) {
    if (it->second->kind() == "plant") { 
      point pos = it->second->position();

      if (input_map.at(pos.y, pos.x) == ' ') {
        plant_regen[it->second->position()] += 1;
        if (plant_regen[pos] >= it->second->growth_coefficient()) {
              input_map.at(pos.y, pos.x) = it->second->id();
        } 
      }
    }
  }
  for(auto it = locations_copy.begin(); it != locations_copy.end(); ++it) { 
    if (it->second->kind() == "herbivore") { 
      move_by( point(1,0), input_map, ids, it->first, locations );
      reproduce( input_map, it->first, locations, orgs );
    }
  }
  locations_copy = locations;

  for(auto it = locations_copy.begin(); it != locations_copy.end(); ++it) {
    if (it->second->kind() == "omnivore") { 
      move_by( point(0,1), input_map, ids, it->first, locations );
      reproduce( input_map, it->first, locations, orgs );
    }
  }
}
