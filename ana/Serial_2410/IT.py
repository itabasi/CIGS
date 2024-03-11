#!/usr/bin/env python
# -*- coding: utf-8 -*-

import time
import threading
import math
import itertools
import csv
from matplotlib import pyplot as plt
from matplotlib import animation
fname="./test.txt";

def read(file):
    csv_file = open("./test.txt","r",encoding="ms932",errors="",newline="")

    
    
def _redraw(_, x, y):
    """グラフを再描画するための関数"""
    # 現在のグラフを消去する
    plt.cla()
    # 折れ線グラフを再描画する
    plt.plot(x, y)

def main():
    # 描画領域
    fig = plt.figure(figsize=(10, 6))
    # 描画するデータ (最初は空っぽ)
    x = []
    y = []

    def _update():
        """データを一定間隔で追加するスレッドの処理"""
        for frame in itertools.count(0, 0.1):
            x.append(frame)
            y.append(math.sin(frame))
            # データを追加する間隔 (100ms)
            time.sleep(0.1)

    def _init():
        """データを一定間隔で追加するためのスレッドを起動する"""
        t = threading.Thread(target=_update)
        t.daemon = True
        t.start()

    params = {
        'fig': fig,
        'func': _redraw,  # グラフを更新する関数
        'init_func': _init,  # グラフ初期化用の関数 (今回はデータ更新用スレッドの起動)
        'fargs': (x, y),  # 関数の引数 (フレーム番号を除く)
        'interval': 250,  # グラフを更新する間隔 (ミリ秒)
    }
    anime = animation.FuncAnimation(**params)

    # グラフを表示する
    plt.show()


if __name__ == '__main__':
    main()
