#include <iostream>
#include <sstream>
#include <unistd.h>

unsigned int MAXCONN = 20;
unsigned int MAXINIT = 1;

struct node
{
	unsigned int* conn = new unsigned int[MAXCONN];
	int numconn = 0;
	int numinit = 0;
	bool received = false;
	bool sent = false;
};

node** nodes = NULL;
int numnodes = 0;

node* add_node()
{
	node** temp = nodes;
	numnodes++;
	nodes = new node*[numnodes];
	for(int i = 0; i < numnodes - 1; i++)
		nodes[i] = temp[i];
	delete[] temp;
	nodes[numnodes - 1] = new node;
	return nodes[numnodes - 1];
}

bool connect_nodes(unsigned int i0, unsigned int i1)
{
	node* n0 = nodes[i0];
	node* n1 = nodes[i1];
	if(i0 == i1)return false;
	if(n1->numconn >= MAXCONN)return false;
	for(int i = 0; i < n1->numconn; i++)
		if(n1->conn[i] == i0)return false;
	n0->conn[n0->numconn] = i1;
	n1->conn[n1->numconn] = i0;
	n0->numconn++;
	n1->numconn++;
	n0->numinit++;
	return true;
}

void autoconnect(unsigned int i0, unsigned int i1, bool* attempted)
{
	
}

void write_node(unsigned int i)
{
	std::cout << i << " - ";
	node* n0 = nodes[i];
	for(int i = 0; i < n0->numconn; i++)
		std::cout << n0->conn[i] << " ";
	std::cout << std::endl;
}

void run_sim(int maxtime)
{
	node** buf = new node*[numnodes];
	for(int i = 0; i < numnodes; i++) {
		nodes[i]->received = false;
		nodes[i]->sent = false;
		buf[i] = new node;
		*(buf[i]) = *(nodes[i]);
	}
	int iter = 0;
	int received = 1, oreceived = 1;
	int sent = 0, osent = 0;
	nodes[rand() % numnodes]->received = true;
	float r[10000];
	float t = 0;
	while(1)
	{
		oreceived = received;
		osent = sent;
		std::cout << "Iteration " << iter << ": ";
		for(int i = 0; i < numnodes; i++)
			if(nodes[i]->received && !nodes[i]->sent)
			{
				for(int j = 0; j < nodes[i]->numconn; j++)
					buf[nodes[i]->conn[j]]->received = true;
				buf[i]->sent = true;
			}
		for(int i = 0; i < numnodes; i++)
		{
			if(buf[i]->received && !nodes[i]->received)
				received++;
			if(buf[i]->sent && !nodes[i]->sent)
				sent++;
		}
		bool cont = false;
		for(int i = 0; i < numnodes; i++) {
			*(nodes[i]) = *(buf[i]);
			if(!nodes[i]->received)cont = true;
		}
		std::cout << received << " peers received ("
			<< received - oreceived << " new), "
			<< sent << " peers sent ("
			<< sent - osent << " new), R = "
			<< (float)(received - oreceived) / (sent - osent);
		std::cout << std::endl;
		if(received - oreceived == 0)
		{
			std::cout << std::endl << "An error occured!" << std::endl
				<< "There are peers in the network who were unable to connect." << std::endl
				<< "Therefore, the message cannot reach the whole network." << std::endl;
			return;
		}
		r[iter] = (float)(received - oreceived) / (sent - osent);
		iter++;
		if(!cont)break;
		float it = rand() % maxtime;
		t += it / 1000000;
		usleep(it);
	}
	float R = 0;
	for(int i = 0; i < iter; i++)
		R += r[i];
	R /= iter;
	std::cout << std::endl;
	std::cout << "RESULTS" << std::endl << "=======" << std::endl
		<< "P = " << numnodes << " peers" << std::endl
		<< "I = " << iter << " iterations" << std::endl
		<< "s = " << (float)t / iter << " seconds/iteration " << std::endl
		<< "t = " << t << " seconds" << std::endl
		<< "R = " << R << " peers" << std::endl;
}

void generate_net(int num)
{
	for(int i = 0; i < num; i++)
	{
		add_node();
		for(int j = 0; j < MAXINIT; j++)
			for(int k = 0; k < 256; k++)
			{
				int i1 = rand() % numnodes;
				if(connect_nodes(i, i1))break;
			}
	}
	for(int i = 0; i < numnodes; i++)
		write_node(i);
}

int main(int argc, char** argv)
{
	srand(time(0));
	int num = 25;
	int maxtime = 500000;
	std::cout << "Number of nodes: ";
	std::cin >> num;
	std::cout << std::endl << "Maximum iteration time (mircroseconds): ";
	std::cin >> maxtime;
	std::cout << std::endl << "Maximum number of connections per peer: ";
	std::cin >> MAXCONN;
	std::cout << std::endl << "Maximum number of connections initiated by each peer: ";
	std::cin >> MAXINIT;
	std::cout << std::endl << "Generating network..." << std::endl << std::endl;;
	generate_net(num);

	std::cout << std::endl << "Running simulation..." << std::endl << std::endl;
	run_sim(maxtime);
	std::cout << std::endl;
	return 0;
}
