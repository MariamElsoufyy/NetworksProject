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
#define MAX_SEQ (par("WS").intValue() + 1) // WS+1
typedef enum
{
    frame_arrival,
    timeout,
} event_type;
string Node::FrameAndFlag(string msg)
{

    char flag = '$';
    char ESC = '/';
    string currentmsg_string = "$";
    for (int i = 0; i < msg.size(); i++)
    {
        if (msg[i] == '$' || msg[i] == '/')
        {

            currentmsg_string = currentmsg_string + ESC;
        }

        currentmsg_string = currentmsg_string + msg[i];
        // EV << i << " " << currentmsg_string << endl;
    }

    currentmsg_string = currentmsg_string + flag;

    return currentmsg_string;
}
void Node::ModifyBit(string &bitstring)
{
    int rand = uniform(0, bitstring.size() - 1);

    if (bitstring[rand] == '1')
    {
        bitstring[rand] = '0';
    }
    else
    {
        bitstring[rand] = '1';
    }
}
bool Node::between(int a, int b, int c)
{
    /* Return true if a <= b < c circularly; false otherwise. */
    if (((a <= b) && (b < c)) || ((c < a) && (a <= b)) || ((b < c) && (c < a)))
        return true;
    else
        return false;
}
bitset<8> Node::CalculateParity(string Data, string &bitstring)
{
    std::bitset<8> parity = 0000000;
    for (int i = 0; i < Data.size(); i++)
    {
        std::bitset<8> bits(Data[i]);
        parity = parity ^ bits;
    }
    return parity;
}
void Node::CreateError(string &bitstring, bitset<4> currentmsg_bits, NodeMessage_Base *nodemsg, NodeMessage_Base *nodemsg2, int &DelayValue, int &dupDelay)
{
    if (!strcmp(currentmsg_bits.to_string().c_str(), "0000")) // NO ERROR
    {

        nodemsg->setM_Payload(bitstring.c_str());
        DelayValue = par("PT").doubleValue() + par("TD").doubleValue();
    }
    else if (!strcmp(currentmsg_bits.to_string().c_str(), "0001")) // Delay Error
    {

        nodemsg->setM_Payload(bitstring.c_str());
        DelayValue = par("PT").doubleValue() + par("TD").doubleValue() + par("ED").doubleValue();
    }
    else if (!strcmp(currentmsg_bits.to_string().c_str(), "0011")) // Delay and duplication Error
    {

        nodemsg->setM_Payload(bitstring.c_str());
        nodemsg2->setM_Payload(nodemsg->getM_Payload());
        nodemsg2->setTrailer(nodemsg->getTrailer());
        DelayValue = par("PT").doubleValue() + par("TD").doubleValue() + par("ED").doubleValue();
        dupDelay = par("PT").doubleValue() + par("TD").doubleValue() + par("ED").doubleValue() + par("DD").doubleValue();
    }
    else if (!strcmp(currentmsg_bits.to_string().c_str(), "0010")) // duplication Error
    {

        nodemsg->setM_Payload(bitstring.c_str());

        nodemsg2->setM_Payload(nodemsg->getM_Payload());
        nodemsg2->setTrailer(nodemsg->getTrailer());

        DelayValue = par("PT").doubleValue() + par("TD").doubleValue();
        dupDelay = par("PT").doubleValue() + par("TD").doubleValue() + par("DD").doubleValue();
    }
    else if ((!strcmp(currentmsg_bits.to_string().c_str(), "0100")) || (!strcmp(currentmsg_bits.to_string().c_str(), "0101"))) // Loss Error & loss and delay
    {

        nodemsg->setM_Payload(bitstring.c_str());
        scheduleAt(simTime() + par("PT").doubleValue(), new cMessage("lost"));
    }
    else if ((!strcmp(currentmsg_bits.to_string().c_str(), "0110")) || (!strcmp(currentmsg_bits.to_string().c_str(), "0111"))) // loss + dup + delay || loss + dup
    {

        nodemsg->setM_Payload(bitstring.c_str());

        nodemsg2->setM_Payload(nodemsg->getM_Payload());
        nodemsg2->setTrailer(nodemsg->getTrailer());
        scheduleAt(simTime() + par("PT").doubleValue(), new cMessage("lost"));
        // scheduleAt(simTime() + par("PT").doubleValue(), new cMessage("lost"));
    }
    else if (currentmsg_bits[3] == 1) // Modification Error
    {

        ModifyBit(bitstring);

        if (!strcmp(currentmsg_bits.to_string().c_str(), "1000")) // Modification only
        {

            nodemsg->setM_Payload(bitstring.c_str());
            DelayValue = par("PT").doubleValue() + par("TD").doubleValue();
        }

        else if (!strcmp(currentmsg_bits.to_string().c_str(), "1001")) // Delay + modification  Error
        {

            nodemsg->setM_Payload(bitstring.c_str());
            DelayValue = par("PT").doubleValue() + par("TD").doubleValue() + par("ED").doubleValue();
        }
        else if (!strcmp(currentmsg_bits.to_string().c_str(), "1010")) // Modification and Duplication
        {

            nodemsg->setM_Payload(bitstring.c_str());

            nodemsg2->setM_Payload(nodemsg->getM_Payload());
            nodemsg2->setTrailer(nodemsg->getTrailer());
            DelayValue = par("PT").doubleValue() + par("TD").doubleValue();
            dupDelay = par("PT").doubleValue() + par("TD").doubleValue() + par("DD").doubleValue();
        }
        else if (!strcmp(currentmsg_bits.to_string().c_str(), "1011")) // Modification and Duplication +delay
        {

            nodemsg->setM_Payload(bitstring.c_str());

            nodemsg2->setM_Payload(nodemsg->getM_Payload());
            nodemsg2->setTrailer(nodemsg->getTrailer());
            DelayValue = par("PT").doubleValue() + par("TD").doubleValue() + par("ED").doubleValue();
            dupDelay = par("PT").doubleValue() + par("TD").doubleValue() + par("ED").doubleValue() + par("DD").doubleValue();
        }
        else if ((!strcmp(currentmsg_bits.to_string().c_str(), "1100")) || (!strcmp(currentmsg_bits.to_string().c_str(), "1101"))) // Modification and loss || modification + loss + delay
        {
            EV << simTime() << endl;
            nodemsg->setM_Payload(bitstring.c_str());
            scheduleAt(simTime() + par("PT").doubleValue(), new cMessage("lost"));
        }
        else if ((!strcmp(currentmsg_bits.to_string().c_str(), "1110")) || (!strcmp(currentmsg_bits.to_string().c_str(), "1111"))) // Modification and loss  +duplication || modification + loss + duplication + delay
        {

            nodemsg->setM_Payload(bitstring.c_str());
            scheduleAt(simTime() + par("PT").doubleValue(), new cMessage("lost"));
        }
    }
}
void Node::initialize()
{
    frame_expected = 0;
    window_size = par("WS").intValue();           // Set window size from .ini file
    send_window.resize(window_size, nullptr);     // Allocate buffer
    timeout_event = new cMessage("TimeoutEvent"); // Create timeout event
}

void Node::handleMessage(cMessage *msg)
{
    if (msg->getArrivalGateId() == 0) /// msg from coordinator
    {
            string filename;
            string line;
            sender = this->getName();
            if (strcmp(this->getName(), "node0"))
            {
                filename = "../src/input0.txt";
            }
            else
            {
                filename = "../src/input1.txt";
            }
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
        }

    }
    else
    {
        // sending frame//
        NodeMessage_Base *ReceivedMessage = check_and_cast<NodeMessage_Base *>(msg);
        while (next_frame_to_send < ack_expected + window_size)
        { // Window is not full
            NodeMessage_Base *frame = new NodeMessage_Base("");
            NodeMessage_Base *frame2 = new NodeMessage_Base("");
            int seq_num = next_frame_to_send % (MAX_SEQ + 1); // Sequence number wraps around
            frame->setHeader(seq_num);                        // Add sequence number
            string currentmsg_data = msgs[0].data;
            bitset<4> currentmsg_bits = msgs[0].prefix;
            msgs.erase(msgs.begin());
            ///////framing//////////////
            string currentmsg_string = FrameAndFlag(currentmsg_data);
            ///////parity
            string bitstring = "";
            bitset<8> parity = CalculateParity(currentmsg_string, bitstring);
            frame->setTrailer(parity.to_string().c_str());
            frame2->setTrailer(parity.to_string().c_str());

            ////error creation

            int DelayValue = -1;
            int dupDelay = -1;
            NodeMessage_Base *nodemsg2 = new NodeMessage_Base();

            CreateError(bitstring, currentmsg_bits, frame, frame2, DelayValue, dupDelay);

            send_window[seq_num] = frame;
            // Save frame in the send window
            if (DelayValue != -1)
            {
                sendDelayed(frame->dup(), DelayValue, "portnode$o"); // Send frame
            }
            if (dupDelay != -1)
            {
                sendDelayed(frame2, dupDelay, "portnode$o"); // Send frame
            }

            EV << "Sent frame " << seq_num << endl;

            next_frame_to_send++; // Move to the next frame

            if (!timeout_event->isScheduled())
            {                                                                   // Start timer if not already started
                scheduleAt(simTime() + par("TO").doubleValue(), timeout_event); // send time out
            }
        }
        if (msg == timeout_event)
        { // Timeout occurred
            EV << "Timeout. Retransmitting window." << endl;
            for (int i = ack_expected; i < next_frame_to_send; i++)
            {
                int seq_num = i % (MAX_SEQ + 1); // Sequence number wraps around
                if (send_window[seq_num])
                {
                    sendDelayed(send_window[seq_num]->dup(), par("PT").doubleValue(), "out"); // Re-send frame
                    EV << "Retransmitted frame " << seq_num << endl;
                }
            }
            // scheduleAt(simTime() + par("TO").doubleValue(), timeout_event); // Restart timer
        }
        if (NodeMessage_Base *ack = dynamic_cast<NodeMessage_Base *>(msg))
        { // If it's an ACK
            if (ack->getFrame_Type() == 1)
            {                                   // ACK type
                int ack_num = ack->getHeader(); // Get ACK number
                EV << "Received ACK for frame " << ack_num << endl;

                // Slide the window
                while (ack_expected != (ack_num + 1) % (MAX_SEQ + 1))
                {
                    delete send_window[ack_expected % window_size]; // Free frame memory
                    send_window[ack_expected % window_size] = nullptr;
                    ack_expected = (ack_expected + 1) % (MAX_SEQ + 1); // Move the window
                }

                // Stop timer if all frames are acknowledged
                if (ack_expected == next_frame_to_send)
                {
                    cancelEvent(timeout_event);
                }
            }
        }
    }
    else /// receiver code
    {
        NodeMessage_Base *ReceivedMessage = check_and_cast<NodeMessage_Base *>(msg);
        int seqnum = ReceivedMessage->getHeader();
        string payload = ReceivedMessage->getM_Payload(); // Extract payload
        string trailer = ReceivedMessage->getTrailer();   // Extract trailer

        if (ReceivedMessage->isSelfMessage())
        {
            EV << "At time[" << simTime() << "], Node[" << ReceivedMessage->getHeader() << "] Sending [";
            EV << ReceivedMessage->getFrame_Type() << "] with number [" << ReceivedMessage->getACK();
            string ack_loss = "YES";
            if (uniform(0, 1) > par("LP").doubleValue())
            {
                ack_loss = "NO";
                sendDelayed(ReceivedMessage, par("TD").doubleValue(), "out");
            }
            EV << "] , loss [" << ack_loss << "]";
            return;
        }
        std::bitset<8> parity_check = CalculateParity(payload, trailer);
        NodeMessage_Base *ack_nack = new NodeMessage_Base("NACK");
        if (parity_check != 00000000)
        {
            // There is an error detected so send NACK
            ack_nack->setHeader(seqnum); // NACK for the received sequence number
            ack_nack->setM_Payload("");  // No payload needed for NACK
            ack_nack->setFrame_Type(0);  // Trailer for NACK can be set to 0
        }
        else if (parity_check == 00000000 && seqnum == frame_expected) // correct frame sent in order
        {
            frame_expected = (frame_expected + 1) % (MAX_SEQ + 1); // Update frame_expected to the next sequence number and wrap around
            ack_nack->setHeader(seqnum);                           // ACK for the current sequence number
            ack_nack->setM_Payload("");

            /// Process the payload
            string deframed_payload = "";
            bool escape_next = false;
            string ESC = "00011011";
            string s = "";
            // Start deframing
            for (int i = 8; i < payload.size() - 9; i = i + 8) // Skip the first and last characters (flags)
            {
                if (escape_next)
                {
                    // Handle escaped character
                    deframed_payload += payload.substr(i, 8);
                    escape_next = false;
                }
                else if (!strcmp(payload.substr(i, 8).c_str(), ESC.c_str()))
                {
                    // Set flag to escape the next character
                    escape_next = true;
                }
                else
                {
                    // Normal character
                    deframed_payload += payload.substr(i, 8);
                    int asciiCode = std::bitset<8>(payload.substr(i, 8)).to_ulong();
                    char letter = (char)asciiCode;
                    s = s + letter;
                }
            }

            EV << "Deframed payload: " << deframed_payload << endl;
            EV << s;
        }
        // No error but msg sent isnt the frame expected
        else
        {
            EV << "Frame [Seq: " << seqnum << "] is out of order. Ignoring.\n";
        }
        send(ack_nack, "portnode$o");
    }
}
