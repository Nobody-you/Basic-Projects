#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct student
{
    char name[10];
    char id[15];
    int age;
    float score;
};

struct Class
{
    struct student stu[30];
    int n;
}cls;

static int max = 0;

void Login_Interface();

void Add_Interface()
{
    int i, num;
renum:
    puts("请输入学生的个数: ");
    scanf("%d", &num);
    if (num + max > 30)
    {
        printf("人数过多，最多再输入%d人。", 30-max);
        goto renum;
    }
    for (i=max; i<max+num; i++)
    {
        cls.n = i;
        int ret;
again:
        puts("请输入学生的信息: ");
        ret = scanf("%s %s %d %f", cls.stu[i].name, cls.stu[i].id, &cls.stu[i].age, &cls.stu[i].score);
        if (ret != 4)
        {
            puts("输入有误！请重新输入: ");
            while (getchar() != '\n');
            goto again;
        }
    }
    max += num;
    getchar();
    puts("输入成功！返回主界面？[y/n]");

    char ch;
back:
    ch = getchar();
    getchar();
    if (ch == 'y')
        Login_Interface();
    else
        if (ch == 'n')
            return;
        else
        {
            puts("输入有误！请再次输入: ");
            goto back;
        }
}


void Swap(int a, int b)
{
    char name_temp[10];
    char id_temp[15];
    int age_temp;
    float score_temp;

    strcpy(name_temp, cls.stu[a].name);
    strcpy(id_temp, cls.stu[a].id);
    age_temp = cls.stu[a].age;
    score_temp = cls.stu[a].score;
    strcpy(cls.stu[a].name, cls.stu[b].name);
    strcpy(cls.stu[a].id, cls.stu[b].id);
    cls.stu[a].age = cls.stu[b].age;
    cls.stu[a].score = cls.stu[b].score;
    strcpy(cls.stu[b].name,name_temp);
    strcpy(cls.stu[b].id, id_temp);
    cls.stu[b].age = age_temp;
    cls.stu[b].score = score_temp;

    return;
}


void Search_Interface()
{
    int list[30];
    int f, i, j;
reff:
    puts("按学号搜索[1]还是按姓名搜索[2]？");
    scanf("%d", &f);
    switch (f)
    {
        case 1:
            {
                char ID[15];
reidd:
                printf("按学号搜索: ");
                scanf("%s", ID);
                int flag = 0;
                for (i=0; i<max; i++)
                    if (strcmp(cls.stu[i].id, ID) == 0)
                        list[flag++] = i;
                if (flag)
                {
                    puts("搜索已完成！搜索结果如下: ");
                    puts("姓名          学号            年龄   分数");
                    for (i=0; i<flag; i++)
                        printf("%-12s%-18s%-5d%-6.2f\n", cls.stu[list[i]].name, cls.stu[list[i]].id, cls.stu[list[i]].age, cls.stu[list[i]].score);
                }
                else
                {
                    puts("输入有误！请重新输入学号！");
                    goto reidd;
                }
                break;
            }
        case 2:
            {
                char NAME[10];
renamee:
                printf("按姓名搜索: ");
                scanf("%s", NAME);
                int flag = 0;
                for (i=0; i<max; i++)
                    if (strcmp(cls.stu[i].name, NAME) == 0)
                        list[flag++] = i;
                if (flag)
                {
                    puts("搜索已完成！搜索结果如下: ");
                    puts("姓名          学号            年龄   分数");
                    for (i=0; i<flag; i++)
                        printf("%-12s%-18s%-5d%-6.2f\n", cls.stu[list[i]].name, cls.stu[list[i]].id, cls.stu[list[i]].age, cls.stu[list[i]].score);
                }
                else
                {
                    puts("输入有误！请重新输入名字！");
                    goto renamee;
                }
                break;
            }
        default:puts("输入有误！请重新输入！");goto reff;
    }

    puts("返回主界面？[y/n]");
    getchar();
    char ch;
back:
    ch = getchar();
    getchar();
    if (ch == 'y')
        Login_Interface();
    else
        if (ch == 'n')
            return;
        else
        {
            puts("输入有误！请再次输入: ");
            goto back;
        }
}

void Delete_Interface()
{
    puts("学生列表如下: ");
    puts("姓名          学号            年龄   分数");
    int i, j;
    for (i=0; i<max; i++)
    {
        printf("%-12s%-18s%-5d%-6.2f\n", cls.stu[i].name, cls.stu[i].id, cls.stu[i].age, cls.stu[i].score);
    }
    
    int f;
ref:
    puts("按学号删除[1]还是按姓名删除[2]？");
    scanf("%d", &f);
    switch (f)
    {
        case 1:
            {
                char ID[15];
reid:
                printf("按学号删除: ");
                scanf("%s", ID);
                int flag = 0;
                for (i=0; i<max; i++)
                {
                    if (strcmp(cls.stu[i].id, ID) == 0)
                    {
                        for (j=i; j<max-1; j++)
                            Swap(j, j+1);
                        flag++;
                        max--;
                    }
                }
                if (flag)
                {
                    puts("删除已完成！删除后学生列表如下: ");
                    for (i=0; i<max; i++)
                        printf("%-12s%-18s%-5d%-6.2f\n", cls.stu[i].name, cls.stu[i].id, cls.stu[i].age, cls.stu[i].score);
                }
                else
                {
                    puts("输入有误！请重新输入学号！");
                    goto reid;
                }
                break;
            }
        case 2:
            {
                char NAME[10];
rename:
                printf("按名字删除: ");
                scanf("%s", NAME);
                int flag = 0;
                for (i=0; i<max; i++)
                    if (strcmp(cls.stu[i].name, NAME) == 0)
                    {
                        for (j=i; j<max-1; j++)
                            Swap(j, j+1);
                        flag++;
                        max--;
                        i--;
                    }
                if (flag)
                {
                    puts("删除已完成！删除后学生列表如下: ");
                    for (i=0; i<max; i++)
                        printf("%-12s%-18s%-5d%-6.2f\n", cls.stu[i].name, cls.stu[i].id, cls.stu[i].age, cls.stu[i].score);
                }
                else
                {
                    puts("输入有误！请重新输入名字！");
                    goto rename;
                }
                break;
            }
        default:puts("输入有误！请重新输入！");goto ref;
    }

    puts("返回主界面？[y/n]");
    getchar();
    char ch;
back:
    ch = getchar();
    getchar();
    if (ch == 'y')
        Login_Interface();
    else
        if (ch == 'n')
            return;
        else
        {
            puts("输入有误！请再次输入: ");
            goto back;
        }
}



void Browse_Interface()
{
    puts("姓名          学号            年龄   分数");
    int i;
    for (i=0; i<max; i++)
    {
        printf("%-12s%-18s%-5d%-6.2f\n", cls.stu[i].name, cls.stu[i].id, cls.stu[i].age, cls.stu[i].score);
    }

    puts("返回主界面？[y/n]");
    getchar();
    char ch;
back:
    ch = getchar();
    getchar();
    if (ch == 'y')
        Login_Interface();
    else
        if (ch == 'n')
            return;
        else
        {
            puts("输入有误！请再次输入: ");
            goto back;
        }
}


void Rank_Interface()
{
    puts("顺序从大到小");

    int i, j;
    for (i=0; i<max-1; i++)
        for (j=0;j<max-i-1;j++)
            if (cls.stu[j+1].score > cls.stu[j].score)
                Swap(j,j+1);
    
    puts("姓名          学号            年龄   分数");
    for (i=0; i<max; i++)
    {
        printf("%-12s%-18s%-5d%-6.2f\n", cls.stu[i].name, cls.stu[i].id, cls.stu[i].age, cls.stu[i].score);
    }

    puts("返回主界面？[y/n]");
    getchar();
    char ch;
back:
    ch = getchar();
    getchar();
    if (ch == 'y')
        Login_Interface();
    else
        if (ch == 'n')
            return;
        else
        {
            puts("输入有误！请再次输入: ");
            goto back;
        }
}



void Login_Interface()
{
    puts("**************************************************************");
    puts("**************| 学  生  信  息  管  理  系  统 |**************");
    puts("**************************************************************");
    puts("*                                                            *");
    puts("*                     1.添加学生信息                         *");
    puts("*                     2.删除学生信息                         *");
    puts("*                     3.搜索学生信息                         *");
    puts("*                     4.按成绩排序                           *");
    puts("*                     5.浏览信息列表                         *");
    puts("*                     6.退出                                 *");
    puts("*                                                            *");
    puts("**************************************************************");
    puts("*---------------------请输入你的选择-------------------------*");
    char choice;
Login_again:
    choice = getchar();
    switch(choice)
    {
        case '1':Add_Interface();break;
        case '2':Delete_Interface();break;
        case '3':Search_Interface();break;
        case '4':Rank_Interface();break;
        case '5':Browse_Interface();break;
        case '6':puts("谢谢使用！");exit(0);
        default:puts("输入错误！请再次输入: ");goto Login_again;
    }
}


int main()
{
    Login_Interface();
    return 0;
}
