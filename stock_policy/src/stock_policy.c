#include "stdio.h"
#include "stdlib.h"

//#include "windows.h"

#define HANDLE_COUNT  10

/* 输入参数 */
float profit_ratio = 30; /* 盈利率 = 盈利金额 / 初始金额 * 100 */
float loss_ratio = 10;   /* 亏损率 = 亏损金额 / 初始金额 * 100 */
float bonus_ratio = 50;  /* 存银行比率 = 存银行金额 / 盈利金额 * 100 */

float origin_money = 1000; /* 初始金额 */
float handle_count = HANDLE_COUNT;   /* 交易次数 */

/* 输出参数 */
struct  _OUTPUT_RESULT
{
	float final_bonus;  /* 最后存入银行的总金额  */
	float final_money;  /* 最后账户上的总金额  */
	float corrent_handle_count; /* 正确的交易次数 */
} output_stack[HANDLE_COUNT];

float final_profit_ratio = 0;   /* 最终的利润率 = (final_bonus + final_money - origin_money) / origin_money * 100 */
float corrent_handle_ratio = 0; /* 正确交易比率 */

/**/
//int current_handle = 0;

void print_result(void)
{
	//printf("final_bonus: %.2f\n", output_stack[HANDLE_COUNT - 1].final_bonus);
	//printf("final_money: %.2f\n", output_stack[HANDLE_COUNT - 1].final_money);
	//printf("corrent_handle_count: %d\n", output_stack[HANDLE_COUNT - 1].corrent_handle_count);

	final_profit_ratio = (output_stack[HANDLE_COUNT - 1].final_bonus + output_stack[HANDLE_COUNT - 1].final_money - origin_money) / origin_money * 100;
	corrent_handle_ratio = output_stack[HANDLE_COUNT - 1].corrent_handle_count / (handle_count - 1) * 100;

	//if ((corrent_handle_ratio < 50) && (final_profit_ratio > 0))
	{
		printf("final_profit_ratio: %.2f%%", final_profit_ratio);
		printf("\tcorrent_handle_ratio: %.2f%%\n", corrent_handle_ratio);
	}
}

void stock_policy(int current_handle, int is_correct)
{
	if (is_correct)
	{
		output_stack[current_handle + 1].corrent_handle_count = output_stack[current_handle].corrent_handle_count + 1;
		output_stack[current_handle + 1].final_bonus = output_stack[current_handle].final_bonus + output_stack[current_handle].final_money * profit_ratio * bonus_ratio / 10000;
		output_stack[current_handle + 1].final_money = output_stack[current_handle].final_money + output_stack[current_handle].final_money * profit_ratio * (100 - bonus_ratio) / 10000;
	}
	else
	{
		output_stack[current_handle + 1].corrent_handle_count = output_stack[current_handle].corrent_handle_count;
		output_stack[current_handle + 1].final_bonus = output_stack[current_handle].final_bonus;
		output_stack[current_handle + 1].final_money = output_stack[current_handle].final_money - output_stack[current_handle].final_money * loss_ratio / 100;
	}

	current_handle += 1;
	if (current_handle >= (handle_count - 1))
	{
		print_result();
		return;
	}

	stock_policy(current_handle, 1);
	stock_policy(current_handle, 0);

	return;
}

void init_result(void)
{
	output_stack[0].final_bonus = 0;  /* 最后存入银行的总金额  */
	output_stack[0].final_money = origin_money;  /* 最后账户上的总金额  */
	output_stack[0].corrent_handle_count = 0; /* 正确的交易次数 */
	final_profit_ratio = 0;   /* 最终的利润率 = (final_bonus + final_money - origin_money) / origin_money * 100 */
	corrent_handle_ratio = 0; /* 正确交易比率 */
}

int main(int argc, void **argv)
{
	init_result();
	stock_policy(0, 1);
	stock_policy(0, 0);
	system("pause");

	return 0;
}