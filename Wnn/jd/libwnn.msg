#
#   $Id: ./libwnn.msg $
#
#
# FreeWnn is a network-extensible Kana-to-Kanji conversion system.
# This file is part of FreeWnn.
# 
# Copyright Kyoto University Research Institute for Mathematical Sciences
#                 1987, 1988, 1989, 1990, 1991, 1992
# Copyright OMRON Corporation. 1987, 1988, 1989, 1990, 1991, 1992, 1999
# Copyright ASTEC, Inc. 1987, 1988, 1989, 1990, 1991, 1992
# Copyright FreeWnn Project 1999, 2000
# 
# Maintainer:  FreeWnn Project   <freewnn@tomo.gr.jp>
# 
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#
#	massage file for libwnn.a
# wnnerrmsg.c
#0	no_error 0
1	ファイルが存在しません。
#2	no_error 2
3	メモリallocで失敗しました。
#4	no_error 4
5	辞書ではありません。
6	頻度ファイルではありません。
7	付属語ファイルではありません。
#8	no_error 8
9	辞書テーブルが一杯です。
10	頻度ファイルが、指定された辞書の頻度ファイルではありません。
#11	no_error 11
#12	no_error 12
#13	no_error 13
#14	no_error 14
#15	no_error 15
16	ファイルがオープンできません。
17	正しい頻度ファイルではありません。
18	正しい付属語ファイルではありません。
19	付属語の個数、ベクタ長さなどが多過ぎます。
20	その番号の辞書は、使われていません。
#21	no_error 21
#22	no_error 22
#23	no_error 23
24	付属語ファイルの内容が正しくありません。
25	疑似品詞番号が異常ですhinsi.dataが正しくありません。
26	未定義の品詞が前端品詞として定義されています。
27	付属語ファイルが読み込まれていません。
#28	no_error 28
#29	no_error 29
30	辞書のエイントリが多過ぎます。
31	変換しようとする文字列が長過ぎます。
32	付属語解析領域が不足しています。
#33	no_error 33
34	次候補領域が不足しています。
35	候補が 1 つも作れませんでした。
#36	no_error 36
#37	no_error 37
#38	no_error 38
#39	no_error 39
40	読みが長過ぎます。
41	漢字が長過ぎます。
42	指定された辞書は、登録可能ではありません。
43	読みの長さが0です。
44	指定された辞書は、逆引き可能ではありません。
45	リードオンリーの辞書に登録/削除しようとしました。
46	環境に存在しない辞書に登録しようとしました。
#47	no_error 47
#48	no_error 48
49	リードオンリーの頻度を変更しようとしました。
50	指定された単語が存在しません。
#51	no_error 51
#52	no_error 52
#53	no_error 53
#54	no_error 54
#55	no_error 55
#56	no_error 56
#57	no_error 57
#58	no_error 58
#59	no_error 59
60	メモリallocで失敗しました。
#61	no_error 61
#62	no_error 62
#63	no_error 63
#64	no_error 64
#65	no_error 65
#66	no_error 66
#67	no_error 67
68	何かのエラーが起こりました。
69	バグが発生している模様です。
70	サーバが死んでいます。
71	allocに失敗しました。
72	サーバと接続できませんでした。
73	通信プロトコルのバージョンが合っていません。
74	クライアントの生成した環境ではありません。
#75	no_error 75
#76	no_error 76
#77	no_error 77
#78	no_error 78
#79	no_error 79
80	ディレクトリを作ることができません。
#81	no_error 81
#82	no_error 82
#83	no_error 83
#84	no_error 84
#85	no_error 85
#86	no_error 86
#87	no_error 87
#88	no_error 88
#89	no_error 89
90	ファイルを読み込むことができません。
91	ファイルを書き出すことができません。
92	クライアントの読み込んだファイルではありません。
93	これ以上ファイルを読み込むことができません。
94	パスワードが間違っています。
95	ファイルが読み込まれています。
96	ファイルが削除できません。
97	ファイルが作成出来ません。
98	Ｗｎｎのファイルでありません。
99	ファイルのI-nodeとFILE_UNIQを一致させる事ができません。
100	品詞ファイルが大き過ぎます。
101	品詞ファイルが大き過ぎます。
102	品詞ファイルが存在しません。
103	品詞ファイルの内容が間違っています。
#104	no_error 104
105	品詞ファイルが読み込まれていません。
106	品詞名が間違っています。
107	品詞番号が間違っています。
#108	no_error 108
109	その操作はサポートされていません。
110	パスワードの入っているファイルがオープンできません。
111	uumrcファイルが存在しません。
112	uumrcファイルの形式が誤っています。
113	これ以上環境を作ることはできません。
114	このクライアントが読み込んだファイルでありません。
115	辞書に頻度ファイルがついていません。
116	パスワードのファイルが作成出来ません。
### jl.c
200	辞書ファイル
201	が無いよ。
202	作る?(Y/N)
203	頻度ファイル
204	辞書と頻度 \"%s\" の整合性が無いよ。作り直す?(Y/N)
205	password_file
206	include のレベルが多過ぎます。
207	file \"%s\"が open できません。
208	ファイル \"%s\" で環境設定中に、エラーが発生したために、設定を中止します。\n
209	を作りました。
210	directry
###
