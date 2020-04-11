# ShitGraph
Windows용 그래프 시각화 프로그램

## 컴파일
```
$ git clone https://github.com/kmc7468/ShitGraph.git
$ cd ShitGraph
$ cmake .
$ msbuild ShitGraph.sln /t:ALL_BUILD /p:Configuration=Release
```

## 조작법
- **이동**: 마우스를 왼쪽으로 누른 상태로 움직이세요.
- **확대/축소**: 마우스 휠을 굴리세요.
	- **Ctrl 키**를 누르고 굴리면 화면의 중심을 기준으로 확대/축소할 수 있습니다.

## 이미지
<div align="center">
	<img src="https://github.com/kmc7468/ShitGraph/blob/master/docs/Example1.png" />
</div>

- ![equation](https://latex.codecogs.com/gif.latex?y%3D%28x-1%29%28x-2%29%28x-3%29)
- ![equation](https://latex.codecogs.com/gif.latex?y%3D%5Csin%20x)
- ![equation](https://latex.codecogs.com/gif.latex?y%3D%5Ccos%20x)
- ![equation](https://latex.codecogs.com/gif.latex?x%5E2&plus;y%5E2%3D1)

<div align="center">
	<img src="https://github.com/kmc7468/ShitGraph/blob/master/docs/Example2.png" />
</div>

- ![equation](https://latex.codecogs.com/gif.latex?%5Csum%5E%7B100%7D_%7Bn%3D0%7D0.5%5En%5Ccos%283%5En%5Cpi%20x%29)

## 라이선스
- [MIT 라이선스](https://github.com/kmc7468/ShitGraph/blob/master/LICENSE)의 보호를 받습니다.
- 라이선스 파일이 포함되지 않은 커밋도 MIT 라이선스가 적용됩니다.