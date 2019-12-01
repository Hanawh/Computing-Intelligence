//created by WANG HUAN
#include<iostream>
#include<vector>
#include<cmath>
#include<cstdlib>
#include<ctime>
#include<float.h>
#include<numeric>
#include<set>
using namespace std;

class Hopfield{
public:
    double N; //城市个数
    double u0; //初始输入电压
    double step; //更新步长
    double A; //权值
    double D; //权值
    Hopfield(double a, double b, double c):N(a),u0(b),step(c){
        A = a*a;
        D = a/2;
    }

    // 更新神经网络的输出电压V
    vector<vector<double> > V_cal(vector<vector<double> > &u){
        vector<vector<double> > V(N, vector<double>(N));
        for(int i = 0; i < N; ++i){
            for(int j = 0; j < N; ++j){
                V[i][j] = 0.5 * (1 + tanh(u[i][j] / u0));
            }
        }
        
        return V;
    }

    // 更新神经网络的微分方程du
    vector<vector<double> > du_cal(vector<vector<double> > &v, vector<vector<double> > &distance){
        vector<vector<double> > du(N, vector<double>(N));
        vector<double> a(N); //每一行的和
        vector<double> b(N); //每一列的和
        // 计算A的部分
        for(int i = 0; i < N; ++i){
            a[i] = accumulate(v[i].begin(), v[i].end(), 0.0)-1;//第三个参数是累加的初值
            for(int j = 0; j < N; ++j){
                //a[i] += v[i][j];
                b[i] += v[j][i];
            }
            //a[i] -= 1;
            b[i] -= 1;
        }
        
        vector<vector<double> > temp(N, vector<double>(N));
        vector<vector<double> > res(N, vector<double>(N));
        // 将v整体左移
        for(int i = 0; i < N; ++i){
            for(int j = 0; j < N; ++j){
                if(j == N-1){
                    temp[i][j] = v[i][0];
                }
                else{
                    temp[i][j] = v[i][j+1];
                }
            }
        }

        // 计算D的部分
        for(int i = 0; i < N; ++i){
            for(int j = 0; j < N; ++j){
                for(int k = 0; k < N; ++k){
                    res[i][j] += distance[i][k] * temp[k][j];
                }
            }
        }
        // 计算du
        for(int i = 0; i < N; ++i){
            for(int j = 0; j < N; ++j){
                du[i][j] = - A * (a[i] + b[j]) - D * res[i][j];
            }
        }
        return du;
    }

    // 更新神经网络的输入电压U
    vector<vector<double> > U_cal(vector<vector<double> > &u, vector<vector<double> > &du){
        vector<vector<double> > U(N, vector<double>(N));
        for(int i = 0; i < N; ++i){
            for(int j = 0; j < N; ++j){
                U[i][j] = u[i][j] + step * du[i][j];
            }
        }
        return U;
    }

    // 计算神经网络能量
    double energy_cal(vector<vector<double> > &v, vector<vector<double> > &distance){
        vector<double> a(N); //每一行的和
        vector<double> b(N); //每一列的和
        for(int i = 0; i < N; ++i){
            a[i] = pow((accumulate(v[i].begin(), v[i].end(), 0.0)-1),2);
            for(int j = 0; j < N; ++j){
                b[i] += v[j][i];
            }
            b[i] = pow(b[i]-1, 2);
        }

        double t1 = accumulate(a.begin(), a.end(), 0.0);
        double t2 = accumulate(b.begin(), b.end(), 0.0);

        vector<vector<double> > temp(N, vector<double>(N));
        vector<vector<double> > res(N, vector<double>(N));

        for(int i = 0; i < N; ++i){
            for(int j = 0; j < N; ++j){
                if(j == N-1){
                    temp[i][j] = v[i][0];
                }
                else{
                    temp[i][j] = v[i][j+1];
                }
            }
        }

        double t3 = 0;
        for(int i = 0; i < N; ++i){
            for(int j = 0; j < N; ++j){
                t3 += v[i][j] * distance[i][j] * temp[i][j];
            }
        }

        return 0.5*((t1+t2)*A+t3*D);
    }

    //检查路径
    vector<int> check_path(vector<vector<double> > &V){
        vector<int> route;
        double max = -1;
        int tag;
        for(int i = 0; i < N; ++i){ //列
            for(int j = 0; j < N; ++j){ //行
                if(V[j][i] > max){
                    max = V[j][i];
                    tag = j;
                }
            }
            route.push_back(tag);
        }
        return route;
    }

    double dist_cal(vector<int> &route, vector<vector<double> > &distance){
        double res = 0;
        int len = route.size()-1;
        for(int i = 0; i < len; ++i){
            int a = route[i];
            int b = route[i+1];
            if(distance[a][b] == -1) return DBL_MAX;
            res += distance[a][b];
        }
        return res;
    }

    //训练
    vector<int> train(int num_iter, vector<vector<double> > &distance){
        srand(time(0));
        // 初始化神经网络的输入电压Uxi
        vector<vector<double> > U(N, vector<double>(N));
        for(int i = 0; i < N; ++i){
            for(int j = 0; j < N; ++j){
                U[i][j] = 0.5 * u0 * log(N-1) + 2*(rand()%10/(float)(10))-1;
            }
        }
        // 初始化神经网络的输出电压Vxi
        vector<vector<double> > V = V_cal(U);

        // 初始化能量矩阵
        vector<double> energy(num_iter);
        // 初始化最佳路径及长度
        double best_distance = DBL_MAX;
        vector<int> best_way;

        vector<vector<double> > du;
        //进入循环
        for(int i = 0; i < num_iter; ++i){
            du = du_cal(V, distance);
            U = U_cal(U, du);
            V = V_cal(U);
            
            energy[i] = energy_cal(V, distance);
            vector<int> route = check_path(V);
            set<int> st(route.begin(),route.end());
            if(st.size() == N){
                route.push_back(route[0]);
                double dis = dist_cal(route, distance);
                if(dis < best_distance){
                    best_distance = dis;
                    best_way = route;
                    cout << "iter: " << i << " dist: " << best_distance << " energy: " << energy[i] << endl;
                    cout << "route: ";
                    for(int k = 0; k < best_way.size()-1; ++k){
                        cout << best_way[k] <<"->";
                    }
                    cout<<*(best_way.rbegin())<<endl;
                    cout << endl;
                }
            }
        }
        return best_way;
    }
};

int main(){
   int nums = 5;//记录城市的个数
   double a[5][5] = {{0, 10, -1, 4, 12},
                  {10, 0, 15, 8, 5},
                  {-1, 15, 0, 7, 30},
                  {4, 8, 7, 0, 6},
                  {12, 5, 30, 6, 0}};
   vector<vector<double> > graph(nums, vector<double>(nums));
   cout<<"distance graph is"<<endl;
   for(int i = 0; i < nums; ++i){
      for(int j = 0; j < nums; ++j){
         graph[i][j] = a[i][j];
         cout<<a[i][j]<<" ";
      }
      cout<<endl;
   }
   cout<<endl;
   //求解
   Hopfield h(nums, 0.0009, 0.0002); //城市个数，u0， step
   vector<int> result = h.train(500000, graph);
   cout << "the best route is : " ;

   for(int i = 0; i < result.size()-1; ++i){
       cout << result[i] <<"->";
   }
   cout<<*(result.rbegin())<<endl;
   cout << "the shortest distance is : " << h.dist_cal(result, graph);
   return 0;
}
