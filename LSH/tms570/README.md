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


```
