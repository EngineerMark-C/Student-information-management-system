# 学生信息管理系统
## 项目背景
这是一次大一下的C语言程序设计作业，作业要求是以C语言为内核设计一个学生信息管理系统，同时制作可视化界面。由于各方面知识的缺失，在东拼西凑下用前端的方式完成了该作业。运用的知识有C,MySQL,JavaScript,Html,CSS。
## 使用方法
1. 配置好MySQL环境，运行MySQL服务
2. 在项目文件夹下运行server.js
3. 用浏览器进入http://localhost:3000/
## 实验报告
### 实验目的
1. 强化和巩固理论基础，掌握数据库编程的基本技巧
2. 用C语言和C++或其他语言及相应开发环境，实现一个小型完整程序的设计与开发
3. 巩固深化所学课程的知识，培养学生运用知识，分析和解决实际问题的能力
4. 学生根据所学的数据库原理与程序设计的知识，能够针对一个小型的数据库管理系统，进行系统的需求分析，系统设计，数据库设计，编码，测试等，完成题目要求的功能，从而达到开发一个小型数据库的目的
### 实验内容
- 实现对学生管理系统的多端登录，增添，删除，修改，修改单项信息，查找，学生查找个人信息，模糊查找，遍历，排序，导出学生信息文件，退出14个功能
### 需求分析
1.	实现用XXX制作增删改查等各个对话框以及添加类和变量
2.	实现对学生信息和教师信息进行增加，删除，修改，查找
3.	实现学生可以查看自己的信息
4.	实现对学生信息信息按XX排序的功能
5.	实现对学生信息的单项修改
6.	实现可以导出学生的信息为Excel文件或者其他文件
7.	实现查找学生信息时的模糊查找
8.	实现正常退出系统的功能
9.	实现系统的稳定运行，不出现bug
### 概要设计
![流程图](https://github.com/EngineerMark-C/Student-information-management-system/blob/main/picture/%E6%B5%81%E7%A8%8B%E5%9B%BE.svg)

用户通过前端界面进行插入、删除、查询、更新或排序数据等操作，当用户执行操作时，前端JavaScript代码会捕获这些事件，并构造一个HTTP请求发送到服务器。服务器（使用Node.js和Express框架）接收到HTTP请求，并根据请求的路径（如/insert、/delete、/query等）将请求路由到相应的处理函数。处理函数提取前端发送参数并构造一个命令字符串，用exec函数调用外部C语言，C语言程序接收命令行参数，与MySQL数据库进行交互，执行相应的数据库操作。C语言程序将操作结果输出到标准输出（stdout）或标准错误（stderr），这些输出将被exec函数捕获。后端服务器接收到C语言程序的输出后，将其解析并构造为JSON格式的HTTP响应，发送回前端。前端接收到HTTP响应后，更新用户界面并显示操作结果。
### 设计过程
这里着重讲解服务器及后端程序
关于C程序 `main.c`：
包含了与MySQL数据库交互的函数，如`insert_data`、`delete_data`、`query_data`、`update_data`、`sort_data_by_score`、`find_student_by_id`和display_total_count。使用了MySQL C API来执行SQL语句，并处理查询结果。程序通过命令行参数接收操作类型和相关参数。
关于后端服务器 `server.js`：
使用express框架创建了一个Web服务器。定义了路由来处理不同的HTTP请求，并使用body-parser中间件来解析请求体中的JSON数据。使用exec函数调用外部程序（C语言编写的程序），并将命令行参数传递给它。
C程序以`insert_data()`为例：
```c
void insert_data(MYSQL *conn, int id, char *name, int score1, int score2)
{
    char query[512];
    sprintf(query, "INSERT INTO table1 (id, name, score1, score2, changeTime) VALUES (%d, '%s', %d, %d, NOW())", id, name, score1, score2);

    if (mysql_query(conn, query))
    {
        printf("{\"error\": \"Insert data error: %s\"}\n", mysql_error(conn));
    }
    else
    {
        printf("{\"message\": \"Insert data successfully.\"}\n");
    }
}
```
该程序使用sprintf函数格式化一个字符串，构造一个SQL的INSERT语句，调用mysql_query函数执行上面构造的SQL语句，然后根据程序执行成功与否打印一个JSON格式字符串。
服务器程序以`app.post('/insert', (req, res))`为例
```js
app.post('/insert', (req, res) => {
    const { id, name, score1, score2 } = req.body;
    const command = `main insert ${id} "${name}" ${score1} ${score2}`;
    executeCommand(command, res);
});
```
`app.post`方法定义了一个处理POST请求的路由。`/insert`是路由的路径，表示当客户端向服务器的/insert路径发送POST请求时，将触发此函数。(req, res)是回调函数的参数，其中req代表请求对象，res代表响应对象。const command = `main insert ${id} "${name}" {score1} ${score2}`声明了一个名为command的变量，用于存储将要执行的命令，调用前面定义的`executeCommand`函数执行命令，并处理命令的输出，最终将结果发送回客户端。
### 调试过程
1. 本项目使用了C语言连接MySQL数据库。在连接过程中，我学习了CMake搭建C语言项目。在连接过程中遇到了许多麻烦。原先我电脑上的C语言编译器是Linux下的gcc。而官网提供的MySQL C API仅提供了Windows环境下的链接库。而Linux环境所需要的.a文件链接库实在难以寻找，最后我使用了MSVC编译器 才解决了数据库连接问题。
2. 由于本项目要求由C语言作为内核实现学生信息管理系统的可视化界面。我曾尝试过许多方法如Qt，GTK，MFC，WindowsAPI，但实在不了解C++，最后采用了Web端实现可视化的方法。
3. 在初步完成了整个系统的搭建后，最初版本的数据展现是C语言终端格式化输出的形式。我想实现数据库数据更好地展现，想要采用表格。此时便需要C语言输出JSON格式的数据。但这样还没完，前端HTML制作的表格始终无法接收该数据。后来在ChatGPT的帮助下，调用了现成的表格库，经过长时间的调试才实现了表格的展现。
4. 数据库中有一条数据是更改时间。这个更改时间可以用多种方式实现，由服务器获取时间，由c程序获取时间，由MySQL库获取时间。三种方法都试过，前两种方法在传递过程中总会格式不兼容，最后使用了MySQL库的NOW（）函数才得以很好解决。同时使c程序和服务器程序更加精简。
