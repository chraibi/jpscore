/**
 * \file        EventManager.h
 * \date        Jul 4, 2014
 * \version     v0.6
 * \copyright   <2009-2014> Forschungszentrum Jülich GmbH. All rights reserved.
 *
 * \section License
 * This file is part of JuPedSim.
 *
 * JuPedSim is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * JuPedSim is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with JuPedSim. If not, see <http://www.gnu.org/licenses/>.
 *
 * \section Description
 *
 *
 **/

#include <vector>
#include <string>

class Building;
class Router;
class GlobalRouter;
class QuickestPathRouter;
class RoutingEngine;

extern OutputHandler* Log;

class EventManager
{
private:
     std::vector<double> _event_times;
     std::vector<std::string> _event_types;
     std::vector<std::string> _event_states;
     std::vector<int> _event_ids;
     std::string _projectFilename;
     std::string _projectRootDir;
     Building *_building;
     FILE *_file;
     bool _dynamic;
     int _eventCounter;
     long int _lastUpdateTime;
     //save the router corresponding to the actual state of the building
     std::map<std::string, RoutingEngine*> _eventEngineStorage;
     //save the available routers defined in the simulation
     std::vector<RoutingStrategy> _availableRouters;

private:
     /**
      * collect the close doors and generate a new graph
      * @param _building
      */
     bool CreateRoutingEngine(Building* _b, int first_engine=false);

     /**
      * Create a router corresponding to the given strategy
      * @param strategy
      * @return a router/NULL for invalid strategies
      */
     Router * CreateRouter(const RoutingStrategy& strategy);

     /**
      * Update the knowledge about closed doors.
      * Each pedestrian who is xx metres from a closed door,
      * will save that information
      * @param _b, the building object
      */
     bool UpdateAgentKnowledge(Building* _b);

     /**
      * Merge the knowledge of the two pedestrians.
      * The information with the newest timestamp
      * is always accepted with a probability of one.
      * @param p1, first pedestrian
      * @param p2, second pedestrian
      */
     void MergeKnowledge(Pedestrian* p1, Pedestrian* p2);

     /**
      * Update the pedestrian route based on the new information
      * @param p1
      * @return
      */
     bool UpdateRoute(Pedestrian* p1);

public:
     ///constructor
     EventManager(Building *_b);

     /**
      * Read and parse the events
      * @return false if an error occured
      */
     bool ReadEventsXml();

     /**
      * Print the parsed events
      */
     void ListEvents();

     /**
      * Read and parse events from a text file
      * @param time
      */
     void ReadEventsTxt(double time);

     /**
      * Process the events at runtime
      * @param time
      */
     void Update_Events(double time);

     //process the event using the current time stamp
     //from the pedestrian class
     void ProcessEvent();
     //Eventhandling
     void CloseDoor(int id);
     void OpenDoor(int id);
     void ChangeRouting(int id, const std::string& state);
     void GetEvent(char* c);
};
