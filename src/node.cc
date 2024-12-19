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
typedef enum
{
    frame_arrival,
    timeout,
} event_type;
using namespace std;
Define_Module(Node);

//////Go Back N utility functions/////
static boolean between(seq_nr a, seq_nr b, seq_nr c)
{
    /* Return true if a <=b<c circularly; false otherwise. */
    if (((a <= b) && (bc)) || ((c < a) && (a <= b)) || ((b < c) && (c < a)))
        return (true);
    else
        return (false);
}

static void send_data(seq_nr frame_nr, seq_nr frame_expected, packet buffer[])
{
    /* Construct and send a data frame. */
    frame s;                   /* scratch variable */
    s.info = buffer[frame_nr]; /* insert packet into frame */
    s.seq = frame_nr;
    /* insert sequence number into frame */
    /* transmit the frame */
    start_timer(frame_nr); /* start the timer running */
}

void Node::initialize()
{
    frame_expected = 0;
}

void Node::handleMessage(cMessage *msg)
{

    if (msg->getArrivalGateId() == 0) /// msg from coordinator
    {

        Comsg_Base *coordinatormsg = check_and_cast<Comsg_Base *>(msg);
        node_id = coordinatormsg->getNode_id();
        filename = "D:\\Projects\\Networks-Project\\NetworksProject\\src\\input" + to_string(node_id) + ".txt";
        cMessage *selfmsg = new cMessage("");
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
            EV << "At time " << simTime() << " Node[" << node_id << "], Introducing channel error with code =[" << m.prefix << "]" << endl;
            m.data = line.substr(5);
            msgs.push_back(m);
        }

        string currentmsg_data = msgs[0].data;
        bitset<4> currentmsg_bits = msgs[0].prefix;
        bitset<8> flag('$');
        bitset<8> ESC('/');
        string currentmsg_string = flag.to_string();
        vector<bitset<8>> currentmsg_vector;
        currentmsg_vector.push_back(flag);
        for (int i = 0; i < currentmsg_data.size(); i++)
        {
            EV << i << " " << currentmsg_data[i] << endl;
            if (currentmsg_data[i] == '$' || currentmsg_data[i] == '/')
            {

                currentmsg_vector.push_back(ESC);
                currentmsg_string = currentmsg_string + ESC.to_string();
            }

            std::bitset<8> bits(currentmsg_data[i]);
            currentmsg_vector.push_back(bits);
            currentmsg_string = currentmsg_string + bits.to_string();
        }

        currentmsg_string = currentmsg_string + flag.to_string();
        currentmsg_vector.push_back(flag);
    }
    else if (msg->getArrivalGateId() == 1) /// ana receiver
    {

        NodeMessage_Base *ReceivedMessage = check_and_cast<NodeMessage_Base *>(msg);
        int seqnum = ReceivedMessage->getHeader();
        string payload = msg->getPayload(); // Extract payload
        if (seq_num == frame_expected)      // Frame is in order
        {
            EV << "Frame [Seq: " << seq_num << "] is in order. Delivering to application layer.\n";
        }
    }

    /////Go Back N algo/////
}
