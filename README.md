# CP2017
A Mahjong Game implemented in C++
Mahjong，中文為麻將，為一種源自中國的策略遊戲
我們實作的程式為台灣16張麻將，分數計算為20底10台
遊戲規則: http://www.dragona.com.tw/mahjong-rule/
台數計算: http://ezmjtw.tripod.com/mahjong16-big5.htm

## 麻將對照表
![Model Architecture](/images/ids.jpeg)

## 程式結構圖
![Dependency](/images/dependency.jpeg)

## Git tutorial

* To clone from scratch
```
git clone https://github.com/haochunchang/CP2017.git
mv CP2017
```
* To update files
1. First, you need to get the newest files
```
git pull 
```
2. Second, add new files and commit with clear messages(what you have changed)
```
git add [new/changed files]
git commit -m "[messages]"
```
3. Finally, upload committed files to remote
```
git push
```

* To undo your local change
```
git checkout [filename]
```
* To see what files have you changed
```
git status
```
```
