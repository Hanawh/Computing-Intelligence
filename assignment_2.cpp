//created by WangHuan
#include<iostream>
#include<vector>
#include<cstdlib> //srand rand
#include<algorithm> 
#include<ctime> //time
#include<cmath> //exp
using namespace std;

//初始化冷却表(实际上是参数列表)
struct coolingSchedule{
   double Initial_Temperature;
   double Decay_Weight;
   double End_Temperature;
   int Mapkob_Length;
   double threshold; //大于该值则保留新结点
   coolingSchedule(double a, double b, double c, int d, double e) : Initial_Temperature(a),\
                  Decay_Weight(b), End_Temperature(c), Mapkob_Length(d), threshold(e){}
};
//在此处调参
coolingSchedule weights(100, 0.9, 0.00001, 20000, 0.6);
int inf = 100000;

class Solution
{
public:
   vector<char> TSP_SA(vector<vector<int> >& graph){
      srand(time(0)); //以系统时间作为随机种子
      vector<char>  best;
      //初始化路径
      int nums = graph.size(); //城市的个数
      for(int i=0; i<nums; ++i){
         best.push_back('A'+i);
      }
      //repeat
      double curT = weights.Initial_Temperature;
      while (curT >= weights.End_Temperature){
         for (int i = 0; i < weights.Mapkob_Length; ++i){
            vector<char> newPath = GenerateNewPath(best);
            double prob = Metropolis(newPath, best, graph, curT);
            if(prob > weights.threshold) best = newPath;
         }
         curT *= weights.Decay_Weight;
      }
      return best;
   }

   //产生新的路径 2变换法
   vector<char> GenerateNewPath(vector<char> cur){
      int nums = cur.size();
      //随机生成要交换的坐标i,j 1~nums-1
      int i = rand()%(nums-1-1)+1;
      int j = rand()%(nums-1-1)+1;
      //交换序列i,j的位置
      char temp = cur[i];
      cur[i] = cur[j];
      cur[j] = temp;
      return cur;
   }

   //根据Metropolis准则返回保留新解的概率
   double Metropolis(vector<char> newPath, vector<char> best, vector<vector<int> > graph, double t){
      double new_res = f(newPath, graph);
      double best_res = f(best, graph);
      if(new_res < best_res|| new_res == best_res) return 1.0;
      return exp((new_res - best_res)/t);
   }
   
   //计算路径长度
   double f(vector<char> temp, vector<vector<int> > graph){
      double res = 0;
      for(int i = 0; i < temp.size()-1; ++i){
         if(graph[temp[i]-'A'][temp[i+1]-'A'] == -1) return inf;
         res += graph[temp[i]-'A'][temp[i+1]-'A'];
      }
      //再判断能否从终点返回原点
      if(graph[temp[temp.size()-1]-'A'][temp[0]-'A'] == -1) return inf;
      res += graph[temp[temp.size()-1]-'A'][temp[0]-'A'];
      return res;
   }

};

int main()
{
   //创建邻接矩阵 -1代表两地不连通
   int nums = 5;//记录城市的个数
   int a[5][5] = {{0, 10, -1, 4, 12},
                  {10, 0, 15, 8, 5},
                  {-1, 15, 0, 7, 30},
                  {4, 8, 7, 0, 6},
                  {12, 5, 30, 6, 0}};
   vector<vector<int> > graph(nums);
   for(int i = 0; i < nums; ++i){
      graph[i].resize(nums);
   }
   for(int i = 0; i < nums; ++i){
      for(int j = 0; j < nums; ++j){
         graph[i][j] = a[i][j];
      }
   }
   //求解
   vector<char> res = Solution().TSP_SA(graph);
   for(auto &i:res){
      cout << i << "——>";
   }
   cout << *(res.begin());
   cout << "the shortest distance is " << Solution().f(res, graph);
   return 0;
}
