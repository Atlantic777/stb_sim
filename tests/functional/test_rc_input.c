#include <stdio.h>
#include "input.h"
#include "input/remote.h"
#include <stdint.h>

int main()
{
  input_t rc;
  rc_init(&rc);

  uint32_t ev;

	while(1)
	{
		rc.read(&ev);
		if(0xff != ev)
		{
			printf("d: %d x: %x\n", ev, ev);
		}
	}

  rc_deinit();
  return 0;
}
