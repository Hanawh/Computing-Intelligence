# Computing-Intelligence

The Assignment of Computing Intellgence class in XMU

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



