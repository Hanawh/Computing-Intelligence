//created by WangHuan
#include<iostream>
#include<vector>
#include<cstdlib> //srand rand
#include<algorithm> 
#include<ctime> //time
#include<cmath> //exp
#include<unordered_map>
using namespace std;

//初始化参数列表)
struct parameters{
   int count; //计数器
   int iter_max;  //最大迭代次数
   double Pcross; //交叉概率
   double Pmutation; //变异概率
   int population; //种群大小
   parameters(int a, int b, double c, double d, int e) : count(a),\
                  iter_max(b), Pcross(c), Pmutation(d), population(e){}
};
//在此处调参
parameters weights(0, 10, 1, 0.1, 10);
int inf = 100000;
vector<string> generation; //父代

class Solution
{
public:
   string TSP_GA(vector<vector<int> >& graph){
      srand(time(0)); //以系统时间作为随机种子
      int nums = graph.size(); //城市的个数
      init(nums);
      vector<string> newgeneration;
      string bestpath;
      int best = inf, bestid = 0;
      for(int i = 0; i < weights.iter_max; ++i){
         //计算种群适应度 并记录最好的
         vector<double> distance;
         for(int j = 0; j < weights.population; ++j){
            int cur = f(generation[j], graph);
            distance.push_back(cur);
            if(cur < best){
               bestid = j;
               best = cur;
               bestpath = generation[bestid];
            }
         }
         //轮盘赌选择子代个体
         newgeneration.clear();
         vector<double> roulette = accumulate(distance);
         for(int j = 0; j < weights.population; ++j){
            float p = rand() % 100 / (float)(100);
            int k;
            for(k = 0; k < weights.population; ++k){
               if (p <= roulette[k]) break;
            }
            newgeneration.push_back(generation[k]);
         }
         generation.clear();
         generation = newgeneration;
         
         //交叉
         cross(bestpath, nums);
         //变异
         mutation(bestpath, nums);
      }
      return bestpath;
   }

   void init(int nums){
      for(int i = 0; i < weights.population; ++i){
         string s;
         vector<int> temp = randPerm(nums);
         for(auto &i : temp){
            s += char(i + 'A');
         }
         generation.push_back(s);
      }
   }
   vector<int> randPerm(int n) { //返回随机序列
      vector<int> c;
      for (int i = 0; i < n; i++) {
         c.push_back(i);
      }
      for (int i = 1; i < n; i++) {
         swap(c[i], c[rand() % (n-2)+1]);
      }
      return c;
   }

   double f(string temp, vector<vector<int> > graph){
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

   //计算累积概率
   vector<double> accumulate(vector<double> &distance){
      double sum = 0, pre = 0;
      vector<double> result;
      for(auto &i:distance) sum += i;
      for(int i = 0; i < distance.size(); ++i){
         pre += distance[i]/sum; 
         result.push_back(pre);
      }
      return result;
   }

   //交叉变异：顺序交叉法
   void cross(string best, int nums){
      unordered_map<char,char> mapping1;
      unordered_map<char,char> mapping2;
      for(int i = 0; i < weights.population; ++i){
         if(generation[i] == best) continue; 

         if(rand() % 100 / (float)(100) > weights.Pcross){
            //选择要交配的个体 精英个体不进行交叉
            int j = rand()%(weights.population-1);
            while(generation[j]  == best) j = rand()%(weights.population-1);
            //生成交叉点
            int head = rand()%(nums-2)+1;//生成1～nums-1之间的整数
            int rear = rand()%(nums-2)+1;
            if(head > rear) swap(head, rear);
            //将对应片段进行交换
            mapping1.clear();
            mapping2.clear();
            for(int t = head; t <= rear; ++t){
               mapping1.insert(make_pair(generation[i][t],generation[j][t])); //对父代j进行映射
               mapping2.insert(make_pair(generation[j][t],generation[i][t])); //对父代i进行映射
               swap(generation[i][t],generation[j][t]);
            }
            //将剩下的片段按照映射关系处理
            for(int t = 1; t < nums; ++t){
               if(head <= t && t <= rear) continue;
               char key = generation[j][t];
               while(mapping1.count(key)){
                  key = mapping1[key];
               }
               generation[j][t] = key;

               key = generation[i][t];
               while(mapping2.count(key)){
                  key = mapping2[key];
               }
               generation[i][t] = key;
            }
         }
      }
   }

   void mutation(string best, int nums){
      for(int i = 0; i < weights.population; ++i){
         if(generation[i] == best) continue;
         if(rand() % 100 / (float)(100) > weights.Pmutation){
            //随机生成要交换的坐标i,j 1~nums-1
            int head = rand()%(nums-1-1)+1;
            int rear = rand()%(nums-1-1)+1;
            swap(generation[i][head], generation[i][rear]);
         }
      }
   }

  

};

int main()
{
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
   string res = Solution().TSP_GA(graph);
   for(auto &i:res){
      cout << i << "——>";
   }
   cout << *(res.begin());
   cout << "the shortest distance is " << Solution().f(res, graph);
   return 0;
}
