#include <fstream.h>
#include <iostream.h>
#include <math.h>
#include <string.h>
struct point
{
	int x,y;
	point *next;
};
void erase(point *&list,int x,int y);
void enchain(int x,int y,int N,point *&list);
point *winset(int x,int y,int flag,int N,point * array,point *players);
int closer_to_most(int x1,int x2,int y1,int y2,int N,point *list);
double dis(int x1,int x2,int y1,int y2);
void add_point(point *&list,int num1,int num2);
void vanish(point *&list);
void main()
{
	int N=0,i,multi=1;
	char line[256],last_line[256]="temp";
	ifstream F2;
	F2.open("C:\\ideal points.csv");
	point *list=NULL,*array=NULL;
	cout<<"give the axis length please"<<endl;
	cin>>N;
	int x,y,flag=0,avgx=0,avgy=0,p_num=0;
	while (strcmp(last_line,line)!=0)  
	{
		x=0;
		y=0;
		multi=1;
		for (i=0;(i<=5) and (line[i]>='0') and (line[i]<='9');i++);
		int j=i;
		for (i=i-1;i>=0;i--)
		{
			x+=(int(line[i])-int('0'))*multi;
			multi*=10;
		}
		multi=1;
		for (i=j+1;(i<=10) and (line[i]>='0') and (line[i]<='9');i++);
		for (i=i-1;i>j;i--)
		{
			y+=(int(line[i])-int('0'))*multi;
			multi*=10;
		}
		avgx+=x;
		avgy+=y;
		p_num++;
		add_point(list,x,y);
		strcpy(last_line,line);
		F2>>line;	
	}
	F2.close();
	avgx/=p_num;
	avgy/=p_num;
	enchain(avgx,avgy,N,array);
	point *start=array;
	ofstream f1;
	f1.open("C:\\uncovered_set RESULT.CSV");
	while (array)
	{
		point *temp=winset(array->x,array->y,0,N,start,list);
		point *temp2=temp;
		point *temp3=winset(array->x,array->y,1,N,start,list);
		point *temp4=temp3;
		point *temp5=temp3;
		while (temp3)
		{
			while (temp)
			{
				flag=0;
				if (closer_to_most(temp3->x,temp->x,temp3->y,temp->y,N,list))
				{
					flag=1;
					break;
				}
				temp=temp->next;
			}
			temp=temp2;
			if (!flag)
				erase(start,temp3->x,temp3->y);
			temp3=temp3->next;
		}
		vanish(temp2);
		vanish(temp4);
		cout<<endl<<array->x<<" "<<array->y<<endl;
		array=array->next;
	}
	while (start)
	{
		cout<<start->x<<","<<start->y<<" ";
		f1<<start->x<<","<<start->y<<endl;
		start=start->next;
	}
		f1.close();
}
int closer_to_most(int x1,int x2,int y1,int y2,int N,point *list)
{
	int sum1=0,sum2=0;
	while (list)
	{
		if (dis(x1,list->x,y1,list->y)<dis(x2,list->x,y2,list->y))
			sum1++;
		else
			sum2++;
		list=list->next;
	}
	if (sum1>=sum2)
		return 1;
	return 0;
}
inline double dis(int x1,int x2,int y1,int y2)
{
	return sqrt(((x1-x2)*(x1-x2))+((y1-y2)*(y1-y2)));
}
void add_point(point *&list,int num1,int num2)
{
	if (!list)
	{
		list=new point;
		list->x=num1;
		list->y=num2;
		list->next=NULL;
	}
	else
	{
		point *temp=list;
		list=new point;
		list->next=temp;
		list->x=num1;
		list->y=num2;
	}
}
point *winset(int x,int y,int flag,int N,point *array,point *players)
{
	point *list=NULL;
	if (flag==0)
	{
		for (int x1=0;x1<N;x1++)
			for (int y1=0;y1<N;y1++)
				if ((!closer_to_most(x,x1,y,y1,N,players)) && !((x1==x) && (y1==y)))
					add_point(list,x1,y1);
	}
	else
	{
		while (array)
		{
			if ((closer_to_most(x,array->x,y,array->y,N,players)) && !((array->x==x) && (array->y==y)))
				add_point(list,array->x,array->y);	
			array=array->next;
		}
	}
	return list;
}
void vanish(point *&list)
{
	point *temp=list;
	while (list)
	{
		list=list->next;
		delete temp;
		temp=list;
	}
	list=NULL;
}
void enchain(int x,int y,int N,point *&list)
{
	add_point(list,x,y);
	point *temp2=list;
	for(int dis=1;((N-x-dis>0) || (N-y-dis>0) || (x>dis) || (y>dis));dis++)
	{
		int temp;
		for (temp=0-dis;temp<=dis;temp++)
			if ((x-dis>=0) && (y+temp>=0) && (y+temp<N)) 
			{
				add_point(temp2->next,x-dis,y+temp);
				temp2=temp2->next;
			}
		for (temp=0-dis+1;temp<dis;temp++)
		{
			if ((x+temp>=0) && (x+temp<N) && (y-dis>=0)) 
			{
				add_point(temp2->next,x+temp,y-dis);
				temp2=temp2->next;
			}
			if ((x+temp>=0) && (x+temp<N) && (y+dis<N)) 
			{
				add_point(temp2->next,x+temp,y+dis);
				temp2=temp2->next;
			}
		}
		for (temp=0-dis;temp<=dis;temp++)
			if ((x+dis<N) && (y+temp>=0) && (y+temp<N)) 
			{
				add_point(temp2->next,x+dis,y+temp);
				temp2=temp2->next;
			}
	}
}
void erase(point *&list,int x,int y)
{
	point *temp=list;
	if ((list->x==x) &&  (list->y==y))
	{
		list=list->next;
		delete temp;
		return;
	}
	while (temp->next)
	{
		if ((temp->next->x==x) &&  (temp->next->y==y))
		{
			point *temp2=temp->next;
			temp->next=temp->next->next;
			delete temp2;
			return;
		}
		temp=temp->next;
	}
}


