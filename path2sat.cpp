//
// Created by neha on 23.03.17.
//

#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include<set>
using namespace std;
int **edge, **ord, **ver;
int nv,ne,nl;
#define twin
bool** compute_twin();

void adde(int vu,int vv)
{
    if(!edge[vu][vv])
        edge[vu][vv]=edge[vv][vu]=++ne;
}
inline bool has_edge(unsigned long long int u, unsigned long long int v) {
    if(u==v){
        return false;
    }
    for (unsigned long long int e = 0; e < ne; e++) {
        if (edge[e][u] != 0 && edge[e][v] != 0) {
            return true;
        }
    }
    return false;
}

int order(int i, int j){
//    return i<j?ord[i][j]:-ord[j][i];
    if(i<=j)
        return ord[i][j];
    else
        return -ord[j][i];
}

inline int min(int x, int y)
{
    return y ^ ((x ^ y) & -(x < y));
}

int main(int argc, char **argv) {
    int i, j, tmp, k;
    int u, v, w, x;
    int vv, vu, tw;
    int nclauses = 0;
    int na = 0;
    if (argc < 3) {
        printf("wrong input\n ./path2sat <width> <output_file> < <input_file>\n");
        exit(0);
    }
//    FILE *ifile = fopen(argv[1], "r");
    FILE *file = fopen(argv[2], "w");
    tw = atoi(argv[1]);
    do {
        tmp = scanf("p edge %i %i \n", &nv, &nl);
        if (tmp > 0 && tmp != EOF)
            break;
        tmp = scanf("%*s\n");
    } while (tmp != 2 && tmp != EOF);
    ne = 0;
    edge = (int **) malloc(sizeof(int *) * (nv + 2));
    for (u = 1; u <= nv; u++)
        edge[u] = (int *) malloc(sizeof(int) * (nv + 1));
    int ***ctr;
    ctr = (int ***) malloc(sizeof(int **) * (nv + 1));
    for (u = 1; u <= nv; u++)
        ctr[u] = (int **) malloc(sizeof(int *) * (nv + 1));
    for (u = 1; u <= nv; u++)
        for (v = 1; v <= nv; v++)
            ctr[u][v] = (int *) malloc(sizeof(int) * (nv + 1));
    for (u = 1; u <= nv; u++)
        for (v = 1; v <= nv; v++)
            edge[u][v] = 0;
    for (i = 1; i <= nl; i++) {
        tmp = scanf(" e %i %i \n", &vu, &vv);
        if (vu < vv)
            adde(vu, vv);
        else
            adde(vv, vu);
    }
    printf("c new\nc no of nodes = %i\nc tree width = %i\n", nv, tw);
//    tw-=1;
    ord = (int **) malloc(sizeof(int *) * (nv + 1));
    ver = (int **) malloc(sizeof(int *) * (nv + 1));
    for (u = 1; u <= nv; u++) {
        ord[u] = (int *) malloc(sizeof(int) * (nv + 1));
        ver[u] = (int *) malloc(sizeof(int) * (nv + 1));
    }
    int ng = 0;
    for (u = 1; u <= nv; u++) {
        for (v = u; v <= nv; v++) {
            ord[u][v] = ++ng;
//            printf("c ord[%d][%d]:%d\n", u, v, ord[u][v]);
        }
    }
    for (u = 1; u <= nv; u++) {
        for (v = 1; v <= nv; v++) {
            ver[u][v] = ++ng;
//            printf("c ord[%d][%d]:%d\n", u, v, ord[u][v]);
        }
    }
    for (u = 1; u <= nv; u++) {
        for (v = 1; v <= nv; v++) {
            for (w = 1; w <= min(v, tw); w++) {
                ctr[u][v][w] = ++ng;
            }
        }
    }

#if defined(twin)
    bool **is_twin;
        is_twin=compute_twin();

#endif
    na = ng;
    for (u = 1; u <= nv; u++) {
        for (v = 1; v <= nv; v++) {
            if (u == v) {
                continue;
            }
            for (w = 1; w <= nv; w++) {
                if (u == w or v == w) {
                    continue;
                }
//                fprintf(file, "%d %d %d 0\n", -1 * order(u, v), -1 * order(v, w), order(u, w));
                nclauses++;
            }
        }
    }
    for(u=1;u<=nv;u++){
        for(v=u+1;v<=nv;v++){
            if(is_twin[u][v]){
//                fprintf(file,"%d 0\n",order(u,v));
                nclauses++;
            }
        }
    }
    for (u = 1; u <= nv; u++) {
        for (v = 1; v <= nv; v++) {
            if (edge[u][v]) {
                for (w = 1; w <= nv; w++) {
                    if (w == v) {
                        continue;
                    }
                    if(w==u){
//                        fprintf(file, "%d %d 0\n", -1 * order(w, v), ver[w][u]);
                        nclauses++;
                    }
                    else {
//                        fprintf(file, "%d %d %d 0\n", -1 * order(u, w), -1 * order(w, v), ver[w][u]);
                        nclauses++;
                    }
                }
            }
        }
    }
    for (u = 1; u <= nv; u++) {
        for (w = 1; w <= nv; w++) {
//            fprintf(file, "-%d %d 0\n", ver[w][u], ctr[w][u][1]);
            nclauses++;
        }
    }
    for (w = 1; w <= nv; w++) {
        for (u = 2; u <= nv; u++) {
            for (j = 2; j <= min(u, tw); j++) {
//                fprintf(file, "-%d -%d %d 0\n", ver[w][u], ctr[w][u - 1][j - 1], ctr[w][u][j]);
                nclauses++;
            }
        }
    }
    for (w = 1; w <= nv; w++) {
        for (u = 2; u <= nv; u++) {
            for (j = 1; j <= min(u - 1, tw); j++) {
//                fprintf(file, "-%d %d 0\n", ctr[w][u - 1][j], ctr[w][u][j]);
                nclauses++;
            }
        }
    }
    for (w = 1; w <= nv; w++) {
        for (u = tw + 1; u <= nv; u++) {
            if (tw >= 1) {
//                fprintf(file, "-%d -%d 0\n", ver[w][u], ctr[w][u - 1][tw]);
                nclauses++;
            }
        }
    }
    fprintf(file,"p cnf %d %d\n",ng,nclauses);
    nclauses=0;
    for (u = 1; u <= nv; u++) {
        for (v = 1; v <= nv; v++) {
            if (u == v) {
                continue;
            }
            for (w = 1; w <= nv; w++) {
                if (u == w or v == w) {
                    continue;
                }
                fprintf(file, "%d %d %d 0\n", -1 * order(u, v), -1 * order(v, w), order(u, w));
                nclauses++;
            }
        }
    }
#if defined(twin)
    for(u=1;u<=nv;u++){
        for(v=u+1;v<=nv;v++){
            if(is_twin[u][v]){
                fprintf(file,"%d 0\n",order(u,v));
                nclauses++;
            }
        }
    }
#endif //twin
    for (u = 1; u <= nv; u++) {
        for (v = 1; v <= nv; v++) {
            if (edge[u][v]) {
                for (w = 1; w <= nv; w++) {
                    if (w == v) {
                        continue;
                    }
                    if(w==u){
                        fprintf(file, "%d %d 0\n", -1 * order(w, v), ver[w][u]);
                    }
                    else {
                        fprintf(file, "%d %d %d 0\n", -1 * order(u, w), -1 * order(w, v), ver[w][u]);
                    }
                }
            }
        }
    }
    for (u = 1; u <= nv; u++) {
        for (w = 1; w <= nv; w++) {
            fprintf(file, "-%d %d 0\n", ver[w][u], ctr[w][u][1]);
            nclauses++;
        }
    }
    for (w = 1; w <= nv; w++) {
        for (u = 2; u <= nv; u++) {
            for (j = 2; j <= min(u, tw); j++) {
                fprintf(file, "-%d -%d %d 0\n", ver[w][u], ctr[w][u - 1][j - 1], ctr[w][u][j]);
                nclauses++;
            }
        }
    }
    for (w = 1; w <= nv; w++) {
        for (u = 2; u <= nv; u++) {
            for (j = 1; j <= min(u - 1, tw); j++) {
                fprintf(file, "-%d %d 0\n", ctr[w][u - 1][j], ctr[w][u][j]);
                nclauses++;
            }
        }
    }
    for (w = 1; w <= nv; w++) {
        for (u = tw + 1; u <= nv; u++) {
            if (tw >= 1) {
                fprintf(file, "-%d -%d 0\n", ver[w][u], ctr[w][u - 1][tw]);
                nclauses++;
            }
        }
    }
    fclose(file);
//    fclose(ifile);
    return 0;
}

bool** compute_twin(){
    unsigned long long int u,v;
    vector<set<unsigned long long int> > adj;
    bool **is_twin;

    is_twin=(bool **)malloc(sizeof(bool*)*nv+2);
    for(u=0;u<=nv;u++)
        is_twin[u]=(bool *)malloc(sizeof(bool)*nv+2);
    for(u=0;u<=nv;u++){
        adj.push_back({});
    }

    for(u=1;u<=nv;u++){
        for(v=1;v<=nv;v++){
            is_twin[u][v]=false;
            if(u!=v){
                if(edge[u][v]!=0){
                    adj[u].insert(v);
                    adj[v].insert(u);
                }
            }
        }
    }

    for (u = 1; u <= nv; u++) {
        for (v = u + 1; v <= nv; v++) {
            set<unsigned long long int> adju, adjv;
            if (edge[u][v]!=0) {
                adj[u].erase(v);
                adj[v].erase(u);
            }
            adju = adj[u];//.erase(v);
            adjv = adj[v];//.erase(u);
            if(adju==adjv) {
                is_twin[u][v] = true;
            }
            if (edge[u][v]!=0) {
                adj[u].insert(v);
                adj[v].insert(u);
            }
        }
    }
    return is_twin;
}