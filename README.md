# gossipsim
A simulation of a gossip protocol in a P2P network.

To compile it, run the following command:

```g++ main.cpp -o gossipsim```

Then run the compiled program by running `./gossipsim`.

The program takes a number of parameters:

  - Number of virtual nodes in the network,
  - Maximum iteration time in microseconds,
  - Maximum number of connections accepted by each virtual node, and
  - Maximum number of connections initiated by each virtual node upon bootstrapping.

It constructs a virtual network based off of these parameters, then runs the simulation by picking a random virtual peer to originate the message from.

Upon completing the simulation outputs the following variables:

  - *P*, the number of peers in the network,
  - *I*, the number of iterations of the gossip procedure,
  - *s*, the average number of seconds per iteration,
  - *t*, the total time the message took to reach the entirety of the network, and
  - *R*, the average number of peers who newly received the message for each peer that relayed it.
