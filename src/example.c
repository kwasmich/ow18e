#include "ow18b.h"

#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static volatile bool s_terminated = false;

// SIGINT handler for clean shutdown
static void signal_handler(int signal){
   s_terminated = true;
}

//Function to retrieve the samples:
static bool sample_func(ow_sample_t sample, void* context)
{
	//Print the sample:
	printf("%lf %s\n", sample.value, ow_unit_to_short_str(sample.unit));

	//Are we done yet?
	return !s_terminated;
}

int main(void)
{
	signal(SIGINT, signal_handler);

	//Use automatic parameters (first Bluetooth interface, automatic address scanning):
	ow_config_t config =
	{
		.dev_id = OW_DEV_ID_AUTOMATIC,
		.scan_mode = OW_SCAN_MODE_AUTOMATIC,
		.connect_mode = OW_CONNECT_MODE_AUTOMATIC
	};

	if (!ow_recv(&config, sample_func, NULL))
	{
		perror("Receiving failed");
		return EXIT_FAILURE;
	}

	return 0;
}
