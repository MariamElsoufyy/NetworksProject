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

#ifndef __NETWORKSPROJECT_NODE_H_
#define __NETWORKSPROJECT_NODE_H_


#include <omnetpp.h>
#include <string>
#include<bitset>
#include<vector>
#include "comsg_m.h"
#include "NodeMessage_m.h"
using namespace omnetpp;
using namespace std;

struct MessageData{

    string data;
    bitset<4> prefix;  ////////written from right to left 4<------0
                                  //prefix[0] -> Delay
                                  //prefix[1] -> duplication
                                  //prefix[2] -> Loss
                                  //prefix[3] -> Modification
};

class Node : public cSimpleModule
{
private:
    string filename;
    int node_id = -1 ;
    vector<MessageData> msgs;

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

#endif
