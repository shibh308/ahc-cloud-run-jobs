# 各ケース毎に呼ばれる処理
# $1 が入力, エラーにScoreを出すようにする
# これはインタラクティブ問題の時の設定なので, 適宜変える
./tester/tester ./solution/a.out < $1 > /dev/null
