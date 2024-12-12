//
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

#include "node.h"
#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
using namespace std;
Define_Module(Node);

void Node::initialize()
{
}

void Node::handleMessage(cMessage *msg)
{
    Comsg_Base *coordinatormsg = check_and_cast<Comsg_Base *>(msg);
    if (coordinatormsg) /// if it's coordinator msg
    {
        int node_id = coordinatormsg->getNode_id();
        string filename = "node" + to_string(node_id) + ".txt";
        ifstream inputFile(filename);
        if (simTime()==coordinatormsg->getStart_time())
        {
            if (!inputFile.is_open())
            {
                cout << "Error in opening input file";

                return;
            }

            string line;
            while (getline(inputFile, line))
            {

                string pre = line.substr(0, 4);
                MessageData m;
                m.prefix = bitset<4>(pre);
                //////// prefix is written from right to left 4<------0
                // prefix[0] -> Delay
                // prefix[1] -> duplication
                // prefix[2] -> Loss
                // prefix[3] -> Modification
                m.data = line.substr(5);
                msgs.push_back(m);
            }
        }
    }
}
