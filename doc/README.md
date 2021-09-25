# Prepare

```make
1) 카페의 USB Installer로 시동디스크 작성후 Ubuntu 20.04 설치
```

# 초기 설정

```make
1) 터미널을 연다.
2) sudo apt-get update
3) sudo apt-get install vim
4) sudo apt-get install git
5) pwd(현재 위치 확인 명령)
6) mkdir(디렉토리 생성 명령)
   mkdir proj
7) cd(디렉토리 이동 명령)
   cd proj
8) git clone (우리의 저장소 URL) - github에서 초록색 code부분
9) cd EmbeddedMasterLv2
10) mkdir 자신의 이니셜
11) cd 자신의 이니셜
12) touch prepare.txt
13) git status(현재 깃 상태 정보를 볼 수 있음)
14) git add ./(현재 위치의 내용 저장)
15) git push origin main
16) 여기서 만약 보안 문제로 토큰을 발급하라고 나온다면
    깃 계정에서 토큰 발급하여 비밀번호 대신 토큰을 넣습니다.
17) 우측 상단 계정 프로필 메뉴바 버튼 클릭 -> Settings ->
    Developer settings -> Personal access tokens ->
    우측 상단 근처의 Generate new token 클릭 ->
    Note에는 적당한 이름, 만료일, repo, admin:repo_hook
18) git push origin 이후 계정과 비밀번호 대신 토큰으로 대체
19)   

```
