# 제목1
## 제목2
### 제목3
#### 제목4
##### 제목5

제목 3
---
기타
---
터미널 탭벼환
alt 1, 2 :

git config --global credential.helper cache
토큰 재 입력 필요 없게 하는거



찾아볼것
---
vim종류중 어떤거 설치할지 고민
리눅스 녹화 기능
캡쳐방법
레지스터 메모리 연산??
캡쳐한 내용 찾아보기
git 활용법 다시 확인


gdb

---
gcc -g -O0 function_asm.c -o fasm : -g 주고 컴파일
gdb fasm
shell clear :  gdb 창 초기화
q : gdb 종료
b main : breakpoint
r:run
disas
b *0x000055555555465d push 쪽에다가 break를 가리킨다.

rbp            0x5555555546b0	0x5555555546b0 <__libc_csu_init>
rsp            0x7fffffffdd78	0x7fffffffdd78

l : 현재 진행하고 있는 코드 리스팅
실제 주소가 안나오는 경우가 있음

l mult : mult 함수 가리킴

s:step into (c언어 레벨 벼로)

변수  특정 데이터 타입을 저장하는 메모리 공간

si : step into ~ 한칸 이동 (인스트럭션 레벨 별로)

push : stack 에 밀어넣음

rbp: rbp레지스터에 들어있는 값

info registers : 내 PC의 레지스터 정보

rax,rbx, bl, al, eax ... 인텔 계열

al , bl : a, b 로 시작하는 인텔 계열 레지스터중 하위 바이트 (8byte)

csic vs risc : 다시 보기 메모리 투 메모리는 인텔만 되고 arm은 메모리 투 레지스터 갔다가 레지스터에서 메모리에 다시 저장하는 방식....

ax  : 함수의 리턴값이 저장되는 레지스터(ax 계열들) r은 비트 수니까 상관x
bp : base pointer 출발점 기준점 역할

sp : stack pointer 스텍 최상위 나타냄 , 스택은 아래로 쌓음(heap은 위로 쌓음)
ip : instruction pointer 다음에 실행할 프로그램의 주소값 포인팅
eflags : 인터럽트 등의 flag 처리 제어 마스킹 등의 역할

p/x $rsp : p 출력 /x 16진수로  뿌려라 $레지스터 값을 보겠다.
ps -ef : 현재 프로그램에 돌고 있는 모든 프로세스 들
x $rsp :  메모리 뿌려서 볼 수 있음

펌웨어로 영상 못하는 이유는 메모리 순회 검색을 못하기 때문이다.

mov : 복사 명령과 같다 c언어 num =3 의 등호 = 같은 역할$ 
movl : 4byte 데이터 이용시킬 때 movl

재귀함수는 동작하는 코드만 동일할뿐 새로 push pop 하는 것
pop: 현재 stac의 최상의 의 값이 저장됨
retq : pop rip랑 같은 의미

l : 4바이트
q: 8바이트

n : step out 
ni : step out (instruction 단위)
(gdb) x/c $rsp : 문자열로 register rsp의 메모리 확인 askii value같은 것을 확인
imul : int mul 


질문

---
rax, rbp, rsp 는 CPU 레지스터이고 eax esi 등은 무엇? 
=> 같은 CPU 안의 레지스터인데 비트수에 따라 rax 가 64bit면 그 중에 32bit만 사용할 때 eax로 말하는것인가?
	info registers 보면 eax에 넣은값이 rax에 그대로 들어와 있는것을 확인할 수 있음
	왜 r로 안표현하고 e를 사용해서 했을까 굳이 크기가 r만큼이 아니어서?
가상메모리 개념 : 1기 github 참고
sub $0x10, %rsp : 0x10이라는 값은 어떻게 판단한걸까?
stack에 들어가는 게 뭐뭐 있는지 찾아보기 (지역변수, 등등)
esi, edi도 범용 레지스터 인가? 무슨 역할이지?
=> 일반저ㅏㄱ인 연상세너는 역할이 없다.
call 진행시 스택에 복귀 주소 저장하는데 이놈은 리틀엔디안 방식이던데 왜?
res 변수 메모리공간이 나중에 값을 입력할 때야 넣어준다.
l 명령의 정확한 사용법?
지금 명령어 가 C의 어디위치인지 보는 방법? l로 보는 것인가?
컴파일 최적화 시 어떻게 달라지는지도 확인필요
rdi 랑 edi 랑 다른 레지스터가 되는 건가? x로 볼 때 하나는 메모리로 인식해서 되고 하나는 안되는데? LEA할 때
=> rdi가 되고 edi가 안될것 같은데 rdi에 메모리 8byte로 구성되기 때문에 edi는 메모리 주소가 아니게 된다.
lea로 문자열을 임의의  가상 메모리 공간에 할당 받는 것 같다.

jumpq로 하면 4byte 어긋나는 것 같은데???

준비물

---
줄없는 공책

