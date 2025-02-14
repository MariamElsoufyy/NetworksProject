
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

#include "coordinator.h"
#include "node.h"
#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include "comsg_m.h"
using namespace std;
Define_Module(Coordinator);

void Coordinator::initialize()
{
    ifstream inputFile("../src/coordinator.txt");
    if (!inputFile.is_open())
    {
        EV << "Error in opening input file in coordinator";
        return;
    }

    string line;
    getline(inputFile, line);
    int node_id = line[0] - '0';           // Node_id= 0 or 1
    int start_time = stoi(line.substr(2)); /// line[1] = space

    sendDelayed("message", start_time, "port$o");
    // Close the file
    inputFile.close();
}

void Coordinator::handleMessage(cMessage *msg)
{
}
