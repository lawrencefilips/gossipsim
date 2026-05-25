# gossipsim
A simulation of a gossip protocol in a P2P network.

To compile it, run the following command:

```g++ main.cpp -o gossipsim```

Then run the compiled program by running `./gossipsim`.

The program takes a number of parameters, then runs the simulation by picking a random virtual peer to originate the message from. It outputs the following variables:

  - *P*, the number of peers in the network,
  - *I*, the number of iterations of the gossip procedure,
  - *s*, the average number of seconds per iteration,
  - *t*, the total time the message took to reach the entirety of the network, and
  - *R*, the average number of new peers who received the message for each peer that relayed the message.
