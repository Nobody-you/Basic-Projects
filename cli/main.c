#include "cli.h"
#include "interactive.h"

int main(int argc, const char *argv[])
{
	int sfd, res;
	sfd = cli_create();
	
	while (1) {
		res = Begin_interface(sfd);
		if (res == -1) {
			break;
		}
	}
	return 0;
}
