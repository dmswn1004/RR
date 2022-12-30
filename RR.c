#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_PROCESS   99
#define MAX_QUEUE   99

typedef int(*comp)(const void*, const void*);
typedef struct process {
	int pid; //프로세스번호
	int ar_t; //도착시간
	int ser_t; //수행시간
	bool is_finished; //종료판단변수
	int remain_time; //수행후 남아있는 시간
	int waiting_time; //대기시간
	int end_time; //종료시간
	int put; //프로세스 실행 판단 변수(0or1)
} process;

process g_process[MAX_PROCESS];
process* prc_queue[MAX_QUEUE] = { NULL };
int q_head = 0;
int q_tail = -1;
int pr_cnt = -1;

//프로세스 종료 확인 함수
int is_all_finish(){
	int i;
	for (i = 0; i <= pr_cnt; i++){
		if (g_process[i].remain_time != 0)
			return 0;
	}
	return 1;
}

//프로세스를 큐 끝에 삽입
int prc_enqueue(process* prc){
	prc_queue[++q_tail] = prc;
	return 1;
}

//프로세스를 큐에서 제거
int prc_dequeue(){
	prc_queue[q_head] = NULL;
	q_head++;
	if (q_head > q_tail){
		return 0;
	}
	return 1;
}

//RR 알고리즘
void RR(int cnt, process s[]) {
	int tm = 0;
	int time_q = 1, time = 0; //q값 = 1
	int max_wait_index = 0; //가장 오래기다린 프로세스 Index
	int one_q = 1; //colck 인터럽트 발생 여부 판단 변수
	int temp_time = 0; //임시 시간 변수
	int exit = 0; //종료 프로세스 개수
	
	while (1) {
		for (int i = 0; i < cnt; i++) {
			//현재 시간 = 도착시간 -> 프로세스 실행판단 변수 1
			if (g_process[i].ar_t == time) g_process[i].put = 1;

			//프로세스 생성 && 클럭인터럽트 발생 
			if (g_process[i].put == 1 && one_q == 1) {

				//ser_t != 0 && 가장 오래 기다린 프로세스의 대기시간보다 큰 대기시간을 가질때
				if (g_process[i].ser_t != 0 && g_process[i].waiting_time >= g_process[max_wait_index].waiting_time) {
					//max_wait_index의 인덱스 값 저장
					max_wait_index = i;
				}
			}
		}
		//실행 후 대기시간 0으로 초기화
		g_process[max_wait_index].waiting_time = 0;

		//실행 했으므로 수행 시간 -1
		g_process[max_wait_index].ser_t--;
		//time quantum = 1일 때 종료를 위해 +1
		temp_time++;

		for (int i = 0; i < cnt; i++) {
			//대기중인 프로세스&& 현재 비 실행 프로세스일 경우
			if (i != max_wait_index && g_process[i].put == 1) {
				//대기시간 +1
				g_process[i].waiting_time++;
			}
		}
		//가장 오래 기다린 프로세스의 수행시간이 0 && 1초 이상 실행 시 중단
		if (g_process[max_wait_index].ser_t != 0 && temp_time != time_q) {
			one_q = 0;
		}
		else {
			//수행시간이 0일 때 프로세스 실행 종료
			if (g_process[max_wait_index].ser_t == 0) {
				g_process[tm].put = 0;
				//tm = g_process[max_wait_index].pid - 1;
				tm = max_wait_index;
				//프로세스 정보 출력
				printf("%d\t\t %ds\t\t  %ds\t\t  %ds\t\t %ds\t\t %.2fs \n", s[tm].pid, s[tm].ar_t, s[tm].ser_t,
					time + 1, time + 1 - s[tm].ar_t, (time + 1 - s[tm].ar_t) / (double)s[tm].ser_t);
				exit++;
			}
			//q=1와 비교할 시간 값 0으로 초기화
			temp_time = 0;
			//인터럽트 발생 여부 변수 1로 변경
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
	printf("1. Read index.txt\n2. RR\n0. 종료\n");
	printf("==============================================\n");
	while (1) {
		int u;
		printf("메뉴를 선택해주세요.>>");
		scanf_s("%d", &u); 

		switch (u) {
		case 1:
			printf("====================INDEX======================\n");
			printf("프로세스id\t도착시간\t서비스시간\n");
			for (i = 0; i < count; i++) {
				copy_sys[i] = g_process[i];
				printf("%d\t\t%d\t\t%d \n", g_process[i].pid, g_process[i].ar_t, g_process[i].ser_t);
			}
			printf("==============================================\n");
			break;
		case 2:
			printf("\n<RR 스케줄링>\n");
			printf("프로세스 id\t 도착시간\t 서비스 시간\t 종료 시간\t 반환 시간\t 정규화된 반환 시간\n");
			printf("=======================================================================================================\n");
			RR(count, copy_sys);
			printf("=======================================================================================================\n");
			break;
		case 0:
			return 0;
		default: printf("잘못 입력했습니다."); break;
		}
	}
}
