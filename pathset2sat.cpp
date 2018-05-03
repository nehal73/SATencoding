//
// Created by neha on 26.04.17.
//

#include<iostream>
#include<sstream>
#include<vector>
#include<stdio.h>
#include<stdlib.h>
#include<set>
//#include <bits/valarray_before.h>

using namespace std;
//#define card                                                                /*!< Enables sequential counter */
#define twin
#define max(a,b) (((a)>(b))?(a):(b))
#define min(a,b) (((a)<(b))?(a):(b))
unsigned long long int **edge,nv,ne,nl;
string s,s1;
stringstream ss;
bool** compute_twin();
inline void adde(unsigned long long int ed,unsigned long long int u) {
    /*! \fn inline void adde(unsigned long long int ed,unsigned long long int u)
    \brief Creats the incidence matrix
    \param ed The edge
    \param u The vertex
	*/
    if (!edge[ed][u-1])
        edge[ed][u-1] = ne+1;
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
int main(int argc, char **argv) {
    //clock_t start_t=clock(),end_t;
    long long int tmp;
    unsigned long long int i, j;
    vector<unsigned long long int> var;
    unsigned long long int u, v, w;
//	unsigned long long int vv,vu;
    unsigned long long int nsteps, nclauses = 0;
    unsigned long long int pw, ng = 0;
    char c;
    if (argc < 3) {
        printf("wrong input\n to run type ./pathset2sat width output_file < input_file\n");
        exit(0);
    }//*/
    FILE *file = fopen(argv[2], "w");
    pw = strtoull(argv[1], NULL, 0);
    do {
        tmp = scanf("p edge %llu %llu \n", &nv, &nl);
        if (tmp > 0 && tmp != EOF)
            break;
        tmp = scanf("%*s\n");
    } while (tmp != 2 && tmp != EOF);
	printf("%llu %llu\n",nv,nl);
    ne = 0;
    pw++;
    edge = (unsigned long long int **) malloc(sizeof(unsigned long long int *) * (nl + 1));

    for (u = 0; u <= nl; u++)
        edge[u] = (unsigned long long int *) malloc(sizeof(unsigned long long int) * (nv));
    for (u = 0; u < nl; u++)
        for (v = 0; v < nv; v++)
            edge[u][v] = 0;

    for (i = 1; i <= nl; i++) {
//        ++ne;
        getline(cin, s);
        ss.clear();
        var.clear();
        //cout<<ss<<endl;
        ss << s;
        ss >> c;
        if (c == 'e') {
            while (ss >> u) {
                var.push_back(u);
            }
            for (u = 0; u != var.size(); u++) {
                adde(ne, var[u]);
            }
        } else {
            i--;
            --ne;
        }
        ne++;
    }
    if (nl > 0 && pw > 0) {
        unsigned long long int **set, **del;
        set = (unsigned long long int **) malloc(sizeof(unsigned long long int *) * nv);
        del = (unsigned long long int **) malloc(sizeof(unsigned long long int *) * nv);
        for (u = 0; u < nv; u++) {
            set[u] = (unsigned long long int *) malloc(sizeof(unsigned long long int) * nv);
            del[u] = (unsigned long long int *) malloc(sizeof(unsigned long long int) * nv);
        }
        for (u = 0; u < nv; u++) {
            for (i = 0; i < nv; i++) {
                set[u][i] = ++ng;
                fprintf(file, "c set[%lld][%lld]:%lld\n", u, i, set[u][i]);
            }
        }
        for (u = 0; u < nv; u++) {
            for (i = 0; i < nv; i++) {
                del[u][i] = ++ng;
                fprintf(file, "c del[%lld][%lld]:%lld\n", u, i, del[u][i]);
            }
        }
        unsigned long long int ***ctr;
        ctr = (unsigned long long int ***) malloc(sizeof(unsigned long long int **) * nv);
        for (u = 0; u < nv; u++) {
            ctr[u] = (unsigned long long int **) malloc(sizeof(unsigned long long int *) * nv);
        }
        for (u = 0; u < nv; u++) {
            for (v = 0; v < nv; v++) {
                ctr[u][v] = (unsigned long long int *) malloc(sizeof(unsigned long long int) * nv);
            }
        }
        for (i = 0; i < nv; i++) {
            for (u = 0; u < nv; u++) {
                for (j = 0; j < min(pw, u + 1); j++) {
                    ctr[i][u][j] = ++ng;
                    fprintf(file, "c ctr[%lld][%lld][%lld]:%lld\n", i, u, j, ctr[i][u][j]);
                }
            }
        }
#if defined(twin)
        bool **is_twin;
        is_twin=compute_twin();
#endif
        for (u = 0; u < nv; u++) {
//            fprintf(file, "-%lld 0\n", del[u][0]);
            nclauses++;
        }
        for (u = 0; u < nv; u++) {
//            fprintf(file, "%lld 0\n", del[u][nv - 1]);
            nclauses++;
        }
        for (u = 0; u < nv; u++) {
            for (i = 0; i < nv; i++) {
//                fprintf(file, "-%lld -%lld 0\n", del[u][i], set[u][i]);
                nclauses++;
            }
        }
        for (u = 0; u < nv; u++) {
            for (i = 0; i < nv - 1; i++) {
//                fprintf(file, "-%lld %lld 0\n", del[u][i], del[u][i + 1]);
                nclauses++;
            }
        }

#if defined(twin)
        for(u=0;u<nv;u++){
            for(v=u+1;v<nv;v++){
                if(is_twin[u][v]){
                    for(i=0;i<nv;i++){
//                        fprintf(file,"-%lld %lld 0\n",del[v][i],del[u][i]);
                        nclauses++;
                    }
                }

            }
        }
#endif
        for (u = 0; u < nv; u++) {
            for (i = 0; i < nv - 1; i++) {
//                fprintf(file, "-%lld %lld %lld 0\n", set[u][i], set[u][i + 1], del[u][i + 1]);
                nclauses++;
            }
        }
        for (u = 0; u < nv; u++) {
            for (v = 0; v < nv; v++) {
                if (has_edge(u, v)) {
                    for (i = 0; i < nv - 1; i++) {
//                        fprintf(file, "%lld %lld -%lld %lld 0\n", del[u][i], del[v][i], del[u][i + 1], set[u][i]);
//                        fprintf(file, "%lld %lld -%lld %lld 0\n", del[u][i], del[v][i], del[u][i + 1], set[v][i]);
                        nclauses += 2;
                    }
                }
            }
        }
        for (i = 0; i < nv; i++) {
            for (u = 0; u < nv; u++) {
//                fprintf(file, "-%lld %lld 0\n", set[u][i], ctr[i][u][0]);
                nclauses++;
            }
        }
        for (i = 0; i < nv; i++) {
            for (u = 1; u < nv; u++) {
                for (j = 1; j < min(u + 1, pw); j++) {
//                    fprintf(file, "-%lld -%lld %lld 0\n",set[u][i], ctr[i][u - 1][j - 1], ctr[i][u][j]);
                    nclauses++;
                }
            }
        }
        for (i = 0; i < nv; i++) {
            for (u = 1; u < nv; u++) {
                for (j = 0; j < min(u, pw); j++) {
//                    fprintf(file, "-%lld %lld 0\n", ctr[i][u - 1][j], ctr[i][u][j]);
                    nclauses++;
                }
            }
        }
        for (i = 0; i < nv; i++) {
            for (u = pw; u < nv; u++) {
//                fprintf(file, "-%lld -%lld 0\n",set[u][i], ctr[i][u - 1][pw-1]);
                nclauses++;
            }

        }
        fprintf(file, "p cnf %lld %lld\n", ng, nclauses);
        nclauses = 0;
        for (u = 0; u < nv; u++) {
            fprintf(file, "-%lld 0\n", del[u][0]);
            nclauses++;
        }
        for (u = 0; u < nv; u++) {
            fprintf(file, "%lld 0\n", del[u][nv - 1]);
            nclauses++;
        }
        for (u = 0; u < nv; u++) {
            for (i = 0; i < nv; i++) {
                fprintf(file, "-%lld -%lld 0\n", del[u][i], set[u][i]);
                nclauses++;
            }
        }
        for (u = 0; u < nv; u++) {
            for (i = 0; i < nv - 1; i++) {
                fprintf(file, "-%lld %lld 0\n", del[u][i], del[u][i + 1]);
                nclauses++;
            }
        }
#if defined(twin)
        for(u=0;u<nv;u++){
            for(v=u+1;v<nv;v++){
                if(is_twin[u][v]){
//                    printf("found twins %lld %lld",u,v);
                    for(i=0;i<nv;i++){
                        fprintf(file,"-%lld %lld 0\n",del[v][i],del[u][i]);
                        nclauses++;
                    }
                }

            }
        }
#endif
        for (u = 0; u < nv; u++) {
            for (i = 0; i < nv - 1; i++) {
                fprintf(file, "-%lld %lld %lld 0\n", set[u][i], set[u][i + 1], del[u][i + 1]);
                nclauses++;
            }
        }
        for (u = 0; u < nv; u++) {
            for (v = 0; v < nv; v++) {
                if (has_edge(u, v)) {
                    for (i = 0; i < nv - 1; i++) {
                        fprintf(file, "%lld %lld -%lld %lld 0\n", del[u][i], del[v][i], del[u][i + 1], set[u][i]);
                        fprintf(file, "%lld %lld -%lld %lld 0\n", del[u][i], del[v][i], del[u][i + 1], set[v][i]);
                        nclauses += 2;
                    }
                }
            }
        }
        for (i = 0; i < nv; i++) {
            for (u = 0; u < nv; u++) {
                fprintf(file, "-%lld %lld 0\n", set[u][i], ctr[i][u][0]);
                nclauses++;
            }
        }
        for (i = 0; i < nv; i++) {
            for (u = 1; u < nv; u++) {
                for (j = 1; j < min(u + 1, pw); j++) {
                    fprintf(file, "-%lld -%lld %lld 0\n", set[u][i], ctr[i][u - 1][j - 1], ctr[i][u][j]);
                    nclauses++;
                }
            }
        }
        for (i = 0; i < nv; i++) {
            for (u = 1; u < nv; u++) {
                for (j = 0; j < min(u, pw); j++) {
                    fprintf(file, "-%lld %lld 0\n", ctr[i][u - 1][j], ctr[i][u][j]);
                    nclauses++;
                }
            }
        }
        for (i = 0; i < nv; i++) {
            for (u = pw; u < nv; u++) {
                fprintf(file, "-%lld -%lld 0\n", set[u][i], ctr[i][u - 1][pw - 1]);
                nclauses++;
            }

        }
    }
    return 0;
}


bool** compute_twin(){
    unsigned long long int u,v;
    vector<set<unsigned long long int> > adj;
    bool **is_twin;

    is_twin=(bool **)malloc(sizeof(bool*)*nv+1);
    for(u=0;u<nv;u++)
        is_twin[u]=(bool *)malloc(sizeof(bool)*nv+1);
    for(u=0;u<nv;u++){
        adj.push_back({});
    }
    for(u=0;u<nv;u++){
        for(v=0;v<nv;v++){
            is_twin[u][v]=false;
            if(has_edge(u,v)){
                adj[u].insert(v);
                adj[v].insert(u);
            }
        }
    }
    for (u = 0; u < nv; u++) {
        for (v = u + 1; v < nv; v++) {
            set<unsigned long long int> adju, adjv;
            adju = adj[u];//.erase(v);
            adjv = adj[v];//.erase(u);
            if (has_edge(u, v)) {
                adju.erase(v);
                adjv.erase(u);
            }
            if(adju==adjv) {
////                printf("1 found twins %lld %lld\n",u,v);
//                for(set <unsigned long long int>::iterator it=adju.begin();it!=adju.end();it++){
//                    cout << *it;
//                }
//                cout<<endl;
//                for(set <unsigned long long int>::iterator it=adjv.begin();it!=adjv.end();it++){
//                    cout << *it;
//                }
                is_twin[u][v] = true;
            }
            if (has_edge(u, v)) {
                adj[u].insert(v);
                adj[v].insert(u);
            }
        }
    }
    return is_twin;
}