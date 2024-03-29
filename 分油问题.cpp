//created by WangHuan 

#include<iostream>
#include<vector>
#include<queue>
using namespace std;
struct OilState{
    int x, y, z; //对应10 7 3的状态
    int n; //对应父节点序号
    OilState(int a, int b, int c, int d) : x(a), y(b), z(c), n(d){}
};

queue<OilState> OilQueue; //用于存放广度优先搜索树中的节点
vector<OilState> arr; //用于广度优先搜索树中不重复的节点
vector<OilState> result; //用于存放结果

class Solution{
public: 
    bool isrepeat(OilState oil){
        for(auto &i : arr){
            if(i.x == oil.x and i.y == oil.y and i.z == oil.z) return true;
        }
        return false;
    }

    void GenerateNewOil(OilState old, int size){ //根据规则生成子节点
        //7不满时用10装满
        if(old.y < 7){
            OilState newOil(10-7-old.z, 7, old.z, size-1);
            if(!isrepeat(newOil)) OilQueue.push(newOil);
        }
        //3不满时用10装满
        if(old.z < 3){
            OilState newOil(10-3-old.y, old.y, 3, size-1);
            if(!isrepeat(newOil)) OilQueue.push(newOil);
        }
        //7不空时倒入10中
        if(old.y > 0){
            OilState newOil(10-old.z, 0, old.z, size-1);
            if(!isrepeat(newOil)) OilQueue.push(newOil);
        }
        //3不空时倒入10中
        if(old.z > 0){
            OilState newOil(10-old.y, old.y, 0, size-1);
            if(!isrepeat(newOil)) OilQueue.push(newOil);
        }
        //7全部倒入3中
        if(old.y > 0 and old.y + old.z <= 3){
            OilState newOil(10-old.y-old.z, 0, old.y + old.z, size-1);
            if(!isrepeat(newOil)) OilQueue.push(newOil);
        }
        //3全部倒入7中
        if(old.z > 0 and old.y + old.z <= 7){
            OilState newOil(10-old.y-old.z, old.y + old.z, 0, size-1);
            if(!isrepeat(newOil)) OilQueue.push(newOil);
        }
        //7被3装满
        if(old.y < 7 and old.y + old.z >= 7){
            OilState newOil(10-old.y-old.z, 7, old.y + old.z - 7, size-1);
            if(!isrepeat(newOil)) OilQueue.push(newOil);
        }
        //3被7装满
        if(old.z < 3 and old.y + old.z >= 3){
            OilState newOil(10-old.y-old.z, old.y + old.z - 3, 3, size-1);
            if(!isrepeat(newOil)) OilQueue.push(newOil);
        }
    }

    bool isresult(OilState s){
        if(s.x == 5 and s.y == 5 and s.z == 0) return true;
        return false;
    }
};

int main(){
    OilState rootOil(10, 0, 0, -1);
    OilQueue.push(rootOil);
    while(!OilQueue.empty()){
        OilState oil = OilQueue.front();
        if(!Solution().isrepeat(oil)){
            arr.push_back(oil);
        }
        int size = arr.size();
        OilQueue.pop();
        Solution().GenerateNewOil(oil, size);
    }

    int pos = 0; //记录（5,5,0）在arr里面的位置
    for(int i = 0; i < arr.size(); ++i){
        if(Solution().isresult(arr[i])){
            pos = i;
            break;
        }
    }

    while(arr[pos].n != -1){
        result.push_back(arr[pos]);
        pos = arr[pos].n;
    }

    if(arr[pos].n == -1){
        result.push_back(arr[pos]);
    }

    for(int i = result.size()-1; i >= 0; i--){
        cout<<"("<<result[i].x<<","<<result[i].y<<","<<result[i].z<<")"<<endl;
    }
    return 0;
}
