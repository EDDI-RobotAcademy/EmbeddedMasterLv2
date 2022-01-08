## 권장 사항

```make
1. PC 및 노트북 USB 단자 전원이 원래 불안정합니다.
2. JTAG 활용시 외부 전원이 제공되는 USB Hub를 사용해서 활용하는 것을 권장합니다.
3. 전원이 불안정할시 노트북 혹은 PC 그리고 보드의 JTAG 모두 손상이 될 수 있습니다.
```

## 초기 설정

```make
1. CCS를 다운 받으러 간다.

https://software-dl.ti.com/ccs/esd/documents/ccs_downloads.html

2. 11 버전을 선택한다.

3. Linux single file installer for CCS IDE 를 선택해서 다운 받는다.

4. 적당한 위치를 지정하고 압축을 해제한다.

cd ~
mkdir sw
cd sw
mkdir ti
cd ti
mv ~/Downloads/CCS11.1.0.00011_linux-x64.tar.gz ./
tar zxvf CCS11.1.0.00011_linux-x64.tar.gz

5. 압축을 해제했다면 CCS 디렉토리가 보일 것이다.

cd CCS11.1.0.00011_linux-x64
./ccs_setup_11.1.0.00011.run
  
  
* 32-bit Dependency Error 발생시

https://software-dl.ti.com/ccs/esd/documents/ccsv11_linux_host_support.html

결론:
sudo apt install libc6:i386 libusb-0.1-4 libgconf-2-4 libncurses5 libpython2.7 libtinfo5

6. 이후 Custom Installation을 눌러서 설치를 진행한다.

7. Hercules Safety MCU를 선택 합니다.

8. 이후 Forward를 하면 설치가 진행됩니다.

9. Launch를 눌러서 실행이 잘 되면 OK
```

## HalCoGen 설정

```make
0. PlayOnLinux와 HalCoGen이 필요합니다.

1. HalCoGen을 다운로드 받습니다.

https://www.ti.com/tool/HALCOGEN

2. 아래 절차를 따라서 압축을 해제합니다.

cd ~/sw/ti
mkdir halcogen
cd halcogen
mv ~/Downloads/spnc030g.zip ./
unzip spnc030g.zip

3. PlayOnLinux를 설치합니다.

sudo apt install playonlinux

4. playonlinux를 구동합니다.

5. Install을 누른다.

6. 좌측 하단에 Install a non-listed program을 누른다.

7. Next -> Next를 누릅니다.

8. Install a program in a new virtual drive를 선택하고 Next

9. 이름을 반드시 써줘야 한다 - z정도가 무난함

10. 32 bits Windows installation을 누릅니다.

11. Browse에서 다운로드 받은 HalCoGen.exe를 넣어주면 됩니다.

12. 설치 도중 Diagnostic(진단) 라이브러리 설치 사항이 나타난다.
    우리가 사용하는 tms570lc4357만 살리고 나머지는 끄도록 한다(용량 세이브)
```

## CCS 프로젝트 만들기

```make
1. File -> New -> Project or Other(CCS Proejct)
2. Empty Project
3. Target: Unclassifie Devices
4. TMS570LC43xx
5. Connection: Texas Instruments XDS100v2 USB Debug Probe

* 만약 Verify 과정에서 문제가 발생한다면 아래와 같은 절차를 진행합니다.

cd ~/ti/ccs숫자/ccs/install_scripts
sudo ./install_drivers.sh

6. 다시 Verify를 통해 검증을 수행합니다.
7. 

```

