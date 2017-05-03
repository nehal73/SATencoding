//
// Created by neha on 26.04.17.
//

#include<iostream>
#include<string>
#include<sstream>
#include<stdio.h>
#include<stdlib.h>
#include<vector>
//#include<climits>
//#include<ctime>
//#include <sys/time.h>
//#include <sys/resource.h>
//#include <unistd.h>
using namespace std;
//#define card
#define seq_counter                                                                 /*!< Enables sequential counter */
#define max(a,b) (((a)>(b))?(a):(b))
#define min(a,b) (((a)<(b))?(a):(b))
//#define extra
unsigned long long int **edge,nv,ne,nl;
string s,s1;
stringstream ss;

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
    for (unsigned long long int e = 0; e < ne; e++) {
        if (edge[e][u] != 0 && edge[e][v] != 0) {
            return true;
        }
    }
    return false;
}
/*
inline unsigned long long int min(unsigned long long int x, unsigned long long int y)
{
	return (x < y) ? x : y;
}
inline unsigned long long int max(unsigned long long int x, unsigned long long int y)
{
	return (x > y) ? x : y;
}
*/
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
        printf("wrong input\n to run type ./hybw2sat pw output_file depth(optional)<input_file\n");
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
//	printf("%llu %llu\n",nv,nl);
    ne = 0;
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
#ifdef seq_counter
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
                for (j = 0; j < min(pw , u +1); j++) {
                    ctr[i][u][j] = ++ng;
                    fprintf(file, "c ctr[%lld][%lld][%lld]:%lld\n", i, u, j, ctr[i][u][j]);
                }
            }
        }
#endif
        fprintf(file, "p cnf %lld 0\n", ng);
#ifndef seq_counter
        for(i=0;i<nv;i++){
            for(u=0;u<nv;u++){
                fprintf(file,"%lld ",set[u][i]);
            }
            fprintf(file," <= %lld\n",pw);
            nclauses++;
        }
#endif
        for (u = 0; u < nv; u++) {
            fprintf(file, "-%lld 0\n", del[u][0]);
            nclauses++;
        }
        for (u = 0; u < nv; u++) {
            fprintf(file, "%lld 0\n", del[u][nv - 1]);
            nclauses++;
        }
//        for(u=0;u<nv;u++){
//            for(i=0;i<nv;i++){
//                fprintf(file,"%lld ",set[u][i]);
//            }
//            fprintf(file,"0\n");
//        }
        for (u = 0; u < nv; u++) {
            for (i = 0; i < nv; i++) {
                fprintf(file, "-%lld -%lld 0\n", del[u][i], set[u][i]);
                nclauses++;
            }
        }
//        for (i = 1; i < nv; i++) {
//            for (u = 0; u < nv; u++) {
//                fprintf(file, "%lld -%lld %lld 0\n", del[u][i], del[u][i - 1], set[u][i - 1]);
//                fprintf(file, "%lld -%lld -%lld 0\n", del[u][i], del[u][i - 1], set[u][i]);
//                nclauses++;
//            }
//        }
        for (u = 0; u < nv; u++) {
            for (i = 0; i < nv - 1; i++) {
                fprintf(file, "-%lld %lld 0\n", del[u][i], del[u][i + 1]);
                nclauses++;
            }
        }
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
//        for (u = 0; u < nv; u++) {
//            for (v = 0; v < nv; v++) {
//                if (has_edge(u, v)) {
//                    for (i = 0; i < nv - 1; i++) {
//                        fprintf(file, "-%lld %lld %lld %lld 0\n", set[u][i], set[u][i + 1], del[v][i], set[v][i]);
//                        nclauses++;
//                    }
//                }
//            }
//        }
#ifdef seq_counter
        for (i = 0; i < nv; i++) {
            for (u = 0; u < nv; u++) {
                fprintf(file, "-%lld %lld 0\n", set[u][i], ctr[i][u][0]);
                nclauses++;
            }
        }
        for (i = 0; i < nv; i++) {
            for (u = 1; u < nv; u++) {
                for (j = 1; j < min(u+1, pw); j++) {
                    fprintf(file, "-%lld -%lld %lld 0\n",set[u][i], ctr[i][u - 1][j - 1], ctr[i][u][j]);
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
                fprintf(file, "-%lld -%lld 0\n",set[u][i], ctr[i][u - 1][pw-1]);
                nclauses++;
            }

        }
#endif
    }
    return 0;
}