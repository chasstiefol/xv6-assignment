# Code Modification Report

## UIDs and GIDs and PPIDs
### proc.h
```diff --git a/proc.h b/proc.h
index 0a0b4c5..dc0578a 100644
--- a/proc.h
+++ b/proc.h
@@ -49,6 +49,15 @@ struct proc {
   struct file *ofile[NOFILE];  // Open files
   struct inode *cwd;           // Current directory
   char name[16];               // Process name (debugging)
	#ifdef CS333_P1
	uint start_ticks;
	#endif // CS333_P1
+  #ifdef CS333_P2
+  uint uid;                    // Process User ID
+  uint gid;                    // Process Group ID
+  #endif // CS333_P2
 }
```

### user.h
```diff --git a/user.h b/user.h
index 31d9134..5e4f461 100644
--- a/user.h
+++ b/user.h
@@ -43,3 +43,14 @@ int atoi(const char*);
 int atoo(const char*);
 int strncmp(const char*, const char*, uint);
 #endif // PDX_XV6
 #ifdef CS333_P1
 int date(struct rtcdate*);
 #endif // CS333_P1
+#ifdef CS333_P2
+uint getuid(void);
+uint getgid(void);
+uint getppid(void);
+int setuid(uint);
+int setgid(uint);
+#endif // CS333_P2
```

### syscall.h
```diff --git a/syscall.h b/syscall.h
index 7fc8ce1..660117a 100644
--- a/syscall.h
+++ b/syscall.h
@@ -1,24 +1,31 @@
 #define SYS_halt       SYS_close+1
 // student system calls begin here. Follow the existing pattern.
 #define SYS_date       SYS_halt+1
+#define SYS_getuid     SYS_date+1
+#define SYS_getgid     SYS_getuid+1
+#define SYS_getppid    SYS_getgid+1
+#define SYS_setuid     SYS_getppid+1
+#define SYS_setgid     SYS_setuid+1
```

### syscall.c
```diff --git a/syscall.c b/syscall.c
index 9105b52..175a64a 100644
--- a/syscall.c
+++ b/syscall.c
@@ -106,6 +106,17 @@ extern int sys_uptime(void);
 #ifdef PDX_XV6
 extern int sys_halt(void);
 #endif // PDX_XV6
 #ifdef CS333_P1
 extern int sys_date(void);
 #endif // CS333_P1
+#ifdef CS333_P2
+extern int sys_getuid(void);
+extern int sys_getgid(void);
+extern int sys_getppid(void);
+extern int sys_setuid(void);
+extern int sys_setgid(void);
+#endif // CS333_P2

 static int (*syscalls[])(void) = {
 [SYS_fork]    sys_fork,
@@ -132,6 +143,17 @@ static int (*syscalls[])(void) = {
 #ifdef PDX_XV6
 [SYS_halt]    sys_halt,
 #endif // PDX_XV6
 #ifdef CS333_P1
 [SYS_date]    sys_date,
 #endif // CS333_P1
+#ifdef CS333_P2
+[SYS_getuid]    sys_getuid,
+[SYS_getgid]    sys_getgid,
+[SYS_getppid]   sys_getppid,
+[SYS_setuid]    sys_setuid,
+[SYS_setgid]    sys_setgid,
+#endif // CS333_P2
 };

 #ifdef PRINT_SYSCALLS
@@ -160,6 +182,17 @@ static char *syscallnames[] = {
 #ifdef PDX_XV6
   [SYS_halt]    "halt",
 #endif // PDX_XV6
 #ifdef CS333_P1
 [SYS_date]    "date",
 #endif // CS333_P1
+#ifdef CS333_P2
+  [SYS_getuid]    "getuid",
+  [SYS_getgid]    "getgid",
+  [SYS_getppid]   "getppid",
+  [SYS_setuid]    "setuid",
+  [SYS_setgid]    "setgid",
+#endif // CS333_P2
 };
 #endif // PRINT_SYSCALLS
```

### usys.S
```diff --git a/usys.S b/usys.S
index 0d4eaed..7c0ca74 100644
--- a/usys.S
+++ b/usys.S
@@ -30,3 +30,10 @@ SYSCALL(sbrk)
 SYSCALL(sleep)
 SYSCALL(uptime)
 SYSCALL(halt)
 SYSCALL(date)
+SYSCALL(getuid)
+SYSCALL(getgid)
+SYSCALL(getppid)
+SYSCALL(setuid)
+SYSCALL(setgid)
```

### sysproc.c
```diff --git a/sysproc.c b/sysproc.c
index 98563ea..68f423d 100644
--- a/sysproc.c
+++ b/sysproc.c
@@ -8,6 +8,7 @@
 #include "proc.h"
 #ifdef PDX_XV6
 #include "pdx-kernel.h"
 #endif // PDX_XV6

 int
@@ -97,3 +98,88 @@ sys_halt(void)
   return 0;
 }
 #endif // PDX_XV6

 #ifdef CS333_P1
 int
 sys_date(void)
 {
   struct rtcdate *d;
 
   if(argptr(0, (void*)&d, sizeof(struct rtcdate))   0)
     return -1;
   cmostime(d);
   return 0;
 }
 #endif // CS333_P1
+
+#ifdef CS333_P2
+int
+sys_getuid(void)
+{
+  return myproc()->uid;
+}
+
+int
+sys_getgid(void)
+{
+  return myproc()->gid;
+}
+
+int
+sys_getppid(void)
+{
+  if(myproc()->parent == NULL){
+    return myproc()->pid;
+  }
+  return myproc()->parent->pid;
+}
+
+int
+sys_setuid(void)
+{
+  int uid;
+
+  if(argint(0, &uid) < 0){
+    return -1;
+  }
+  if(uid < 0 || uid > 32767){
+    return -1;
+  }
+
+  myproc()->uid = (uint)uid;
+  return 0;
+}
+
+int
+sys_setgid(void)
+{
+  int gid;
+
+  if(argint(0, &gid) < 0){
+    return -1;
+  }
+
+  if(gid < 0 || gid > 32767){
+    return -1;
+  }
+
+  myproc()->gid = (uint)gid;
+  return 0;
+}
+#endif // CS333_P2
```

### proc.c
```diff --git a/proc.c b/proc.c
index d030537..3909bd7 100644
--- a/proc.c
+++ b/proc.c
@@ -6,6 +6,10 @@
 #include "x86.h"
 #include "proc.h"
 #include "spinlock.h"
+#ifdef CS333_P2
+#include "pdx.h"
+#endif // CS333_P2


@@ -186,6 +199,11 @@ userinit(void)
   acquire(&ptable.lock);
   p->state = RUNNABLE;
   release(&ptable.lock);
+
+  #ifdef CS333_P2
+  p->uid = DEFAULT_UID;
+  p->gid = DEFAULT_GID;
+  #endif // CS333_P2
 }

 // Grow current process's memory by n bytes.
@@ -247,6 +265,10 @@ fork(void)
   safestrcpy(np->name, curproc->name, sizeof(curproc->name));

   pid = np->pid;
+  #ifdef CS333_P2
+  np->uid = curproc->uid;
+  np->gid = curproc->gid;
+  #endif
```

## Process Execution Time
### proc.h
```diff --git a/proc.h b/proc.h
index 0a0b4c5..dc0578a 100644
--- a/proc.h
+++ b/proc.h
@@ -49,6 +49,15 @@
	 #ifdef CS333_P2
	 uint uid;                    // Process User ID
	 uint gid;                    // Process Group ID
+  uint cpu_ticks_total;        // total elapsed ticks in CPU
+  uint cpu_ticks_in;           // ticks when scheduled
	 #endif // CS333_P2
 };
 ```

 ### proc.c
```diff --git a/proc.c b/proc.c
index d030537..3909bd7 100644
--- a/proc.c
+++ b/proc.c
@@ -149,6 +153,15 @@ allocproc(void)
   memset(p->context, 0, sizeof *p->context);
   p->context->eip = (uint)forkret;

	 #ifdef CS333_P1
	 p->start_ticks = ticks;
	 #endif // CS333_P1
+
+  #ifdef CS333_P2
+  p->cpu_ticks_in = 0;
+  p->cpu_ticks_total = 0;
+  #endif //CS333_P2
+
   return p;
 }

@@ -389,6 +411,9 @@ scheduler(void)
       c->proc = p;
       switchuvm(p);
       p->state = RUNNING;
+      #ifdef CS333_P2
+      p->cpu_ticks_in = ticks;
+      #endif // CS333_P2
       swtch(&(c->scheduler), p->context);
       switchkvm();

@@ -429,6 +454,9 @@ sched(void)
   if(readeflags()&FL_IF)
     panic("sched interruptible");
   intena = mycpu()->intena;
+  #ifdef CS333_P2
+  p->cpu_ticks_total += ticks - (p->cpu_ticks_in);
+  #endif // CS333_P2
   swtch(&p->context, mycpu()->scheduler);
   mycpu()->intena = intena;
 }
 ```

 ## The "ps" Command
 ### user.h
```diff --git a/user.h b/user.h
index 31d9134..5e4f461 100644
--- a/user.h
+++ b/user.h
@@ -43,3 +43,14 @@ 
 #ifdef CS333_P2
 uint getuid(void);
 uint getgid(void);
 uint getppid(void);
 int setuid(uint);
 int setgid(uint);
+int getprocs(uint, struct uproc*);
 #endif // CS333_P2
```

### syscall.h
```diff --git a/syscall.h b/syscall.h
index 7fc8ce1..660117a 100644
--- a/syscall.h
+++ b/syscall.h
@@ -1,24 +1,31 @@
 // student system calls begin here. Follow the existing pattern.
 #define SYS_date       SYS_halt+1
 #define SYS_getuid     SYS_date+1
 #define SYS_getgid     SYS_getuid+1
 #define SYS_getppid    SYS_getgid+1
 #define SYS_setuid     SYS_getppid+1
 #define SYS_setgid     SYS_setuid+1
+#define SYS_getprocs   SYS_setgid+1
```

### sycall.c
```diff --git a/syscall.c b/syscall.c
index 9105b52..175a64a 100644
--- a/syscall.c
+++ b/syscall.c
@@ -106,6 +106,17 @@ extern int sys_uptime(void);
 #ifdef PDX_XV6
 extern int sys_halt(void);
 #endif // PDX_XV6
 #ifdef CS333_P1
 extern int sys_date(void);
 #endif // CS333_P1
 #ifdef CS333_P2
 extern int sys_getuid(void);
 extern int sys_getgid(void);
 extern int sys_getppid(void);
 extern int sys_setuid(void);
 extern int sys_setgid(void);
+extern int sys_getprocs(void);
 #endif // CS333_P2

 static int (*syscalls[])(void) = {
 [SYS_fork]    sys_fork,
@@ -132,6 +143,17 @@ static int (*syscalls[])(void) = {
 #ifdef PDX_XV6
 [SYS_halt]    sys_halt,
 #endif // PDX_XV6
 #ifdef CS333_P1
 [SYS_date]    sys_date,
 #endif // CS333_P1
 #ifdef CS333_P2
 [SYS_getuid]    sys_getuid,
 [SYS_getgid]    sys_getgid,
 [SYS_getppid]   sys_getppid,
 [SYS_setuid]    sys_setuid,
 [SYS_setgid]    sys_setgid,
+[SYS_getprocs]  sys_getprocs,
 #endif // CS333_P2
 };

 #ifdef PRINT_SYSCALLS
@@ -160,6 +182,17 @@ static char *syscallnames[] = {
 #ifdef PDX_XV6
   [SYS_halt]    "halt",
 #endif // PDX_XV6
 #ifdef CS333_P1
	 [SYS_date]    "date",
 #endif // CS333_P1
 #ifdef CS333_P2
	 [SYS_getuid]    "getuid",
	 [SYS_getgid]    "getgid",
	 [SYS_getppid]   "getppid",
	 [SYS_setuid]    "setuid",
	 [SYS_setgid]    "setgid",
+  [SYS_getprocs]  "getprocs",
 #endif // CS333_P2
 };
 #endif // PRINT_SYSCALLS
```

### usys.S
```diff --git a/usys.S b/usys.S
index 0d4eaed..7c0ca74 100644
--- a/usys.S
+++ b/usys.S
@@ -30,3 +30,10 @@ SYSCALL(sbrk)
 SYSCALL(sleep)
 SYSCALL(uptime)
 SYSCALL(halt)
 SYSCALL(date)
 SYSCALL(getuid)
 SYSCALL(getgid)
 SYSCALL(getppid)
 SYSCALL(setuid)
 SYSCALL(setgid)
+SYSCALL(getprocs)
```

### sysproc.c
```diff --git a/sysproc.c b/sysproc.c
index 98563ea..68f423d 100644
--- a/sysproc.c
+++ b/sysproc.c
@@ -8,6 +8,7 @@
 #include "proc.h"
 #ifdef PDX_XV6
 #include "pdx-kernel.h"
+#include "uproc.h"
 #endif // PDX_XV6

@@ -97,3 +98,88 @@ 

   myproc()->gid = (uint)gid;
   return 0;
 }
+
+int
+sys_getprocs(void)
+{
+  int max;
+  struct uproc* table;
+
+  if(argint(0, &max) < 0){
+    return -1;
+  }
+  if(argptr(1, (void*)&table, sizeof(struct uproc) * max) < 0){
+    return -1;
+  }
+
+  return getprocs((uint) max, table);
+}
 #endif // CS333_P2
```

### proc.c
```diff --git a/proc.c b/proc.c
index d030537..3909bd7 100644
--- a/proc.c
+++ b/proc.c
@@ -6,6 +6,10 @@
 #include "x86.h"
 #include "proc.h"
 #include "spinlock.h"
 #ifdef CS333_P2
 #include "pdx.h"
+#include "uproc.h"
 #endif // CS333_P2

@@ -915,3 +973,38 @@ checkProcs(const char *file, const char *func, int line)
 }
 #endif // DEBUG

+#ifdef CS333_P2
+int
+getprocs(uint max, struct uproc* table)
+{
+  struct proc* p;
+  int counter;
+
+  counter = 0;
+  acquire(&ptable.lock);
+  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
+    if(counter >= max){
+      break;
+    }
+    if(p->state != EMBRYO && p->state != UNUSED){
+      table[counter].pid = p->pid;
+      table[counter].uid = p->uid;
+      table[counter].gid = table->gid;
+      if(p->parent == NULL){
+        table[counter].ppid = p->pid;
+      }else{
+        table[counter].ppid = p->parent->pid;
+      }
+      table[counter].elapsed_ticks = ticks - p->start_ticks;
+      table[counter].CPU_total_ticks = p->cpu_ticks_total;
+      safestrcpy(table[counter].state, states[p->state], sizeof(table[counter].state));
+      table[counter].size = p->sz;
+      safestrcpy(table[counter].name, p->name, sizeof(table[counter].name));
+      counter++;
+    }
+  }
+  release(&ptable.lock);
+
+  return counter;
+}
+#endif // CS333_P2
```

### defs.h
```diff --git a/defs.h b/defs.h
index f85557d..004e831 100644
--- a/defs.h
+++ b/defs.h
@@ -9,6 +9,9 @@ struct spinlock;
 struct sleeplock;
 struct stat;
 struct superblock;
+#ifdef CS333_P2
+struct uproc;
+#endif // CS333_P2

 // bio.c
 void            binit(void);
@@ -124,12 +127,14 @@ void            userinit(void);
 int             wait(void);
 void            wakeup(void*);
 void            yield(void);
+#ifdef CS333_P2
+int             getprocs(uint, struct uproc*);
+#endif // CS333_P2
 #ifdef CS333_P3
 void            printFreeList(void);
 void            printList(int);
 void            printListStats(void);
 #endif // CS333_P3

 // swtch.S
 void            swtch(struct context**, struct context*);
 ```

### ps.c
```diff
+#ifdef CS333_P2
+#include "types.h"
+#include "user.h"
+#include "uproc.h"
+
+int
+main(void)
+{
+    uint max;
+    struct uproc* table;
+    int counter;
+
+    max = 16;
+    table = malloc(sizeof(struct uproc) * max);
+
+    counter = getprocs(max, table);
+    if(counter < 0){
+        printf(2, "error");
+    }else{
+        int elapsed, cpu;
+        printf(1, "\nPID\tName         +UID\tGID\tPPID\tElapsed\tCPU\tState\tSize\n"+);
+        for(int i = 0; i < counter; i++){
+            printf(1, "%d\t%s\t     +%d\t\t%d\t%d\t", table[i].pid, table[i].+name, table[i].uid, table[i].gid, table+[i].ppid);
+            elapsed = table[i].elapsed_ticks;
+            if(elapsed % 1000 < 10){
+                printf(1, "%d.00%d\t", elapsed/+1000, elapsed%1000);
+            }else if(elapsed % 1000 < 100){
+                printf(1, "%d.0%d\t", elapsed/1000, +elapsed%1000);
+            }else{
+              printf(1, "%d.%d\t", elapsed/1000, +elapsed%1000);
+            }
+
+            cpu = table[i].CPU_total_ticks;
+            if(cpu % 1000 < 10){
+            	printf(1, "%d.00%d\t%s\t%d\n", cpu/+1000, cpu%1000, table[i].state, table+[i].size);
+            }else if(cpu % 1000 < 100){
+                printf(1, "%d.0%d\t%s\t%d\n", cpu/+1000, cpu%1000, table[i].state, +table[i].size);
+            }else{
+              printf(1, "%d.%d\t%s\t%d\n", cpu/+1000, cpu%1000, table[i].state, table+[i].size);
+            }
+        }
+    }
+    exit();
+}
+
+#endif // CS333_P2
```

## The time Command
### time.c
```diff
+#ifdef CS333_P2
+#include "types.h"
+#include "user.h"
+
+int
+main(int argc, char* argv[])
+{
+		int start = uptime();
+    int elapsed;
+
+    if(argc < 2){
+        elapsed = uptime() - start;
+        if(elapsed % 1000 < 10){
+          printf(1, "(null) ran in %d.00%d +seconds\n", elapsed/1000, elapsed%1000);
+        }else if(elapsed % 1000 < 100){
+					printf(1, "(null) ran in %d.0%d +seconds\n", elapsed/1000, elapsed%1000);
+				}else {
+					printf(1, "(null) ran in %d.%d +seconds\n", elapsed/1000, elapsed%1000);
+				}
+    }else {
+			if(fork() == 0){
+				exec(argv[1], &argv[1]);
+			}else {
+				wait();
+				elapsed = uptime() - start;
+        if(elapsed % 1000 < 10){
+          printf(1, "%s ran in %d.00%d seconds\n", +argv[1], elapsed/1000, elapsed%1000);
+        }else if(elapsed % 1000 < 100){
+					printf(1, "%s ran in %d.0%d seconds\n", +argv[1], elapsed/1000, elapsed%1000);
+				}else {
+					printf(1, "%s ran in %d.%d seconds\n", +argv[1], elapsed/1000, elapsed%1000);
+				}
+			}
+		}
+
+		exit();
+}
+#endif // CS333_P2
```

## Modifying the Console
```diff
@@ -556,14 +584,44 @@ kill(int pid)
 void
 procdumpP2P3P4(struct proc *p, char *state_string)
 {
-  cprintf("TODO for Project 2, delete this line and implement procdumpP2P3P4() in proc.c to print a row\n");
+  int elapsed = ticks - (p->start_ticks);
+  int cpu = p->cpu_ticks_total;
+  int ppid;
+  if(p->parent == NULL){
+    ppid = p->pid;
+  }else{
+    ppid = p->parent->pid;
+  }
+  cprintf("%d\t%s\t     %d\t\t%d\t%d\t", p->pid, p->name, p->uid, p->gid, ppid);
+  if(elapsed % 1000 < 10){
+    cprintf("%d.00%d\t", elapsed/1000, elapsed%1000);
+  }else if(elapsed % 1000 < 100){
+    cprintf("%d.0%d\t", elapsed/1000, elapsed%1000);
+  }else {
+    cprintf("%d.%d\t", elapsed/1000, elapsed%1000);
+  }
+  if(cpu % 1000 < 10){
+    cprintf("%d.00%d\t%s\t%d\t", cpu/1000, cpu%1000, state_string, p->sz);
+  }else if(cpu % 1000 < 100) {
+    cprintf("%d.0%d\t%s\t%d\t", cpu/1000, cpu%1000, state_string, p->sz);
+  }else {
+    cprintf("%d.%d\t%s\t%d\t", cpu/1000, cpu%1000, state_string, p->sz);
+  }
   return;
 }
```