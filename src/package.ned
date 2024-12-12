package networksproject;

import ned.IdealChannel;


@license(LGPL);
//
// TODO documentation
//
network Network
{
    submodules:
        node0: Node {
            @display("p=49,111;i=device/switch");
        }
        node1: Node {
            @display("p=231,111;i=device/switch");
        }
        coordinator: Coordinator {
            @display("p=145,198;i=block/cogwheel");
        }
    connections:
        coordinator.ports++ <--> IdealChannel <--> node0.port;
        coordinator.ports++ <--> IdealChannel <--> node1.port;
}

