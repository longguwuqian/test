#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <limits.h>

#define SIZE_view 70
#define SIZE_line  100
#define SIZE_way   300

#define MAXNODE 30
#define MAX 1000

void stdin_flush()
{
    static int c;
    while((c = getchar()) != '\n' && c != EOF);
}

int user_says_yes(char *message)
{
    static char ans[5];
    if (message != NULL) printf("%s", message);
    while(1) {
        scanf("%4s", ans);
        stdin_flush();
        if( ans[3] == '\0'                   &&
            (ans[2] == 's' || ans[2] == 'S') &&
            (ans[1] == 'e' || ans[1] == 'E') &&
            (ans[0] == 'y' || ans[0] == 'Y')  ) return 1;
        if( ans[2] == '\0'                   &&
            (ans[1] == 'o' || ans[1] == 'O') &&
            (ans[0] == 'n' || ans[0] == 'N')  ) return 0;
    }
}

#define CHECK_FOPEN(ptr) do { \
        if(NULL == ptr) { \
            fprintf(stderr,"LINE %d:无法打开文件!\n",__LINE__); \
            exit(EXIT_FAILURE); \
        } \
    }while (0)

int way_count;
int line_count;
int view_count;

struct view_info {  //存放车站信息的结构体
    int id;       //车站身份编码
    char name[20]; //车站名字
    int  code;     //车站编码
    char shortname[20];  //车站简写
    char LName[100];    //车站所属的铁路线
} views[SIZE_view];


struct line_info { //铁路信息
    int Lid;   //铁路身份编码
    char LName[20];//铁路名字
    int start_id; //起始车站编码
    int end_id;   //终点车站编码
    int dist;     //铁路线长度
    char sign[10]; //通行标志
} lines[SIZE_line];

struct way_info { //道路信息
    int station1; //车站1
    int station2; //车站2
    int dist;     //两车站间距离
} ways[SIZE_way];
struct path_info  //最短路径
{
    int count;//最短路径的路径个数
    int path[SIZE_view];//表明通过的车站
} path_list[SIZE_view][SIZE_view];

void readviews()
{
    FILE *fp;
    int i;

    fp=fopen("views","rb");
    CHECK_FOPEN(fp);
    printf("id            name              code           shortname        LName   \n");

    for(i=0; fread(&views[i],sizeof(struct  view_info),1,fp)!=0; i++) {//文件中的信息赋给station_info结构体，并且把车站信息输出到终端
        printf("%d%-10s%d%-10s%-10s\n",views[i].id,views[i].name,views[i].code,views[i].shortname,views[i].LName);

    }
    view_count=i;
    fclose(fp);
}
void readways()
{
    FILE *fp;
    int i;
    fp=fopen("ways","rb");
    CHECK_FOPEN(fp);
    printf("station1 station2 dist");
    for(i=0; i<SIZE_way; i++) {
        fscanf(fp,"%d%d%d",&ways[i].station1,&ways[i].station2,&ways[i].dist);
        printf("%d     %d     %d\n",ways[i].station1,ways[i].station2,ways[i].dist);//把文件中的信息赋给way_info结构体，并且把道路信息输出到终端上

    }
    fclose(fp);
}
void readlines()
{
    FILE *fp;
    int i;

    fp=fopen("lines","rb");
    CHECK_FOPEN(fp);

    printf("Lid                  LName                  start_id          end_id             dist       sign");
    for(i=0; fread(&lines[i],sizeof(struct  line_info),1,fp)!=0; i++) {
        printf("%d%-10s%d%-10d%d%-10s\n",lines[i].Lid,lines[i].LName,lines[i].start_id,lines[i].end_id,lines[i].dist,lines[i].sign);
    }//把文件中的信息赋给line_info结构体，并且把铁路信息输出到终端上

    line_count=i+1;
    fclose(fp);
}
void search()
{
    FILE *fp;
    int i;
    char sta_name[256];
    printf("请输入车站名：");
    scanf("%250s", sta_name);
    fp=fopen("views","rb");
    CHECK_FOPEN(fp);
    for(i=0; i<view_count; i++) {
        fread(&views[i],sizeof(struct view_info),1,fp);
    }
    fclose(fp);
    for(i=0; i<view_count; i++) {
        if(strcmp(sta_name,views[i].name)==0) {//对比输入的车名和结构体中车站名字，如果相同就输出该车站的相关信息。
            printf("车站信息：\n");
            printf("id    name   code    shortname   LName");
            printf("%d         %s           %d          %s      %s",views[i].id,views[i].name,views[i].code,views[i].shortname,views[i].LName);
            return;
        }
    }
    printf("不能找到所要查询的车站。");
}

void addview()
{
    FILE *fp;
    int i;

    printf("请输入新车站的信息\n");
    printf("id:");
    scanf("%d",&views[view_count].id);
    stdin_flush();
    printf("name:");
    scanf("%s",views[view_count].name);
    stdin_flush();
    printf("code:");
    scanf("%d",&views[view_count].code);
    stdin_flush();
    printf("shortname:");
    scanf("%s",views[view_count].shortname);
    stdin_flush();
    printf("LName:");
    scanf("%s",views[view_count].LName);
    stdin_flush();
    if((fp=fopen("views","ab"))==NULL) {

        printf("\n不能打开文件，按任意键退出!");
        exit(0);
    }
    for(i=view_count; i<view_count+1; i++)
        fwrite(&views[i],sizeof(struct view_info),1,fp);
    fclose(fp);
    printf("已成功添加新的车站！\n");
    printf("新车站的编码是： %d:\n",view_count);
}

void addway()
{
    FILE *fp;
    fp=fopen("ways","ab");
    CHECK_FOPEN(fp);

    printf("请输入新道路的信息\n");
    printf("station1：\n");
    scanf("%d",&ways[way_count].station1);
    stdin_flush();
    printf("station2:\n");
    scanf("%d",&ways[way_count].station2);
    stdin_flush();
    printf("dist:\n");
    scanf("%d",&ways[way_count].dist);
    stdin_flush();

    fprintf(fp,"%d                       %d                   %d",ways[way_count].station1,ways[way_count].station2,ways[way_count].dist);
    //将新的路径信息输入到文件中

    fclose(fp);
    printf("已成功添加新的道路！");
    way_count=way_count+1;
    printf("新的道路编码是:%d",way_count);
}

void addline()
{
    FILE *fp;

    fp=fopen("lines","ab");
    CHECK_FOPEN(fp);

    printf("请输入新铁路线的信息:\n");
    printf("Lid:\n");
    scanf("%d",&lines[line_count].Lid);
    stdin_flush();
    printf("LName:\n");
    scanf("%s",lines[line_count].LName);
    stdin_flush();
    printf("start_id:\n");
    scanf("%d",&lines[line_count].start_id);
    stdin_flush();
    printf("end_id:\n");
    scanf("%d",&lines[line_count].end_id);
    stdin_flush();
    printf("dist:\n");
    scanf("%d",&lines[line_count].dist);
    stdin_flush();
    printf("sign:\n");
    scanf("%s",lines[line_count].sign);
    stdin_flush();
    fprintf(fp,"%d   %s   %d   %d   %d   %s", lines[line_count].Lid,lines[line_count].LName,lines[line_count].start_id,lines[line_count].end_id,lines[line_count].dist,lines[line_count].sign);//将新的铁路信息输出到文件中
    fclose(fp);
    printf("已成功添加新的铁路线路！");
    line_count=line_count+1;
    printf("新的铁路线编码是:%d",line_count);
}

void floyed()
{
    int i,j,k,m,t,start_num,end_num;
    int dist_list[SIZE_view][SIZE_view];
    int shortest[SIZE_view][SIZE_view];
    int MAXCOST = INT_MAX;

    view_count=view_count+1;

    for(i=1; i<=view_count; i++)
        for(j=1; j<=view_count; j++)
            dist_list[i][j]=MAXCOST;

    for(t=0; t<=way_count; t++) {
        i=ways[t].station1;
        j=ways[t].station2;
        dist_list[i][j]=ways[t].dist; //将路径长度赋给数组dist_list
    }

    for(i=0; i<=view_count; i++) {
        for(j=0; j<=view_count; j++) {
            if(i==j) {
                dist_list[i][j]=0;
                continue;
            }

            dist_list[i][j]=-1;
            path_list[i][j].count=0;
            for (k=0; k<way_count; k++) {
                if(ways[k].station1==i&&ways[k].station2==j) {
                    dist_list[i][j]=ways[k].dist;
                    path_list[i][j].count=2;
                    path_list[i][j].path[0]=i;
                    path_list[i][j].path[1]=j;  //把文件中的路径起始点和终点分别赋给i和j
                    break;
                }
            }
        }
    }

    for(k=0; k<=view_count-1; k++) {
        for(i=0; i<view_count; i++)
            for(j=0; j<view_count; j++) {
                if(i==k||j==k||i==j)
                    continue;
                if(dist_list[i][k]==-1||dist_list[k][j]==-1)
                    continue;
                if((dist_list[i][j]==-1)||((dist_list[i][j]!=-1)&&(dist_list[i][k] + dist_list[k][j]<dist_list[i][j]))) {//查找i与j之间的顶点k是否能够满足最短路径
                    dist_list[i][j]=dist_list[i][k]+dist_list[k][j];
                    shortest[i][j]=shortest[i][k]+shortest[k][j];
                    path_list[i][j].count=path_list[i][k].count+path_list[k][j].count-1;
                    for(m=0; m<path_list[i][k].count; m++)
                        path_list[i][j].path[m]=path_list[i][k].path[m];
                    for(m=0; m<path_list[k][j].count; m++)
                        path_list[i][j].path[m+path_list[i][k].count]=path_list[k][j].path[m+1];
                }
            }
    }

    printf("Floyde table:\n");
    printf("the informations:\n");
    for(i=0; i<view_count-1; i++)
        printf("   %d:%20s",i+1,views[i].name);
    printf(" Please input the start number:");
    scanf("%10d",&start_num);
    printf(" Please input the end number:");
    scanf("%10d",&end_num);
    printf("From  %s  to   %s",views[start_num-1].name,views[end_num-1].name);
    if(dist_list[start_num][end_num]==-1)
        printf("no way.");
    else {
        printf("distance is %d and path is:",dist_list[start_num][end_num]);
        k=path_list[start_num][end_num].path[0]-1;
        printf("%s",views[k].name);
        for(m=1; m<path_list[start_num][end_num].count; m++) {
            k=path_list[start_num][end_num].path[m]-1;
            printf("->%s",views[k].name);
        }
    }
    printf("end");
}

int main()
{
    int menu;
    readviews();
    readways();
    readlines();

    while(1) {
        printf("\n              The railway system         \n");
        printf("***************************************************\n");
        printf("                        1、添加新的车站\n");
        printf("                        2、添加新的铁路线\n");
        printf("                        3、查找车站信息\n");
        printf("                        4、查找两站间最短路径\n");
        printf("                        5、结束                   \n");
        printf("*****************************************************\n");

        printf("请输入数字（1-5）：");
        scanf("%d",&menu);
        stdin_flush();
        while(menu<1||menu>5) {
            printf("错误！请再次输入：");
            scanf("%d",&menu);
            stdin_flush();
        }
        switch(menu) {
        case 1:
            do {
                addview();
                addway();
            } while(user_says_yes("\n是否继续?(yes/no)"));
            break;
        case 2:
            do {
                addline();
            } while(user_says_yes("\n是否继续?(yes/no)"));
            break;
        case 3:
            do {
                search();
            } while(user_says_yes("\n是否继续?(yes/no)"));
            break;
        case 4:
            do {
                floyed();
            } while(user_says_yes("\n是否继续?(yes/no)"));
            break;
        case 5: {
            printf("thanks!");
            return 0;
        }
        }
    }
    return 0;
}
