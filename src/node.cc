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
#define MAX_SEQ ((1 << par("PT").intValue()) - 1) //2^WS-1
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
                //EV<<bits<<endl;



                parity = parity ^ bits;

                bitstring=bitstring + bits.to_string();



            }


        NodeMessage_Base* nodemsg = new NodeMessage_Base("");
        nodemsg->setTrailer(parity.to_string().c_str());
        ////error creation

        if(currentmsg_bits==0000) //NO ERROR
        {

            nodemsg->setM_Payload(bitstring.c_str());
            sendDelayed(nodemsg, par("PT").doubleValue()+par("TD").doubleValue(), 1);

        }
        else if(currentmsg_bits==0001)//Delay Error
        {


            nodemsg->setM_Payload(bitstring.c_str());
            EV<<par("PT").doubleValue()+par("TD").doubleValue()+par("ED").doubleValue();
            sendDelayed(nodemsg, par("PT").doubleValue()+par("TD").doubleValue()+par("ED").doubleValue(), "portnode$o");



        }
        else if(currentmsg_bits==0011)//Delay Error
        {


            nodemsg->setM_Payload(bitstring.c_str());
            EV<<par("PT").doubleValue()+par("TD").doubleValue()+par("ED").doubleValue();
            sendDelayed(nodemsg, par("PT").doubleValue()+par("TD").doubleValue()+par("ED").doubleValue(), "portnode$o");



        }







    else if ( node_id!=-1) /// ana sender
    {






    }
    else ///ana receiver

    {

        NodeMessage_Base *ReceivedMessage = check_and_cast<NodeMessage_Base *>(msg);
        int seqnum = ReceivedMessage->getHeader();
        string payload = ReceivedMessage->getM_Payload(); // Extract payload
        std::bitset<8> parity = 0000000;
                   // calculating parity bits (bitset 8)
                   for (int i = 0; i < payload.size(); i++)
                   {
                       std::bitset<8> bits(payload[i]);
                       EV << bits << endl;
                       parity = parity ^ bits;

                       bitstring = bitstring + bits.to_string();
                   }

                   std::bitset<8> error = parity ^ bitset<8>(ReceivedMessage->getTrailer());
                   if (error != 00000000)
                   {
                       // There is an error detected so send NOACK
                       //  Create and send NACK
                       NodeMessage_Base *nack = new NodeMessage_Base("NACK");
                       nack->setHeader(seqnum); // NACK for the received sequence number
                       nack->setM_Payload("");     // No payload needed for NACK
                       nack->setACK(0);      // Trailer for NACK can be set to 0
                       nack->setTrailer("");
                       send(nack, "out");
                       return;

                   }
                   //No error Msg and msg sent is the expected one//

                   else if(seqnum == frame_expected)//correct frame sent in order
                   {
                       frame_expected = (frame_expected + 1) % (MAX_SEQ + 1);// Update frame_expected to the next sequence number and wrap around
                       NodeMessage_Base *ackmsg = new NodeMessage_Base("ACK");
                       ackmsg->setHeader(seqnum);  // ACK for the current sequence number
                       ackmsg->setM_Payload("");
                       ackmsg->setACK(1);
                       ackmsg->setTrailer("");
                       send(ackmsg, "out");

                       ///Process the payload
                       string deframed_payload = "";
                           bool escape_next = false;
                           char ESC = '/';
                           // Start deframing
                           for (int i = 1; i < payload.size() - 1; i++) // Skip the first and last characters (flags)
                           {
                               if (escape_next)
                               {
                                   // Handle escaped character
                                   deframed_payload += payload[i];
                                   escape_next= false;
                               }
                               else if (payload[i] == ESC)
                               {
                                   // Set flag to escape the next character
                                   escape_next = true;
                               }
                               else
                               {
                                   // Normal character
                                   deframed_payload += payload[i];
                               }
                           }

                           EV << "Deframed payload: " << deframed_payload << endl;
                   }

                   //No error but msg sent isnt the frame expected
                   else
                   {
                       EV << "Frame [Seq: " << seqnum << "] is out of order. Ignoring.\n";

                   }


                 }

    }
    }
    /////Go Back N algo/////
