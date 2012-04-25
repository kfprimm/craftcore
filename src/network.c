
#include <craftcore.h>
#include <enet/enet.h>

ENetHost *host;

void cc_network_update()
{
	
}

void cc_shutdown_network()
{
	enet_deinitialize();
}

void cc_startup_network()
{
	if (enet_initialize () != 0)
  {
      fprintf (stdout, "An error occurred while initializing ENet.\n");
      return EXIT_FAILURE;
  }
  atexit(cc_shutdown_network);
}
