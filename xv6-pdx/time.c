#ifdef CS333_P2
#include "types.h"
#include "user.h"

int
main(int argc, char* argv[])
{
		int start = uptime();
    int elapsed;

    if(argc < 2){
        elapsed = uptime() - start;
        if(elapsed % 1000 < 10){
          printf(1, "(null) ran in %d.00%d seconds\n", elapsed/1000, elapsed%1000);
        }else if(elapsed % 1000 < 100){
					printf(1, "(null) ran in %d.0%d seconds\n", elapsed/1000, elapsed%1000);
				}else {
					printf(1, "(null) ran in %d.%d seconds\n", elapsed/1000, elapsed%1000);
				}
    }else {
			if(fork() == 0){
				exec(argv[1], &argv[1]);
			}else {
				wait();
				elapsed = uptime() - start;
        if(elapsed % 1000 < 10){
          printf(1, "%s ran in %d.00%d seconds\n", argv[1], elapsed/1000, elapsed%1000);
        }else if(elapsed % 1000 < 100){
					printf(1, "%s ran in %d.0%d seconds\n", argv[1], elapsed/1000, elapsed%1000);
				}else {
					printf(1, "%s ran in %d.%d seconds\n", argv[1], elapsed/1000, elapsed%1000);
				}
			}
		}

		exit();
}
#endif // CS333_P2