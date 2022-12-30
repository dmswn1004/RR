#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_PROCESS   99
#define MAX_QUEUE   99

typedef int(*comp)(const void*, const void*);
typedef struct process {
	int pid; //���μ�����ȣ
	int ar_t; //�����ð�
	int ser_t; //����ð�
	bool is_finished; //�����Ǵܺ���
	int remain_time; //������ �����ִ� �ð�
	int waiting_time; //���ð�
	int end_time; //����ð�
	int put; //���μ��� ���� �Ǵ� ����(0or1)
} process;

process g_process[MAX_PROCESS];
process* prc_queue[MAX_QUEUE] = { NULL };
int q_head = 0;
int q_tail = -1;
int pr_cnt = -1;

//���μ��� ���� Ȯ�� �Լ�
int is_all_finish(){
	int i;
	for (i = 0; i <= pr_cnt; i++){
		if (g_process[i].remain_time != 0)
			return 0;
	}
	return 1;
}

//���μ����� ť ���� ����
int prc_enqueue(process* prc){
	prc_queue[++q_tail] = prc;
	return 1;
}

//���μ����� ť���� ����
int prc_dequeue(){
	prc_queue[q_head] = NULL;
	q_head++;
	if (q_head > q_tail){
		return 0;
	}
	return 1;
}

//RR �˰���
void RR(int cnt, process s[]) {
	int tm = 0;
	int time_q = 1, time = 0; //q�� = 1
	int max_wait_index = 0; //���� ������ٸ� ���μ��� Index
	int one_q = 1; //colck ���ͷ�Ʈ �߻� ���� �Ǵ� ����
	int temp_time = 0; //�ӽ� �ð� ����
	int exit = 0; //���� ���μ��� ����
	
	while (1) {
		for (int i = 0; i < cnt; i++) {
			//���� �ð� = �����ð� -> ���μ��� �����Ǵ� ���� 1
			if (g_process[i].ar_t == time) g_process[i].put = 1;

			//���μ��� ���� && Ŭ�����ͷ�Ʈ �߻� 
			if (g_process[i].put == 1 && one_q == 1) {

				//ser_t != 0 && ���� ���� ��ٸ� ���μ����� ���ð����� ū ���ð��� ������
				if (g_process[i].ser_t != 0 && g_process[i].waiting_time >= g_process[max_wait_index].waiting_time) {
					//max_wait_index�� �ε��� �� ����
					max_wait_index = i;
				}
			}
		}
		//���� �� ���ð� 0���� �ʱ�ȭ
		g_process[max_wait_index].waiting_time = 0;

		//���� �����Ƿ� ���� �ð� -1
		g_process[max_wait_index].ser_t--;
		//time quantum = 1�� �� ���Ḧ ���� +1
		temp_time++;

		for (int i = 0; i < cnt; i++) {
			//������� ���μ���&& ���� �� ���� ���μ����� ���
			if (i != max_wait_index && g_process[i].put == 1) {
				//���ð� +1
				g_process[i].waiting_time++;
			}
		}
		//���� ���� ��ٸ� ���μ����� ����ð��� 0 && 1�� �̻� ���� �� �ߴ�
		if (g_process[max_wait_index].ser_t != 0 && temp_time != time_q) {
			one_q = 0;
		}
		else {
			//����ð��� 0�� �� ���μ��� ���� ����
			if (g_process[max_wait_index].ser_t == 0) {
				g_process[tm].put = 0;
				//tm = g_process[max_wait_index].pid - 1;
				tm = max_wait_index;
				//���μ��� ���� ���
				printf("%d\t\t %ds\t\t  %ds\t\t  %ds\t\t %ds\t\t %.2fs \n", s[tm].pid, s[tm].ar_t, s[tm].ser_t,
					time + 1, time + 1 - s[tm].ar_t, (time + 1 - s[tm].ar_t) / (double)s[tm].ser_t);
				exit++;
			}
			//q=1�� ���� �ð� �� 0���� �ʱ�ȭ
			temp_time = 0;
			//���ͷ�Ʈ �߻� ���� ���� 1�� ����
			one_q = 1;
		}
		time++;
		if (exit == cnt) break;
	}
}
int main(){
	process copy_sys[MAX_QUEUE] = { 0, };

	int count = 0, i;
	int max_wait_index = 0, one_q = 1, exit = 0;

	memset(g_process, 0, sizeof(g_process));

	FILE* file = fopen("input.txt", "r");
	while (!(feof(file))) {
		fscanf_s(file, "%d,%d,%d", &g_process[count].pid,&g_process[count].ar_t,&g_process[count].ser_t);
		g_process[count].remain_time = g_process[count].ser_t;
		g_process[count].waiting_time = 0;
		if (g_process[count].pid == 0) break;
		count++;
	}
	fclose(file);
	
	printf("=================Main Menu====================\n");
	printf("1. Read index.txt\n2. RR\n0. ����\n");
	printf("==============================================\n");
	while (1) {
		int u;
		printf("�޴��� �������ּ���.>>");
		scanf_s("%d", &u);

		switch (u) {
		case 1:
			printf("====================INDEX======================\n");
			printf("���μ���id\t�����ð�\t���񽺽ð�\n");
			for (i = 0; i < count; i++) {
				copy_sys[i] = g_process[i];
				printf("%d\t\t%d\t\t%d \n", g_process[i].pid, g_process[i].ar_t, g_process[i].ser_t);
			}
			printf("==============================================\n");
			break;
		case 2:
			printf("\n<RR �����ٸ�>\n");
			printf("���μ��� id\t �����ð�\t ���� �ð�\t ���� �ð�\t ��ȯ �ð�\t ����ȭ�� ��ȯ �ð�\n");
			printf("=======================================================================================================\n");
			RR(count, copy_sys);
			printf("=======================================================================================================\n");
			break;
		case 0:
			return 0;
		default: printf("�߸� �Է��߽��ϴ�."); break;
		}
	}
}