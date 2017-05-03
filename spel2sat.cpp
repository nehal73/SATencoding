//
// Created by neha on 24.02.17.
//

#include<stdio.h>
#include<stdlib.h>

int **edge, **ord, **arc,**par;
int nv,ne,nl;

int *car,**spcar;

//#define tree
#define card
#define spcard
//#define card1
void adde(int vu,int vv)
{
    if(!edge[vu][vv])
        edge[vu][vv]=edge[vv][vu]=++ne;
}

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
    FILE *file = fopen(argv[1], "r");
    tw = atoi(argv[2]);
    do {
        tmp = fscanf(file, "p edge %i %i \n", &nv, &nl);
        if (tmp > 0 && tmp != EOF)
            break;
        tmp = fscanf(file, "%*s\n");
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
        tmp = fscanf(file, " e %i %i \n", &vu, &vv);
        if (vu < vv)
            adde(vu, vv);
        else
            adde(vv, vu);
    }
// 	printf("c no of nodes = %i\nc tree width = %i\n",nv,tw);
    ord = (int **) malloc(sizeof(int *) * (nv + 1));
    arc = (int **) malloc(sizeof(int *) * (nv + 1));
#ifndef tree
    par = (int **) malloc(sizeof(int *) * (nv + 1));
#ifdef spcard
    spcar = (int **) malloc(sizeof(int *) * (nv + 1));
#endif
#endif
    for (u = 1; u <= nv; u++) {
        ord[u] = (int *) malloc(sizeof(int) * (nv + 1));
        arc[u] = (int *) malloc(sizeof(int) * (nv + 1));
#ifndef tree
        par[u] = (int *) malloc(sizeof(int) * (nv + 1));
#ifdef spcard
        spcar[u] = (int *) malloc(sizeof(int) * (nv + 1));
#endif
#endif
    }
    int ng = 0;
    for (u = 1; u <= nv; u++) {
        for (v = u; v <= nv; v++) {
            ord[u][v] = ++ng;
//            printf("c ord[%d][%d]:%d\n", u, v, ord[u][v]);
        }
    }
    na = ng;
    for (u = 1; u <= nv; u++) {
        for (v = 1; v <= nv; v++) {
            arc[u][v] = ++na;
//            printf("c arc[%d][%d]:%d\n", u, v, arc[u][v]);
        }
    }
#ifndef tree
    for (u = 1; u <= nv; u++)
        for (v = 1; v <= nv; v++)
            par[u][v] = ++na;
#ifdef spcard
    for (u = 1; u <= nv; u++) {
        for (v = 1; v <= nv; v++) {
            spcar[u][v] = ++na;
        }
    }
#endif
#endif
#ifdef card
    for (u = 1; u <= nv; u++) {
        for (v = 1; v < nv; v++) {
            for (w = 1; w <= min(tw, v); w++) {
                ctr[u][v][w] = ++na;
//                printf("ctr[%d][%d][%d]:%d \n", u, v, w, ctr[u][v][w]);
            }
        }
    }
#endif
#ifndef card
    {
        car = (int *) malloc(sizeof(int) * (nv + 1));
        for (u = 1; u <= nv; u++) {
            car[u] = ++na;
        }
    }
#endif
    {
        nclauses = 0;
#ifndef card
        for(i=1;i<=nv;i++){
//        for(j=1;j<=nv;j++){
//            printf("%d ",arc[i][j]);
//        }
//        printf("<= %d\n",tw);
        nclauses++;
    }
#endif

#ifdef spcard
#ifdef card1
        for(u=1;u<=nv;u++){
//        for(v=1;v<=nv;v++){
//            printf("%d ",spcar[u][v]);
//        }
//        printf("<= %d \n",tw);
        nclauses++;
    }
#endif
        for (u = 1; u <= nv; u++) {
            for (v = 1; v <= nv; v++) {
                for (w = 1; w <= nv; w++) {
                    if (v == w) {
//                    printf("%d 0\n",spcar[v][w]);
                        continue;
                    }
//                    printf("-%d -%d %d %d 0\n", arc[u][w], par[u][v], -1 * order(v, w), spcar[v][w]);
                    nclauses++;
                }
            }
        }
#endif
        for (i = 1; i <= nv; i++) {
            for (j = 1; j <= nv; j++) {
                if (i == j) {
                    continue;
                }
                for (k = 1; k <= nv; k++) {
                    if (i == k or j == k) {
                        continue;
                    }
//                    printf("%d %d %d 0\n", -1 * order(i, j), -1 * order(j, k), order(i, k));
                    nclauses++;
                }
            }
        }
        for (i = 1; i <= nv; i++) {
//            printf("-%i 0\n", arc[i][i]);            //no sef loops
            nclauses++;
        }
#ifdef tree
        for (u = 1; u <= nv; u++) {
        for (v = u + 1; v <= nv; v++) {
            if (edge[u][v]) {
//                printf("-%d %d 0\n", ord[u][v], arc[u][v]);
//                printf("%d %d 0\n",ord[u][v],arc[v][u]);
                nclauses+=2;
            }
        }
    }
#endif
#ifndef tree
        for (u = 1; u <= nv; u++) {
            for (v = u + 1; v <= nv; v++) {
                if (edge[u][v]) {
//                    printf("%d %d 0\n", arc[u][v], arc[v][u]);
                    nclauses++;
                }
            }
        }
        for (u = 1; u <= nv; u++) {
            for (v = 1; v <= nv; v++) {
                if (u == v)
                    continue;
//                printf("-%d %d 0\n", arc[u][v], order(u, v));
                nclauses++;
            }
        }
#endif
#ifdef tree
        for (u = 1; u <= nv; u++) {
        for (v = 1; v <= nv; v++) {
            if (u == v)
                continue;
            for (w = v + 1; w <= nv; w++) {
                if (u == w)
                    continue;
//                printf("-%d -%d -%d %d 0\n", arc[u][v], arc[u][w], ord[v][w], arc[v][w]);
//                printf("-%d -%d %d %d 0\n", arc[u][v], arc[u][w], ord[v][w], arc[w][v]);
                nclauses += 2;
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
//                printf("-%d -%d %d %d 0\n", arc[i][j], arc[i][k], arc[j][k], arc[k][j]);
                nclauses++;
            }
        }
    }
#ifdef card
    for (v = 1; v <= nv; v++) {
        for (w = 2; w < nv; w++) {
            for (j = 1; j <= min(w , tw); j++) {
                if(j<w) {
//                    printf("-%d %d 0\n", ctr[v][w - 1][j], ctr[v][w][j]);
                    }
                    nclauses++;
                }
            }
        }
    }
    for (v = 1; v <= nv; v++) {
        for (w = 1; w < nv; w++) {
//            printf("-%d %d 0\n", arc[v][w], ctr[v][w][1]);
            nclauses++;
        }
    }
    for (v = 1; v <= nv; v++) {
        for (w = 2; w < nv; w++) {
            for (j = 2; j <= min(w - 1, tw); j++) {
//                printf("-%d -%d %d 0\n", arc[v][w], ctr[v][w - 1][j - 1], ctr[v][w][j]);
                nclauses++;
            }
        }
    }
    for (v = 1; v <= nv; v++) {
        for (w = tw + 1; w <= nv; w++) {
//            printf("-%d -%d 0\n", arc[v][w], ctr[v][w - 1][tw]);
//                if(ctr[v][w-1][tw-1]==0){
//                    printf("u:%d w:%d tw:%d",u,w,tw);
//                    break;
//                }
            nclauses++;
        }
    }
#endif
#endif
#ifndef tree
        for (u = 1; u <= nv; u++) {
//            printf("%d 0\n", par[u][u]);
        }
        for (u = 1; u <= nv; u++) {
            for (w = 1; w <= nv; w++) {
                if (u == w)
                    continue;
                for (v = 1; v <= nv; v++) {
                    if (v == w)
                        continue;
//                    printf("-%d -%d %d %d 0\n", arc[u][w], par[u][v], -1 * order(w, v), par[w][v]);
//                    printf(" -%d -%d %d %d 0\n", arc[w][u], par[u][v], -1 * order(w, v), par[w][v]);
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
//                        printf("-%d -%d -%d %d %d 0\n", arc[u][x], arc[w][x], par[w][v], -1 * order(u, v), par[u][v]);
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
//                            printf("-%d -%d -%d -%d %d %d %d %d 0\n", par[u][v], par[w][v], arc[u][i], arc[w][j],
//                                   -1 * order(v, i), -1 * order(v, j), arc[i][j],
//                                   arc[j][i]);
                            nclauses++;
                        }
                    }
                }
            }
        }
#ifndef spcard
        for (v = 1; v <= nv; v++) {
        for (w = 2; w < nv; w++) {
            for (j = 1; j <= min(w - 1, tw); j++) {
//                printf("-%d %d 0\n", ctr[v][w - 1][j], ctr[v][w][j]);
                nclauses++;
            }
        }
    }
    for (u = 1; u <= nv; u++) {
        for (v = 1; v <= nv; v++) {
            for (w = 1; w < nv; w++) {
                if (v == w or u == w)
                    continue;
//                printf("-%d -%d %d %d 0\n", arc[u][w], par[u][v], -1 * order(v, w), ctr[v][w][1]);
                nclauses++;
            }
        }
    }
    for (u = 1; u <= nv; u++) {
        for (v = 1; v <= nv; v++) {
            for (w = 2; w < nv; w++) {
                if (v == w or u == w)
                    continue;
                for (j = 2; j <= min(w - 1, tw); j++) {
//                    printf("-%d -%d %d -%d %d 0\n", arc[u][w], par[u][v], -1 * order(v, w), ctr[v][w - 1][j - 1],
//                           ctr[v][w][j]);
                    nclauses++;
                }
            }
        }
    }
    for (u = 1; u <= nv; u++) {
        for (v = 1; v <= nv; v++) {
            for (w = tw + 1; w < nv; w++) {
                if (v == w or u == w)
                    continue;
//                printf("-%d -%d %d -%d 0\n", arc[u][w], par[u][v], -1 * order(v, w), ctr[v][w - 1][tw]);
                nclauses++;
            }
        }
    }
#endif
#ifdef spcard
#ifndef card1
        for (v = 1; v <= nv; v++) {
            for (w = 2; w < nv; w++) {
                for (j = 1; j <= min(w - 1, tw); j++) {
//                    printf("-%d %d 0\n", ctr[v][w - 1][j], ctr[v][w][j]);
                    if (ctr[v][w - 1][j] == 0) {
//                    printf("v:%d W:%d j:%d ", v, w-1, j);
                        exit(0);
                    }
                    nclauses++;
                }
            }
        }
        for (v = 1; v <= nv; v++) {
            for (w = 1; w < nv; w++) {
//                printf("-%d %d 0\n", spcar[v][w], ctr[v][w][1]);
                nclauses++;
            }
        }
        for (v = 1; v <= nv; v++) {
            for (w = 2; w < nv; w++) {
                for (j = 2; j <= min(w, tw); j++) {
//                    printf("-%d -%d %d 0\n", spcar[v][w], ctr[v][w - 1][j - 1], ctr[v][w][j]);
                    nclauses++;
                }
            }
        }
        for (v = 1; v <= nv; v++) {
            for (w = tw + 1; w <= nv; w++) {
//                printf("-%d -%d 0\n", spcar[v][w], ctr[v][w - 1][tw]);
                nclauses++;
            }
        }
#endif
#endif
#endif
    }
    nclauses*=10;
    printf("p cnf %d %d\n", na, nclauses);
    nclauses = 0;
#ifndef card
    for(i=1;i<=nv;i++){
        for(j=1;j<=nv;j++){
            printf("%d ",arc[i][j]);
        }
        printf("<= %d\n",tw);
        nclauses++;
    }
#endif

#ifdef spcard
#ifdef card1
    for(u=1;u<=nv;u++){
        for(v=1;v<=nv;v++){
            printf("%d ",spcar[u][v]);
        }
        printf("<= %d \n",tw);
        nclauses++;
    }
#endif
    for (u = 1; u <= nv; u++) {
        for (v = 1; v <= nv; v++) {
            for (w = 1; w <= nv; w++) {
                if (v == w) {
//                    printf("%d 0\n",spcar[v][w]);
                    continue;
                }
                printf("-%d -%d %d %d 0\n", arc[u][w], par[u][v], -1 * order(v, w), spcar[v][w]);
                nclauses++;
            }
        }
    }
#endif
    for (i = 1; i <= nv; i++) {
        for (j = 1; j <= nv; j++) {
            if (i == j) {
                continue;
            }
            for (k = 1; k <= nv; k++) {
                if (i == k or j == k) {
                    continue;
                }
                printf("%d %d %d 0\n", -1 * order(i, j), -1 * order(j, k), order(i, k));
                nclauses++;
            }
        }
    }
    for (i = 1; i <= nv; i++) {
        printf("-%i 0\n", arc[i][i]);            //no sef loops
        nclauses++;
    }
#ifdef tree
    for (u = 1; u <= nv; u++) {
        for (v = u + 1; v <= nv; v++) {
            if (edge[u][v]) {
                printf("-%d %d 0\n", ord[u][v], arc[u][v]);
                printf("%d %d 0\n",ord[u][v],arc[v][u]);
                nclauses+=2;
            }
        }
    }
#endif
#ifndef tree
    for (u = 1; u <= nv; u++) {
        for (v = u + 1; v <= nv; v++) {
            if (edge[u][v]) {
                printf("%d %d 0\n", arc[u][v], arc[v][u]);
                nclauses++;
            }
        }
    }
    for (u = 1; u <= nv; u++) {
        for (v = 1; v <= nv; v++) {
            if (u == v)
                continue;
            printf("-%d %d 0\n", arc[u][v], order(u, v));
            nclauses++;
        }
    }
#endif
#ifdef tree
    for (u = 1; u <= nv; u++) {
        for (v = 1; v <= nv; v++) {
            if (u == v)
                continue;
            for (w = v + 1; w <= nv; w++) {
                if (u == w)
                    continue;
                printf("-%d -%d -%d %d 0\n", arc[u][v], arc[u][w], ord[v][w], arc[v][w]);
                printf("-%d -%d %d %d 0\n", arc[u][v], arc[u][w], ord[v][w], arc[w][v]);
                nclauses += 2;
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
                printf("-%d -%d %d %d 0\n", arc[i][j], arc[i][k], arc[j][k], arc[k][j]);
                nclauses++;
            }
        }
    }
#ifdef card
    for (v = 1; v <= nv; v++) {
        for (w = 2; w < nv; w++) {
            for (j = 1; j <= min(w , tw); j++) {
                if(j<w) {
                    printf("-%d %d 0\n", ctr[v][w - 1][j], ctr[v][w][j]);
                    if (ctr[v][w - 1][j] == 0) {
                        printf("v:%d W:%d j:%d ", v, w - 1, j);
                        exit(0);
                    }
                    nclauses++;
                }
            }
        }
    }
    for (v = 1; v <= nv; v++) {
        for (w = 1; w < nv; w++) {
            printf("-%d %d 0\n", arc[v][w], ctr[v][w][1]);
            nclauses++;
        }
    }
    for (v = 1; v <= nv; v++) {
        for (w = 2; w < nv; w++) {
            for (j = 2; j <= min(w - 1, tw); j++) {
                printf("-%d -%d %d 0\n", arc[v][w], ctr[v][w - 1][j - 1], ctr[v][w][j]);
                nclauses++;
            }
        }
    }
    for (v = 1; v <= nv; v++) {
        for (w = tw + 1; w <= nv; w++) {
            printf("-%d -%d 0\n", arc[v][w], ctr[v][w - 1][tw]);
//                if(ctr[v][w-1][tw-1]==0){
//                    printf("u:%d w:%d tw:%d",u,w,tw);
//                    break;
//                }
            nclauses++;
        }
    }
#endif
#endif
#ifndef tree
    for (u = 1; u <= nv; u++) {
        printf("%d 0\n", par[u][u]);
    }
    for (u = 1; u <= nv; u++) {
        for (w = 1; w <= nv; w++) {
            if (u == w)
                continue;
            for (v = 1; v <= nv; v++) {
                if (v == w)
                    continue;
                printf("-%d -%d %d %d 0\n", arc[u][w], par[u][v], -1 * order(w, v), par[w][v]);
                printf(" -%d -%d %d %d 0\n", arc[w][u], par[u][v], -1 * order(w, v), par[w][v]);
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
                    printf("-%d -%d -%d %d %d 0\n", arc[u][x], arc[w][x], par[w][v], -1 * order(u, v), par[u][v]);
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
                        printf("-%d -%d -%d -%d %d %d %d %d 0\n", par[u][v], par[w][v], arc[u][i], arc[w][j],
                               -1 * order(v, i), -1 * order(v, j), arc[i][j],
                               arc[j][i]);
                        nclauses++;
                    }
                }
            }
        }
    }
#ifndef spcard
    for (v = 1; v <= nv; v++) {
        for (w = 2; w < nv; w++) {
            for (j = 1; j <= min(w - 1, tw); j++) {
                printf("-%d %d 0\n", ctr[v][w - 1][j], ctr[v][w][j]);
                nclauses++;
            }
        }
    }
    for (u = 1; u <= nv; u++) {
        for (v = 1; v <= nv; v++) {
            for (w = 1; w < nv; w++) {
                if (v == w or u == w)
                    continue;
                printf("-%d -%d %d %d 0\n", arc[u][w], par[u][v], -1 * order(v, w), ctr[v][w][1]);
                nclauses++;
            }
        }
    }
    for (u = 1; u <= nv; u++) {
        for (v = 1; v <= nv; v++) {
            for (w = 2; w < nv; w++) {
                if (v == w or u == w)
                    continue;
                for (j = 2; j <= min(w - 1, tw); j++) {
                    printf("-%d -%d %d -%d %d 0\n", arc[u][w], par[u][v], -1 * order(v, w), ctr[v][w - 1][j - 1],
                           ctr[v][w][j]);
                    nclauses++;
                }
            }
        }
    }
    for (u = 1; u <= nv; u++) {
        for (v = 1; v <= nv; v++) {
            for (w = tw + 1; w < nv; w++) {
                if (v == w or u == w)
                    continue;
                printf("-%d -%d %d -%d 0\n", arc[u][w], par[u][v], -1 * order(v, w), ctr[v][w - 1][tw]);
                nclauses++;
            }
        }
    }
#endif
#ifdef spcard
#ifndef card1
    for (v = 1; v <= nv; v++) {
        for (w = 2; w < nv; w++) {
            for (j = 1; j <= min(w - 1, tw); j++) {
                printf("-%d %d 0\n", ctr[v][w - 1][j], ctr[v][w][j]);
                if (ctr[v][w - 1][j] == 0) {
//                    printf("v:%d W:%d j:%d ", v, w-1, j);
                    exit(0);
                }
                nclauses++;
            }
        }
    }
    for (v = 1; v <= nv; v++) {
        for (w = 1; w < nv; w++) {
            printf("-%d %d 0\n", spcar[v][w], ctr[v][w][1]);
            nclauses++;
        }
    }
    for (v = 1; v <= nv; v++) {
        for (w = 2; w < nv; w++) {
            for (j = 2; j <= min(w, tw); j++) {
                printf("-%d -%d %d 0\n", spcar[v][w], ctr[v][w - 1][j - 1], ctr[v][w][j]);
                nclauses++;
            }
        }
    }
    for (v = 1; v <= nv; v++) {
        for (w = tw + 1; w <= nv; w++) {
            printf("-%d -%d 0\n", spcar[v][w], ctr[v][w - 1][tw]);
//                if(ctr[v][w-1][tw-1]==0){
//                    printf("u:%d w:%d tw:%d",u,w,tw);
//                    break;
//                }
            nclauses++;
        }
    }
#endif
#endif
#endif
//    fprintf(stderr,"nclauses %d",nclauses);
    return 0;
}