/*
待分析的简单语言的语法
用扩充的BNF表示如下：
⑴<程序>：：=begin<语句串>end
⑵<语句串>：：=<语句>{；<语句>}
⑶<语句>：：=<赋值语句>
⑷<赋值语句>：：=ID：=<表达式>
⑸<表达式>：：=<项>{+<项> | -<项>}
⑹<项>：：=<因子>{*<因子> | /<因子>
⑺<因子>：：=ID | NUM | （<表达式>）
*/
 
 
#include "stdio.h"
#include "string.h"
char prog[100],token[8],ch;//prog[100]，用来存储要处理的对象，token用来与关键字比较，ch用来存储一个字符
char *rwtab[6]={"begin","if","then","while","do","end"};//关键字表
int syn,p,m,n,sum;
/*syn是种别码，p为prog数组的指针，m为token数组的指针，n为rwtab数组的指针，sum为词法分析器里的数字数值大小*/
int flag;//flag与判断是否end有关
 
void factor(void);//因式 factor
void expression(void);//表达式 expression
void yucu(void);
void term(void);//项 term
void statement(void);// 语句 statement
void parser(void);
void scaner(void);//扫描器
 
 
int main(void)
{
	p=flag=0;
	printf("\nplease input a string (end with '#'): \n");
 
    /*从命令行读取要处理的对象，并存储在prog[]数组中*/
	do
	{
		scanf("%c",&ch);
		//printf("\n input %c now\n",ch);
		prog[p++]=ch;
	}while(ch!='#');
 
	p=0;
	scaner();//主要完成赋值种别码等词法分析功能
	parser();//调用各种递归子程序，完成语法分析的过程
	//getch();
}
 
/*调用各种递归子程序，完成语法分析的过程*/
void parser(void)
{
	if(syn==1)//begin
	{
		scaner();       /*读下一个单词符号*/
		yucu();         /*调用yucu()函数；*/
 
		if(syn==6)//end
		{
			scaner();
			if((syn==0)&&(flag==0))//出现#且flag=0
			printf("success!\n");
		}
		else
		{
			if(flag!=1) printf("the string haven't got a 'end'!\n");//flag来判断是否end
			flag=1;
		}
	}
	else
	{
		printf("haven't got a 'begin'!\n");
		flag=1;
	}
 
	return;
}
 
void yucu(void)
{
	statement();         /*调用函数statement();*/
 
	while(syn==26)//分号
	{
		scaner();          /*读下一个单词符号*/
		if(syn!=6)
			statement();         /*调用函数statement();*/
	}
 
	return;
}
 
void statement(void)
{
	if(syn==10)
	{
		scaner();        /*读下一个单词符号*/
		if(syn==18)
		{
			scaner();      /*读下一个单词符号*/
			expression();      /*调用函数expression();*/
		}
		else
		{
			printf("the sing ':=' is wrong!\n");
			flag=1;
		}
	}
	else
	{
		printf("wrong sentence!\n");
		flag=1;
	}
 
	return;
}
 
void expression(void)
{
	term();
 
  	while((syn==13)||(syn==14))
    {
    	scaner();             /*读下一个单词符号*/
      	term();               /*调用函数term();*/
    }
 
 	return;
}
 
void term(void)
{
	factor();
 
  	while((syn==15)||(syn==16))
    {
    	scaner();             /*读下一个单词符号*/
      	factor();              /*调用函数factor(); */
    }
 
	return;
}
 
void factor(void)//因式处理函数
{
	if((syn==10)||(syn==11))//标识符，数字
	{
		scaner();
	}
  	else if(syn==27)//开头是左括号（
    {
    	scaner();           /*读下一个单词符号*/
     	expression();        /*调用函数statement();*/
 
		if(syn==28)//出现右括号）
		{
			scaner();          /*读下一个单词符号*/
		}
      	else
	  	{
	  		printf("the error on '('\n");
      		flag=1;
     	}
    }
  	else
	{
		printf("the expression error!\n");
  		flag=1;
    }
 
  	return;
}
 
/*主要完成赋值种别码等词法分析功能*/
void scaner(void)//扫描器，词法分析器内容
{
	sum=0;//数字初始化为0
 
	for(m=0;m<8;m++)//初始化token
		token[m++]=NULL;
 
	m=0;//m为token的指针
	ch=prog[p++];//数组指针+1
 
	while(ch==' ')//遇到空格+1
		ch=prog[p++];
 
	if(((ch<='z')&&(ch>='a'))||((ch<='Z')&&(ch>='A')))//遇到字母
	{
		while(((ch<='z')&&(ch>='a'))||((ch<='Z')&&(ch>='A'))||((ch>='0')&&(ch<='9')))
		{
			token[m++]=ch;
			ch=prog[p++];//p+1,下次循环使用
		}
		p--;//循环跳出，要－1
		syn=10;//10，字母开头
		token[m++]='\0';//\0为字符串结束符
 
		/*判别是否为关键字*/
		for(n=0;n<6;n++)//n为rwtab的指针
		if(strcmp(token,rwtab[n])==0)//strcmp返回值为0，则两个参数大小相同
		{
			syn=n+1;
			break;
		}
	}
 
 
	else if((ch>='0')&&(ch<='9'))//遇到数字
	{
		while((ch>='0')&&(ch<='9'))
		{
			sum=sum*10+ch-'0';
			ch=prog[p++];
		}
		p--;//回溯
		syn=11;//11为数字
	}
 
	/*除数字和字母开头以外的其他符号*/
	else
	switch(ch)
	{
		case '<':
			m=0;
			ch=prog[p++];
			if(ch=='>')
			{
				syn=21;
			}
			else if(ch=='=')
			{
				syn=22;
			}
			else
			{
				syn=20;
				p--;//回溯
			}
		break;
 
		case '>':
			m=0;
			ch=prog[p++];
			if(ch=='=')
			{
				syn=24;
			}
			else
			{
				syn=23;
				p--;
			}
		break;
 
		case ':':
			m=0;
			ch=prog[p++];
			if(ch=='=')
			{
				syn=18;
			}
			else
			{
				syn=17;
				p--;
			}
			break;
 
		case '+':
			syn=13;
		break;
 
		case '-':
			syn=14;
		break;
 
		case '*':
			syn=15;
		break;
 
		case '/':
			syn=16;
		break;
 
		case '(':
			syn=27;
		break;
 
		case ')':
			syn=28;
		break;
 
		case '=':
			syn=25;
		break;
 
		case ';':
			syn=26;
		break;
 
		case '#':
			syn=0;
		break;
 
		default:
			syn=-1;
		break;
	}
}