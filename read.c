#include <stdio.h>


typedef struct {
        char class; /*available: E,L,K,T,Z,D,C,G,X*/
        int train_no;
        char from[20];
        char dest[20];
        char action;
        int arrive_hour;
        int arrive_min;
        int leave_hour;
        int leave_min;
        int track;
        int room;
} trains;

int station_print(char station[20])
{
    if (strcmp (station,"beijingxi") == 0 ) printf("北京西");  
    else if (strcmp (station,"shijiazhuang") == 0 ) printf("石家庄");  
    else if (strcmp (station,"hankou") == 0 ) printf("汉口");  
    else if (strcmp (station,"wuchang") == 0 ) printf("武昌");  
    else if (strcmp (station,"zhengzhou") == 0 ) printf("郑州"); 
    else if (strcmp (station,"xian") == 0 ) printf("西安");  
    else if (strcmp (station,"handan") == 0 ) printf("邯郸");  
    else if (strcmp (station,"haerbin") == 0 ) printf("哈尔滨"); 
    else if (strcmp (station,"tianjin") == 0 ) printf("天津");    
    else if (strcmp (station,"chongqing") == 0 ) printf("重庆");  
    else if (strcmp (station,"anyang") == 0 ) printf("安阳");  
    else if (strcmp (station,"guangzhou") == 0 ) printf("广州");  
    else if (strcmp (station,"guangzhoudong") == 0 ) printf("广州东");  
    else if (strcmp (station,"changsha") == 0 ) printf("长沙"); 
    else if (strcmp (station,"taiyuan") == 0 ) printf("太原");   
    else if (strcmp (station,"lanzhou") == 0 ) printf("兰州");  
    else if (strcmp (station,"nanning") == 0 ) printf("南宁");  
    else if (strcmp (station,"baotou") == 0 ) printf("包头");  
    else if (strcmp (station,"kunming") == 0 ) printf("昆明"); 
    else if (strcmp (station,"qinhuangdao") == 0 ) printf("秦皇岛");  
    else if (strcmp (station,"zhangjiakou") == 0 ) printf("张家口"); 
    else if (strcmp (station,"changchun") == 0 ) printf("长春");  
    else if (strcmp (station,"shenyangbei") == 0 ) printf("沈阳北");  
    else if (strcmp (station,"shanghai") == 0 ) printf("上海");  
    else if (strcmp (station,"xining") == 0 ) printf("西宁");   
    else if (strcmp (station,"guiyang") == 0 ) printf("贵阳");  
    else if (strcmp (station,"wulumuqi") == 0 ) printf("乌鲁木齐");   
    else if (strcmp (station,"chengde") == 0 ) printf("承德"); 
    else if (strcmp (station,"shiyan") == 0 ) printf("十堰");   
    else if (strcmp (station,"nanchang") == 0 ) printf("南昌");
    else if (strcmp (station,"hengyang") == 0 ) printf("衡阳");    
    else if (strcmp (station,"luoyang") == 0 ) printf("洛阳");  
    else if (strcmp (station,"chengdu") == 0 ) printf("成都");  
    else if (strcmp (station,"nanyang") == 0 ) printf("南阳");  
    else if (strcmp (station,"dalian") == 0 ) printf("大连");  
    else if (strcmp (station,"yichang") == 0 ) printf("宜昌");  
    else if (strcmp (station,"panzhihua") == 0 ) printf("攀枝花");  
    else if (strcmp (station,"datong") == 0 ) printf("大同");  
    else if (strcmp (station,"zhoukou") == 0 ) printf("周口"); 
    else if (strcmp (station,"hanzhong") == 0 ) printf("汉中");  
    else if (strcmp (station,"huaihua") == 0 ) printf("怀化");  
    else if (strcmp (station,"zhanjiang") == 0 ) printf("湛江");  
    else if (strcmp (station,"changzhibei") == 0 ) printf("长治北");   
    else if (strcmp (station,"sanya") == 0 ) printf("三亚"); 
    else if (strcmp (station,"guilin") == 0 ) printf("桂林");  
    else if (strcmp (station,"baoji") == 0 ) printf("宝鸡");   
    else if (strcmp (station,"zunyi") == 0 ) printf("遵义");  
    else if (strcmp (station,"lasa") == 0 ) printf("拉萨"); 
    else if (strcmp (station,"yanan") == 0 ) printf("延安");   
    else if (strcmp (station,"shexian") == 0 ) printf("涉县");  
    else if (strcmp (station,"jiulong") == 0 ) printf("九龙");  
    else if (strcmp (station,"gaobeidian") == 0 ) printf("高碑店");  
    else if (strcmp (station,"xinyang") == 0 ) printf("信阳");  
    else  {  printf("无此车站！\n"); return 0;}
    return 1;
}

int convert_time(char rawtime[6])
{
    int hh = 0, hl = 0, mh = 0, ml = 0, hour = 0, minute = 0;
    if (rawtime[1] == ':')  {
	hl = rawtime[0] - 48;
	mh = rawtime[2] - 48;
	ml = rawtime[3] - 48;
	hour = hl;
	minute = mh * 10 + ml;
	if (hour > 24 || hour < 0 || minute > 59 || minute < 0) {
	    printf("时间输入错误！");
	    return -1;
	}
	else {
	    if(minute < 10)
		printf("%d:0%d", hour, minute);
	    else printf("%d:%d", hour,minute);
	    return 0;
	}
    }
    else if (rawtime[2] == ':') {
	hh = rawtime[0] - 48;
	hl = rawtime[1] - 48;
	mh = rawtime[3] - 48;
	ml = rawtime[4] - 48;
	hour = hh * 10 + hl;
	minute = mh * 10 + ml;
	if (hour > 24 || hour < 0 || minute > 59 || minute < 0) {
	    printf("时间输入错误！");
	    return -1;
	}
	else {
	    if(minute < 10)
		printf("%d:0%d", hour, minute);
	    else printf("%d:%d", hour,minute);
	    return 0;
	}
    }

}




int main (int argc, char *argv[])
{
    FILE* fp;
    char c, from_station[20], to_station[20], time[6];
    int trainno = 0, i, num[4] = {0,0,0,0}, totalnum = 0, SUCCESS = -1, train_action, train_platform, train_track;

    if ( (fp = fopen("data","r")) == NULL)  {
    	printf("Open file \"data\" failed! Program will now exit!\n");
   	return -1;
    }
    while(1) {
	printf("\n");
	if ( (c = fgetc(fp)) != EOF)
	{
/*数据结束符：@*/
	    if (c == '@') break;
	    switch (c)  {
		case 'E': printf("普快");break;
		case 'L': printf("临客");break;
		case 'K': printf("快速");break;
		case 'T': printf("特快");break;
		case 'Z': printf("直达特快");break;
		case 'D': printf("动车");break;
		case 'C': printf("城际动车");break;
		case 'G': printf("高速动车");break;
		case 'X': printf("行包");break;
		default: printf("Syntax Error! Program will now exit!\n");return (-1);
	    }
        } 
	else {
	    printf("Syntax Error! Program will now exit!\n");return (-1);
	}
	if( (c = fgetc(fp)) != ',')
	    {	printf("Syntax Error! Program will now exit!\n");return (-1);}
/*车次*/
	totalnum = 0; trainno = 0;
        while ( (c = fgetc(fp)) != ',')	{	
		num[totalnum] = c - 48;
		totalnum ++;
	}
	for (i = 0; totalnum > 0;  totalnum--, i++)
		trainno = trainno * 10 + num[i];

	printf("%d", trainno);

	if( (c = fgetc(fp)) != '"')
	    {	printf("Syntax Error! Program will now exit!\n");return (-1);}
	printf("次列车(");

/*显示出发车站站名*/
	i = 0; SUCCESS = -1;
	while ( (c = fgetc(fp)) != '"' )  {
	    from_station[i] = c;
	    i++;
	}
	from_station[i] = '\0';
	if( (SUCCESS = station_print(from_station)) == 0)
	    return -1;
	 
	printf("->");
/*显示终点站站名*/
	if( (c = fgetc(fp)) != ',')
	    {	printf("Syntax Error! Program will now exit!\n");return (-1);}
	if( (c = fgetc(fp)) != '"')
	    {	printf("Syntax Error! Program will now exit!\n");return (-1);}
	i = 0; SUCCESS = -1;
	while ( (c = fgetc(fp)) != '"' )  {
	    to_station[i] = c;
	    i++;
	}
	to_station[i] = '\0';
	if( (SUCCESS = station_print(to_station)) == 0)
	    return -1;
/*判断列车动作，0-始发，1-终到，2-经停，3-通过*/
	if( (c = fgetc(fp)) != ',')
	    {	printf("Syntax Error! Program will now exit!\n");return (-1);}
	train_action = fgetc(fp) - 48;
/*读取时间1*/
	i = 0;

	if( (c = fgetc(fp)) != ',')
	    {	printf("Syntax Error! Program will now exit!\n");return (-1);}
	while( (c = fgetc(fp)) != ',') {
	    time[i] = c;
	    i++;
	}
	time[i] = '\0';
	SUCCESS = -1;
	if(train_action == 0) {
	    printf(")就要开车了，开车时间是");
	    SUCCESS = convert_time(time);
	    if(SUCCESS == -1 )  return -1;
	}
	else if (train_action == 1) {
	    printf(")就要到站，到站时间是");
	    SUCCESS = convert_time(time);
	    if(SUCCESS == -1 )  return -1;
	}
	else if (train_action == 2) {
	    printf(")就要到站，到站时间是");
	    SUCCESS = convert_time(time);
	    if(SUCCESS == -1 )  return -1;
	}
	else if (train_action == 3) {
	    printf(")即将通过，通过时间是");
	    SUCCESS = convert_time(time);
	    if(SUCCESS == -1 )  return -1;
	}
/*判断是否还需要读取第二个时间*/
	i = 0;
	if(train_action == 2) {
	    while( (c = fgetc(fp)) != ',') {
	        time[i] = c;
	        i++;
	    }
	    time[i] = '\0';
	    printf("，开车时间是");
	    SUCCESS = 0;
	    SUCCESS = convert_time(time);
	    if(SUCCESS == -1 )  return -1;	
	}
/*股道*/
	num[1] = num[2] = -1;
	if ( (c = fgetc(fp)) != ',')  num[1] = c - 48;
	if ( ((c = fgetc(fp)) != ',') && (c != ';') )  num[2] = c - 48;
	if (num[2] == -1) train_track = num[1];
	else   train_track = num[1]*10 + num[2];

	switch (train_track) {
	    case 1: train_platform = 1; break;
	    case 2:case 3:  train_platform = 2; break;
	    case 7:case 8:  train_platform = 3; break;
	    case 10:case 11: train_platform = 4; i = -2; break;
	    default: train_platform = -1; break;
	}	

	if(train_action == 0) 
	    printf("，列车停靠%d站台%d道", train_platform, train_track);
	if(train_action == 1) 
	    printf("，列车进%d站台%d道。", train_platform, train_track);
	if(train_action == 2) 
	    printf("，列车进%d站台%d道", train_platform, train_track);
	if(train_action == 3) 
	    printf("，列车%d道通过。", train_track);
/*候车室*/
	if(i == -2) {
	    if( ((c = fgetc(fp)) != ',') && (c != ';'))
	    {	printf("Syntax Error! Program will now exit!\n");return (-1);}
	}
	if( ((train_action == 0) || (train_action == 2)) && c == ';') {
	    printf("语法错误：没有候车室！");
	    return 1;
	}
	else if( ((train_action == 0) || (train_action == 2)) && c != ';') {
	    if( (c = fgetc(fp)) != ';' )  printf("，请到第%d候车室候车。", c - 48);
	}	
	if( ((train_action == 1) || (train_action == 3)) && c == ';') {
	    if( (c = fgetc(fp)) != '\n' ) {
	        printf("行末没有换行符！\n");
	        return 1;
	    }
	    else continue;
        }
	while( (c = fgetc(fp)) != '\n' );
    }
     
    
    printf("\n数据读取结束！\n");

    fclose(fp);
    return 0;
    
}

