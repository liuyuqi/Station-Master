#include <stdio.h>
#include <time.h>
#include <stdlib.h>

typedef struct {
    char class; /*available: E,L,K,T,Z,D,C,G,X*/
    int train_no;
    char from[20];
    char dest[20];
    int action;
    int time1_hour;
    int time1_min;
    int time2_hour;
    int time2_min;
    int track;
    int platform;
    int room;
} trains;
trains traindata[183];
int lastread;
typedef struct { 
    int serial; 
    int code;
	} issue;
issue issuequeue[10];
int head, tail; /*事件队列的头指针和尾指针*/
int todo; /*待处理事件个数*/
int flag_exist, return_value;
typedef struct {
    int tm_sec;
    int tm_min;
    int tm_hour;
    int tm_mday;
    int tm_mon;
    int tm_year;
    int tm_wday;
    int tm_yday;
    int tm_isdst;
} tm;  
tm* tblock;
void readtime()
{
    time_t timer;
    timer = time(NULL);
    tblock = localtime( &timer );
    /*printf("现在时间是%02d:%02d:%02d\n", tblock->tm_hour, tblock->tm_min, tblock->tm_sec);*/
}

int readdata()
{
    FILE* fp;
    char c, from_station[20], to_station[20], maintime[6];
    int trainno = 0, i, serial, num[4] = {0,0,0,0}, totalnum = 0, SUCCESS = -1, train_action, train_platform, train_track; 

    printf("\n正在读取数据文件data...");
    if ( (fp = fopen("data","r")) == NULL)  {
    	printf("Open file \"data\" failed! Program will now exit!\n");
   	return -1;
    }
    serial = 0;/*循环前的初始化*/
    
    for (serial = 0; serial < 183; serial++) {
	if ( (c = fgetc(fp)) != EOF)
	{
/*数据结束符：@*/
	    if (c == '@') break;
	    traindata[serial].class = c;
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
	traindata[serial].train_no = trainno;

	if( (c = fgetc(fp)) != '"')
	    {	printf("Syntax Error! Program will now exit!\n");return (-1);}
/*显示出发车站站名*/
	i = 0; SUCCESS = -1;
	while ( (c = fgetc(fp)) != '"' )  {
	    from_station[i] = c;
	    i++;
	}
	from_station[i] = '\0';
	strcpy ( traindata[serial].from , from_station );
	 
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
	strcpy ( traindata[serial].dest , to_station );
/*判断列车动作，0-始发，1-终到，2-经停，3-通过*/
	if( (c = fgetc(fp)) != ',')
	    {	printf("Syntax Error! Program will now exit!\n");return (-1);}
	train_action = fgetc(fp) - 48;
	traindata[serial].action = train_action;
/*读取时间1*/
	i = 0;
	if( (c = fgetc(fp)) != ',')
	    {	printf("Syntax Error! Program will now exit!\n");return (-1);}
	while( (c = fgetc(fp)) != ',') {
	    maintime[i] = c;
	    i++;
	}
	maintime[i] = '\0';
	SUCCESS = -1;
	SUCCESS = convert_time(maintime, serial, 1);
	if(SUCCESS == -1 )  return -1;
/*判断是否还需要读取第二个时间*/
	i = 0;
	if(train_action == 2) {
	    while( (c = fgetc(fp)) != ',') {
	        maintime[i] = c;
	        i++;
	    }
	    maintime[i] = '\0';
	    SUCCESS = 0;
	    SUCCESS = convert_time(maintime, serial, 2);
	    if(SUCCESS == -1 )  return -1;	
	}
/*股道*/
	num[1] = num[2] = -1;
	if ( (c = fgetc(fp)) != ',')  num[1] = c - 48;
	if ( ((c = fgetc(fp)) != ',') && (c != ';') )  num[2] = c - 48;
	if (num[2] == -1) train_track = num[1];
	else   train_track = num[1]*10 + num[2];
	traindata[serial].track = train_track;

	switch (traindata[serial].track) {
	    case 1: traindata[serial].platform = 1; break;
	    case 2:case 3:  traindata[serial].platform = 2; break;
	    case 7:case 8:  traindata[serial].platform = 3; break;
	    case 10:case 11: traindata[serial].platform = 4; i = -2; break;
	    default: traindata[serial].platform = -1; break;
	}	
/*候车室*/
	if(i == -2) {
	    if( ((c = fgetc(fp)) != ',') && (c != ';'))
	    {	printf("Syntax Error! Program will now exit!\n");return (-1);}
	}
	if( ((traindata[serial].action == 0) || (traindata[serial].action == 2)) && c == ';') {
	    printf("语法错误：没有候车室！");
	    return 1;
	}
	else if( ((train_action == 0) || (train_action == 2)) && c != ';') {
	    if( (c = fgetc(fp)) != ';' )  traindata[serial].room = c - 48;
	}
	if( ((traindata[serial].action == 1) || (traindata[serial].action == 3)) && c == ';') {
	    if( (c = fgetc(fp)) != '\n' ) {
	        printf("行末没有换行符！\n");
	        return 1;
	    }
	    else continue;
        }
	while( (c = fgetc(fp)) != '\n' );
    }
    printf(" 完成！\n");
    fclose(fp);
}

int deltatime(trains* ptrain, int time_seq)
{
    int hour, hour_now, min, min_now, deltamin, deltahour, deltatotal;
    if (time_seq == 1) {
	hour = ptrain -> time1_hour;
	min = ptrain -> time1_min;
     }
    else if (time_seq == 2) {
	hour = ptrain -> time2_hour;
	min = ptrain -> time2_min;
    }
    hour_now = tblock -> tm_hour;
    min_now = tblock -> tm_min;

    deltamin = min_now - min;
    if(hour == 0 && hour_now == 23)
	deltahour = hour_now + 24 - hour;
    else deltahour = hour_now - hour;
	
    deltatotal = deltahour * 60 + deltamin;
    return deltatotal;
}

int printclass(char temp_class)
{
    switch (temp_class)  {
	case 'E': break;
	case 'L': printf("L");break;
	case 'K': printf("K");break;
	case 'T': printf("T");break;
	case 'Z': printf("Z");break;
	case 'D': printf("D");break;
	case 'C': printf("C");break;
	case 'G': printf("G");break;
	case 'X': printf("X");break;
	default: printf("Syntax Error! Program will now exit!\n");return 0;
        }
    return 1;   
}

int inqueue( int in_serial, int in_code )
{
    int queuei;
    if ((head == tail) && (todo == 10)) {
        printf("队列已满！\n");
	    return -1;
    }
    flag_exist = 0;
    if(head <= tail) {
        for (queuei = tail; queuei < head; queuei++ ) {
	    if (queuei = 10) queuei = 0;
	    if ((issuequeue[queuei].serial == in_serial) && (issuequeue[queuei].code == in_code)) {
                flag_exist = 1;
	        return 1; /*队列里已经有这个事件了，不应重复加入*/
	    }
        }
    }
    else if(head > tail) {
        for (queuei = tail; ; queuei++ ) {
	    if (queuei = 10)  break;
	    if ((issuequeue[queuei].serial == in_serial) && (issuequeue[queuei].code == in_code)) {
                flag_exist = 1;
	        return 1; /*队列里已经有这个事件了，不应重复加入*/
	    }
	}
	for (queuei = 0; queuei < head; queuei++) {
	    if ((issuequeue[queuei].serial == in_serial) && (issuequeue[queuei].code == in_code)) {
                flag_exist = 1;
	        return 1; /*队列里已经有这个事件了，不应重复加入*/
	    }
	}
    }
    if (flag_exist == 0) {
	issuequeue[tail].serial = in_serial;
	issuequeue[tail].code = in_code;
	tail++; todo++;
	if (tail == 10) tail = 0;
	return 0; /*队列里没有这个事件，入队成功*/
    }
}

void scan()
{
    int ii, queue_stat = -2, dttime1, dttime2;
    lastread = tblock->tm_min;

    for(ii = 0; ii < 183; ii++)  {
	queue_stat = -2;
	dttime1 = deltatime(&traindata[ii], 1);
	dttime2 = deltatime(&traindata[ii], 2);

	if (traindata[ii].action == 0)  {
	    if ( dttime1 == -20 ) 
		queue_stat = inqueue(ii, 1);
	    if ( dttime1 == -15 ) 
		queue_stat = inqueue(ii, 2);
	    if ( dttime1 == -10 ) 
		queue_stat = inqueue(ii, 3);
	    if ( dttime1 == -5 ) 
		queue_stat = inqueue(ii, 4);
	    if ( dttime1 == -2 ) 
		queue_stat = inqueue(ii, 5);
	}
	else if (traindata[ii].action == 1) {
	    if ( dttime1 == -15 ) 
		queue_stat = inqueue(ii, 10);
	    if ( dttime1 == -2 ) 
		queue_stat = inqueue(ii, 11);
	    if ( dttime1 == 0 ) 
		queue_stat = inqueue(ii, 12);
	}
        else if (traindata[ii].action == 2) {
	    if ( dttime1 == (-15) ) 
		queue_stat = inqueue(ii, 10);
	    if ( dttime2 == (-13) ) 
		queue_stat = inqueue(ii, 1);
	    if ( dttime2 == (-10) )
		queue_stat = inqueue(ii, 3);
	    if ( dttime2 == (-5) ) 
		queue_stat = inqueue(ii, 4);
	    if ( dttime1 == (-2) ) 
		queue_stat = inqueue(ii, 11);
	    if ( dttime1 == (-1) ) 
		queue_stat = inqueue(ii, 25);
	    if ( dttime1 == 0 ) 
		queue_stat = inqueue(ii, 12);
	    if ( dttime2 == (-2) )
		queue_stat = inqueue(ii, 5);
	}
	else if (traindata[ii].action == 3) 
	    if ( dttime1 == 0 )
		queue_stat = inqueue(ii, 31);
    }
}

int isqueueempty()
{
    if ((head == tail) && todo == 0) return 0; /*队空*/
    else return 1; /*队非空*/
}

int occur()
{
    int times = 0; char temp_class; int temp_code, temp_no, temp_serial, SUCCESS;
    
    while (isqueueempty() == 1) {
	readtime();
	temp_serial = issuequeue[head].serial;
	temp_class = traindata[temp_serial].class;
	temp_no = traindata[temp_serial].train_no;
	temp_code = issuequeue[head].code;

	SUCCESS = 0;
	temp_code = issuequeue[head].code;
	if(temp_code < 10) 
	    SUCCESS = issue0( temp_serial, temp_code );
	else if(temp_code >= 10 && temp_code < 20)
	    SUCCESS = issue1( temp_serial, temp_code );
	else if(temp_code >= 20 && temp_code < 30)
	    SUCCESS = issue2( temp_serial, temp_code );
	else if(temp_code >= 30)
	    SUCCESS = issue3( temp_serial, temp_code );
	    
	if (SUCCESS == 0) {
	    printf("%c%d的事件%d发生失败了！\n", temp_class, temp_no, temp_code);
	    exit (0);
	}
	head++; todo--;
	if(head == 10)  head = 0;
	times++;
	sleep(20);
    }
    return times;
}

int issuecommon(int temp_serial)
{
    int cmsuccess = 1;
    printf("由");
    cmsuccess = station_print(traindata[temp_serial].from);
    printf("开往");
    cmsuccess = station_print(traindata[temp_serial].dest);
    printf("的");
    cmsuccess = printclass(traindata[temp_serial].class);
    printf("%d次列车，", traindata[temp_serial].train_no);
    return cmsuccess;
}

int issue0(int temp_serial, int temp_code)
{
    int SUCCESS = 1;
    printf("* * *(%02d:%02d:%02d,  ", tblock -> tm_hour, tblock -> tm_min, tblock -> tm_sec);
    if( (temp_code == 1) || (temp_code == 2) || (temp_code == 3) || (temp_code == 5) ) {
	printf("全局广播)：");
	SUCCESS = issuecommon(temp_serial);
	if (temp_code == 1)
	    printf("现在开始检票。");
	else if (temp_code != 5) 
	    printf("已经请旅客检票进站了。");
	else if (temp_code == 5) 
	    printf("开车时间到了。站台上还没有上车的旅客，请您抓紧时间上车。旅客们，您就要离开石家庄车站了。下次旅行，再见。\n");
	if (temp_code != 5)
	    printf("请到第%d候车室检票进站，%d站台上车！\n",  traindata[temp_serial].room, traindata[temp_serial].platform);
    }
    else if (temp_code == 4) {
	printf("候车室广播)：");
	printf("工作人员请注意，");
	SUCCESS = printclass(traindata[temp_serial].class);
	printf("%d次列车，", traindata[temp_serial].train_no);
	printf("请停止检票！\n");
    }
    return SUCCESS;
}

int issue1(int temp_serial, int temp_code)
{
    int SUCCESS = 1;
    printf("* * *(%02d:%02d:%02d, ", tblock -> tm_hour, tblock -> tm_min, tblock -> tm_sec);
    if(temp_code == 10) {
	printf("全局广播)：");
	SUCCESS = issuecommon(temp_serial);
	printf("就要到达石家庄车站。列车进%d站台%d道，工作人员请做好接车准备！\n", traindata[temp_serial].platform, traindata[temp_serial].track);
    }
    else if(temp_code == 11) {
	printf("全局广播)：");
	printf("接车组同志，接客组同志，请您做好准备。");
	SUCCESS = issuecommon(temp_serial);
	printf("进%d站台%d道，工作人员请注意接车！\n", traindata[temp_serial].platform, traindata[temp_serial].track);
    }
    else if (temp_code == 12) {
	printf("%d站台广播): ", traindata[temp_serial].platform);
	printf("各位旅客，您好！石家庄车站到了。您乘坐的");
	SUCCESS = printclass(traindata[temp_serial].class);
	printf("%d次列车，现在到达石家庄车站。在石家庄车站下车的旅客，请您不要慌忙。整理好随身携带的行李物品，不要遗忘在列车上。已经下车的旅客，请您走出站口地下通道出站。\n", traindata[temp_serial].train_no);
    }
    return SUCCESS;
}

int issue2(int temp_serial, int temp_code)
{
    int SUCCESS = 1, direction = 0;/*东为1,西为2*/
    if (traindata[temp_serial].platform == 1)  direction = 1;
    else if (traindata[temp_serial].platform == 2) {
	if (traindata[temp_serial].track == 2)  direction = 2;
	else if (traindata[temp_serial].track == 3)  direction = 1;
    }
    else if (traindata[temp_serial].platform == 3) {
	if (traindata[temp_serial].track == 7)  direction = 2;
	else if (traindata[temp_serial].track == 8)  direction = 1;
    }
    else if (traindata[temp_serial].platform == 4) {
	if (traindata[temp_serial].track == 10)  direction = 2;
	else if (traindata[temp_serial].track == 11)  direction = 1;
    }
    printf("* * *(%02d:%02d:%02d, ", tblock -> tm_hour, tblock -> tm_min, tblock -> tm_sec);
    if (temp_code == 25) {
	printf("%d站台广播): 开往", traindata[temp_serial].platform);
	SUCCESS = station_print(traindata[temp_serial].dest);
	printf("方向的");
	SUCCESS = printclass(traindata[temp_serial].class);
	printf("%d次列车就要进站了。站台上等车的旅客，请您站在白色安全线后面来，以免发生危险。站台", traindata[temp_serial].train_no);
	if (direction == 2)  printf("西侧");
	else if (direction == 1) printf("东侧");
	printf("正在进站的是开往");
	SUCCESS = station_print(traindata[temp_serial].dest);
	printf("方向的");
	SUCCESS = printclass(traindata[temp_serial].class);
	printf("%d次列车，请您注意不要上错车！\n", traindata[temp_serial].train_no);
    }
    return SUCCESS;
}

int issue3(int temp_serial, int temp_code)
{
    int SUCCESS = 1;
    printf("* * *(%02d:%02d:%02d, ", tblock -> tm_hour, tblock -> tm_min, tblock -> tm_sec);
    if (temp_code == 31) {
	printf("站台广播): ", traindata[temp_serial].platform);
	printf("工作人员请注意，");
	SUCCESS = issuecommon(temp_serial);
        printf("%d道现在通过。请工作人员注意安全，做好防护！\n", traindata[temp_serial].track);
    }
    return SUCCESS;
}

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

int convert_time(char rawtime[6], int train_serial, int index)
{
    int hh = 0, hl = 0, mh = 0, ml = 0, hour = 0, minute = 0;
    if (rawtime[1] == ':')  {
	hl = rawtime[0] - 48;
	mh = rawtime[2] - 48;
	ml = rawtime[3] - 48;
	hour = hl;
	minute = mh * 10 + ml;
    }
    else if (rawtime[2] == ':') {
	hh = rawtime[0] - 48;
	hl = rawtime[1] - 48;
	mh = rawtime[3] - 48;
	ml = rawtime[4] - 48;
	hour = hh * 10 + hl;
	minute = mh * 10 + ml;
    }
    if (hour > 24 || hour < 0 || minute > 59 || minute < 0) {
	printf("时间输入错误！");
	return -1;
    }
    else {
	if(index == 1) {
	    traindata[train_serial].time1_hour = hour;
	    traindata[train_serial].time1_min = minute;
	}
	else if (index == 2) {
	    traindata[train_serial].time2_hour = hour;
	    traindata[train_serial].time2_min = minute;
	}
    return 0;
    } 	
}

int main (int argc, char *argv[])
{
    return_value = readdata();
    head = tail = 0;
    while(1) {
	readtime();
	if (lastread != tblock->tm_min) {
	    scan();
	    return_value = 0;
	    return_value = occur();
	    printf("(在%02d:%02d共发生了%d个事件！)\n\n", tblock -> tm_hour, tblock -> tm_min, return_value);
	}
	sleep(1);
    }
    return 0;
}

