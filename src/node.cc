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

    if (msg->getArrivalGateId() == 0) /// msg from coordinator
    {

        Comsg_Base *coordinatormsg = check_and_cast<Comsg_Base *>(msg);
        node_id = coordinatormsg->getNode_id();
        filename = "input" + to_string(node_id) + ".txt";
        cMessage *selfmsg = new cMessage("");
        EV << simTime();
        scheduleAt(coordinatormsg->getStart_time(), selfmsg);
    }
    else if (msg->isSelfMessage()) /// if it's self msg so sim reached starttime
    {

        ifstream inputFile(filename);
        if (!inputFile.is_open())
        {
            EV << "Error in opening input file in node" << endl;

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
            int i = 5;
            while (line[i] == ' ')
            {
                i++;
            }
            m.data = line.substr(i);
            msgs.push_back(m);
            EV << m.data << " " << m.prefix.to_string() << endl;
        }

        ////mn awel hena should be copied in sender
        string currentmsg_data = msgs[0].data;
        bitset<4> currentmsg_bits = msgs[0].prefix;
        msgs.erase(msgs.begin());


        ///////framing//////////////

        char flag = '$';
        char ESC = '/';
        string currentmsg_string = "$";
        for (int i = 0; i < currentmsg_data.size(); i++)
        {
            if (currentmsg_data[i] == '$' || currentmsg_data[i] == '/')
            {

                currentmsg_string = currentmsg_string + ESC;
            }

            currentmsg_string = currentmsg_string + currentmsg_data[i];
            EV << i << " " << currentmsg_string << endl;
        }

        currentmsg_string = currentmsg_string + flag;


        ///////parity
        string bitstring ="";
        std::bitset<8> parity=0000000;
        for(int i =0 ; i < currentmsg_string.size();i++ )
            {

                std::bitset<8> bits(currentmsg_string[i]);
                EV<<bits<<endl;

                //stringvec.push_back(bits);
                std::bitset<8> parity=0000000;

                parity = parity ^ bits;

                bitstring=bitstring + bits.to_string();



            }



        if(){



        }
    }
    else if (msg->getArrivalGateId() == 1) /// ana receiver
    {
    }
}
