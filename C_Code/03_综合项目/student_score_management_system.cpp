#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 学生结构体定义
struct Student 
{
    int id;                 // 学号
    char name[20];          // 姓名
    char cla[20];           // 班级
    char major[20];         // 专业
    char gender;            // 性别 'M' 或 'W'
    int age;                // 年龄
    int scores[5];          // 5门课成绩,按照英语、高数、物理、化学、C程序的顺序
    float avg;              // 平均分
    struct Student* next;
};

// 创建链表，返回头指针
struct Student* createList() 
{
    // 10个学生的初始数据（学号、姓名、班级、专业、性别、年龄、5门成绩）
    struct Student initData[10] = {
        {1001, "张三", "12505", "自动化", 'M', 20, {78, 85, 82, 90, 88}, 0},
        {1002, "李四", "12505", "自动化", 'M', 19, {65, 70, 68, 72, 69}, 0},
        {1003, "王芳", "12505", "自动化", 'W', 20, {92, 88, 94, 91, 89}, 0},
        {1004, "赵磊", "12505", "自动化", 'M', 21, {55, 60, 58, 62, 57}, 0},
        {1005, "孙丽", "12505", "自动化", 'W', 19, {88, 85, 90, 86, 87}, 0},
        {1006, "周强", "12505", "自动化", 'M', 20, {72, 68, 75, 70, 73}, 0},
        {1007, "吴敏", "12505", "自动化", 'W', 19, {95, 92, 96, 93, 94}, 0},
        {1008, "郑伟", "12505", "自动化", 'M', 20, {45, 50, 48, 52, 47}, 0},
        {1009, "林霞", "12505", "自动化", 'W', 19, {82, 79, 84, 81, 80}, 0},
        {1010, "郭峰", "12505", "自动化", 'M', 21, {68, 72, 66, 70, 69}, 0}
    };
    struct Student* head = NULL;
    struct Student* tail = NULL;
    //用循环创建链表
    for (int i = 0; i < 10; i++) 
    {
        struct Student* newNode = (struct Student*)malloc(sizeof(struct Student));
        if (newNode == NULL) 
        {
            printf("内存分配失败\n");
            exit(1);
        }
        // 复制数据给新开辟的节点
        newNode->id = initData[i].id;
        strcpy_s(newNode->name, initData[i].name);
        strcpy_s(newNode->cla, initData[i].cla);
        strcpy_s(newNode->major, initData[i].major);
        newNode->gender = initData[i].gender;
        newNode->age = initData[i].age;
        for (int j = 0; j < 5; j++) 
        {
            newNode->scores[j] = initData[i].scores[j];
        }
        newNode->avg = 0.0;   
        newNode->next = NULL;
        if (head == NULL)//将头节点连接到第一个节点上
        {
            head = newNode;
            tail = newNode;
        }
        else //将节点连接
        {
            tail->next = newNode;
            tail = newNode;
        }
    }
    return head;
}
//打印函数
void printnode(struct Student* head)
{
    struct Student* p = head;
    printf("学号\t姓名\t班级\t专业\t性别\t年龄\t英语\t数学\t物理\t化学\tC语言\n");
    while (p != NULL)
    {
        printf("%d\t%s\t%s\t%s\t%c\t%d\t%d\t%d\t%d\t%d\t%d\n", p->id, p->name, p->cla, p->major, p->gender, p->age, 
            p->scores[0], p->scores[1], p->scores[2], p->scores[3], p->scores[4]);
        p = p->next;//更新p为下一个节点
    }
}
//函数功能：计算每个人的平均分和每一门课程的平均分
void avgcal(struct Student* head)
{
    struct Student* p = head;
    long sumE = 0, sumM = 0, sumP = 0, sumCH = 0, sumC = 0;
    printf("学号\t姓名\t班级\t专业\t性别\t年龄\t英语\t数学\t物理\t化学\tC语言\t平均成绩\n");
    while (p != NULL)
    {
        p->avg = (p->scores[0]+ p->scores[1]+ p->scores[2]+ p->scores[3]+ p->scores[4]) / 5.0;
        printf("%d\t%s\t%s\t%s\t%c\t%d\t%d\t%d\t%d\t%d\t%d\t%.2f\n", p->id, p->name, p->cla, p->major, p->gender, p->age, 
            p->scores[0], p->scores[1], p->scores[2], p->scores[3], p->scores[4],p->avg);
        p = p->next;
    }
    p = head;//将p重新指向head
    while (p != NULL)
    {
        sumE = sumE + p->scores[0], sumM = sumM + p->scores[1], sumP = sumP + p->scores[2], sumCH = sumCH + p->scores[3], sumC = sumC + p->scores[4];
        p = p->next;
    }
    printf("英语平均分:%.2f\n", sumE / 10.0);
    printf("数学平均分:%.2f\n", sumM / 10.0);
    printf("物理平均分:%.2f\n", sumP / 10.0);
    printf("化学平均分:%.2f\n", sumCH / 10.0);
    printf("C语言平均分:%.2f\n", sumC / 10.0);
}
//查找函数
void findstudent(struct Student* head)
{
    struct Student* p = head;
    char findname[20];
    printf("请输入你要查找的学生姓名:");
    scanf_s("%s", findname, (unsigned)sizeof(findname));
    //先找到被查找的学生的位置
    while (p!=NULL&&strcmp(p->name, findname) != 0)
    {
        p = p->next;
    }
    if (p == NULL)
    {
        printf("无该学生!\n");
    }
    else
    {
        printf("学号\t姓名\t班级\t专业\t性别\t年龄\t英语\t数学\t物理\t化学\tC语言\t平均成绩\n");
        printf("%d\t%s\t%s\t%s\t%c\t%d\t%d\t%d\t%d\t%d\t%d\t%.2f\n", p->id, p->name, p->cla, p->major, p->gender, p->age, 
            p->scores[0], p->scores[1], p->scores[2], p->scores[3], p->scores[4], p->avg);
    }
    printf("\n");
    p = head;//将p重新指向head，下同
    //打印平均成绩60分以下的学生
    printf("平均成绩60分以下的学生:\n");
    printf("学号\t姓名\t班级\t专业\t性别\t年龄\t英语\t数学\t物理\t化学\tC语言\t平均成绩\n");
    while (p != NULL)
    {
        if (p->avg < 60)
        {
            printf("%d\t%s\t%s\t%s\t%c\t%d\t%d\t%d\t%d\t%d\t%d\t%.2f\n", p->id, p->name, p->cla, p->major, p->gender, p->age, 
                p->scores[0], p->scores[1], p->scores[2], p->scores[3], p->scores[4], p->avg);
        }
        p = p->next;
    }
    p = head;
    int count = 0;//用来记录一个学生不及格课程的个数
    //打印2门以上课程不及格的学生
    printf("\n2门以上课程不及格的学生:\n");
    printf("学号\t姓名\t班级\t专业\t性别\t年龄\t英语\t数学\t物理\t化学\tC语言\t平均成绩\n");
    while (p != NULL)
    {
        count = 0;
        for (int i = 0;i < 5;i++)
        {
            if (p->scores[i] < 60)
            {
                count++;
            }
        }
        if (count > 2)
        {
            printf("%d\t%s\t%s\t%s\t%c\t%d\t%d\t%d\t%d\t%d\t%d\t%.2f\n", p->id, p->name, p->cla, p->major, p->gender, p->age, 
                p->scores[0], p->scores[1], p->scores[2], p->scores[3], p->scores[4], p->avg);
        }
        p = p->next;
    }
    p = head;
    printf("\n");
    //打印平均成绩在90分以上并且每科成绩均在85分以上的学生
    printf("平均成绩在90分以上并且每科成绩均在85分以上的学生:\n");
    printf("学号\t姓名\t班级\t专业\t性别\t年龄\t英语\t数学\t物理\t化学\tC语言\t平均成绩\n");
    while (p != NULL)
    {
        if (p->avg > 90&&p->scores[0]>85&& p->scores[1]>85&& p->scores[2]>85&& p->scores[3]>85&& p->scores[4]>85)
        {
            printf("%d\t%s\t%s\t%s\t%c\t%d\t%d\t%d\t%d\t%d\t%d\t%.2f\n", p->id, p->name, p->cla, p->major, p->gender, p->age, 
                p->scores[0], p->scores[1], p->scores[2], p->scores[3], p->scores[4], p->avg);
        }
        p = p->next;
    }
}
//函数功能：按平均成绩由大到小排序并打印
void order(struct Student* head)
{
    struct Student* p1, * p2,*p=head;
    int tempid, tempage, tempscores;
    char tempname[20], tempcla[20], tempmajor[20], tempgender;
    float tempavg;
    //排序之前重新计算每个学生的平均成绩，防止添加新学生后平均成绩未赋值
    while (p != NULL)
    {
        p->avg = (p->scores[0] + p->scores[1] + p->scores[2] + p->scores[3] + p->scores[4]) / 5.0;
        p = p->next;
    }
    p = head;
    //冒泡排序
    for (p1 = head;p1 != NULL;p1 = p1->next)
    {
        for (p2 = p1->next;p2 != NULL;p2 = p2->next)
        {
            if (p2->avg > p1->avg)
            {
                //交换学生信息
                tempid = p1->id, p1->id = p2->id, p2->id = tempid;
                strcpy_s(tempname, p1->name), strcpy_s(p1->name, p2->name), strcpy_s(p2->name, tempname);
                strcpy_s(tempcla, p1->cla), strcpy_s(p1->cla, p2->cla), strcpy_s(p2->cla, tempcla);
                strcpy_s(tempmajor, p1->major), strcpy_s(p1->major, p2->major), strcpy_s(p2->major, tempmajor);
                tempgender = p1->gender, p1->gender = p2->gender, p2->gender = tempgender;
                tempage = p1->age, p1->age = p2->age, p2->age = tempage;
                for (int i = 0;i < 5;i++)
                {
                    tempscores = p1->scores[i], p1->scores[i] = p2->scores[i], p2->scores[i] = tempscores;
                }
                tempavg = p1->avg, p1->avg = p2->avg, p2->avg = tempavg;
            }
        }
    }
    printf("按平均成绩由大到小排序:\n");
    printf("学号\t姓名\t班级\t专业\t性别\t年龄\t英语\t数学\t物理\t化学\tC语言\t平均成绩\n");
    while (p != NULL)
    {
        p->avg = (p->scores[0] + p->scores[1] + p->scores[2] + p->scores[3] + p->scores[4]) / 5.0;
        printf("%d\t%s\t%s\t%s\t%c\t%d\t%d\t%d\t%d\t%d\t%d\t%.2f\n", p->id, p->name, p->cla, p->major, p->gender, p->age,
            p->scores[0], p->scores[1], p->scores[2], p->scores[3], p->scores[4], p->avg);
        p = p->next;
    }
}
//删除函数
struct Student* deletstudent(struct Student* head)
{
    char deletname[20];
    printf("请输入要删除的学生姓名:");
    scanf_s("%s", deletname, (unsigned)sizeof(deletname));
    struct Student* cru, * preul, * newhead = (struct Student*)malloc(sizeof(struct Student));//用虚拟头节点，后面不要单独考虑删除头节点的情况
    newhead->next = head, cru = head, preul = newhead;
    //先找到被删除学生的位置
    while (cru != NULL && strcmp(cru->name, deletname) != 0)
    {
        preul = cru;
        cru = cru->next;
    }
    //如果无该学生，直接将head返回
    if (cru == NULL)
    {
        printf("无该学生!\n");
        return(head);
    }
    preul->next = cru->next;
    head = newhead->next;
    free(cru), free(newhead);
    return(head);
}
//添加学生函数，我采用的尾插法
struct Student* addstudent(struct Student* head)
{
    struct Student* p1, * p2,*p3;
    p2 =p3= head;
    p1 = (struct Student*)malloc(sizeof(struct Student));//为新学生开辟一个内存
    printf("请输入添加的学生信息（空格隔开）:");
    scanf_s("%d %s %s %s %c %d %d %d %d %d %d", &p1->id, p1->name,(unsigned)sizeof(p1->name), p1->cla, (unsigned)sizeof(p1->cla),
        p1->major, (unsigned)sizeof(p1->major), &p1->gender,1, &p1->age, &p1->scores[0], &p1->scores[1], &p1->scores[2], &p1->scores[3], &p1->scores[4]);
    //找到尾节点
    while (p2 != NULL)
    {
        p3 = p2;
        p2 = p2->next;
    }
    p3->next = p1;//将尾节点和新开辟的节点连接
    p1->next = NULL;//让链表有尾
    return(head);
}
//释放链表所有节点
void freelist(struct Student* head)
{
    struct Student* p=head, * temp=NULL;
    while (p != NULL)
    {
        temp = p;
        p = p->next;
        free(temp);
    }
}

int main()
{
    struct Student* head = NULL;
    int choice;
    head = createList();//先得到一个最初链表头
    //循环菜单
    do
    {
        printf("\n1.输出功能\n");
        printf("2.计算功能\n");
        printf("3.查询功能\n");
        printf("4.排序功能\n");
        printf("5.删除功能\n");
        printf("6.插入功能\n");
        printf("7.退出程序\n");
        printf("请输入对应的数:");
        scanf_s("%d", &choice);
        printf("\n");
        switch (choice)
        {
        case 1:
            printnode(head);break;
        case 2:
            avgcal(head);break;
        case 3:
            findstudent(head);break;
        case 4:
            order(head);break;
        case 5:
            head = deletstudent(head);
            order(head);
            break;
        case 6:
            head=addstudent(head);
            order(head);
            break;
        case 7:
            printf("退出程序\n");
            break;
        default:
            printf("无效选择，请重新输入\n");
        }
    } while (choice != 7);
    freelist(head);
    return 0;
}