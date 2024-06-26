#include<LPC17xx.h>
#define RS_CTRL 0x08000000 //P0.27
#define EN_CTRL 0x10000000 //P0.28
#define DT_CTRL 0x07800000 //P0.23 to P0.26 data lines
unsigned char col,row,flag,key;
unsigned long int i,var1,temp,temp1,temp2,temp3;
unsigned char scan_code[16]={0x11,0x21,0x41,0x81,0x12,0x22,0x42,0x82,0x14,0x24,0x44,0x84,0x18};
unsigned char ascii_code[16]={'0','1','2','3','4','5','6','7','8','9','+','-','='};
int idx = 1, ans = 0, a=0, b=0, count=0;
unsigned char finans[3] = {'0','0','\0'};
char op;
void delay_lcd(unsigned int r1)
{
	unsigned int r;
	for(r=0;r<r1;r++);
	return;
}
void clear_ports(void)
{
	/* Clearing the lines at power on */
	LPC_GPIO0->FIOCLR = DT_CTRL; //Clearing data lines
	LPC_GPIO0->FIOCLR = RS_CTRL; //Clearing RS line
	LPC_GPIO0->FIOCLR = EN_CTRL; //Clearing Enable line
	return;
}
void wr_cn(void) //write command reg
{
	clear_ports();
	LPC_GPIO0->FIOPIN = temp2; // Assign the value to the data
	//lines
	LPC_GPIO0->FIOCLR = RS_CTRL; // clear bit RS
	LPC_GPIO0->FIOSET = EN_CTRL; // EN=1
	delay_lcd(25);
	LPC_GPIO0->FIOCLR = EN_CTRL; // EN =0
	return;
}
void lcd_com(void)
{
	temp2 = temp1 & 0xf0;//move data (26-8+1) times : 26 - HN
	//place, 4 - Bits
	temp2 = temp2 << 19; //data lines from 23 to 26
	wr_cn();
	temp2 = temp1 & 0x0f; //26-4+1
	temp2 = temp2 << 23;
	wr_cn();
	delay_lcd(1000);
	return;
}
void clr_disp(void)
{
	temp1 = 0x01;
	lcd_com();
	delay_lcd(10000);
	return;
}
 
void wr_dn(void)
{
	clear_ports();
	LPC_GPIO0->FIOPIN = temp2; // Assign the value to the
	//data lines
	LPC_GPIO0->FIOSET = RS_CTRL; // set bit RS
	LPC_GPIO0->FIOSET = EN_CTRL; // EN=1
	delay_lcd(25);
	LPC_GPIO0->FIOCLR = EN_CTRL; // EN =0
	return;
}
void lcd_data(void)
{
	temp2 = temp1 & 0xf0;
	temp2 = temp2 << 19;
	wr_dn();
	temp2= temp1 & 0x0f;
	temp2= temp2 << 23;
	wr_dn();
	delay_lcd(1000);
	return;
}
void lcd_puts(unsigned char *buf1)
{
	unsigned int i=0;
	while(buf1[i]!='\0')
	{
		temp1 = buf1[i];
		lcd_data();
		i++;
		if(i==16)
		{
			temp1 = 0xc0;
			lcd_com();
		}
	}
	return;
}
void lcd_init()
{
	/* Ports initialized as GPIO */
	LPC_PINCON->PINSEL1 &= 0xFC003FFF; //P0.23 to P0.28
	/* Setting the directions as output */
	LPC_GPIO0->FIODIR |= DT_CTRL;
	LPC_GPIO0->FIODIR |= RS_CTRL;
	LPC_GPIO0->FIODIR |= EN_CTRL;
	clear_ports();
	delay_lcd(3200);
	temp2 = (0x30<<19);
	wr_cn();
	delay_lcd(30000);
	temp2 = (0x30<<19);
	wr_cn();
	delay_lcd(30000);
	temp2 = (0x30<<19);
	wr_cn();
	delay_lcd(30000);
	temp2 = (0x20<<19);
	wr_cn();
	delay_lcd(30000);
	temp1 = 0x28;
	lcd_com();
	delay_lcd(30000);
	temp1 = 0x0c;
	lcd_com();
	delay_lcd(800);
	temp1 = 0x06;
	lcd_com();
	delay_lcd(800);
	temp1 = 0x01;
	lcd_com();
	delay_lcd(10000);
	temp1 = 0x80;
	lcd_com();
	delay_lcd(800);
	return;
}
 
void scan(void)
{
	temp3 = LPC_GPIO0->FIOPIN;
	temp3 &= 0x07800000;
	if(temp3!=0)
	{
		flag = 1;
		if(temp3==1<<23)
			col=0;
		else if(temp3==1<<23)
			col=0;
	}
}
 
 
int main(void)
{ 
	SystemInit();
	SystemCoreClockUpdate();
	LPC_PINCON->PINSEL4 = 0X0;
	LPC_GPIO2->FIODIR = 0x00003C00; //CNB
	LPC_PINCON->PINSEL0&=0X3FFFFFFF;  
	LPC_PINCON->PINSEL1&=0XFFFFFFC0;
	LPC_GPIO0->FIODIR &= 0xFFF87FFF; //CNC
	lcd_init();
	temp1 = 0x80;
	lcd_com();
	delay_lcd(800);
	while(count<3)
	{
		while(1)
		{
			for(row=1; row<5; row++)
			{
				if(row==1)
					var1 = 0x400;
				else if(row==2)
					var1 = 0x800;
				else if(row==3)
					var1 = 0x1000;
				else if(row==4)
					var1 = 0x2000;
				temp = var1;
				LPC_GPIO2->FIOCLR = 0x3c00;
				LPC_GPIO2->FIOSET = var1;
				flag = 0;
				scan();
				if(flag==1)
				{
					count++;
					break;
				}
			}
			if(flag==1)
				break;
		}
		for(i=0; i<13; i++)
		{
			if(key==scan_code[i])
			{
				key = ascii_code[i];
				lcd_puts(&key);
				delay_lcd(50000);
				if(count==1)
					a = key-48;
				else if(count==2)
					op = key;
				else if(count==3)
					b = key-48;
				break;
			}
		}
	}
	temp1 = 0xc0;
	lcd_com();
	delay_lcd(800);
	switch(op)
	{
		case '+':
			ans = a+b;
			break;
		case '-':
			ans = a-b;
			break;
	}
	while(ans!=0)
	{
		finans[idx--] = (ans%10)+48;
		ans = ans/10;
	}
	lcd_puts(&finans[0]);
	return 0;
}