//
// Created by neha on 24.02.17.
//

#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include<set>
using namespace std;
int **edge, **ord, **arc,**par;
int nv,ne,nl;

int *car,**spcar;
#define twin
//#define tree
#define card
#define spcard
//#define card1
void adde(int vu,int vv)
{
    if(!edge[vu][vv])
        edge[vu][vv]=edge[vv][vu]=++ne;
}
bool** compute_twin();


int order(int i, int j){
//    return i<j?ord[i][j]:-ord[j][i];
    if(i<j)
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
        printf("wrong input\n");
        exit(0);
    }
    FILE *file = fopen(argv[2], "w");
    tw = atoi(argv[1]);
    do {
        tmp = scanf("p edge %i %i \n", &nv, &nl);
        if (tmp > 0 && tmp != EOF)
            break;
        tmp = scanf( "%*s\n");
    } while (tmp != 2 && tmp != EOF);
    ne = 0;
    edge = (int **) malloc(sizeof(int *) * (nv + 1));
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
// 	fprintf(file,"c no of nodes = %i\nc tree width = %i\n",nv,tw);
    ord = (int **) malloc(sizeof(int *) * (nv + 1));
    arc = (int **) malloc(sizeof(int *) * (nv + 1));
    par = (int **) malloc(sizeof(int *) * (nv + 1));
    spcar = (int **) malloc(sizeof(int *) * (nv + 1));
    for (u = 1; u <= nv; u++) {
        ord[u] = (int *) malloc(sizeof(int) * (nv + 1));
        arc[u] = (int *) malloc(sizeof(int) * (nv + 1));
        par[u] = (int *) malloc(sizeof(int) * (nv + 1));
        spcar[u] = (int *) malloc(sizeof(int) * (nv + 1));
    }
    int ng = 0;
    for (u = 1; u <= nv; u++) {
        for (v = u; v <= nv; v++) {
            ord[u][v] = ++ng;
//            fprintf(file,"c ord[%d][%d]:%d\n", u, v, ord[u][v]);
        }
    }
    na = ng;
    for (u = 1; u <= nv; u++) {
        for (v = 1; v <= nv; v++) {
            arc[u][v] = ++na;
//            fprintf(file,"c arc[%d][%d]:%d\n", u, v, arc[u][v]);
        }
    }
    for (u = 1; u <= nv; u++)
        for (v = 1; v <= nv; v++)
            par[u][v] = ++na;
    for (u = 1; u <= nv; u++) {
        for (v = 1; v <= nv; v++) {
            spcar[u][v] = ++na;
        }
    }
    for (u = 1; u <= nv; u++) {
        for (v = 1; v < nv; v++) {
            for (w = 1; w <= min(tw, v); w++) {
                ctr[u][v][w] = ++na;
//                fprintf(file,"ctr[%d][%d][%d]:%d \n", u, v, w, ctr[u][v][w]);
            }
        }
    }
#if defined(twin)
    bool **is_twin;
    is_twin=compute_twin();
#endif
    {
        nclauses = 0;
        for (u = 1; u <= nv; u++) {
            for (v = 1; v <= nv; v++) {
                for (w = 1; w <= nv; w++) {
                    if (v == w) {
//                    fprintf(file,"%d 0\n",spcar[v][w]);
                        continue;
                    }
//                    fprintf(file,"-%d -%d %d %d 0\n", arc[u][w], par[u][v], -1 * order(v, w), spcar[v][w]);
                    nclauses++;
                }
            }
        }

        for (i = 1; i <= nv; i++) {
            for (j = 1; j <= nv; j++) {
                if (i == j) {
                    continue;
                }
                for (k = 1; k <= nv; k++) {
                    if (i == k or j == k) {
                        continue;
                    }
//                    fprintf(file,"%d %d %d 0\n", -1 * order(i, j), -1 * order(j, k), order(i, k));
                    nclauses++;
                }
            }
        }
        for (i = 1; i <= nv; i++) {
//            fprintf(file,"-%i 0\n", arc[i][i]);            //no sef loops
            nclauses++;
        }
#if defined(twin)
        {
        for(u=1;u<=nv;u++){
            for(v=u+1;v<=nv;v++){
                if(is_twin[u][v]){
//                    fprintf(file,"%d 0\n",order(u,v));
                    nclauses++;
                }
            }
        }
    }
#endif
        for (u = 1; u <= nv; u++) {
            for (v = u + 1; v <= nv; v++) {
                if (edge[u][v]) {
//                    fprintf(file,"%d %d 0\n", arc[u][v], arc[v][u]);
                    nclauses++;
                }
            }
        }
        for (u = 1; u <= nv; u++) {
            for (v = 1; v <= nv; v++) {
                if (u == v)
                    continue;
//                fprintf(file,"-%d %d 0\n", arc[u][v], order(u, v));
                nclauses++;
            }
        }
        for (u = 1; u <= nv; u++) {
//            fprintf(file,"%d 0\n", par[u][u]);
            nclauses++;
        }
        for (u = 1; u <= nv; u++) {
            for (w = 1; w <= nv; w++) {
                if (u == w)
                    continue;
                for (v = 1; v <= nv; v++) {
                    if (v == w)
                        continue;
//                    fprintf(file,"-%d -%d %d %d 0\n", arc[u][w], par[u][v], -1 * order(w, v), par[w][v]);
//                    fprintf(file," -%d -%d %d %d 0\n", arc[w][u], par[u][v], -1 * order(w, v), par[w][v]);
                    nclauses += 2;
                }
            }
        }
        for (u = 1; u <= nv; u++) {
            for (w = 1; w <= nv; w++) {
                if (u == w)
                    continue;
                for (x = 1; x <= nv; x++) {
                    if (u == x or x == w)
                        continue;
                    for (v = 1; v <= nv; v++) {
                        if (v == u)
                            continue;
//                        fprintf(file,"-%d -%d -%d %d %d 0\n", arc[u][x], arc[w][x], par[w][v], -1 * order(u, v), par[u][v]);
                        nclauses++;
                    }
                }
            }
        }
        for (u = 1; u <= nv; u++) {
            for (w = 1; w <= nv; w++) {
                for (i = 1; i <= nv; i++) {
                    for (j = 1; j <= nv; j++) {
                        if (i == j)
                            continue;
                        for (v = 1; v <= nv; v++) {
//                            fprintf(file,"-%d -%d -%d -%d %d %d %d %d 0\n", par[u][v], par[w][v], arc[u][i], arc[w][j],
//                                   -1 * order(v, i), -1 * order(v, j), arc[i][j], arc[j][i]);
                            nclauses++;
                        }
                    }
                }
            }
        }
        for (v = 1; v <= nv; v++) {
            for (w = 2; w < nv; w++) {
                for (j = 1; j <= min(w - 1, tw); j++) {
//                    fprintf(file,"-%d %d 0\n", ctr[v][w - 1][j], ctr[v][w][j]);
                    if (ctr[v][w - 1][j] == 0) {
//                    fprintf(file,"v:%d W:%d j:%d ", v, w-1, j);
                        exit(0);
                    }
                    nclauses++;
                }
            }
        }
        for (v = 1; v <= nv; v++) {
            for (w = 1; w < nv; w++) {
//                fprintf(file,"-%d %d 0\n", spcar[v][w], ctr[v][w][1]);
                nclauses++;
            }
        }
        for (v = 1; v <= nv; v++) {
            for (w = 2; w < nv; w++) {
                for (j = 2; j <= min(w, tw); j++) {
//                    fprintf(file,"-%d -%d %d 0\n", spcar[v][w], ctr[v][w - 1][j - 1], ctr[v][w][j]);
                    nclauses++;
                }
            }
        }
        for (v = 1; v <= nv; v++) {
            for (w = tw + 1; w <= nv; w++) {
//                fprintf(file,"-%d -%d 0\n", spcar[v][w], ctr[v][w - 1][tw]);
//                if(ctr[v][w-1][tw-1]==0){
//                    fprintf(file,"u:%d w:%d tw:%d",u,w,tw);
//                    break;
//                }
                nclauses++;
            }
        }
    }
//    nclauses=0;
    fprintf(file,"p cnf %d %d\n", na, nclauses);
    nclauses = 0;
    for (u = 1; u <= nv; u++) {
        for (v = 1; v <= nv; v++) {
            for (w = 1; w <= nv; w++) {
                if (v == w) {
//                    fprintf(file,"%d 0\n",spcar[v][w]);
                    continue;
                }
                fprintf(file,"-%d -%d %d %d 0\n", arc[u][w], par[u][v], -1 * order(v, w), spcar[v][w]);
                nclauses++;
            }
        }
    }
    for (i = 1; i <= nv; i++) {
        for (j = 1; j <= nv; j++) {
            if (i == j) {
                continue;
            }
            for (k = 1; k <= nv; k++) {
                if (i == k or j == k) {
                    continue;
                }
                fprintf(file,"%d %d %d 0\n", -1 * order(i, j), -1 * order(j, k), order(i, k));
                nclauses++;
            }
        }
    }
    for (i = 1; i <= nv; i++) {
        fprintf(file,"-%i 0\n", arc[i][i]);            //no sef loops
        nclauses++;
    }
    for (u = 1; u <= nv; u++) {
        for (v = u + 1; v <= nv; v++) {
            if (edge[u][v]) {
                fprintf(file,"%d %d 0\n", arc[u][v], arc[v][u]);
                nclauses++;
            }
        }
    }

    for (u = 1; u <= nv; u++) {
        for (v = 1; v <= nv; v++) {
            if (u == v)
                continue;
            fprintf(file,"-%d %d 0\n", arc[u][v], order(u, v));
            nclauses++;
        }
    }
#if defined(twin)
    {
        for(u=1;u<=nv;u++){
            for(v=u+1;v<=nv;v++){
                if(is_twin[u][v]){
                    fprintf(file,"%d 0\n",order(u,v));
                    nclauses++;
                }
            }
        }
    }
#endif
    for (u = 1; u <= nv; u++) {
        fprintf(file,"%d 0\n", par[u][u]);
    }
    for (u = 1; u <= nv; u++) {
        for (w = 1; w <= nv; w++) {
            if (u == w)
                continue;
            for (v = 1; v <= nv; v++) {
                if (v == w)
                    continue;
                fprintf(file,"-%d -%d %d %d 0\n", arc[u][w], par[u][v], -1 * order(w, v), par[w][v]);
                fprintf(file," -%d -%d %d %d 0\n", arc[w][u], par[u][v], -1 * order(w, v), par[w][v]);
                nclauses += 2;
            }
        }
    }
    for (u = 1; u <= nv; u++) {
        for (w = 1; w <= nv; w++) {
            if (u == w)
                continue;
            for (x = 1; x <= nv; x++) {
                if (u == x or x == w)
                    continue;
                for (v = 1; v <= nv; v++) {
                    if (v == u)
                        continue;
                    fprintf(file,"-%d -%d -%d %d %d 0\n", arc[u][x], arc[w][x], par[w][v], -1 * order(u, v), par[u][v]);
                    nclauses++;
                }
            }
        }
    }
    for (u = 1; u <= nv; u++) {
        for (w = 1; w <= nv; w++) {
            for (i = 1; i <= nv; i++) {
                for (j = 1; j <= nv; j++) {
                    if (i == j)
                        continue;
                    for (v = 1; v <= nv; v++) {
                        fprintf(file,"-%d -%d -%d -%d %d %d %d %d 0\n", par[u][v], par[w][v], arc[u][i], arc[w][j],
                               -1 * order(v, i), -1 * order(v, j), arc[i][j],
                               arc[j][i]);
                        nclauses++;
                    }
                }
            }
        }
    }
    for (v = 1; v <= nv; v++) {
        for (w = 2; w < nv; w++) {
            for (j = 1; j <= min(w - 1, tw); j++) {
                fprintf(file,"-%d %d 0\n", ctr[v][w - 1][j], ctr[v][w][j]);
                if (ctr[v][w - 1][j] == 0) {
//                    fprintf(file,"v:%d W:%d j:%d ", v, w-1, j);
                    exit(0);
                }
                nclauses++;
            }
        }
    }
    for (v = 1; v <= nv; v++) {
        for (w = 1; w < nv; w++) {
            fprintf(file,"-%d %d 0\n", spcar[v][w], ctr[v][w][1]);
            nclauses++;
        }
    }
    for (v = 1; v <= nv; v++) {
        for (w = 2; w < nv; w++) {
            for (j = 2; j <= min(w, tw); j++) {
                fprintf(file,"-%d -%d %d 0\n", spcar[v][w], ctr[v][w - 1][j - 1], ctr[v][w][j]);
                nclauses++;
            }
        }
    }
    for (v = 1; v <= nv; v++) {
        for (w = tw + 1; w <= nv; w++) {
            fprintf(file,"-%d -%d 0\n", spcar[v][w], ctr[v][w - 1][tw]);
//                if(ctr[v][w-1][tw-1]==0){
//                    fprintf(file,"u:%d w:%d tw:%d",u,w,tw);
//                    break;
//                }
            nclauses++;
        }
    }
//    fprintf(stderr,"nclauses %d",nclauses);
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