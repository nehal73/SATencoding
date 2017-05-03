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
//#define bw_no_print                                                               /*!< Disables printing partition based encoding */
//#define et_no_print                                                               /*!< Disables printing partition based encoding */
#define tw_part                                                                     /*!< Enables partition based encoding */
//#define encode_tree                                                               /*!< Enables tree based encoding */
//#define negate_var                                                                /*!< Enables negated cutvar clauses */
//#define led_ver                                                                     /*!< Enables counting only for leader */
//#define new_ver                                                                   /*!< Enables counting only for newly merged*/
//#define edge_ver                                                                  /*!< Enables counting for every edge*/
//#define propagate_var                                                             /*!< Enables propagating cutvar*/
//#define order                                                                     /*!< Enables counting for every edge*/
#define dtree
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
    unsigned long long int bw, ng = 0;
    char c;
    if (argc < 3) {
        printf("wrong input\n to run type ./sptw2sat width output_file < input_file\n");
        exit(0);
    }//*/
    FILE *file = fopen(argv[2], "w");
    bw = strtoull(argv[1], NULL, 0);
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
    if (nl > 1 && bw > 0) {
#ifdef tw_part
        {
            unsigned long long int **led, ***block;//,**nm;
#if defined(seq_counter) || defined(order)
            unsigned long long int ****ctr;
            ctr = (unsigned long long int ****) malloc(sizeof(unsigned long long int ***) * (nv));
#endif
            block = (unsigned long long int ***) malloc(sizeof(unsigned long long int **) * (nv));
            if (argc == 4) {
                nsteps = strtoull(argv[3], NULL, 0);
            } else {
                nsteps = nv - bw + 2;
            }
            for (u = 0; u < nv; u++) {
                block[u] = (unsigned long long int **) malloc(sizeof(unsigned long long int *) * (nv));
#if defined(seq_counter) || defined(order)
                ctr[u] = (unsigned long long int ***) malloc(sizeof(unsigned long long int **) * (nv));
#endif // defined(seq_counter) || defined(order)
            }
            for (u = 0; u < nv; u++) {
                for (v = 0; v < nv; v++) {
                    block[u][v] = (unsigned long long int *) malloc(sizeof(unsigned long long int) * (nsteps + 1));
                }
#if defined(seq_counter) || defined(order)
                {
                    for (v = 0; v < nv; v++) {
                        ctr[u][v] = (unsigned long long int **) malloc(sizeof(unsigned long long int *) * (bw + 2));
                    }
                }
#endif // seq_counter
            }
#if defined(seq_counter) || defined(order)
            {
                for (i = 0; i <= bw + 1; i++) {
                    for (u = 0; u < nv; u++) {
                        for (v = 0; v < nv; v++) {
                            ctr[u][v][i] = (unsigned long long int *) malloc(
                                    sizeof(unsigned long long int) * (nsteps + 1));
                        }
                    }
                }
            }
#endif // seq_counter
            //initialising variables
            for (i = 0; i <= nsteps; i++) {
                for (u = 0; u < nv; u++) {
                    for (v = u; v < nv; v++) {
                        block[u][v][i] = ++ng;
                        //if(u==v)
//                        printf("c block[%llu][%llu][%llu]=%llu\n", u, v, i, block[u][v][i]);
                    }
                }
            }
#if defined(seq_counter) || defined(order)
            {
                for (i = 0; i <= nsteps; i++) {
                    for (u = 0; u < nv; u++) {
                        for (v = 0; v < nv; v++) {
                            for (j = 1; j <= bw + 1; j++) {
                                ctr[u][v][j][i] = ++ng;
                                //printf("c ctr[%llu][%llu][%llu][%llu]=%llu\n",u,v,j,i,ctr[u][v][j][i]);
                            }
                        }
                    }
                }
            }
#endif // seq_counter
            nclauses = 0;
            {
#ifdef card
                {
                    for(i=0;i<=nsteps;i++) {
                        for (u =0; u < nv-bw; u++) {
//                            fprintf(file,"%llu ",block[u][u][i]);
//                            for (v = u + 1; v < nv; v++) {
//                                fprintf(file,"%llu ",block[u][v][i]);
//                            }
//                            fprintf(file,"<=%llu\n",bw+1);
                            nclauses++;
                        }
                    }
                }
#endif
#ifdef extra
                {
                    for (u =0; u < nv; u++) {
//                        fprintf(file,"-%llu 0\n",block[u][u][nsteps]);
                        nclauses++;
                        for (v = u + 1; v < nv; v++) {
//                            fprintf(file, "-%llu 0\n", block[u][v][0]);
//                            fprintf(file, "-%llu 0\n", block[u][v][nsteps]);
                            nclauses += 2;
                        }
                    }
                }
#endif
                for (i = 0; i <= nsteps; i++) {
                    for (u = 0; u < nv; u++) {
                        for (v = u + 1; v < nv; v++) {
                            for (w = v + 1; w < nv; w++) {
//                                fprintf(file, "-%llu -%llu %llu 0\n", block[u][v][i], block[u][w][i], block[v][w][i]);
//                                fprintf(file, "-%llu -%llu %llu 0\n", block[u][v][i], block[v][w][i], block[u][w][i]);
//                                fprintf(file, "-%llu -%llu %llu 0\n", block[u][w][i], block[v][w][i], block[u][v][i]);
                                nclauses += 3;
                            }
                        }
                    }
                }
                for (i = 0; i <= nsteps; i++) {
                    for (u = 0; u < nv; u++) {
                        for (v = u + 1; v < nv; v++) {
//                            fprintf(file, "-%llu %llu 0\n", block[u][v][i], block[u][u][i]);
//                            fprintf(file, "-%llu %llu 0\n", block[u][v][i], block[v][v][i]);
//                        fprintf(file,"%llu -%llu 0\n",block[u][u][i],block[u][v][i]);
//                        fprintf(file,"%llu -%llu 0\n",block[v][v][i],block[u][v][i]);
                            nclauses += 2;
                        }
                    }
                }
                for (u = 0; u < nv; u++) {
//                    fprintf(file, "%llu 0\n", block[u][u][0]);
//                fprintf(file, "-%llu 0\n", block[u][u][nsteps]);
                    nclauses++;
//                nclauses++;
                }
                for (i = 0; i < nsteps; i++) {
                    for (u = 0; u < nv; u++) {
//                        fprintf(file, "%llu -%llu 0\n", block[u][u][i], block[u][u][i + 1]);
                        nclauses++;
                        for (v = u + 1; v < nv; v++) {
//                        fprintf(file, "-%llu %llu 0\n", block[u][v][i], block[u][v][i + 1]);
//                            fprintf(file, "-%llu -%llu -%llu %llu 0\n", block[u][v][i], block[u][u][i + 1],
//                                    block[v][v][i + 1], block[u][v][i + 1]);
                            nclauses++;
                        }
                    }
                }
                for (i = 0; i < nsteps; i++) {
                    for (u = 0; u < nv; u++) {
                        for (v = u + 1; v < nv; v++) {
                            if (has_edge(u, v)) {
//                                fprintf(file, "-%llu -%llu %llu %llu 0\n", block[u][u][i], block[v][v][i],
//                                        block[u][u][i + 1], block[u][v][i]);
//                                fprintf(file, "-%llu -%llu %llu %llu 0\n", block[u][u][i], block[v][v][i],
//                                        block[v][v][i + 1], block[u][v][i]);
                                nclauses += 2;
                            }
                        }
                    }
                }
#ifndef extra
                {
                    for (u = 0; u < nv; u++) {
                        for (v = u + 1; v < nv; v++) {
                            if (has_edge(u, v)) {
//                                fprintf(file, "-%llu -%llu %llu 0\n", block[u][u][nsteps], block[v][v][nsteps],
//                                        block[u][v][nsteps]);
                                nclauses++;
                            }
                        }
                    }
                }
#endif
#ifdef seq_counter
                {
                    for (i = 0; i <= nsteps; i++) {
                        for (u = 0; u < nv; u++) {
//                        for (v = u; v < nv; v++) {
//                            fprintf(file, "-%llu %llu 0\n", block[u][u][i], ctr[u][u][1][i]);
                            nclauses++;
//                        }
                        }
                    }
                    for (i = 0; i <= nsteps; i++) {
                        for (u = 0; u < nv; u++) {
                            for (v = u + 1; v < nv; v++) {
                                for (j = 1; j <= bw + 1; j++) {
//                                    fprintf(file, "-%llu %llu 0\n", ctr[u][v - 1][j][i], ctr[u][v][j][i]);
                                    nclauses++;
                                }
                            }
                        }
                    }
                    for (i = 0; i <= nsteps; i++) {
                        for (u = 0; u < nv; u++) {
                            for (v = u + 1; v < nv; v++) {
                                for (j = 2; j <= bw + 1; j++) {
//                                    fprintf(file, "-%llu -%llu %llu 0\n", block[u][v][i], ctr[u][v - 1][j - 1][i],
//                                            ctr[u][v][j][i]);
                                    nclauses++;
                                }
                            }
                        }
                    }
                    for (i = 0; i <= nsteps; i++) {
                        for (u = 0; u < nv; u++) {
                            for (v = u + 1; v < nv; v++) {
//                                fprintf(file, "-%llu -%llu 0\n", block[u][v][i], ctr[u][v - 1][bw + 1][i]);
                                nclauses++;
                            }
                        }
                    }
                }
#endif
            }
            fprintf(file, "p cnf %llu %llu\n", ng, nclauses);
            printf("%llu ", nclauses);
            nclauses = 0;
            {
#ifdef card
                {
                    for(i=0;i<=nsteps;i++) {
                        for (u =0; u < nv-bw; u++) {
                            fprintf(file,"%llu ",block[u][u][i]);
                            for (v = u + 1; v < nv; v++) {
                                fprintf(file,"%llu ",block[u][v][i]);
                            }
                            fprintf(file,"<=%llu\n",bw+1);
                            nclauses++;
                        }
                    }
                }
#endif
#ifdef extra
                {
                    for (u =0; u < nv; u++) {
                        fprintf(file,"-%llu 0\n",block[u][u][nsteps]);
                        nclauses++;
                        for (v = u + 1; v < nv; v++) {
                            fprintf(file, "-%llu 0\n", block[u][v][0]);
                            fprintf(file, "-%llu 0\n", block[u][v][nsteps]);
                            nclauses += 2;
                        }
                    }
                }
#endif
                for (i = 0; i <= nsteps; i++) {
                    for (u = 0; u < nv; u++) {
                        for (v = u + 1; v < nv; v++) {
                            for (w = v + 1; w < nv; w++) {
                                fprintf(file, "-%llu -%llu %llu 0\n", block[u][v][i], block[u][w][i], block[v][w][i]);
                                fprintf(file, "-%llu -%llu %llu 0\n", block[u][v][i], block[v][w][i], block[u][w][i]);
                                fprintf(file, "-%llu -%llu %llu 0\n", block[u][w][i], block[v][w][i], block[u][v][i]);
                                nclauses += 3;
                            }
                        }
                    }
                }
                for (i = 0; i <= nsteps; i++) {
                    for (u = 0; u < nv; u++) {
                        for (v = u + 1; v < nv; v++) {
                            fprintf(file, "-%llu %llu 0\n", block[u][v][i], block[u][u][i]);
                            fprintf(file, "-%llu %llu 0\n", block[u][v][i], block[v][v][i]);
//                        fprintf(file,"%llu -%llu 0\n",block[u][u][i],block[u][v][i]);
//                        fprintf(file,"%llu -%llu 0\n",block[v][v][i],block[u][v][i]);
                            nclauses += 2;
                        }
                    }
                }
                for (u = 0; u < nv; u++) {
                    fprintf(file, "%llu 0\n", block[u][u][0]);
//                fprintf(file, "-%llu 0\n", block[u][u][nsteps]);
                    nclauses++;
//                nclauses++;
                }
                for (i = 0; i < nsteps; i++) {
                    for (u = 0; u < nv; u++) {
                        fprintf(file, "%llu -%llu 0\n", block[u][u][i], block[u][u][i + 1]);
                        nclauses++;
                        for (v = u + 1; v < nv; v++) {
//                        fprintf(file, "-%llu %llu 0\n", block[u][v][i], block[u][v][i + 1]);
                            fprintf(file, "-%llu -%llu -%llu %llu 0\n", block[u][v][i], block[u][u][i + 1],
                                    block[v][v][i + 1], block[u][v][i + 1]);
                            nclauses++;
                        }
                    }
                }
                for (i = 0; i < nsteps; i++) {
                    for (u = 0; u < nv; u++) {
                        for (v = u + 1; v < nv; v++) {
                            if (has_edge(u, v)) {
                                fprintf(file, "-%llu -%llu %llu %llu 0\n", block[u][u][i], block[v][v][i],
                                        block[u][u][i + 1], block[u][v][i]);
                                fprintf(file, "-%llu -%llu %llu %llu 0\n", block[u][u][i], block[v][v][i],
                                        block[v][v][i + 1], block[u][v][i]);
                                nclauses += 2;
                            }
                        }
                    }
                }
#ifndef extra
                {
                    for (u = 0; u < nv; u++) {
                        for (v = u + 1; v < nv; v++) {
                            if (has_edge(u, v)) {
                                fprintf(file, "-%llu -%llu %llu 0\n", block[u][u][nsteps], block[v][v][nsteps],
                                        block[u][v][nsteps]);
                                nclauses++;
                            }
                        }
                    }
                }
#endif
#ifdef seq_counter
                {
                    for (i = 0; i <= nsteps; i++) {
                        for (u = 0; u < nv; u++) {
//                        for (v = u; v < nv; v++) {
                            fprintf(file, "-%llu %llu 0\n", block[u][u][i], ctr[u][u][1][i]);
                            nclauses++;
//                        }
                        }
                    }
                    for (i = 0; i <= nsteps; i++) {
                        for (u = 0; u < nv; u++) {
                            for (v = u + 1; v < nv; v++) {
                                for (j = 1; j <= bw + 1; j++) {
                                    fprintf(file, "-%llu %llu 0\n", ctr[u][v - 1][j][i], ctr[u][v][j][i]);
                                    nclauses++;
                                }
                            }
                        }
                    }
                    for (i = 0; i <= nsteps; i++) {
                        for (u = 0; u < nv; u++) {
                            for (v = u + 1; v < nv; v++) {
                                for (j = 2; j <= bw + 1; j++) {
                                    fprintf(file, "-%llu -%llu %llu 0\n", block[u][v][i], ctr[u][v - 1][j - 1][i],
                                            ctr[u][v][j][i]);
                                    nclauses++;
                                }
                            }
                        }
                    }
                    for (i = 0; i <= nsteps; i++) {
                        for (u = 0; u < nv; u++) {
                            for (v = u + 1; v < nv; v++) {
                                fprintf(file, "-%llu -%llu 0\n", block[u][v][i], ctr[u][v - 1][bw + 1][i]);
                                nclauses++;
                            }
                        }
                    }
                }
#endif
            }
        }
    }
#endif // tw_part
    cout << nclauses << " " << bw << " " << nsteps << endl;
    fclose(file);
    return 0;
}
