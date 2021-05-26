#ifdef CS333_P2
#include "types.h"
#include "user.h"
#include "uproc.h"

int
main(void)
{
    uint max;
    struct uproc* table;
    int counter;

    max = 16;
    table = malloc(sizeof(struct uproc) * max);

    counter = getprocs(max, table);
    if(counter < 0){
        printf(2, "error");
    }else{
        int elapsed, cpu;
        printf(1, "\nPID\tName         UID\tGID\tPPID\tElapsed\tCPU\tState\tSize\n");
        for(int i = 0; i < counter; i++){
            printf(1, "%d\t%s\t     %d\t\t%d\t%d\t", table[i].pid, table[i].name, table[i].uid, table[i].gid, table[i].ppid);
            elapsed = table[i].elapsed_ticks;
            if(elapsed % 1000 < 10){
                printf(1, "%d.00%d\t", elapsed/1000, elapsed%1000);
            }else if(elapsed % 1000 < 100){
                printf(1, "%d.0%d\t", elapsed/1000, elapsed%1000);
            }else{
              printf(1, "%d.%d\t", elapsed/1000, elapsed%1000);
            }

            cpu = table[i].CPU_total_ticks;
            if(cpu % 1000 < 10){
            	printf(1, "%d.00%d\t%s\t%d\n", cpu/1000, cpu%1000, table[i].state, table[i].size);
            }else if(cpu % 1000 < 100){
                printf(1, "%d.0%d\t%s\t%d\n", cpu/1000, cpu%1000, table[i].state, table[i].size);
            }else{
              printf(1, "%d.%d\t%s\t%d\n", cpu/1000, cpu%1000, table[i].state, table[i].size);
            }
        }
    }
    exit();
}

#endif // CS333_P2