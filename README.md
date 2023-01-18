# RR (Round Robin Scheduling)

## 개요
사용자로부터 스케줄링 할 프로세스의 정보를 input.txt 파일로 입력 받아, Round-Robin 알고리즘에 따른 프로세스의 스케줄링 결과를 프로세스 ID, 도착 시간, 서비스 시간, 종료 시간, 반환 시간, 정규화 된 반환 시간으로 출력하기 위함입니다.

## Round-Robin
개념: 선점형 스케줄링 알고리즘으로, 프로세스들 사이에 우선순위를 두지 않고, 순서대로 시간단위로 CPU를 할당하는 방식입니다. 보통 시간 단위는 10ms ~ 100ms 정도이고 시간 단위동안 수행한 프로세스는 준비 큐의 끝으로 밀려나게 됩니다. 문맥 전환의 오버헤드가 큰 반면, 응답시간이 짧아지는 장점이 있어 실시간 시스템에 유리하고, 할당되는 시간이 클 경우 비선점 FIFO기법과 같아지게 됩니다.


## 코드 설명
① 각각의 프로세스에 대한 정보를 구조체 프로세스에 저장
![image](https://user-images.githubusercontent.com/101851472/210195528-317aeac6-3a7d-4c25-b3c5-d08442c1adf3.png)

② 각각의 프로세스가 큐에 삽입된 후 종료되어 제거하는 알고리즘
![image](https://user-images.githubusercontent.com/101851472/210195584-6791f3dc-0152-4f81-a75a-25519a6e1324.png)

③ 가장 오래 대기하고 있던 프로세스의 값 저장
ⅰ) 현재 시간과 도착 시간이 같을 때, g_process[i].put(프로세스 실행 판단)변수에 1값을 저장
ⅱ) 프로세스가 생성되고 클럭 인터럽트가 발생할 때, 아래 if문 실행
ⅲ) 수행 시간이 0이 아닌, 가장 오래 기다린 프로세스의 대기 시간보다 큰 대기 시간을 갖게 될 때,
  max_wait_index의 인덱스 값을 저장
![image](https://user-images.githubusercontent.com/101851472/210195627-18aee4f6-b65d-4a38-8fea-705ec01a67e6.png)

④ 실행 후, 대기 시간을 0으로 초기화

![image](https://user-images.githubusercontent.com/101851472/210195669-43083308-19bc-4059-a14d-fec807ac55f6.png)

⑤ 프로세스가 수행된 후 시간 할당량은 1이므로, 수행 시간에 -1값을 주어 할당된 시간이 지난 후의 종료를 위해 +1함
![image](https://user-images.githubusercontent.com/101851472/210195687-e6763f73-bed1-45c4-bda7-89500cb07476.png)

⑥ 프로세스의 상태가 대기 중이거나 실행되지 않았을 때 대기 시간을 +1함
![image](https://user-images.githubusercontent.com/101851472/210196035-cd2924bc-1843-446c-91a9-c7f79fbd027e.png)

⑦ 프로세스가 수행이 완료되어 종료되는 알고리즘
![image](https://user-images.githubusercontent.com/101851472/210195884-08f815d2-65ed-44e0-a086-4e5afdebad09.png)

⑧ input.txt 파일을 읽어 각각의 숫자에 의미를 부여한 뒤, 파일의 마지막 줄이 0일 때 반복문이 끝나므로 파일을 닫음
![image](https://user-images.githubusercontent.com/101851472/210195899-b99be729-d56d-4ba9-99a2-bf4145f0bf3d.png)

⑨ 결과값 출력
![image](https://user-images.githubusercontent.com/101851472/210195906-9aa65a56-cd3a-4307-b5b3-969eab75fc96.png)

## 실행 결과
![image](https://user-images.githubusercontent.com/101851472/210194691-2767ce18-3386-4f1f-b56f-c647837d76ab.png)
