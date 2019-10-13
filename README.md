# Computing-Intelligence

The Assignments of Computing Intellgence class in XMU

## Assignment 1 : 分油问题

### 问题描述
> 两个小孩去打油，一人带了一个一斤的空瓶，另一个带了一个七两、一个三两的空瓶。
原计划各打一斤油，可是由于所带的钱不够，只好两人合打了一斤油，在回家的路上，
两人想平分这一斤油，可是又没有其它工具，试仅用三个瓶子（一斤、七两、三两）精
确地分成两个半斤油来。

### 算法设计
- `OilQueue`用于存放广度优先搜索树中的节点
- `arr`用于广度优先搜索树中不重复的节点
- 设计储存油状态的节点`OilState`，`x,y,z`分别代表一斤、七两、三两桶内的油量，`n`代表父节点在`arr`中的编号
- `bool isrepeat(OilState oil)`用于判断所生成节点是否出现过
- `void GenerateNewOil(OilState old, int size)`用于根据规则生成子节点
- `bool isresult(OilState s)`用于判断是否到达目标状态

### 变换规则
|规则序号|规则|规则解释|
|:------:|:------:|:------:|
|1|(S,R) and S<7 --> (7,R)|7不满时用10装满|
|2|(S,R) and R<7 --> (S,3)|3不满时用10装满|
|3|(S,R) and S>0 --> (0,R)|7不空时倒入10中|
|4|(S,R) and R>0 --> (S,0)|3不空时倒入10中|
|5|(S,R) and S>0 and S+R<=3 --> (0,S+R)|7全部倒入3中|
|6|(S,R) and R>0 and S+R<=7 --> (S+R,0)|3全部倒入7中|
|7|(S,R) and S<7 and S+R>=7 --> (7,S+R-7)|7被3装满|
|8|(S,R) and R<3 and S+R>=3 --> (S+R-3,3)|3被7装满|



## Assignment 2 : 模拟退火算法解决TSP问题
### 问题描述
> 旅行推销员问题（英语：Travelling salesman problem, TSP）给定一系列城市和每对城市之间的距离，求解访问每一座城市一次并回到起始城市的最短回路。它是组合优化中的一个NP困难问题，在运筹学和理论计算机科学中非常重要。  

### 模拟退火算法简介
> 1982年,Kirkpatrick意识到固体退火算法与组合优化问题之间的类似性Metropolis等对孤立在恒定温度下达到热平衡的过程的模拟的启迪：把Metropolis准则引入优化过程中模拟退火算法（Simulated Annealing Algorithm，简称SA），源于对固体退火过程的模拟，采用Metropolis接受准则，并用一组称为冷却表的参数控制算法进程，使算法在多项式时间里给出一个近似最优解，并且可以大程度的跳出局部最优点找寻全局最优点。  

### 算法设计
- `coolingSchedule`用来存放相关参数
- `GenerateNewPath`用2变换法来产生新的路径
- `Metropolis`用来定义Metropolis准则




