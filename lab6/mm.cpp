#include <bits/stdc++.h>
#include <unistd.h>

using namespace std;

#define PROCESS_NAME_LEN 32 //进程名最大长度
#define MIN_SLICE 10 //内碎片最大大小
#define DEFAULT_MEM_SIZE 1024  //总内存大小
#define DEFAULT_MEM_START 0  //内存开始分配时的起始地址
#ifdef Wavator
int DEBUG_COUNT = 0;
#define debug(...) cerr << "DEBUG_" << ++DEBUG_COUNT << ": " << #__VA_ARGS__ << " = " << __VA_ARGS__ << endl
#else
#define debug(...) 99
#endif
typedef pair<int, string> My_algo;

int mem_size = DEFAULT_MEM_SIZE;
bool flag = 0; //当内存以及被分配了之后，不允许更改总内存大小的flag
static int pid = 0;
My_algo algo[123] = {
        make_pair(0, "FirstFit"),
        make_pair(1, "BestFit"),
        make_pair(2, "WorstFit")
};

struct free_block{	//空闲数据块
    int size;
    int start_addr;
    struct free_block *next;
};

struct allocated_block{ //已分配的数据块
    int pid;
    int size;
    int start_addr;
    int *data;
    struct allocated_block *next;
};

free_block *free_block_head; //空闲数据块首指针
allocated_block *allocated_block_head = NULL; //分配块首指针

allocated_block *find_process(int id); //寻找pid为id的分配块
free_block *init_free_block(int mem_size); //空闲块初始化
void display_menu(); //显示选项菜单
void set_mem_size(); //设置内存大小
int allocate_mem(allocated_block *ab); //为制定块分配内存
void rearrange(); // 对块进行重新分配
int create_new_process(); //创建新的进程
int free_mem(allocated_block *ab); //释放分配块
void swap(int *p, int *q); //交换地址
int dispose(allocated_block *ab); //释放分配块结构体
void display_mem_usage(); //显示内存情况
void kill_process(); //杀死对应进程并释放其空间与结构体
void Usemy_algo(int id); //使用对应的分配算法
bool first_fit(free_block* &pre, free_block* &now, allocated_block* &ab);
bool best_fit(free_block* &pre, free_block* &now, allocated_block* &ab);
bool worst_fit(free_block* &pre, free_block* &now, allocated_block* &ab);
int curr_alg;
bool (*algo_func[123])(free_block *&, free_block *&, allocated_block *&) = {
        first_fit,
        best_fit,
        worst_fit
};
//主函数
int main() {
    int op;
    pid = 0;
    free_block_head = init_free_block(mem_size); //初始化一个可以使用的内存块，类似与操作系统可用的总存储空间
    for (;;) {
        sleep(1);
        display_menu();
        fflush(stdin);
        scanf("%d", &op);
        switch (op) {
            case 1: {
                set_mem_size();
                break;
            }
            case 2: {
                printf("Choose an algorithm\n");
                printf("1: Best Fit\n 2: Worst Fit\n 3: First Fit\n 4: Buddy System\n");
                int alg;
                scanf("%d", &alg);
                Usemy_algo(alg);
                break;
            }
            case 3: {
                create_new_process();
                break;
            }
            case 4: {
                kill_process();
                break;
            }
            case 5: {
                display_mem_usage();
                break;
            }
            case 233: {
                puts("bye....");
                sleep(1);
                return 0;
            }
            defaut:
                break;
        }
        if (op >= 1 && op <= 5) {
            flag = 1;
        }
    }
}

allocated_block *find_process(int id) { //循环遍历分配块链表，寻找pid=id的进程所对应的块
    allocated_block *tmp = allocated_block_head;
    while (tmp != NULL) {
        if (tmp->pid == id) {
            return tmp;
        }
        tmp = tmp->next;
    }
    return NULL;
}

free_block *init_free_block(int mem_size){ //初始化空闲块，这里的mem_size表示允许的最大虚拟内存大小
    free_block *p;
    p = (free_block *)malloc(sizeof(free_block));
    if (p == NULL){
        puts("No memory left");
        return NULL;
    }
    p->size = mem_size;
    p->start_addr = DEFAULT_MEM_START;
    p->next = NULL;
    return p;
}

void display_menu(){
    puts("\n\n******************menu*******************");
    printf("1) Set memory size (default = %d)\n", DEFAULT_MEM_SIZE);
    printf("2) Set memory allocation algorithm\n");
    printf("3) Create a new process\n");
    printf("4) Kill a process\n");
    printf("5) Display memory usage\n");
    printf("233) Exit\n");
}

void set_mem_size(){ //更改最大内存大小
    if (flag) { //当内存以及被分配了之后，不允许更改总内存大小的flag
        puts("The memory size has been allocated. Set memory is forbidden.");
        return;
    }
    puts("Please input the maximum memory");
    int mem;
    scanf("%d", &mem);
    flag = 1;
    mem_size = mem;
    free_block_head = init_free_block(mem);
    printf("The current memory size has been reset to %d\n", mem);
}

int allocate_mem(allocated_block *ab) { //为块分配内存，真正的操作系统会在这里进行置换等操作
    if (free_block_head) {
        free_block *pre = free_block_head, *now = free_block_head;
        bool has_space = algo_func[curr_alg](pre, now, ab);
        debug(algo[curr_alg].second);
        debug(has_space);
        if (has_space) {
            free_block *nblock = (free_block *) malloc(sizeof(free_block));
            nblock->start_addr = now->start_addr + ab->size;
            nblock->size = now->size - ab->size;
            if (now == free_block_head) {
                free_block_head = nblock;
            } else {
                pre->next = nblock;
            }
            nblock->next = now->next;
            if (allocated_block_head == NULL) {
                allocated_block_head = ab;
            }
            allocated_block *allocate_now = allocated_block_head;
            allocated_block *allocate_pre = allocated_block_head;
            bool ok = false;
            for (; allocate_now != NULL; allocate_now = allocate_now->next) {
                if (allocate_now->start_addr > ab->start_addr) {
                    ab->next = allocate_now;
                    if (allocate_now == allocated_block_head) {
                        allocated_block_head = ab;
                    } else {
                        allocate_pre->next = ab;
                    }
                    ok = true;
                    break;
                }
                if (allocate_now == allocated_block_head) {
                    continue;
                }
                allocate_pre = allocate_pre->next;
            }
            puts("Successfully allocate.");
            if (ok) {
                return 1;
            }
            allocate_pre->next = ab;
            ab->next = NULL;
            return 1;
        } else {
            return -1;
        }
    } else {
        return -1;
    }
}

int create_new_process(){ //创建新进程
    int mem_sz = 0;
    printf("Please input memory size\n");
    scanf("%d", &mem_sz);
    // Write your code here
    if (mem_sz > 0) {
        allocated_block *ab = (allocated_block *) malloc(sizeof(allocated_block));
        ab->size = mem_sz;
        ab->pid = ++pid;
        if (allocate_mem(ab) != -1) {
            printf("Successfully create process%d\n", pid);
        } else {
            puts("No free memory");
        }
        return pid;
    } else {
        puts("Memory size must be greater than 0.");
        return -1;
    }
}

void swap(int *p, int *q){
    int tmp = *p;
    *p = *q;
    *q = tmp;
    return;
}

void rearrange(){ //将块按照地址大小进行排序
    free_block *tmp, *tmpx;
    puts("Rearrange begins...");
    puts("Rearrange by address...");
    tmp = free_block_head;
    while(tmp != NULL){
        tmpx = tmp->next;
        while (tmpx != NULL){
            if (tmpx->start_addr < tmp->start_addr){
                swap(&tmp->start_addr, &tmpx->start_addr);
                swap(&tmp->size, &tmpx->size);
            }
            tmpx = tmpx->next;
        }
        tmp = tmp->next;
    }
    usleep(500);
    puts("Rearrange Done.");
}


int free_mem(allocated_block *ab){ //释放某一块的内存
    free_block *tmp = (free_block *)malloc(sizeof(free_block));
    if (tmp == NULL) {
        return 0 * puts("No memory left");
    } else {
        tmp->start_addr = ab->start_addr;
        tmp->size = ab->size;
        if (free_block_head) {
            tmp->next = free_block_head->next;
        }
        free_block_head->next = tmp;
        rearrange();
        return 1;
    }
}

int dispose(allocated_block *fab){ //释放结构体所占的内存
    allocated_block *pre, *ab;
    if (fab == allocated_block_head){
        allocated_block_head = allocated_block_head->next;
        free(fab);
        return 1;
    }
    pre = allocated_block_head;
    ab = allocated_block_head->next;
    while (ab != fab){ pre = ab; ab = ab->next;}
    pre->next = ab->next;
    free(ab);
    return 2;
}

void display_mem_usage(){
    free_block *fb = free_block_head;
    allocated_block *ab = allocated_block_head;
    puts("*********************Free Memory*********************");
    printf("%20s %20s\n", "start_addr", "size");
    int cnt = 0;
    while (fb != NULL){
        cnt++;
        printf("%20d %20d\n", fb->start_addr, fb->size);
        fb = fb->next;
    }
    if (!cnt) puts("No Free Memory");
    else printf("Totaly %d free blocks\n", cnt);
    puts("");
    puts("*******************Used Memory*********************");
    printf("%10s %10s %20s\n", "PID", "start_addr", "size");
    cnt = 0;
    while (ab != NULL){
        cnt++;
        printf("%10d %10d %20d\n", ab->pid, ab->start_addr, ab->size);
        ab = ab->next;
    }
    if (!cnt) puts("No allocated block");
    else printf("Totaly %d allocated blocks\n", cnt);
    return;
}

void kill_process(){ //杀死某个进程
    allocated_block *ab;
    int pid;
    puts("Please input the pid of Killed process");
    scanf("%d", &pid);
    ab = find_process(pid);
    if (ab != NULL){
        free_mem(ab);
        dispose(ab);
        puts("successfully kill.");
    } else {
        puts("No such pid.");
    }
}


void Usemy_algo(int id) {
    --id;
    if (id >= 0 && id < 3) {
        curr_alg = id;
    } else if (id == 3) {
        //TODO bonus.
    } else {
        puts("The algorithm id must selected from {1, 2, 3, 4}!");
    }
    printf("Current algorithm is: %s\n", algo[id].second.c_str());
}

bool first_fit(free_block* &pre, free_block* &now, allocated_block* &ab) {
    for (int aim = ab->size; now != NULL; now = now->next) {
        if (now->size >= aim) {
            ab->start_addr = now->start_addr;
            return true;
        }
        if (now == free_block_head) {
            continue;
        }
        pre = pre->next;
    }
    return false;
}

bool best_fit(free_block* &pre, free_block* &now, allocated_block* &ab) {
    free_block *max_pre = NULL, *max_now = NULL;
    for (int mx = INT_MAX; now != NULL; now = now->next) {
        if (now->size >= ab->size) {
            if (now->size - ab->size < mx) {
                mx = now->size - ab->size;
                max_pre = pre;
                max_now = now;
            }
        }
        if (now == free_block_head) {
            continue;
        }
        pre = pre->next;
    }
    if (max_now != NULL) {
        pre = max_pre;
        now = max_now;
        ab->start_addr = now->start_addr;
        return true;
    }
    return false;
}

bool worst_fit(free_block* &pre, free_block* &now, allocated_block* &ab) {
    free_block *max_pre = NULL, *max_now = NULL;
    for (int mx = INT_MIN; now != NULL; now = now->next) {
        if (now->size >= ab->size) {
            if (now->size - ab->size > mx) {
                mx = now->size - ab->size;
                max_pre = pre;
                max_now = now;
            }
        }
        if (now == free_block_head) {
            continue;
        }
        pre = pre->next;
    }
    if (max_now != NULL) {
        pre = max_pre;
        now = max_now;
        ab->start_addr = now->start_addr;
        return true;
    }
    return false;
}