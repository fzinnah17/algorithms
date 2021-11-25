#include <stdio.h>
#include <stdlib.h>

#define k 10
#define W 0
#define G 1
#define B 2
#define MAX_VERTICES 1000
#define BIG 1000000000

int color[MAX_VERTICES]; // needed for breadth-first search               
int path[MAX_VERTICES];  // array to store augmenting path

int head, tail;
int q[MAX_VERTICES + 2];

void enqueue(int x) {
	q[tail] = x;
	tail++;
	color[x] = G;
}

int dequeue() {
	int x = q[head];
	head++;
	color[x] = B;
	return x;
}
int bfs(int n,int start, int target, int* cap, int* flow) {
    int u, v;
    for (u = 0; u < n; u++) {
        color[u] = W;
    }
    head = tail = 0;
    enqueue(start);
    path[start] = -1;
    while (head != tail) {
        u = dequeue();
        // Search all adjacent white vertices v. If the capacity
        // from u to v in the residual network is positive,
        // enqueue v.
        for (v = 0; v < n; v++) {
            int c = *(cap + u * n + v);
            int f = *(flow + u * n + v);
            if (color[v] == W && *(cap+u*n+v) - *(flow+u*n+v) > 0) {
                enqueue(v);
                path[v] = u;
            }
        }
    }
    // If the color of the target vertices is black now,
    // it means that we reached it.
    return color[target] == B;
}

int maxflow(int n, int* cap, int* flow, int* residual)
{
    int u, increment;
	int source, sink;
	source = 0;
	sink = n-1;

	if(bfs(n, source, sink, cap, flow))
	{
        // Determine the amount by which we can increment the flow.
        int increment = BIG;
        for (u = n - 1; path[u] >= 0; u = path[u]) {
            increment = min(increment, (*(cap+path[u]*n+u) - *(flow+path[u]*n+u)));
        }
        // Now increment the flow.
        for (u = n - 1; path[u] >= 0; u = path[u]) {
            *(flow + path[u] * n + u) += increment;
            //*(flow + u*n +path[u]) -= increment;
        }
        return increment;
	}
	return 0;
}

main()
{
	int capacities[4*k+2][4*k+2];
    int flow[4*k+2][4*k+2];
    int residual[4*k+2][4*k+2];
    int i,j, improvement, stepcount, totalflow;
    int inflow, outflow;
    for(i=0; i<4*k+2; i++)
    	for(j=0; j<4*k+2; j++)
		{ capacities[i][j]=0; flow[i][j]=0; residual[i][j]=0;
		}
     /* initialize capacities */
    for(i=1; i< k+1; i++)
	{ capacities[0][i] = 20;
		capacities[i][k+i] = 4;
		capacities[i][k+(i%k)+1] = 4;
		capacities[i][2*k+i] = 5;
		capacities[k+i][3*k+i] = 5;
		capacities[2*k+i][3*k+i] = 3;
	    capacities[2*k+i][3*k+(i%k)+1] = 3;
	    capacities[2*k+i][3*k+((i+1)%k)+1] = 3;
	    capacities[3*k+i][4*k+1] = 12;
    }
    printf("created capacity matrix. %d vertices, expected maximum flow %d.\n", 4*k+2, 10*k);
    improvement=1; stepcount = 0;
    while( improvement > 0)
    { improvement = maxflow( 4*k+2, &(capacities[0][0]), &(flow[0][0]), &(residual[0][0]) );
		stepcount ++;
       	/* check flow matrix: capacity constraint and nonnegativity */
        for(i=0; i<4*k+2; i++)
        	for(j=0; j<4*k+2; j++)
			{ if( flow[i][j] < 0 )
				{ printf("step %d: negative flow value %d on edge %d -> %d\n",
					stepcount, flow[i][j], i, j); fflush(stdout); exit(0);
				}
	     		if( flow[i][j] > capacities[i][j] )
	     		{	printf("step %d: flow %d > capacity %d on edge %d -> %d\n",
		       		stepcount, flow[i][j], capacities[i][j], i, j);
	            	fflush(stdout); exit(0);
	     		}
	  		}
       	/* check flow matrix: inflow = outflow */
       	for( i=1; i< 4*k+1; i++ )
       	{ inflow = 0; outflow = 0;
	  		for( j=0; j<4*k+2; j++ )
	  		{  inflow += flow[j][i];
	    		outflow += flow[i][j];
	  		}
	  		if( inflow != outflow )
	    	{  printf("step %d: flow conservation violated in vertex %d\n",
		    	stepcount, i); fflush(stdout); exit(0);
	    	}
       	}
       	printf("step %d completed; improvement %d\n", stepcount, improvement );
    }
    totalflow =0;
    for( i=1; i<k+1; i++)
    	totalflow += flow[0][i];
    printf("finished after %d steps; total flow %d\n", stepcount, totalflow);
}

