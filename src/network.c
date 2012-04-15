
#include <craftcore.h>
#include <enet/enet.h>

ENetHost *host;

void ccNetworkUpdate()
{
	
}

void ccNetworkShutdown()
{
	enet_deinitialize();
}

void ccNetworkStartup()
{
	if (enet_initialize () != 0)
  {
      fprintf (stdout, "An error occurred while initializing ENet.\n");
      return EXIT_FAILURE;
  }
  atexit(ccNetworkShutdown);
}
