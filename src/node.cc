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
        filename = "input" + to_string(node_id) + ".txt";
        cMessage *selfmsg = new cMessage("-1");
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
            // EV << m.data << " " << m.prefix.to_string() << endl;
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
            // EV << i << " " << currentmsg_string << endl;
        }

        currentmsg_string = currentmsg_string + flag;

        ///////parity
        string bitstring = "";
        std::bitset<8> parity = 0000000;
        for (int i = 0; i < currentmsg_string.size(); i++)
        {

            std::bitset<8> bits(currentmsg_string[i]);
            // EV<<bits<<endl;

            parity = parity ^ bits;

            bitstring = bitstring + bits.to_string();
        }

        NodeMessage_Base *nodemsg = new NodeMessage_Base("");
        nodemsg->setTrailer(parity.to_string().c_str());

        ////error creation

        if (!strcmp(currentmsg_bits.to_string().c_str(),"0000")) // NO ERROR
        {

            nodemsg->setM_Payload(bitstring.c_str());
            sendDelayed(nodemsg, par("PT").doubleValue() + par("TD").doubleValue(), 1);
        }

        else if (!strcmp(currentmsg_bits.to_string().c_str(),"0001")) // Delay Error
               {


                   nodemsg->setM_Payload(bitstring.c_str());

                   sendDelayed(nodemsg, par("PT").doubleValue() + par("TD").doubleValue() + par("ED").doubleValue(), "portnode$o");
               }
        else if (!strcmp(currentmsg_bits.to_string().c_str(),"0011")) // Delay and duplication Error
               {

                   nodemsg->setM_Payload(bitstring.c_str());
                   NodeMessage_Base *nodemsg2 = new NodeMessage_Base();
                   nodemsg2->setM_Payload(nodemsg->getM_Payload());
                   nodemsg2->setTrailer(nodemsg->getTrailer());
                   sendDelayed(nodemsg, par("PT").doubleValue() + par("TD").doubleValue() + par("ED").doubleValue(), "portnode$o");
                   sendDelayed(nodemsg2, par("PT").doubleValue() + par("TD").doubleValue() + par("ED").doubleValue() + par("DD").doubleValue(), "portnode$o");
               }
        else if (!strcmp(currentmsg_bits.to_string().c_str(),"0010")) //duplication Error
                     {

                         nodemsg->setM_Payload(bitstring.c_str());
                         NodeMessage_Base *nodemsg2 = new NodeMessage_Base();
                         nodemsg2->setM_Payload(nodemsg->getM_Payload());
                         nodemsg2->setTrailer(nodemsg->getTrailer());

                         sendDelayed(nodemsg, par("PT").doubleValue() + par("TD").doubleValue() , "portnode$o");
                         sendDelayed(nodemsg2, par("PT").doubleValue() + par("TD").doubleValue() + par("DD").doubleValue(), "portnode$o");
                     }
        else if ((!strcmp(currentmsg_bits.to_string().c_str(),"0100")) || (!strcmp(currentmsg_bits.to_string().c_str(),"0101"))) //Loss Error & loss and delay
                           {

                               nodemsg->setM_Payload(bitstring.c_str());

                           }
        else if ((!strcmp(currentmsg_bits.to_string().c_str(),"0110")) || (!strcmp(currentmsg_bits.to_string().c_str(),"0111"))) //Loss and duplication Error
                           {

            nodemsg->setM_Payload(bitstring.c_str());
                                   NodeMessage_Base *nodemsg2 = new NodeMessage_Base();
                                   nodemsg2->setM_Payload(nodemsg->getM_Payload());
                                   nodemsg2->setTrailer(nodemsg->getTrailer());

                           }
        else if (currentmsg_bits[3]==1)// Modification Error
        {



            if (!strcmp(currentmsg_bits.to_string().c_str(),"1000")) // Delay Error
                           {


                               nodemsg->setM_Payload(bitstring.c_str());


                           }
        }




    }

    else if (node_id != -1) /// ana sender
    {
    }
    else /// ana receiver

    {

        EV << simTime() << endl;

        NodeMessage_Base *ReceivedMessage = check_and_cast<NodeMessage_Base *>(msg);
        int seqnum = ReceivedMessage->getHeader();
        string payload = ReceivedMessage->getM_Payload(); // Extract payload
        if (seqnum == frame_expected)                     // Frame is in order
        {
            EV << "Frame [Seq: " << seqnum << "] is in order. Delivering to application layer.\n";
        }
    }
}

/////Go Back N algo/////
