//1 interprocess communication

#include <unistd.h>
#include <stdio.h>

int main(void)
{
    int n;
    int fd[2];            
    pid_t pid;              
    char line[20];          
    if (pipe(fd) < 0)      
        printf("pipe error\n");
    else
        printf("Pipe created\n");

    if ((pid = fork()) < 0) 
        printf("Fork error\n");
    else
        printf("%d\n", pid);

    while (1)
    {
        if (pid > 0)
        {
         
            close(fd[0]); 
            write(fd[1], "hello world\n", 12); 
        }
        else
        {
            
            close(fd[1]);   
            n = read(fd[0], line, 20);   
            write(STDOUT_FILENO, line, n); 
        }
        break; 
    }

    return 0;
}
























//2 leaky bucket

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>

void main()
{
    int packets[8], i, j, clk, b_size, o_rate, i_rate, p_sz, p_sz_rm = 0, p_time;

    for(i = 0; i < 5; ++i)
    {
        packets[i] = rand() % 10;
        if (packets[i] == 0)
            --i;
    }

    printf("Enter output rate:");
    scanf("%d", &o_rate);

    printf("\nEnter bucket size:");
    scanf("%d", &b_size);

    for(i = 0; i < 5; ++i)
    {
        if ((packets[i] + p_sz_rm) > b_size)
        {
            if (packets[i] > b_size)
                printf("\nIncoming packet size:%d greater than bucket capacity\n", packets[i]);
            else
                printf("Bucket size exceeded\n");
        }
        else
        {
            p_sz = packets[i];
            p_sz_rm += p_sz;

            printf("\n--------------------------------------------------\n");
            printf("Incoming packet:%d", p_sz);
            printf("\nTransmission left:%d\n", p_sz_rm);

            p_time = rand() % 10;
            printf("Next packet will come at %d", p_time);

            for(clk = 0; clk < p_time && p_sz_rm > 0; ++clk)
            {
                printf("\nTime left %d---No packets to transmit!!\n", p_time - clk);
                sleep(1);

                if (p_sz_rm)
                {
                    printf("Transmitted\n");
                    if (p_sz_rm < o_rate)
                        p_sz_rm = 0;
                    else
                        p_sz_rm -= o_rate;

                    printf("Bytes remaining:%d\n", p_sz_rm);
                }
                else
                {
                    printf("No packets to transmit\n");
                }
            }
        }
    }
}





























//3 minimum spanning tree

#include <stdio.h>

struct node {
    int set;
} node[100];

struct edge {
    int first_node, second_node;
    int distance;
    int selected;
} e[100];

int edge_count = 0;

void getdata(int index, int total) {
    int i;
    for (i = index; i < total; ++i) {
        if (i != index) {
            printf("Enter distance between vertex %c and %c: ", index + 65, i + 65);
            scanf("%d", &e[edge_count].distance);
            e[edge_count].first_node = index;
            e[edge_count].second_node = i;
            ++edge_count;
        }
    }
}

void initialise(int total_nodes) {
    int i;
    for (i = 0; i < total_nodes; ++i)
        node[i].set = i;

    for (i = 0; i < edge_count; ++i)
        e[i].selected = -1;
}

void sort_edges() {
    int i, j;
    struct edge temp;
    for (i = 0; i < edge_count - 1; ++i) {
        for (j = 0; j < edge_count - i - 1; ++j) {
            if (e[j].distance > e[j + 1].distance) {
                temp = e[j];
                e[j] = e[j + 1];
                e[j + 1] = temp;
            }
        }
    }
}

int main() {
    int total_vertices, i, j, k, m, n, edges_selected = 0, nodel, noder;

    printf("Enter the number of vertices: ");
    scanf("%d", &total_vertices);

    for (i = 0; i < total_vertices; ++i)
        getdata(i, total_vertices);

    initialise(total_vertices);
    sort_edges();

    printf("\nSorted order of edges:\n");
    for (i = 0; i < edge_count; ++i)
        printf("Edge %d: %c - %c  Distance: %d\n", i,
               e[i].first_node + 65, e[i].second_node + 65, e[i].distance);

    i = 0;
    do {
        e[i].selected = 1;
        nodel = e[i].first_node;
        noder = e[i].second_node;

        if (node[nodel].set == node[noder].set) {
            e[i].selected = -1;
        } else {
            edges_selected++;
            m = node[nodel].set;
            k = node[noder].set;
            for (n = 0; n < total_vertices; ++n) {
                if (node[n].set == k)
                    node[n].set = m;
            }
        }
        ++i;
    } while (edges_selected < (total_vertices - 1));

    printf("\n\nMinimal Spanning Tree:\n");
    for (i = 0; i < edge_count; ++i) {
        if (e[i].selected == 1)
            printf("%c <--> %c  Distance: %d\n", e[i].first_node + 65, e[i].second_node + 65, e[i].distance);
    }

    return 0;
}
